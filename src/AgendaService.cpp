#include "AgendaService.hpp"
#include "AgendaException.hpp"
#include "Logger.hpp"
static Logger l; 
AgendaService::AgendaService()
{
	m_storage=Storage::getInstance();
	l.info("AgendaService launched.");
}
AgendaService::~AgendaService()
{
	l.resetUser();
	if (m_storage!=nullptr) {m_storage->sync();}
	m_storage=nullptr; 
}
bool AgendaService::userLogIn(const std::string &userName,const std::string &password)
{
	std::string s("User trying to Log In. Username: ");
	l.info(s+userName);
	std::function<bool(const User &)> check = [&userName,&password] (const User &u) -> bool
	{
		if (u.getName()==userName&&u.getPassword()==password) return true;
		else return false;
	};
	std::list<User> temp=m_storage->queryUser(check);
	if (temp.size()==0) {l.warning("User dosen't exist or password error!");throw new AgendaException(std::string("User dosen't exist or password error!"));} 
	else {
	l.setUser(userName);
	l.info("User log in sucessfully.");
	return true;}
}
 bool AgendaService::userRegister(const std::string &userName, const std::string &password, 
                    const std::string &email, const std::string &phone)
 {
 	l.info("User trying to Register.");
 	std::function<bool(const User &)> check = [&userName] (const User &u) -> bool
	{
		if (u.getName()==userName) return true;
		else return false;
	};
	std::list<User> temp=m_storage->queryUser(check);
	if (temp.size()!=0) {l.error("Register failed.Username has been used!");throw new AgendaException(std::string("Username has been used!"));}
	m_storage->createUser(User(userName,password,email,phone));
	l.info("Register success.Username: "+userName);
	return true;
 }
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) 
{
	l.warning("trying to delete own account.");
	std::function<bool(const User &)> check = [&userName,&password] (const User &u) -> bool
	{
		if (u.getName()==userName&&u.getPassword()==password) return true;
		else return false;
	};
	int result=m_storage->deleteUser(check);
	if (result==0) return false;
	l.warning("Delete user successfully. deleted user: "+userName);
	std::function<bool(const Meeting &)> check_s = [&userName] (const Meeting &m) -> bool
	{
		if (m.getSponsor()==userName) return true;
		else return false;
	};
	m_storage->deleteMeeting(check_s);
	std::function<bool(const Meeting &)> check_p = [&userName] (const Meeting &m) -> bool
	{
		if (m.isParticipator(userName)) return true;
		else return false;
	};
	std::function<void(Meeting &)> d_p = [&userName] (Meeting &m) -> void
	{
		m.removeParticipator(userName);
	};
	m_storage->updateMeeting(check_p,d_p);
	l.warning("Delete meetings related to the user successfully.");
	l.resetUser();
	return true;
}
 std::list<User> AgendaService::listAllUsers(void) const 
 {
 	l.info("list all users.");
 	std::function<bool(const User &)> check = [] (const User &u) ->bool
	{
		return true;
	};
	std::list<User> temp=m_storage->queryUser(check);
	return temp;
 }
 bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator)
 {
 	Date sd= Date::stringToDate(startDate);
 	Date ed= Date::stringToDate(endDate);
 	std::string s("trying to create meeting. title: ");
 	s = s+title;
 	s = s + " startDate: ";
 	s = s+startDate;
 	s = s + " EndDate: ";
 	s = s+endDate;
 	l.info(s);
 	if (!(Date::isValid(sd)&&Date::isValid(ed))) {l.error("create meeting fails. due to Invalid Date.");throw new AgendaException(std::string("Invalid Date!"));}
 	if (sd>=ed) {l.error("create meeting fails. Begin Date must be a date before the End Date!");throw new AgendaException(std::string("Begin Date must be a date before the End Date!"));}
 	bool isNotUser = m_storage->queryUser([&userName](const User & user) {return user.getName() == userName;}).empty();
 	if (isNotUser) return false;
 	for (int i=0;i<participator.size();++i)
 	{
 		bool isNotParticipator = m_storage->queryUser([&participator,&i] (const User & user) {return user.getName() ==participator[i];}).empty();
 		if (isNotParticipator) {l.error("create meeting fails.Some participators are not registered users!");throw new AgendaException(std::string("Some participators are not registered users!"));}
	 }
	 for (int i=0;i<participator.size();++i) if (participator[i]==userName){l.error("create meeting fails.user duplicated in participators!");
	 throw new AgendaException(std::string("user duplicated in participators"));}
	 for (int i=0;i<participator.size();++i)
	 	for (int j=i+1;j<participator.size();++j) if (participator[i]==participator[j]) {l.error("create meeting fails.participators duplicated!");
		 throw new AgendaException(std::string("participators duplicated!"));}
 	std::function<bool(const Meeting&)> check_busy = [&sd,&ed,&participator,&userName] (const Meeting& m)-> bool
 	{
 	 	if (m.getSponsor()==userName||m.isParticipator(userName))
 		{
 			if (m.getStartDate() >= sd && m.getEndDate()<=ed)
			 return true;
        	else if (m.getStartDate()<ed&& m.getEndDate()>=ed)  
          	return true;
        	else if (m.getStartDate()<=sd&& m.getEndDate()>sd)  
          	return true;
        	else return false;  
 		}
 		for (int i=0;i<participator.size();++i)
 		{
 			if (m.getSponsor()==participator[i]||m.isParticipator(participator[i]))
 			{
 				if (m.getStartDate()>=sd&&m.getEndDate()<=ed)
			 	return true;
        		else if (m.getStartDate()<ed&&m.getEndDate()>=ed)  
          		return true;
        		else if (m.getStartDate()<=sd&&m.getEndDate()>sd)  
          		return true;
        		else return false;  
 			}
 		}
 		return false;
 	};
 	std::list<Meeting> temp2=m_storage->queryMeeting(check_busy);
 	if (!temp2.empty()) {l.error("create meeting fails.During this meeting period,sponsor is busy.");throw new AgendaException(std::string("During this meeting period, you are busy!"));}
 	for (int i=0;i<participator.size();++i) {if (!meetingQuery(participator[i], title).empty())return false;}
 	if (!meetingQuery(userName, title).empty())
	 {
	 	l.error("create meeting fails.During this meeting period, some participators are busy!");
	 throw new AgendaException(std::string("During this meeting period, some participators are busy!"));
	}
 	m_storage->createMeeting(Meeting(userName,participator,sd,ed,title));
 	l.info("create meeting success.");
 	return true;
 }
 bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator)
 {
 	l.info("trying to add meeting participator. title: "+title+" participator: " +participator);
 	std::function<bool(const User &)> checkuser = [&participator] (const User &u) ->bool
	{
		if (u.getName()==participator) return true;
		else return false;
	};
	std::list<User> user= m_storage->queryUser(checkuser);
	if (user.empty()) {l.error("add meeting participator fails. participators Participator dosen't register!");
	throw new AgendaException(std::string("Participator dosen't register!"));}
 	std::function<bool(const Meeting&)> check = [&userName,&title,&participator] (const Meeting& m)-> bool
 	{
 		if (m.getTitle()==title&&m.getSponsor()==userName&&(!m.isParticipator(participator))) return true;
 		return false;
 	};
 	std::list<Meeting> M=m_storage->queryMeeting(check);
 	if (M.empty()) return false;
 	Date sd=(*(M.begin())).getStartDate();
 	Date ed=(*(M.begin())).getEndDate();
/* 	std::function<bool(const Meeting&)> check_busy = [&sd,&ed,&participator] (const Meeting& m)-> bool
 	{
 		if (m.getSponsor()==participator||m.isParticipator(participator))
 		{
 			if (m.getStartDate() >= sd && m.getEndDate()<=ed)
			 return true;
        	else if (m.getStartDate()<ed&& m.getEndDate()>=ed)  
          	return true;
        	else if (m.getStartDate()<=sd&& m.getEndDate()>sd)  
          	return true;
        	else return false;  
 		}
 		return false;
 	};
 	std::list<Meeting> temp=m_storage->queryMeeting(check_busy);
 	if (!temp.empty()) return false;*/
 	std::function<void(Meeting&)> update = [&participator] (Meeting &m) -> void
 	{
 		m.addParticipator(participator);
 	};
 	int result=m_storage->updateMeeting(check,update);
 	if (result!=0) {l.info("add meeting participator success.");return true;}
 	else {l.info("add meeting participator fail. Can't not find meeting with title offered!");
	 throw new AgendaException(std::string("Can't not find meeting with title offered!"));}
 }
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator)
{
	l.warning("trying to remove meeting participator title: "+title+" participator: " +participator);
 	std::function<bool(const Meeting&)> check = [&userName,&title,&participator] (const Meeting& m)-> bool
 	{
 		if (m.getTitle()==title&&m.getSponsor()==userName&&m.isParticipator(participator)) return true;
 		return false;
 	};
 	std::function<void(Meeting&)> update = [&participator] (Meeting &m) -> void
 	{
 		m.removeParticipator(participator);
 	};
 	int result=m_storage->updateMeeting(check,update);
 	if (result!=0) {return true;l.warning("remove meeting participator success.");}
 	else {l.warning("remove meeting participator fail. Can't not find meeting with title offered!");throw new AgendaException(std::string("Can't not find meeting with title offered!"));}
 }
 bool AgendaService::quitMeeting(const std::string &userName, const std::string &title)
 {
 	l.warning("trying to quit meeting. title: "+title);
 	std::function<bool(const Meeting&)> check = [&userName,&title] (const Meeting& m)-> bool
 	{
 		if (m.getTitle()==title&&m.isParticipator(userName)) return true;
 		return false;
 	};
 	std::function<void(Meeting&)> update = [&userName] (Meeting &m) -> void
 	{
 		m.removeParticipator(userName);
 	};
 	int result=m_storage->updateMeeting(check,update);
 	if (result!=0) {l.warning("quit meeting successfully.");return true;}
 	else throw new AgendaException(std::string("Can't not find meeting with title offered!"));
 }
 std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const
 {
 	l.info("trying to query meeting title: "+title);
 	std::function<bool(const Meeting&)> check = [&userName,&title] (const Meeting& m)-> bool
 	{
 		if (m.getTitle()==title&&(m.isParticipator(userName)||m.getSponsor()==userName)) return true;
 		return false;
 	};
 	std::list<Meeting> temp= m_storage->queryMeeting(check);
 	return temp;
 }
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const
{
	Date sd=Date::stringToDate(startDate);
	Date ed=Date::stringToDate(endDate);
	std::string s("trying to query meeting from ");
	s += startDate;
	s = s + " to ";
	s += endDate;
	l.info(s);
	std::list<Meeting> temp;
	if ((!Date::isValid(sd))||(!Date::isValid(ed))) return temp;
	if (sd>ed) return temp;
	if (m_storage->queryUser([&userName](const User & user) {return user.getName() == userName;}).empty()) return temp;
	std::function<bool(const Meeting&)> check = [&userName,&sd,&ed] (const Meeting& m)-> bool
 	{
 		if (m.getSponsor()==userName||m.isParticipator(userName))
 		{
 			if (m.getEndDate()<sd || m.getStartDate()>ed) return false;
 			else return true;
 		}
 		return false;
 	};
 	temp= m_storage->queryMeeting(check);
 	return temp;
}
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const
{
	l.info("list all meetings.");
	std::function<bool(const Meeting&)> check = [&userName] (const Meeting& m)-> bool
 	{
 		if (m.isParticipator(userName)||m.getSponsor()==userName) return true;
 		return false;
 	};
 	std::list<Meeting> temp= m_storage->queryMeeting(check);
 	return temp;
}
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const
{
	l.info("list all sponsor meetings.");
	std::function<bool(const Meeting&)> check = [&userName] (const Meeting& m)-> bool
 	{
 		if (m.getSponsor()==userName) return true;
 		return false;
 	};
 	std::list<Meeting> temp= m_storage->queryMeeting(check);
 	return temp;
}
 std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const
{
	l.info("list all participate meetings.");
	std::function<bool(const Meeting&)> check = [&userName] (const Meeting& m)-> bool
 	{
 		if (m.isParticipator(userName)) return true;
 		return false;
 	};
 	std::list<Meeting> temp= m_storage->queryMeeting(check);
 	return temp;
}
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title)
{
	l.warning("delete meeting title: "+title);
	std::function<bool(const Meeting&)> check = [&userName,&title] (const Meeting& m)-> bool
 	{
 		if (m.getSponsor()==userName&&m.getTitle()==title) return true;
 		return false;
 	};
 	int result=m_storage->deleteMeeting(check);
 	if (result!=0) {l.warning("delete meeting success.");
	 return true;}
 	else 
	 {
	l.warning("delete meeting fail.Meeting not found!");
	 throw new AgendaException(std::string("Meeting not found!"));
	 }
}
 bool AgendaService::deleteAllMeetings(const std::string &userName)
 {
 	l.warning(("delete all meetings."));
 	std::function<bool(const Meeting&)> check = [&userName] (const Meeting& m)-> bool
 	{
 		if (m.getSponsor()==userName) return true;
 		return false;
 	};
 	int result=m_storage->deleteMeeting(check);
 	if (result!=0) {l.warning("delete all meetings success.");
	 return true;}
 	else {l.warning("delete all meetings fail.no meeting to be deleted.");throw new AgendaException(std::string("no meeting to be deleted"));
 	}
}
  void AgendaService::startAgenda(void)
  {
  	if (m_storage==nullptr)
  	{
  		m_storage=Storage::getInstance();
  	}
  }
  void AgendaService::quitAgenda(void)
  {
  	if (m_storage!=nullptr)
  	{
  		m_storage->sync();
  		m_storage=nullptr;
  	}
  }
  void AgendaService::logOut()
  {
  	l.resetUser();
  }
