#include "Logger.hpp"
void Logger::setUser(std::string user) {this->user=user;}
void Logger::resetUser() {user="";}
void Logger::error(std::string msg)
{
	std::ofstream outfile;
	try
	{
	outfile.open("../data/log.txt",std::ios::app);
	outfile<< std::setiosflags(std::ios::left) << std::setfill(' ')<< std::setw(11)<<"[error]";
	outfile<<"["<<getTime()<<"] ";
	if (user!="") outfile<<"["<<user<<"] ";
	outfile<<msg<<std::endl; 
	outfile.close();
	}
	catch(std::exception e) {return;}
}
void Logger::warning(std::string msg)
{
	std::ofstream outfile;
	try
	{
	outfile.open("../data/log.txt",std::ios::app);
	outfile<< std::setiosflags(std::ios::left) << std::setfill(' ')<< std::setw(11)<<"[Warning]";
	outfile<<"["<<getTime()<<"] ";
	if (user!="") outfile<<"["<<user<<"] ";
	outfile<<msg<<std::endl; 
	outfile.close();
	}
	catch(std::exception e) {return;}
} 
void Logger::info(std::string msg)
{
	std::ofstream outfile;
	try
	{
	outfile.open("../data/log.txt",std::ios::app);
	outfile<< std::setiosflags(std::ios::left) << std::setfill(' ')<< std::setw(11)<<"[Info]";
	outfile<<"["<<getTime()<<"] ";
	if (user!="") outfile<<"["<<user<<"] ";
	outfile<<msg<<std::endl;  
	outfile.close();
	}
	catch(std::exception e) {return;}
} 
std::string Logger::getTime()
{
	time_t t;
	t = time(NULL);
	char timechar[40];
	memset(timechar,0,sizeof(timechar));
	struct tm* curtm = localtime(&t); 
	strftime(timechar,sizeof(timechar)-1,"%F %T", curtm);
	return std::string(timechar);
}
Logger::~Logger()
{
	std::ofstream outfile;
	outfile.open("../data/log.txt",std::ios::app);
	info("Data synchronization.");
	info("AgendaService shutdown.");
	outfile<<std::endl; 
}
