#include "include.hpp"

#ifndef _Date
#define _Date

using namespace std;

const int daynum[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class Date{

public:

	int year, month, day;
	Date(const int _month = 1, const int _day = 1, const int _year = 2020){
		year = _year; month = _month; day = _day;
	}
	int getday(int year, int month){
		if(month != 2) return daynum[month];
		if((year % 4) || (year % 100 == 0 && year % 400))
			return daynum[month];
		return daynum[month] + 1;
	}
	Date operator + (const int x){
		Date cur = *this;
		cur.day += x;
		while(cur.day > getday(cur.year, cur.month)){
			cur.day -= getday(cur.year, cur.month);
			cur.month++;
			if(cur.month == 13) {cur.year++; cur.month = 1;}
		}
		return cur;
	}
	Date operator - (const int x){
		Date cur = *this;
		cur.day -= x;
		while(cur.day < 1){
			cur.month--;
			if(cur.month == 0) {cur.year--; cur.month = 12;}
			cur.day += getday(cur.year, cur.month);
		}
		return cur;
	}
	int operator - (const Date other){
		Date a = *this, b = other;
		int fh = 1, ans = 0;
		if(a < b){ swap(a, b); fh = -1; }
		ans += a.day - getday(a.year, a.month);
		ans +=  getday(b.year, b.month) - b.day;
		while(a.month != b.month || a.year != b.year){
			ans += getday(a.year, a.month);
			a.month--;
			if(!a.month){a.month = 12; a.year--;}
		}
		return fh * ans;
	}
	bool operator < (const Date & other) const{
		if(year != other.year) return year < other.year;
		if(month != other.month) return month < other.month;
		return day < other.day;
	}
	friend ostream & operator << (ostream & os, Date date){
		os << setw(2) << setfill('0') << date.month << '-' 
		   << setw(2) << setfill('0') << date.day;
		return os;
	}
};

#endif