#include "include.hpp"

#include "Date.hpp"

#ifndef _Time
#define _Time

using namespace std;

class Time{

public:

	Date date;
	int hour, minute;
	Time(Date _date = Date(), int _hour = 0, int _minute = 0){
		date = _date; hour = _hour; minute = _minute;
	}
	Time operator + (const int x){
		Time cur = *this;
		cur.minute += x;
		cur.hour += cur.minute / 60;
		cur.minute %= 60;
		cur.date = cur.date + cur.hour / 24;
		cur.hour %= 24;
		return cur;
	}
	bool operator < (const Time & other) const{
		if(date < other.date) return 1;
		if(other.date < date) return 0;
		if(hour != other.hour) return hour < other.hour;
		return minute < other.minute;
	}
	friend ostream & operator << (ostream & os, Time time){
		os << time.date << ' '
		   << setw(2) << setfill('0') << time.hour << ':'
		   << setw(2) << setfill('0') << time.minute; 
		return os;
	}
	
	int operator - (const Time other){
		return (date - other.date) * 1440 + 
			   (minute + 60 * hour) - (other.minute + 60 * other.hour);
	}
};


#endif