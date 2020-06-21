#include "include.hpp"
#include "mycode.hpp"

#define mp make_pair

//using namespace std;

//Trains :: Trains(){}

bool cmp_time(Ticket a, Ticket b){
	return (a.arrivetime - a.departtime) < (b.arrivetime - b.departtime);
}
bool cmp_cost(Ticket a, Ticket b){
	return a.price < b.price;
}

void Trains :: print_station(Station s){
	cout << s.stationname << " " << s.traincnt << endl;
}

void Trains :: print_train(train t){
	cout << t.trainid << " " << t.stationnum << " " << t.seatnum << endl;
	for(int i = 0; i < t.stationnum; i++)
		cout << t.stations[i] << " ";
	cout << endl;
	for(int i = 0; i < t.stationnum - 1; i++)
		cout << t.prices[i + 1] - t.prices[i] << " ";
	cout << endl;
	cout << t.starttime << endl;
	for(int i = 0; i < t.stationnum - 1; i++)
		cout << t.traveltimes[i + 1] - t.traveltimes[i] << " ";
	cout << endl;
	for(int i = 1; i < t.stationnum - 1; i++)
		cout << t.stopovertimes[i] << " ";
	cout << endl;
	cout << t.begindate << " " << t.enddate << " " << t.type << " " << t.isreleased << endl;
}

void Trains :: init(Connector *_connector){
	bptrain.init("bptrain_file", "bptrain_disk", "bptrain_disk1");
	bpseat.init("bpseat_file", "bpseat_disk", "bptseat_disk1");
	bpstation.init("bpstation_file", "bpstation_disk", "bpstation_disk1");
	bpstrain.init("bpstrain_file", "bpstrain_disk", "bpstrain_disk1");
}

void Trains :: add_train(const char* _trainid, int _stationnum, int _seatnum, 
			const char _stations[][70], const int *_prices, Time _starttime, 
			const int *_traveltimes, const int *_stopovertimes, 
			Date _begindate, Date _enddate, const char _type){
	pair<int, int> hash_t = Hash.hash(_trainid);
	if(bptrain.exist(hash_t)) {puts("-1"); return ;}
	train new_train(_trainid, _stationnum, _seatnum, _stations, _prices, 
					_starttime, _traveltimes, _stopovertimes, _begindate, _enddate, _type);
	bptrain.insert(hash_t, new_train);
	puts("0"); return ;
}

void Trains :: release_train(const char* i){
	pair<int, int> hash_t = Hash.hash(i);
	if(!bptrain.exist(hash_t)) {puts("-1"); return ;}
	train this_train = bptrain.find(hash_t);
	if(this_train.isreleased){puts("-1"); return ;}
	this_train.isreleased = 1;
	bptrain.change(hash_t, this_train);
	bpseat.insert(hash_t, Seat(this_train.enddate - this_train.begindate + 1, this_train.stationnum, this_train.seatnum));
	for(int i = 0; i < this_train.stationnum; i++){
		pair<int, int> hash_s = Hash.hash(this_train.stations[i]);
		if(!bpstation.exist(hash_s)){
			bpstation.insert(hash_s, Station(this_train.stations[i]));
		}
		Station station = bpstation.find(hash_s);
		station.traincnt++;
		bpstation.change(hash_s, station);
		bpstrain.insert(mp(hash_s, station.traincnt), mp(hash_t, i));
	}
	puts("0"); return ;
}

void Trains :: query_train(const char *i, Date d){
	pair<int, int> hash_t = Hash.hash(i);
	if(!bptrain.exist(hash_t)) {puts("-1"); return ;}
	train T = bptrain.find(hash_t);
	if((d < T.begindate) || (T.enddate < d)){puts("-1"); return ;}
	int dd = d - T.begindate;
	Seat rest;
	if(T.isreleased) rest = bpseat.find(hash_t);
	printf("%s %c\n", T.trainid, T.type);
	printf("%s xx-xx xx:xx -> %02d-%02d %02d:%02d 0 %d\n", T.stations[0], d.month, d.day, 
			T.starttime.hour, T.starttime.minute, T.isreleased ? rest.seat[dd][0] : T.seatnum);
	Time t = Time(d, T.starttime.hour, T.starttime.minute), n;
	for(int i = 1; i + 1 < T.stationnum; i++){
		n = t + T.traveltimes[i];
		printf("%s %02d-%02d %02d:%02d -> ", T.stations[i], n.date.month, n.date.day, n.hour, n.minute);
		n = n + T.stopovertimes[i];
		printf("%02d-%02d %02d:%02d %d %d\n", n.date.month, n.date.day, n.hour, n.minute, T.prices[i], 
												T.isreleased ? rest.seat[dd][i] : T.seatnum);
	}
	n = t + T.traveltimes[T.stationnum - 1];
	printf("%s %02d-%02d %02d:%02d -> xx-xx xx:xx %d x\n", 
			T.stations[T.stationnum - 1], n.date.month, n.date.day, n.hour, n.minute, T.prices[T.stationnum - 1]);
}

void Trains :: delete_train(const char* i){
	pair<int, int> hash_t = Hash.hash(i);
	if(!bptrain.exist(hash_t)) {puts("-1"); return ;}
	train this_train = bptrain.find(hash_t);
	if(this_train.isreleased){puts("-1"); return ;}
	bptrain.erase(hash_t);
	puts("0"); return ;
}

void Trains :: query_ticket(const char* s, const char* t, Date d, const char* p/*, int h = 0, int m = 0*/){
	puts("66666666");
	pair<int, int> hash_s = Hash.hash(s), hash_t = Hash.hash(t);
	if(!bpstation.exist(hash_s)) {puts("0"); return ;}
	if(!bpstation.exist(hash_t)) {puts("0"); return ;}
	if(hash_s == hash_t){puts("-1"); return ;}
	if(strcmp(p, "time") && strcmp(p, "cost")) {puts("-1"); return;}
	Station station_s = bpstation.find(hash_s), station_t = bpstation.find(hash_t);
	vector<Ticket> ticket_list;
	int sz = station_s.traincnt;
	for(int i = 1; i <= sz; i++){
		if(!bpstrain.exist(mp(hash_s, i))){puts("-1"); return ;}
		if(!bptrain.exist(bpstrain.find(mp(hash_s, i)).first)){puts("-1"); return ;}
		train this_train = bptrain.find(bpstrain.find(mp(hash_s, i)).first);
		int pos = bpstrain.find(mp(hash_s, i)).second;
		Time tim = Time(d, this_train.starttime.hour, this_train.starttime.minute)
					+ this_train.traveltimes[pos] + this_train.stopovertimes[pos];
		if(!bpseat.exist(Hash.hash(this_train.trainid))){puts("-1"); return ;}
		Seat rest = bpseat.find(Hash.hash(this_train.trainid));
		int dd = (d - (tim.date - d)) - this_train.begindate;
		if(((d - (tim.date - d)) < this_train.begindate) || (this_train.enddate < (d - (tim.date - d)))) continue;
		int min_seat = rest.seat[dd][pos];
		tim.date = d;
		for(int j = pos + 1; j < this_train.stationnum; j++){
			if(Hash.hash(this_train.stations[j]) == hash_t){
				Time timj = tim + (this_train.traveltimes[j] - (this_train.traveltimes[pos] + this_train.stopovertimes[pos]));
				//cout << tim << " " << timj << endl;
				ticket_list.push_back(Ticket(this_train.trainid, s, t, tim, timj, this_train.prices[j] - this_train.prices[pos], min_seat));
				break;
			}
			if(rest.seat[dd][j] < min_seat) min_seat = rest.seat[dd][j];
		}
	}
	if(p[0] == 't')
		sort(ticket_list.begin(), ticket_list.end(), cmp_time);
	else sort(ticket_list.begin(), ticket_list.end(), cmp_cost);
	sz = ticket_list.size();
	printf("%d\n", sz);
	for(int i = 0; i < sz; i++){
		Ticket this_t = ticket_list[i];
		printf("%s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d\n", this_t.train, 
				this_t.station_s, this_t.departtime.date.month, this_t.departtime.date.day, 
				this_t.departtime.hour, this_t.departtime.minute,
				this_t.station_t, this_t.arrivetime.date.month, this_t.arrivetime.date.day, 
				this_t.arrivetime.hour, this_t.arrivetime.minute, this_t.price, this_t.seat);
	}
	return ;
}

Ticket Trains :: get_ticket(const char* s, const char* t, Date d, const char* p, int h, int m){
	Ticket exp; exp.price = -1;
	pair<int, int> hash_s = Hash.hash(s), hash_t = Hash.hash(t);
	if(!bpstation.exist(hash_s)) return exp;
	if(!bpstation.exist(hash_t)) return exp;
	if(hash_s == hash_t)return exp;
	if(strcmp(p, "time") && strcmp(p, "cost")) return exp;
	Station station_s = bpstation.find(hash_s), station_t = bpstation.find(hash_t);
	vector<Ticket> ticket_list;
	int sz = station_s.traincnt;
	for(int i = 1; i <= sz; i++){
		train this_train = bptrain.find(bpstrain.find(mp(hash_s, i)).first);
		int pos = bpstrain.find(mp(hash_s, i)).second;
		Time tim = Time(d, this_train.starttime.hour, this_train.starttime.minute)
					+ this_train.traveltimes[pos] + this_train.stopovertimes[pos];
		if(this_train.enddate < (d - (tim.date - d))) continue;
		int dd = (d - (tim.date - d)) - this_train.begindate;
		if((d - (tim.date - d)) < this_train.begindate){
			tim = Time(this_train.begindate, this_train.starttime.hour, this_train.starttime.minute)
					+ this_train.traveltimes[pos] + this_train.stopovertimes[pos];
			dd = 0;
		}
		else tim.date = d;
		Seat rest = bpseat.find(Hash.hash(this_train.trainid));
		int min_seat = rest.seat[dd][pos];
		for(int j = pos + 1; j < this_train.stationnum; j++){
			if(Hash.hash(this_train.stations[j]) == hash_t){
				Time timj = tim + (this_train.traveltimes[j] - (this_train.traveltimes[pos] + this_train.stopovertimes[pos]));
				ticket_list.push_back(Ticket(this_train.trainid, s, t, tim, timj, this_train.prices[j] - this_train.prices[pos], min_seat));
				break;
			}
			if(rest.seat[dd][j] < min_seat) min_seat = rest.seat[dd][j];
		}
	}
	if(p[0] == 't')
		sort(ticket_list.begin(), ticket_list.end(), cmp_time);
	else sort(ticket_list.begin(), ticket_list.end(), cmp_cost);
	sz = ticket_list.size();
	if(!sz) return exp;
	return ticket_list[0];
}

void Trains :: query_transfer(const char* s, const char* t, Date d, const char* p){
	pair<int, int> hash_s = Hash.hash(s), hash_t = Hash.hash(t);
	if(!bpstation.exist(hash_s)) {puts("0"); return ;}
	if(!bpstation.exist(hash_t)) {puts("0"); return ;}
	// if(hash_s == hash_t){puts("-1"); return ;}
	if(strcmp(p, "time") && strcmp(p, "cost")) {puts("-1"); return;}
	Station station_s = bpstation.find(hash_s), station_t = bpstation.find(hash_t);
	vector<Ticket> ticket_list;
	int sz = station_s.traincnt;
	Ticket ct1, ct2, ft1, ft2; int ans = 0x7fffffff;
	for(int i = 1; i <= sz; i++){
		train this_train = bptrain.find(bpstrain.find(mp(hash_s, i)).first);
		int pos = bpstrain.find(mp(hash_s, i)).second;
		Time tim = Time(d, this_train.starttime.hour, this_train.starttime.minute)
					+ this_train.traveltimes[pos] + this_train.stopovertimes[pos];
		Seat rest = bpseat.find(Hash.hash(this_train.trainid));
		int dd = (d - (tim.date - d)) - this_train.begindate;
		if(((d - (tim.date - d)) < this_train.begindate) || (this_train.enddate < (d - (tim.date - d)))) continue;
		int min_seat = rest.seat[dd][pos];
		tim.date = d;
		for(int j = pos + 1; j < this_train.stationnum; j++){
			if(Hash.hash(this_train.stations[j]) != hash_t){
				Time timj = tim + (this_train.traveltimes[j] - (this_train.traveltimes[pos] + this_train.stopovertimes[pos]));
				ct1 = Ticket(this_train.trainid, s, this_train.stations[j], tim, timj, this_train.prices[j] - this_train.prices[pos], min_seat);
				ct2 = get_ticket(this_train.stations[j], t, timj.date, p, timj.hour, timj.minute);
				if(ct2.price == -1) continue;
				if(p[0] == 't'){
					if((ct2.arrivetime - ct1.departtime) < ans){
						ft1 = ct1; ft2 = ct2; 
						ans = ct2.arrivetime - ct1.departtime;
					}
				}
				else{
					if(ct1.price + ct2.price < ans){
						ft1 = ct1; ft2 = ct2;
						ans = ct1.price + ct2.price;
					}
				}
			}
			if(rest.seat[dd][j] < min_seat) min_seat = rest.seat[dd][j];
		}
	}
	if(ans == 0x7fffffff) {puts("0"); return ;}
	printf("%s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d\n", ft1.train, 
			ft1.station_s, ft1.departtime.date.month, ft1.departtime.date.day, 
			ft1.departtime.hour, ft1.departtime.minute,
			ft1.station_t, ft1.arrivetime.date.month, ft1.arrivetime.date.day, 
			ft1.arrivetime.hour, ft1.arrivetime.minute, ft1.price, ft1.seat);
	printf("%s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d\n", ft2.train, 
			ft2.station_s, ft2.departtime.date.month, ft2.departtime.date.day, 
			ft2.departtime.hour, ft2.departtime.minute,
			ft2.station_t, ft2.arrivetime.date.month, ft2.arrivetime.date.day, 
			ft2.arrivetime.hour, ft2.arrivetime.minute, ft2.price, ft2.seat);
	return ;
}