#include "Date.hpp"
Date::Date()
{
	m_year=0;
	m_month=0;
	m_day=0;
	m_hour=0;
	m_minute=0;
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
{
	m_year=t_year;
	m_month=t_month;
	m_day=t_day;
	m_hour=t_hour;
	m_minute=t_minute;
}
Date::Date(const std::string &dateString)
{
	*this=stringToDate(dateString);
}
int Date::getYear(void) const {return m_year;}
void Date::setYear(const int t_year) {m_year=t_year;}
int Date::getMonth(void) const {return m_month;}
void Date::setMonth(const int t_month) {m_month=t_month;}
int Date::getDay(void) const{return m_day;}
void Date::setDay(const int t_day) {m_day=t_day;}
int Date::getHour(void) const{return m_hour;}
void Date::setHour(const int t_hour) {m_hour=t_hour;}
int Date::getMinute(void) const{return m_minute;}
void Date::setMinute(const int t_minute) {m_minute=t_minute;}
bool Date::isValid(const Date &t_date)
{
	int month_array[13]= {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
    int year=t_date.getYear();
    int month=t_date.getMonth();
    int day = t_date.getDay();
    int hour = t_date.getHour();
    int minute = t_date.getMinute();
	if (year<1000||year>9999) return false;
	bool isleap=false;
	if ((year%4==0 && year%100!=0))isleap=true;
	if (year%400==0) isleap=true;
	if (isleap) month_array[2]++;
	if (month<1||month>12) return false;
	if (day<1||day>month_array[month]) return false;
	if (hour<0||hour>23) return false;
	if (minute<0||minute>59) return false;
	return true;
}
Date Date::stringToDate(const std::string &t_dateString)
{
	int year=0,month=0,day=0,hour=0,minute=0;
	Date tempdate(0,0,0,0,0);
	if (t_dateString.length()!=16||t_dateString[4]!='-'||t_dateString[7]!='-'||t_dateString[10]!='/'||t_dateString[13]!=':') {return tempdate;}
	for (int i=0;i<4;++i)
	{
		char c=t_dateString[i];
		if (c<48||c>57) return tempdate;
		year *= 10;
		year += ((int)c-48);
	}
	for (int i=5;i<7;++i)
	{
		char c=t_dateString[i];
		if (c<48||c>57) return tempdate;
		month *= 10;
		month += ((int)c-48);
	}
	for (int i=8;i<10;++i)
	{
		char c=t_dateString[i];
		if (c<48||c>57) return tempdate;
		day *= 10;
		day += ((int)c-48);
	}
	for (int i=11;i<13;++i)
	{
		char c=t_dateString[i];
		if (c<48||c>57) return tempdate;
		hour *= 10;
		hour += ((int)c-48);
	}
	for (int i=14;i<16;++i)
	{
		char c=t_dateString[i];
		if (c<48||c>57) return tempdate;
		minute *= 10;
		minute += ((int)c-48);
	}
	tempdate.setYear(year);
	tempdate.setMonth(month);
	tempdate.setDay(day);
	tempdate.setHour(hour);
	tempdate.setMinute(minute);
	return tempdate;
}
std::string Date::dateToString(const Date &t_date)
{
	if (!isValid(t_date)) return std::string("0000-00-00/00:00");
	std::string dateString="";
	char temp;
	temp=t_date.getYear()/1000%10+'0';
	dateString = dateString+temp;
	temp=t_date.getYear()/100%10+'0';
	dateString = dateString+temp;
	temp=t_date.getYear()/10%10+'0';
	dateString = dateString+temp;
	temp=t_date.getYear()%10+'0';
	dateString = dateString+temp;
	temp='-';
	dateString = dateString+temp;
	temp=t_date.getMonth()/10%10+'0';
	dateString = dateString+temp;
	temp=t_date.getMonth()%10+'0';
	dateString = dateString+temp;
	temp='-';
	dateString = dateString+temp;
	temp=t_date.getDay()/10%10+'0';
	dateString = dateString+temp;
	temp=t_date.getDay()%10+'0';
	dateString = dateString+temp;
	temp='/';
	dateString = dateString+temp;
	temp=t_date.getHour()/10%10+'0';
	dateString = dateString+temp;
	temp=t_date.getHour()%10+'0';
	dateString = dateString+temp;
	temp=':';
	dateString = dateString+temp;
	temp=t_date.getMinute()/10%10+'0';
	dateString = dateString+temp;
	temp=t_date.getMinute()%10+'0';
	dateString = dateString+temp;
	return dateString;
}
Date& Date::operator=(const Date &t_date)
{
	m_year=t_date.getYear();
	m_month=t_date.getMonth();
	m_day=t_date.getDay();
	m_hour=t_date.getHour();
	m_minute=t_date.getMinute();
	return *this;
}
bool Date::operator==(const Date &t_date) const
{
	if (m_year!=t_date.getYear()) return false;
	if (m_month!=t_date.getMonth()) return false;
	if (m_day!=t_date.getDay()) return false;
	if (m_hour!=t_date.getHour()) return false;
	if (m_minute!=t_date.getMinute()) return false;
	return true;
}
bool Date::operator>(const Date &t_date) const
{
	if (m_year>t_date.getYear()) return true;
	else if (m_year<t_date.getYear()) return false;
	else
	{
		if (m_month>t_date.getMonth()) return true;
		else if (m_month<t_date.getMonth()) return false;
		else
		{
			if (m_day>t_date.getDay()) return true;
			else if (m_day<t_date.getDay()) return false;
			else
			{
				if (m_hour>t_date.getHour()) return true;
				else if (m_hour<t_date.getHour()) return false;
				else
				{
					if (m_minute>t_date.getMinute()) return true;
					return false;
				}
			}
		}
	}
}
bool Date::operator<(const Date &t_date) const
{
	if (*this==t_date) return false;
	if (*this>t_date) return false;
	return true;
}
bool Date::operator>=(const Date &t_date) const
{
	return (*this>t_date)||(*this==t_date);
}
bool Date::operator<=(const Date &t_date) const
{
	return (*this<t_date)||(*this==t_date);
}
