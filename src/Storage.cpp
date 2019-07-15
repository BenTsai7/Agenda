#include "Storage.hpp"
#include "Path.hpp"
#include <exception>
#include <fstream>
#include <sstream>
std::shared_ptr<Storage> Storage::m_instance=nullptr;
std::vector<std::string> split_csv(std::string s,char mark)
{
	std::vector<std::string> result;
	std::istringstream iss(s);
	std::string w;
	while (getline(iss,w,mark)) {result.push_back(w);}
	return result;
}
Storage::Storage(){
 readFromFile();
 m_dirty = false;
}
bool Storage::readFromFile(void)
{
	std::ifstream infile;
	try
	{
		std::string s;
		infile.open(Path::userPath);
		while (getline(infile,s))
		{
			std::vector<std::string> info=split_csv(s,',');
			User u;
			u.setName(info[0].substr(1,info[0].length()-2));
			u.setPassword(info[1].substr(1,info[1].length()-2));
			u.setEmail(info[2].substr(1,info[2].length()-2));
			u.setPhone(info[3].substr(1,info[3].length()-2));
			m_userList.push_back(u);
		}
		infile.close();
		infile.open(Path::meetingPath);
		while (getline(infile,s))
		{
			std::vector<std::string> info=split_csv(s,',');
			Meeting m;
			m.setSponsor(info[0].substr(1,info[0].length()-2));
			std::vector<std::string> parts=split_csv(info[1].substr(1,info[1].length()-2),'&');
			m.setParticipator(parts);
			m.setStartDate(Date(info[2].substr(1,info[2].length()-2)));
			m.setEndDate(Date(info[3].substr(1,info[3].length()-2)));
			m.setTitle(info[4].substr(1,info[4].length()-2));
			m_meetingList.push_back(m);
		}
		infile.close();
	}
	catch(std::exception e) {return false;}
	return true;
}
bool Storage::writeToFile(void)
{
	std::ofstream outfile;
	try
	{
	outfile.open(Path::userPath,std::ios::out);
	for (auto i=m_userList.begin();i!=m_userList.end();++i)
	{
		outfile<<"\""<<(*i).getName()<<"\","<<"\""<<(*i).getPassword()
		<<"\","<<"\""<<(*i).getEmail()<<"\","<<"\""<<(*i).getPhone()<<"\""<<std::endl;
	}
	outfile.close();
	outfile.open(Path::meetingPath,std::ios::out);
	for (auto i=m_meetingList.begin();i!=m_meetingList.end();++i)
	{
		outfile<<"\""<<(*i).getSponsor()<<"\",";
		std::vector<std::string> participators = (*i).getParticipator();
		outfile<<"\"";
		int j;
		for (j = 0; j < participators.size()-1; ++j)
		{
			outfile<<participators[j]<<"&";
		}
		outfile<<participators[j]<<"\",\"";
		outfile<<Date::dateToString((*i).getStartDate())<<"\",\"";
		outfile<<Date::dateToString((*i).getEndDate())<<"\",";
		outfile<<"\""<<(*i).getTitle()<<"\""<<std::endl;
	}
	outfile.close();
	}
	catch(std::exception e) {return false;}
	return true;
}


std::shared_ptr<Storage> Storage::getInstance(void)
 {
	if(m_instance == nullptr) {
  std::shared_ptr<Storage> new_instance(new Storage());
  m_instance = new_instance;}
  return m_instance;
}
Storage::~Storage() 
{ 
writeToFile();
m_instance = nullptr;
}


void Storage::createUser(const User &t_user)
{
	m_userList.push_back(t_user);
}
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const
{
	std::list<User> newUserList;
	for (User u:m_userList)
	{
		if (filter(u)) newUserList.push_back(u);
	}
	return newUserList;
}
int Storage::updateUser(std::function<bool(const User &)> filter,std::function<void(User &)> switcher)
{
	int sum=0;
	for (auto i=m_userList.begin();i!=m_userList.end();++i)
	{
		if (filter(*i)) {switcher(*i);++sum;}
	}
	return sum;
}
int Storage::deleteUser(std::function<bool(const User &)> filter)
{
	int sum=0;
	std::list<User> newUserList;
	for (User u:m_userList)
	{
		if (filter(u))++sum;
		else newUserList.push_back(u);
	}
	m_userList.swap(newUserList);
	return sum;
}
void Storage::createMeeting(const Meeting &t_meeting)
{
	m_meetingList.push_back(t_meeting);
}
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const
{
	std::list<Meeting> newMeetingList;
	for (auto u:m_meetingList)
	{
		if (filter(u)) newMeetingList.push_back(u);
	}
	return newMeetingList;
}
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,std::function<void(Meeting &)> switcher)
{
	int sum=0;
	for (auto i=m_meetingList.begin();i!=m_meetingList.end();++i)
	{
		if (filter(*i)) {switcher(*i);++sum;}
	}
	return sum;
}
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter)
{
	int sum=0;
	std::list<Meeting> newMeetingList;
	for (auto m:m_meetingList)
	{
		if (filter(m))++sum;
		else newMeetingList.push_back(m);
	}
	m_meetingList.swap(newMeetingList);
	return sum;
}
bool Storage::sync(void)
{
	return writeToFile();
}
