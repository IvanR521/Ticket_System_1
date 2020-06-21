#include "include.hpp"
#include "Trains.hpp"
#include "Users.hpp"
#include "Ticket.hpp"
#include "Order.hpp"

#ifndef _Orders
#define _Orders

class Connector;

class Orders{
public:
	Connector *connector;

	//char bpuorder_file[25], bpuorder_disk[25], bptorder_file[25], bptorder_disk[25];

	Bplustree <pair<pair<int, int>, int>, Order> bpuorder;

	Bplustree <pair<pair<pair<int, int>, int>, int>, Order> bptorder;

	Hash_table Hash;

	//Orders();

	void init(Connector *_connector);

	void buy_ticket(const char *u, const char *i, Date d, int n, const char *f, const char *t, const char *q);

	void query_order(const char *u);

	void refund_ticket(const char *u, int n);

};

#endif