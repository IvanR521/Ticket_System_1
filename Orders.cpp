#include "include.hpp"
#include "mycode.hpp"

#define mp make_pair

using namespace std;

//Orders :: Orders(){}

void Orders :: init(Connector *_connector){
	connector = _connector;
	bpuorder.init("bpuorder_file", "bpuorder_disk");
	bptorder.init("bptorder_file", "bptorder_disk");
}

void Orders :: buy_ticket(const char *u, const char *i, Date d, int n, const char *f, const char *t, const char *q){
	pair<int, int> hash_u = Hash.hash(u), hash_i = Hash.hash(i), hash_f = Hash.hash(f), hash_t = Hash.hash(t);
	if(!(connector -> user_controller.bpuser.exist(hash_u))){puts("-1"); return ;}
	if(!(connector -> train_controller.bptrain.exist(hash_i))){puts("-1"); return ;}
	if(!(connector -> train_controller.bpstation.exist(hash_f))){puts("-1"); return ;}
	if(!(connector -> train_controller.bpstation.exist(hash_t))){puts("-1"); return ;}
	if(strcmp(q, "false") && strcmp(q, "true")){puts("-1"); return ;}
	user this_user = connector -> user_controller.bpuser.find(hash_u);
	if(!this_user.islogin){puts("-1"); return ;}
	train this_train = connector -> train_controller.bptrain.find(hash_i);
	if(!this_train.isreleased){puts("-1"); return ;}
	if(n > this_train.seatnum){puts("-1"); return ;}
	Station be = connector -> train_controller.bpstation.find(hash_f);
	Station en = connector -> train_controller.bpstation.find(hash_t);
	int be_cnt = be.traincnt, en_cnt = en.traincnt;
	int be_pos = -1, en_pos = -1;
	for(int i = 1; i <= be.traincnt; i++)
		if(hash_i == connector -> train_controller.bpstrain.find(mp(hash_f, i)).first){
			be_pos = connector -> train_controller.bpstrain.find(mp(hash_f, i)).second;
			break;
		}
	for(int i = 1; i <= en.traincnt; i++)
		if(hash_i == connector -> train_controller.bpstrain.find(mp(hash_t, i)).first){
			en_pos = connector -> train_controller.bpstrain.find(mp(hash_t, i)).second;
			break;
		}
	if((be_pos == -1) || (en_pos == -1) || (en_pos <= be_pos)){puts("-1"); return ;}
	Time tim_be = this_train.starttime; tim_be.date = d;
	tim_be = tim_be + (this_train.traveltimes[be_pos] + this_train.stopovertimes[be_pos]);
	int dd = (d - (tim_be.date - d)) - this_train.begindate;
	if(((d - (tim_be.date - d)) < this_train.begindate) || (this_train.enddate < (d - (tim_be.date - d))))
		{puts("-1"); return ;}
	tim_be.date = d;
	Time tim_en = tim_be + (this_train.traveltimes[en_pos] - 
					(this_train.traveltimes[be_pos] + this_train.stopovertimes[be_pos]));
	Ticket ticket = Ticket(i, f, t, tim_be, tim_en, this_train.prices[en_pos] - this_train.prices[be_pos], n);
	char status[25];
	Seat seat = connector -> train_controller.bpseat.find(hash_i);
	int mn = 0x7fffffff;
	// puts("----------Buying----------");
	// printf("need == %d\n", n);
	for(int i = be_pos; i < en_pos; i++){
		// printf("seat[%d][%d] == %d\n", dd, i, seat.seat[dd][i]);
		mn = min(mn, seat.seat[dd][i]);
	}
	if(mn >= n){
		for(int i = be_pos; i < en_pos; i++)
			seat.seat[dd][i] -= n;
		// puts("--------After-buying-------");
		// for(int i = 0; i < this_train.stationnum - 1; i++)
		// 	printf("seat[%d][%d] == %d\n", dd, i, seat.seat[dd][i]);
		strcpy(status, "success");
		connector -> train_controller.bpseat.change(hash_i, seat);
	}
	else{
		if(!strcmp(q, "true")) strcpy(status, "pending");
		else {puts("-1"); return ;}
	}
	this_user.ordercnt++;
	connector -> user_controller.bpuser.change(hash_u, this_user);
	if(!strcmp(status, "pending")){
		seat.ordercnt[dd]++;
		connector -> train_controller.bpseat.change(hash_i, seat);
		connector -> train_controller.bptrain.change(hash_i, this_train);
		Order order = Order(status, u, this_user.ordercnt, i, dd, seat.ordercnt[dd], ticket);
		bptorder.insert(mp(mp(hash_i, dd), seat.ordercnt[dd]), order);
		bpuorder.insert(mp(hash_u, this_user.ordercnt), order);
		puts("queue");
	}
	else{
		Order order = Order(status, u, this_user.ordercnt, i, dd, 0, ticket);
		bpuorder.insert(mp(hash_u, this_user.ordercnt), order);
		printf("%d\n", ticket.price * ticket.seat);
	}
	return ;
}

void Orders :: query_order(const char *u){
	pair<int, int> hash_u = Hash.hash(u);
	if(!(connector -> user_controller.bpuser.exist(hash_u))){puts("-1"); return ;}
	user this_user = connector -> user_controller.bpuser.find(hash_u);
	if(!this_user.islogin){puts("-1"); return ;}
	printf("%d\n", this_user.ordercnt);
	for(int i = this_user.ordercnt; i >= 1; i--){
		Order order = bpuorder.find(mp(hash_u, i));
		printf("[%s] %s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d\n", order.status, order.ticket.train, 
				order.ticket.station_s, order.ticket.departtime.date.month, order.ticket.departtime.date.day, 
				order.ticket.departtime.hour, order.ticket.departtime.minute,
				order.ticket.station_t, order.ticket.arrivetime.date.month, order.ticket.arrivetime.date.day, 
				order.ticket.arrivetime.hour, order.ticket.arrivetime.minute, order.ticket.price, order.ticket.seat);
	}
	return ;
}

void Orders :: refund_ticket(const char *u, int n){
	pair<int, int> hash_u = Hash.hash(u);
	if(!(connector -> user_controller.bpuser.exist(hash_u))){puts("-1"); return ;}
	user this_user = connector -> user_controller.bpuser.find(hash_u);
	if(!this_user.islogin){puts("-1"); return ;}
	if(this_user.ordercnt < n){puts("-1"); return ;}
	Order order = bpuorder.find(mp(hash_u, this_user.ordercnt - n + 1));
	pair<int, int> hash_i = Hash.hash(order.trainid);
	train this_train = connector -> train_controller.bptrain.find(hash_i);
	if(!strcmp(order.status, "refunded")){puts("-1"); return ;}
	else if(!strcmp(order.status, "pending")){
		strcpy(order.status, "refunded");
		bpuorder.change(mp(hash_u, order.iduser), order);
		bptorder.change(mp(mp(hash_u, order.dday), order.idtrain), order);
		puts("0");
	}
	else{
		strcpy(order.status, "refunded");
		Seat seat = connector -> train_controller.bpseat.find(hash_i);
		Station be = connector -> train_controller.bpstation.find(Hash.hash(order.ticket.station_s));
		Station en = connector -> train_controller.bpstation.find(Hash.hash(order.ticket.station_t));
		int be_cnt = be.traincnt, en_cnt = en.traincnt;
		int be_pos = -1, en_pos = -1;
		for(int i = 1; i <= be.traincnt; i++)
			if(hash_i == connector -> train_controller.bpstrain.find(mp(Hash.hash(order.ticket.station_s), i)).first){
				be_pos = connector -> train_controller.bpstrain.find(mp(Hash.hash(order.ticket.station_s), i)).second;
				break;
			}
		for(int i = 1; i <= en.traincnt; i++)
			if(hash_i == connector -> train_controller.bpstrain.find(mp(Hash.hash(order.ticket.station_t), i)).first){
				en_pos = connector -> train_controller.bpstrain.find(mp(Hash.hash(order.ticket.station_t), i)).second;
				break;
			}
		// puts("---------Before-refunding---------");
		// for(int i = 0; i < this_train.stationnum; i++)
		// 	printf("seat[%d][%d] == %d\n", order.dday, i, seat.seat[order.dday][i]);
		// puts("---------Refunding---------");
		for(int i = be_pos; i < en_pos; i++){
			seat.seat[order.dday][i] += order.ticket.seat;
			// printf("seat[%d][%d] == %d\n", order.dday, i, seat.seat[order.dday][i]);
		}
		bpuorder.change(mp(hash_u, order.iduser), order);
		bptorder.change(mp(mp(hash_u, order.dday), order.idtrain), order);
		// cout << "ordercnt == " << seat.ordercnt[order.dday] << endl;
		for(int i = 1; i <= seat.ordercnt[order.dday]; i++){
			Order cur_order = bptorder.find(mp(mp(hash_i, order.dday), i));
			// cout << "i = " << i << " " << cur_order.ticket.seat << endl;
			if(strcmp(cur_order.status, "pending")) continue;
			Station cur_be = connector -> train_controller.bpstation.find(Hash.hash(cur_order.ticket.station_s));
			Station cur_en = connector -> train_controller.bpstation.find(Hash.hash(cur_order.ticket.station_t));
			int cur_be_cnt = cur_be.traincnt, cur_en_cnt = cur_en.traincnt;
			int cur_be_pos = -1, cur_en_pos = -1;
			for(int i = 1; i <= cur_be.traincnt; i++)
				if(hash_i == connector -> train_controller.bpstrain.find(mp(Hash.hash(cur_order.ticket.station_s), i)).first){
					cur_be_pos = connector -> train_controller.bpstrain.find(mp(Hash.hash(cur_order.ticket.station_s), i)).second;
					break;
				}
			for(int i = 1; i <= cur_en.traincnt; i++)
				if(hash_i == connector -> train_controller.bpstrain.find(mp(Hash.hash(cur_order.ticket.station_t), i)).first){
					cur_en_pos = connector -> train_controller.bpstrain.find(mp(Hash.hash(cur_order.ticket.station_t), i)).second;
					break;
				}
			int mn = 0x7fffffff;
			for(int i = cur_be_pos; i < cur_en_pos; i++)
				mn = min(mn, seat.seat[order.dday][i]);
			if(mn >= cur_order.ticket.seat){
				strcpy(cur_order.status, "success");
				for(int i = cur_be_pos; i < cur_en_pos; i++)
					seat.seat[order.dday][i] -= cur_order.ticket.seat;
				bpuorder.change(mp(Hash.hash(cur_order.username), cur_order.iduser), cur_order);
				bptorder.change(mp(mp(Hash.hash(cur_order.trainid), cur_order.dday), cur_order.idtrain), cur_order);
			}
		}
		// puts("---------After-refunding---------");
		// for(int i = 0; i < this_train.stationnum; i++)
		// 	printf("seat[%d][%d] == %d\n", order.dday, i, seat.seat[order.dday][i]);
		connector -> train_controller.bpseat.change(hash_i, seat);
		puts("0");
	}
}
