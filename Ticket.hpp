#include "include.hpp"
#include "Time.hpp"
#include "Date.hpp"

#ifndef _Ticket
#define _Ticket

using namespace std;

class Ticket{
public:
	char train[25], station_s[70], station_t[70];
	Time departtime, arrivetime;
	int price, seat;
	Ticket(){}
	Ticket(const char* _train, const char* _station_s, const char* _station_t, 
			Time _departtime, Time _arrivetime, int _price, int _seat){
				strcpy(train, _train);
				strcpy(station_s, _station_s);
				strcpy(station_t, _station_t);
				departtime = _departtime;
				arrivetime = _arrivetime;
				price = _price; seat = _seat;
			}
};

#endif