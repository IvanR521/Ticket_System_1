#include "include.hpp"
#include "mycode.hpp"

using namespace std;

//Users :: Users(){}

void Users :: init(Connector *_connector){
	bpuser.init("bpuser_file", "bpuser_disk");
}

void Users :: print_user(user _user){
	printf("%s %s %s %d\n", _user.username, _user.name, _user.mailaddr, _user.privilege);
	//  printf("%s %s %s %s %d %d %d\n", _user.username, _user.password, _user.name, _user.mailaddr, _user.privilege, 
	//  		_user.islogin, _user.ordercnt);
}

void Users :: add_user(const char *c, const char *u, const char *p, const char *n, const char *m, const int g){
	pair<int, int> hash_c = Hash_table().hash(c), hash_u = Hash_table().hash(u);
	//printf("<%d, %d>\n", hash_u.first, hash_u.second);
	if(bpuser.exist(hash_u)){puts("-1"); return ;}
	if(bpuser.empty()){
		user new_user(u, p, n, m, 10, 0);
		//print_user(new_user);
		bpuser.insert(hash_u, new_user);
		puts("0"); return;
	}
	else{
		if(!bpuser.exist(hash_c)){puts("-1"); return ;}
		user cur_user = bpuser.find(hash_c);
		if((!cur_user.islogin) || (cur_user.privilege <= g)){puts("-1"); return ;}
		user new_user(u, p, n, m, g, 0);
		bpuser.insert(hash_u, new_user);
		puts("0"); return;
	}
}

void Users :: login(const char *u, const char *p){
	pair<int, int> hash_u= Hash_table().hash(u);
	//printf("<%d, %d>\n", hash_u.first, hash_u.second);
	if(!bpuser.exist(hash_u)){puts("-1"); return ;}
	user cur_user = bpuser.find(hash_u);
	if(cur_user.islogin || strcmp(p, cur_user.password)){puts("-1"); return ;}
	cur_user.islogin = 1;
	bpuser.change(hash_u, cur_user);
	puts("0"); return;
}

void Users :: logout(const char *u){
	pair<int, int> hash_u= Hash_table().hash(u);
	if(!bpuser.exist(hash_u)){puts("-1"); return ;}
	user cur_user = bpuser.find(hash_u);
	if((!cur_user.islogin)){puts("-1"); return ;}
	cur_user.islogin = 0;
	bpuser.change(hash_u, cur_user);
	puts("0"); return;
}

void Users :: query_profile(const char *c, const char *u){
	pair<int, int> hash_c = Hash_table().hash(c), hash_u = Hash_table().hash(u);
	if((!bpuser.exist(hash_c)) || (!bpuser.exist(hash_u))) {puts("-1"); return ;}
	user cur_user = bpuser.find(hash_c), query_user = bpuser.find(hash_u);
	/*puts("--------query-begin--------");
	print_user(cur_user);
	print_user(query_user);
	puts("--------query-end--------");*/
	if(!cur_user.islogin){puts("-1"); return ;}
	if((!strcmp(c, u)) || (cur_user.privilege > query_user.privilege)){
		print_user(query_user);
		return; 
	}
	puts("-1");
	return ;
}

void Users :: modify_profile(const char *c, const char *u, const char *p, const char *n, const char *m, const int g){
	pair<int, int> hash_c = Hash_table().hash(c), hash_u = Hash_table().hash(u);
	/*puts("--------modify-begin--------");
	printf("<%d, %d>  <%d, %d> \n", hash_c.first, hash_c.second, hash_u.first, hash_u.second);
	puts("--------modify-end--------");*/
	if((!bpuser.exist(hash_c)) || (!bpuser.exist(hash_u))) {puts("-1"); return ;}
	user cur_user = bpuser.find(hash_c), modify_user = bpuser.find(hash_u);
	/*puts("--------modify-begin--------");
	print_user(cur_user);
	print_user(modify_user);
	puts("--------modify-end--------");*/
	if(!cur_user.islogin){puts("-1"); return ;}
	//printf("%d  %d  %d\n", cur_user.privilege, modify_user.privilege, g);
	if(((!strcmp(c, u)) || (cur_user.privilege > modify_user.privilege)) && (g < cur_user.privilege)){
		if(strcmp(p, "")) strcpy(modify_user.password, p);
		if(strcmp(n, "")) strcpy(modify_user.name, n);
		if(strcmp(m, "")) strcpy(modify_user.mailaddr, m);
		if(g != -1) modify_user.privilege = g;
		bpuser.change(hash_u, modify_user);
		print_user(modify_user);
		return; 
	}
	puts("-1");
	return ;
}



/*
add_user
login
logout
query_profile
modify_profile

add_train
release_train
query_train
delete_train

query_ticket
query_transfer
buy_ticket
query_order
refund_ticket

clean
exit
*/
