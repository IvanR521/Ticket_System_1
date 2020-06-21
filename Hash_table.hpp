#include "include.hpp"

#ifndef _Hash_table
#define _Hash_table

using namespace std;

class Hash_table{
public:
	static const int base1 = 233, base2 = 2333, mod1 = 19260817, mod2 = 19491001;
	Hash_table(){}
	pair<int, int> hash(const char* s){
		int h1 = 0, h2 = 0, l = strlen(s);
		for(int i = 0; i < l; i++){
			h1 = (int)((1ll * h1 * base1 + s[i]) % mod1);
			h2 = (int)((1ll * h2 * base2 + s[i]) % mod2);
		}
		return make_pair(h1, h2);
	}
};

#endif