#include<iostream>
#include<string>
#include <time.h>
#include <exception>
#include <fstream>
#include <ctime>
#include <cstring>
#include <iomanip>
class Logger
{
	public:
	void error(std::string msg);
	void warning(std::string msg);
	void info(std::string msg);
	void setUser(std::string user);
	void resetUser();
	~Logger(); 
	private:
	std::string getTime();
	std::string user;
} ;
