#include "include.hpp"
#include "Time.hpp"
#include "Hash_table.hpp"
#include "Date.hpp"
#include "Ticket.hpp"
#include "Train.hpp"
#include "Seat.hpp"
#include "Station.hpp"

#ifndef _Trains
#define _Trains

using namespace std;

class Connector;

class Trains{

public:


	Connector *connector;

	//char bptrain_file[25], bptrain_disk[25], bpseat_file[25], bpseat_disk[25], bpstation_file[25], bpstation_disk[25];

	Bplustree<pair<int, int>, train> bptrain;

	Bplustree<pair<int, int>, Seat> bpseat;

	Bplustree<pair<int, int>, Station> bpstation;

	Bplustree<pair<pair<int, int>, int >, pair<pair<int, int>, int> > bpstrain;

	Hash_table Hash;

	//Trains();

	void init(Connector *_connector);

	void print_station(Station s);

	void print_train(train t);

	void add_train(const char* _trainid, int _stationnum, int _seatnum, 
			const char _stations[][35], const int *_prices, Time _starttime, 
			const int *_traveltimes, const int *_stopovertimes, 
			Date _begindate, Date _enddate, const char _type);

	void release_train(const char* i);
	
	void query_train(const char *i, Date d);

	void delete_train(const char* i);

	void query_ticket(const char* s, const char* t, Date d, const char* p/*, int h = 0, int m = 0*/);

	Ticket get_ticket(const char* s, const char* t, Date d, const char* p, const char *ii, int h = 0, int m = 0);

	void query_transfer(const char* s, const char* t, Date d, const char* p);

	//static bool cmp_time(Ticket a, Ticket b);
	//static bool cmp_cost(Ticket a, Ticket b);

};

#endif
