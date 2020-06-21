#include "include.hpp"

#ifndef _Order
#define _Order

using namespace std;

class Order{
public:
	char status[10];
	char username[25], trainid[25];
	int iduser, dday, idtrain;
	Ticket ticket;
	Order(){}
	Order(const char *_status, const char *_username, int _iduser, const char *_trainid, int _dday, int _idtrain, Ticket _ticket){
		strcpy(status, _status);
		strcpy(username, _username);
		iduser = _iduser; 
		strcpy(trainid, _trainid);
		dday = _dday;
		idtrain = _idtrain;
		ticket = _ticket;
	}
};

#endif