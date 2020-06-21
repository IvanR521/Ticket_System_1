#include "include.hpp"
#include "Users.hpp"
#include "Trains.hpp"
#include "Orders.hpp"

#ifndef _Connector
#define _Connector

using namespace std;

class Connector{
public:
	Users user_controller;
	Trains train_controller;
	Orders order_controller;

	void init();
	void work();
	void add_user(string str);
	void login(string str);
	void logout(string str);
	void query_profile(string str);
	void modify_profile(string str);
	void add_train(string str);
	void release_train(string str);
	void query_train(string str);
	void delete_train(string str);
	void query_ticket(string str);
	void query_transfer(string str);
	void buy_ticket(string str);
	void query_order(string str);
	void refund_ticket(string str);
	void clean();
	void exit();
	int split(string str, char c, string *&ans);
	int read_int(string str);
	Time read_time(string str);
	Date read_date(string str);
};

#endif