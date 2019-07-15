#include<string>
class AgendaException
{
	public:
		AgendaException(std::string msg) {this->msg=msg;}
		std::string msg;
};
