#include "include.hpp"
#include "Train.hpp"

#ifndef _Station
#define _Station

using namespace std;

class Station{
public:
	char stationname[35];
	int traincnt;
	Station(){traincnt = 0;}
	Station(const char * _stationname){
		traincnt = 0;
		strcpy(stationname, _stationname);
	}
};

#endif
