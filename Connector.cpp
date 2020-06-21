#include "include.hpp"
#include "mycode.hpp"

#define mp make_pair

using namespace std;


void Connector :: init(){
			//cout << "size before0 == " << user_controller.bpuser.sz << " " << train_controller.bptrain.sz << endl;
	user_controller.init(this);
	train_controller.init(this);
	order_controller.init(this);
			//cout << "size before1 == " << user_controller.bpuser.sz << " " << train_controller.bptrain.sz << endl;
}

void Connector :: work() {
	
	using std::string;
	using std::cin;

	string str;
	
	while(1) {
		// cout << "bpstation.sz == " << train_controller.bpstation.sz << endl;
		//cout << "off_t == " << train_controller.bptrain.find2(mp(1324255, 9918694)) << endl;
		//train_controller.print_train(train_controller.bptrain.find(mp(1324255, 9918694)));
		//cout << sizeof(off_t) << endl;
		while(isspace(cin.peek()))
			cin.ignore();
		//cout << sizeof(off_t) << endl;
		getline(cin, str);
		//cout << sizeof(off_t) << endl;
		
		if( str.substr(0, 8) == "add_user" ) 
			add_user(str);
		else if( str.substr(0, 5) == "login" )
			login(str);
		else if( str.substr(0, 6) == "logout" )
			logout(str);
		else if( str.substr(0, 13) == "query_profile" )
			query_profile(str);
		else if( str.substr(0, 14) == "modify_profile" )
			modify_profile(str);
		else if( str.substr(0, 9) == "add_train" )
			add_train(str);
		else if( str.substr(0, 13) == "release_train" )
			release_train(str);
		else if( str.substr(0, 11) == "query_train" )
			query_train(str);
		else if( str.substr(0, 12) == "delete_train" )
			delete_train(str);
		else if( str.substr(0, 12) == "query_ticket" )
			query_ticket(str);
		else if( str.substr(0, 14) == "query_transfer" )
			query_transfer(str);
		else if( str.substr(0, 10) == "buy_ticket" )
			buy_ticket(str);
		else if( str.substr(0, 11) == "query_order" )
			query_order(str);
		else if( str.substr(0, 13) == "refund_ticket" )
			refund_ticket(str);
		else if( str.substr(0, 5) == "clean" ){
			//cout << "size before2 == " << user_controller.bpuser.sz << " " << train_controller.bptrain.sz << endl;
			clean();
			//cout << "size after == " << user_controller.bpuser.sz << " " << train_controller.bptrain.sz << endl;
		}
		else if( str.substr(0, 4) == "exit" ){
			exit();
			break;
		}
		else assert(0);
	}
}

void Connector :: add_user(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	//printf("argc == %d\n", argc);
	string c, u, p, n, m;
	int g;
	//for(int i = 0; i < argc; i++)
	//	cout << argv[i] << endl;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-c") c = argv[i + 1];
		else if(argv[i] == "-u") u = argv[i + 1];
		else if(argv[i] == "-p") p = argv[i + 1];
		else if(argv[i] == "-n") n = argv[i + 1];
		else if(argv[i] == "-m") m = argv[i + 1];
		else if(argv[i] == "-g") g = read_int(argv[i + 1]);
		else assert(0);
	}
	//cout << c << " " << u << " " << p << " " << n << " " << m << " " << g << endl;
	user_controller.add_user(c.c_str(), u.c_str(), p.c_str(), n.c_str(), m.c_str(), g);

	/*puts("---------000--------");
	user_controller.print_user(user_controller.bpuser.find(make_pair(4955454, 10011129)));
	puts("---------111--------");*/

	delete []argv;
}

void Connector :: login(string str){

	/*puts("---------000--------");
	user_controller.print_user(user_controller.bpuser.find(make_pair(4955454, 10011129)));
	puts("---------111--------");*/

	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string u, p;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-u") u = argv[i + 1];
		else if(argv[i] == "-p") p = argv[i + 1];
		else assert(0);
	}
	//cout << u << " " << p << endl;
	user_controller.login(u.c_str(), p.c_str());
	delete []argv;
}

void Connector :: logout(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string u;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-u") u = argv[i + 1];
		else assert(0);
	}
	user_controller.logout(u.c_str());
	delete []argv;
}

void Connector :: query_profile(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string c, u;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-c") c = argv[i + 1];
		else if(argv[i] == "-u") u = argv[i + 1];
		else assert(0);
	}
	//cout << c << " " << u << endl;
	user_controller.query_profile(c.c_str(), u.c_str());
	delete []argv;
}

void Connector :: modify_profile(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string c, u, p, n, m;
	int g = -1;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-c") c = argv[i + 1];
		else if(argv[i] == "-u") u = argv[i + 1];
		else if(argv[i] == "-p") p = argv[i + 1];
		else if(argv[i] == "-n") n = argv[i + 1];
		else if(argv[i] == "-m") m = argv[i + 1];
		else if(argv[i] == "-g") g = read_int(argv[i + 1]);
		else assert(0);
	}
	//cout << "-c=" << c << " -u=" << u << " -p=" << p << " -n=" << n << " -m=" << m << " -g=" << g << endl;
	user_controller.modify_profile(c.c_str(), u.c_str(), p.c_str(), n.c_str(), m.c_str(), g);
	delete []argv;
}

void Connector :: add_train(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string ii, s, p, t, o, d, y;
	int n, m;
	Time x;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-i") ii = argv[i + 1];
		else if(argv[i] == "-s") s = argv[i + 1];
		else if(argv[i] == "-p") p = argv[i + 1];
		else if(argv[i] == "-t") t = argv[i + 1];
		else if(argv[i] == "-o") o = argv[i + 1];
		else if(argv[i] == "-d") d = argv[i + 1];
		else if(argv[i] == "-y") y = argv[i + 1];
		else if(argv[i] == "-n") n = read_int(argv[i + 1]);
		else if(argv[i] == "-m") m = read_int(argv[i + 1]);
		else if(argv[i] == "-x") x = read_time(argv[i + 1]);
		else assert(0);
	}

	Date begindate = read_date(d.substr(0, 5));
	Date enddate = read_date(d.substr(6, 5));

	char stations[105][35];
	int prices[105], traveltimes[105], stopovertimes[105];

	string *tmp = nullptr;
	int cnt;

	cnt = split(s, '|', tmp);
	for(int i = 0; i < cnt; i++)
		strcpy(stations[i], tmp[i].c_str());
	delete []tmp;

	cnt = split(p, '|', tmp);
	for(int i = 0; i < cnt; i++)
		prices[i] = read_int(tmp[i]);
	delete []tmp;

	cnt = split(t, '|', tmp);
	for(int i = 0; i < cnt; i++)
		traveltimes[i] = read_int(tmp[i]);
	delete []tmp;

	if(n > 2){
		cnt = split(o, '|', tmp);
		for(int i = 0; i < cnt; i++)
			stopovertimes[i] = read_int(tmp[i]);
		delete []tmp;
	}
	
	/*cout << ii << " " << n << " " << m << endl;
	for(int i = 0; i < n; i++)
		cout << stations[i] << " ";
	cout << endl;
	for(int i = 0; i < n - 1; i++)
		cout << prices[i] << " ";
	cout << endl;
	cout << x << endl;
	for(int i = 0; i < n - 1; i++)
		cout << traveltimes[i] << " ";
	cout << endl;
	for(int i = 0; i < n - 2; i++)
		cout << stopovertimes[i] << " ";
	cout << endl;
	cout << begindate << " " << enddate << " " << y << endl;*/

	train_controller.add_train(ii.c_str(), n, m, stations, prices, x, 
							   traveltimes, stopovertimes, begindate, enddate, y.c_str()[0]);
	delete []argv;
}

void Connector :: release_train(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string ii;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-i") ii = argv[i + 1];
		else assert(0);
	}
	train_controller.release_train(ii.c_str());
	delete []argv;
}

void Connector :: query_train(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string ii;
	Date d;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-i") ii = argv[i + 1];
		else if(argv[i] == "-d") d = read_date(argv[i + 1]);
		else assert(0);
	}
	train_controller.query_train(ii.c_str(), d);
	delete []argv;
}

void Connector :: delete_train(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string ii;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-i") ii = argv[i + 1];
		else assert(0);
	}
	train_controller.delete_train(ii.c_str());
	delete []argv;
}

void Connector :: query_ticket(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string s, t, p = "time";
	Date d;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-s") s = argv[i + 1];
		else if(argv[i] == "-t") t = argv[i + 1];
		else if(argv[i] == "-p") p = argv[i + 1];
		else if(argv[i] == "-d") d = read_date(argv[i + 1]);
		else assert(0);
	}
//	cout << "s = " << s << " " << t << " " << d << " " << p << endl;
//	cout << "l = " << s.length() << " " << t.length() << " " << p.length() << endl;
	train_controller.query_ticket(s.c_str(), t.c_str(), d, p.c_str());
	delete []argv;
}

void Connector :: query_transfer(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string s, t, p = "time";
	Date d;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-s") s = argv[i + 1];
		else if(argv[i] == "-t") t = argv[i + 1];
		else if(argv[i] == "-p") p = argv[i + 1];
		else if(argv[i] == "-d") d = read_date(argv[i + 1]);
		else assert(0);
	}
	train_controller.query_transfer(s.c_str(), t.c_str(), d, p.c_str());
	delete []argv;
}

void Connector :: buy_ticket(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string u, ii, f, t, q = "false";
	Date d;
	int n;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-u") u = argv[i + 1];
		else if(argv[i] == "-i") ii = argv[i + 1];
		else if(argv[i] == "-f") f = argv[i + 1];
		else if(argv[i] == "-t") t = argv[i + 1];
		else if(argv[i] == "-q") q = argv[i + 1];
		else if(argv[i] == "-d") d = read_date(argv[i + 1]);
		else if(argv[i] == "-n") n = read_int(argv[i + 1]);
		else assert(0);
	}
	order_controller.buy_ticket(u.c_str(), ii.c_str(), d, n, f.c_str(), t.c_str(), q.c_str());
	delete []argv;
}

void Connector :: query_order(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string u;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-u") u = argv[i + 1];
		else assert(0);
	}
	order_controller.query_order(u.c_str());
	delete []argv;
}

void Connector :: refund_ticket(string str){
	string *argv = nullptr;
	int argc = split(str, ' ', argv);
	string u;
	int n = 1;
	for(int i = 1; i < argc; i += 2 ) {
		if(argv[i] == "-u") u = argv[i + 1];
		else if(argv[i] == "-n") n = read_int(argv[i + 1]);
		else assert(0);
	}
	order_controller.refund_ticket(u.c_str(), n);
	delete []argv;
}

void Connector :: clean(){
	user_controller.bpuser.clear();
	train_controller.bpseat.clear();
	train_controller.bpstation.clear();
	train_controller.bpstrain.clear();
	train_controller.bptrain.clear();
	order_controller.bpuorder.clear();
	order_controller.bptorder.clear();
}

void Connector :: exit(){
	//puts("------------exit-----------");
	vector<pair<pair<int, int>, user> > user_list;
	user_list.clear();
	user_controller.bpuser.getall(user_list);
	int sz = user_list.size();
	//cout << "sz == " << sz << endl;
	for(int i = 0; i < sz; i++){
		user this_user = user_list[i].second;
		if(this_user.islogin){
			//user_controller.logout(this_user.username);
			this_user.islogin = 0;
			user_controller.bpuser.change(user_list[i].first, this_user);
		}
		// if(!strcmp(this_user.username, "Chen"))
		// 	printf("islogin = %d\n", user_controller.bpuser.find(user_list[i].first).islogin);
	}
	puts("bye");
}

int Connector :: split(string str, char c, string *&ans){
	if(c == ' '){
		int cnt = 0;
		stringstream ss(str);
		string tmp;
		while(ss >> tmp) cnt++;
		ans = new string [cnt];
		ss = stringstream(str);
		for(int i = 0; i < cnt; i++)
			ss >> ans[i];
		return cnt;
	}
	else{
		int cnt = 1;
		for(int i = 0; i < (int)str.length(); i++)
			if(str[i] == c)
				++cnt;
		ans = new std::string[cnt];
		int now = 0;
		for(int i = 0; i < (int)str.length(); i++) {
			if(str[i] == c) ++now;
			else ans[now].push_back( str[i] );
		}
		return cnt;
	}
}

int Connector :: read_int(string str){
	int x = 0;
	for(int i = 0; i < (int)str.length(); i++)
		x = x * 10 + str[i] - '0';
	return x;
}

Time Connector :: read_time(string str){
	int hr = (str[0] - '0') * 10 + str[1] - '0';
	int mi = (str[3] - '0') * 10 + str[4] - '0';
	return Time(Date(), hr, mi);
}

Date Connector :: read_date(string str){
	//cout << "read_date" << str << endl;
	int mo = (str[0] - '0') * 10 + str[1] - '0';
	int da = (str[3] - '0') * 10 + str[4] - '0';
	//cout << mo << "-" << da << endl;
	return Date(mo, da);
}
