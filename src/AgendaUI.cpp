#include "AgendaUI.hpp"
#include "AgendaException.hpp"

AgendaUI::AgendaUI(){
  startAgenda();
}
void AgendaUI::OperationLoop(void) {
  std::cout << "-------------------------Agenda------------------------\n"
            << "Action : \n"
            << "l   - log in Agenda by user name and password\n"
            << "r   - register an Agenda account\n"
            << "q   - quit Agenda\n"
            << "-------------------------------------------------------\n"
            << "\nAgenda : ~$ ";
  std::string op;
  std::cin >> op;
  std::cout << std::endl;
  while (executeOperation(op)) {
    std::cout << "\n-------------------------Agenda------------------------\n"
              << "Action : \n"
              << "l   - log in Agenda by user name and password\n"
              << "r   - register an Agenda account\n"
              << "q   - quit Agenda\n"
              << "-------------------------------------------------------\n"
              << "\nAgenda : ~$ ";
    std::cin >> op;
    std::cout << std::endl;
  }
  quitAgenda();
}

void AgendaUI::startAgenda(void) {
  m_agendaService.startAgenda();
}
std::string AgendaUI::getOperation(void) { 
  std::string op;
  std::cout << "\n-------------------------Agenda------------------------\n"
            << "Action : \n"
            << "o   - log out Agenda\n"
            << "dc  - delete Agenda account\n"
            << "lu  - list all Agenda user\n"
            << "cm  - create a meeting\n"
            << "amp - add meeting participator\n"
            << "rmp - remove meeting participator\n"
            << "rqm - request to quit meeting\n"
            << "la  - list all meetings\n"
            << "las - list all sponsor meetings\n"
            << "lap - list all participate meetings\n"
            << "qm  - query meeting by title\n"
            << "qt  - query meeting by time interval\n"
            << "dm  - delete meeting by title\n"
            << "da  - delete all meetings\n"
            << "-------------------------------------------------------\n"
            << "\nAgenda@" << m_userName << " : # ";
  std::cin >> op;
  std::cout << std::endl;
  return op;
}
bool AgendaUI::executeOperation(std::string t_operation) {

  if (t_operation == std::string("q")) return false;
   else if (t_operation == std::string("r")) {
    userRegister();
    return true;
	}
   else if (t_operation == std::string("l")) {
     try{userLogIn();}
	catch(AgendaException* e)
  	  {
  	  	std::cout << "[error] "<<e->msg<<std::endl;
  	  	delete e;
	  }
    while (m_userName != "") {
      std::string nextop;
      nextop = getOperation();
      try {
      if (nextop == std::string("o")) {
        userLogOut();
      } else if (nextop == std::string("dc")) {
        deleteUser();
      } else if (nextop == std::string("lu")) {
        listAllUsers();
      } else if (nextop == std::string("cm")) {
        createMeeting();
      } else if (nextop == std::string("la")) {
        listAllMeetings();
      } else if (nextop == std::string("las")) {
        listAllSponsorMeetings();
      } else if (nextop == std::string("lap")) {
        listAllParticipateMeetings();
      } else if (nextop == std::string("qm")) {
        queryMeetingByTitle();
      } else if (nextop == std::string("qt")) {
        queryMeetingByTimeInterval();
      } else if (nextop == std::string("dm")) {
        deleteMeetingByTitle();
      } else if (nextop == std::string("da")) {
        deleteAllMeetings();
      }
      	else if (nextop == std::string("amp")) {
        addMeetingPartcipator();
      }
      	else if (nextop == std::string("rmp")) {
        removeMeetingPartcipator();
      }
      	else if (nextop == std::string("rqm")) {
        quitMeeting();
      }
	  }
      catch(AgendaException* e)
  	  {
  	  	std::cout << "[error] "<<e->msg<<std::endl;
  	  	delete e;
	  }
	  }
    return true;
  } else {
    std::cout << "[error] undefined operation"<<std::endl;
    return true;
  }
}

void AgendaUI::userLogIn(void) {
  std::string userName, password;
  std::cout << "[log in] [user name] [password]\n";
  std::cout << "[log in] ";
  std::cin >> userName >> password;
  if (m_agendaService.userLogIn(userName, password)) {
    m_userName = userName;
    m_userPassword = password;
    std::cout << "[log in] succeed!"<<std::endl;
  } else {
    std::cout << "[log in] Password error or user dosen't exist"<<std::endl;
  }
}

void AgendaUI::userRegister(void) {
  std::string userName, password, email, phone;
  std::cout << "[register] [user name] [password] [email] [phone]"<<std::endl;
  std::cout << "[register] ";

  std::cin >> userName >> password >> email >> phone;

  if (m_agendaService.userRegister(userName, password, email, phone))
    std::cout << "[register] succeed!"<<std::endl;
  else
    std::cout << "[register] This username has been registered!"<<std::endl;
}

void AgendaUI::quitAgenda(void) {
  m_agendaService.quitAgenda();
}

void AgendaUI::userLogOut(void) {
  m_userName = "";
  m_userPassword = "";
  m_agendaService.logOut();
}

void AgendaUI::deleteUser(void) {
  if (m_agendaService.deleteUser(m_userName,m_userPassword)) 
  {
    userLogOut();
    std::cout << "[delete Agenda account] succeed!"<<std::endl;
  } else {
    std::cout << "[error] delete Agenda account fail!"<<std::endl;
  }
}

void AgendaUI::listAllUsers(void) {
  std::list<User> users = m_agendaService.listAllUsers();

  std::cout << "[list all users]\n\n";
  std::cout << std::setiosflags(std::ios::left) << std::setfill(' ')
             << std::setw(8) << "name" << std::setw(16) << "email"
             << std::setw(11) << "phone" << '\n';
  //std::cout << "name\temail\tphone\n";
  for (auto u : users) {
   std::cout << std::setiosflags(std::ios::left) << std::setfill(' ')
             << std::setw(8) << u.getName() << std::setw(16)
             << u.getEmail() << std::setw(11) << u.getPhone() << '\n';
    //std::cout << u.getName() << '\t'<< u.getEmail() << '\t' << u.getPhone() << std::endl;
  }
}

void AgendaUI::createMeeting(void) {
  std::string title,sd, ed;
  std::vector<std::string> participator;
  int num;

  std::cout << "[create meeting] [the number of participators]\n";
  std::cout<<"[create meeting]";
  std::cin>>num;
  for (int i=0;i<num;++i)
  {
  	std::cout <<"[create meeting] [please enter the participator "<<i+1<<" ]"<<std::endl;
  	std::cout<<"[create meeting]";
  	std::string temp;
  	std::cin>>temp;
  	participator.push_back(temp);
  	std::cout<<std::endl;
  }
  std::cout<<"[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mmdd/hh:mm)]\n";
  std::cout<<"[create meeting]";
  std::cin >> title >> sd >> ed;
  if (m_agendaService.createMeeting(m_userName, title, sd, ed,participator))
    std::cout << "[create meeting] succeed!\n";
  else
    std::cout << "[error] create meeting fail!\n";
}

void AgendaUI::listAllMeetings(void) {
  std::cout << "[list all meetings]"<<std::endl<<std::endl;
  printMeetings(m_agendaService.listAllMeetings(m_userName));
}

void AgendaUI::listAllSponsorMeetings(void) {
  std::cout << "[list all sponsor meetings]\n\n";
  printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}

void AgendaUI::listAllParticipateMeetings(void) {
  std::cout << "[list all participate meetings]\n\n";
  printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}

void AgendaUI::queryMeetingByTitle(void) {
  std::cout << "[query meeting] [title]:\n"
            << "[query meeting] ";
  std::string title;
  std::cin>>title;
  std::cout<<std::endl;
  printMeetings(m_agendaService.meetingQuery(m_userName, title));
}

void AgendaUI::queryMeetingByTimeInterval(void) {
  std::string sd, ed;
  std::cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)]"
            << " [end time(yyyy-mm-dd/hh:mm)]\n"
            << "[query meetings] ";
  std::cin >> sd >> ed;
  std::cout<<std::endl;
  printMeetings(m_agendaService.meetingQuery(m_userName,sd, ed));
}

void AgendaUI::deleteMeetingByTitle(void) {
  std::cout << "[delete meeting] [title]\n"
            << "[delete meeting] ";
  std::string title;
  std::cin>>title;
  std::cout<<std::endl;
  if (m_agendaService.deleteMeeting(m_userName, title))
    std::cout << "[delete meeting by title] succeed!\n";
  else
    std::cout << "[error] delete meeting fail!\n";
}

void AgendaUI::deleteAllMeetings(void) {
  if (m_agendaService.deleteAllMeetings(m_userName))
    std::cout << "[delete all meetings] succeed!\n";
  else
    std::cout << "[error] delete meetings fail!\n";
}

void AgendaUI::printMeetings(std::list<Meeting> meetings) {
  //std::cout << "title\tsponsor\tstart time\tend time\tparticipators\n";
  std::cout << std::setiosflags(std::ios::left) << std::setfill(' ')
             << std::setw(10) << "title" << std::setw(10) << "sponsor"
             << std::setw(20) << "start time" << std::setw(20) <<"end time" << "participators" <<'\n';
  for (auto m : meetings) {
    std::cout << std::setiosflags(std::ios::left) << std::setfill(' ')
			  << std::setw(10) <<m.getTitle() 
			  << std::setw(10) <<m.getSponsor()
              << std::setw(20) << Date::dateToString(m.getStartDate()) 
              << std::setw(20) <<Date::dateToString(m.getEndDate());
    std::vector<std::string> v = m.getParticipator();
    int i; 
    for (i=0;i<v.size()-1;++i) std::cout << v[i]<<",";
	if (v.size()!=0)std::cout<<v[i]<<std::endl; 
  }
}
void AgendaUI::addMeetingPartcipator()
{
	std::cout << "[add participator] [meeting title] [participator username]\n"
            << "[add participator] ";
    std::string title,participator;
    std::cin>>title>>participator;
    std::cout<<std::endl;
    if (m_agendaService.addMeetingParticipator(m_userName,title,participator)) std::cout << "[add participator] succeed!\n";
    else
    std::cout << "[add participator] error!\n";
}
void AgendaUI::removeMeetingPartcipator()
{
	std::cout << "[remove participator] [meeting title] [participator username]\n"
            << "[remove participator] ";
    std::string title,participator;
    std::cin>>title>>participator;
    std::cout<<std::endl;
    if (m_agendaService.removeMeetingParticipator(m_userName,title,participator)) std::cout << "[remove participator] succeed!\n";
    else
    std::cout << "[remove participator] error!\n";
}
void AgendaUI::quitMeeting()
{
	std::string title; 
	std::cout << "[quit meeting] [meeting title]\n"
            << "[quit meeting] ";
    std::cin>>title;
    std::cout<<std::endl;
	if (m_agendaService.quitMeeting(m_userName,title)) std::cout<<"[quit meeting] succeed!\n";
	else std::cout<< "[quit meeting] error!\n";
}

