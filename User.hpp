#include "include.hpp"

#ifndef _User
#define _User

using namespace std;

class user{
public:
	char username[25], password[35], name[40], mailaddr[35];
	int privilege, islogin, ordercnt;
	user(){
		privilege = islogin = ordercnt = 0;
	}
	user(const char* _username, const char* _password, const char* _name, 
			const char* _mailaddr, const int _privilege, const int _islogin = 0, int _ordercnt = 0){
		strcpy(username, _username); strcpy(password, _password);
		strcpy(name, _name); strcpy(mailaddr, _mailaddr);
		privilege = _privilege; islogin = _islogin; ordercnt = _ordercnt;
	}
	~user(){}
};

#endif