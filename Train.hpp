#include "include.hpp"
#include "Time.hpp"
#include "Date.hpp"

#ifndef _Train
#define _Train

class train{
public:
	char trainid[25];
	int stationnum;
	char stations[105][35];
	int seatnum, prices[105];
	Time starttime;
	int traveltimes[105], stopovertimes[105];
	Date begindate, enddate;
	char type;
	bool isreleased;
	train(){}
	train(const char* _trainid, int _stationnum, int _seatnum, 
			const char _stations[][35], const int _prices[], Time _starttime, 
			const int _traveltimes[], const int _stopovertimes[], 
			Date _begindate, Date _enddate, const char _type){

				strcpy(trainid, _trainid);
				stationnum = _stationnum;
				starttime = _starttime;
				seatnum = _seatnum;
				prices[0] = traveltimes[0] = stopovertimes[0] = stopovertimes[stationnum] = 0;
				for(int i = 0; i < stationnum; i++){
					strcpy(stations[i], _stations[i]);
					if(i > 0 && i + 1 < stationnum) stopovertimes[i] = _stopovertimes[i - 1];
					if(i + 1 < stationnum){
						prices[i + 1] = _prices[i] + prices[i];
						traveltimes[i + 1] = _traveltimes[i] + stopovertimes[i] + traveltimes[i];
					}
				}
				begindate = _begindate; enddate = _enddate;
				type = _type;
				isreleased = 0;
	}
};

#endif
