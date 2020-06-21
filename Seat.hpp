#include "include.hpp"

#ifndef _Seat
#define _Seat

using namespace std;

class Seat{
public:
	int ordercnt[95];
	int seat[95][105];
	Seat(int daytime = 0, int stationnum = 0, int seatnum = 0){
		for(int i = 0; i < daytime; i++){
			ordercnt[i] = 0;
			for(int j = 0; j < stationnum; j++)
				seat[i][j] = seatnum;
		}
	}
	int * operator [] (const int & x){
		return seat[x];
	}
};

#endif