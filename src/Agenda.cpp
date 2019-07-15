#include "AgendaUI.hpp"
#include <stdlib.h>
#include <signal.h> 
AgendaUI agenda;
static void signal_handler(int sig){
	std::cout<<"\nCtrl+C signal captured."<<std::endl;
	std::cout<<"Data has been synchronized"<<std::endl;
	std::cout<<"Program terminated."<<std::endl;
	agenda.quitAgenda();
	exit(0);
}
int main()
{
signal(SIGINT, signal_handler);  
agenda.OperationLoop();
}
