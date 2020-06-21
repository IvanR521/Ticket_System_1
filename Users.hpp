#include "include.hpp"
#include "Hash_table.hpp"
#include "User.hpp"

using namespace std;


#ifndef _Users
#define _Users

class Connector;

class Users{

public:

	//char bpuser_file[25], bpuser_disk[25];

	Bplustree<pair<int, int>, user> bpuser;

	Connector *connector;

	//Users();

	void init(Connector *_connector);

	void print_user(user _user);

	void add_user(const char *c, const char *u, const char *p, const char *n, const char *m, const int g);

	void login(const char *u, const char *p);

	void logout(const char *u);

	void query_profile(const char *c, const char *u);

	void modify_profile(const char *c, const char *u, const char *p, const char *n, const char *m, const int g);

};



#endif




/*
add_user
login
logout
query_profile
modify_profile

add_train
release_train
query_train
delete_train

query_ticket
query_transfer
buy_ticket
query_order
refund_ticket

clean
exit
*/