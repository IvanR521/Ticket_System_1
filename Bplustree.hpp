#include "disk_manager.hpp"
#include <vector>
#include <functional>
#include <iostream>
#include <assert.h>
#include <cstring>
#include <utility>

#ifndef BPLUSTREE
#define BPLUSTREE

template<class Key,class T,size_t ndsize=4096,class Compare = std::less<Key> >
class Bplustree
{
public:
	struct node;
	typedef char buffer[ndsize-sizeof(node)];
	typedef char* _buffer;
	//
	//bool FLAG=false;
	//
	const size_t order;
	const size_t entry;
	char *fname;
	FILE *file,*Tdata;
	char *Dname;
	disk_manager DISK;
	off_t Root,Head,Tail;
	Compare cmp=Compare();
	int sz;
	inline bool EQUAL(const Key &A,const Key &B){return !cmp(A,B)&&!cmp(B,A);}
	
// CONSTRUCTION------------------------------------------------------------------------------------


	struct node
	{
		off_t self,pre,nxt;
		int tp;
		size_t cnt;
		Key key;
		node(off_t Self=invalid_pos,int Tp=1,off_t Pre=invalid_pos,off_t Nxt=invalid_pos):
			self(Self),pre(Pre),nxt(Nxt),tp(Tp)
		{
			cnt=0;
			key=Key();
		}
	};
	inline void SAVE()
	{
		fseek(file,sizeof(int),SEEK_SET);
		fwrite(&Root,sizeof(off_t),1,file);
		fwrite(&Head,sizeof(off_t),1,file);
		fwrite(&Tail,sizeof(off_t),1,file);
		fflush(file);
	}
	inline void SAVE_NODE(const node &p)
	{
		fseek(file,p.self,SEEK_SET);
		fwrite(&p,sizeof(node),1,file);
		fflush(file);
	}
	inline node LOAD_NODE(off_t pos)
	{
		node p;
		fseek(file,pos,SEEK_SET);
		fread(&p,sizeof(node),1,file);
		return p;
	}
	inline void SAVE_LEAF_BUF(_buffer _buf,const node &p)
	{
		fseek(file,p.self+sizeof(node),SEEK_SET);
		fwrite(_buf,(sizeof(Key)+sizeof(off_t))*p.cnt,1,file);
		fflush(file);
	}
	inline void LOAD_LEAF_BUF(_buffer _buf,const node &p)
	{
		fseek(file,p.self+sizeof(node),SEEK_SET);
		fread(_buf,(sizeof(Key)+sizeof(off_t))*p.cnt,1,file);
	}
	inline void SAVE_NLEAF_BUF(_buffer _buf,const node &p)
	{
		fseek(file,p.self+sizeof(node),SEEK_SET);
		fwrite(_buf,(sizeof(Key)+sizeof(off_t))*p.cnt,1,file);
		fflush(file);
	}
	inline void LOAD_NLEAF_BUF(_buffer _buf,const node &p)
	{
		fseek(file,p.self+sizeof(node),SEEK_SET);
		fread(_buf,(sizeof(Key)+sizeof(off_t))*p.cnt,1,file);
	}
	inline void SAVE_T(off_t pos,const T &Val)
	{
		fseek(Tdata,pos,SEEK_SET);
		fwrite(&Val,sizeof(T),1,Tdata);
		fflush(Tdata);
	}
	inline T LOAD_T(off_t pos)
	{
		T Val;
		fseek(Tdata,pos,SEEK_SET);
		fread(&Val,sizeof(T),1,Tdata);
		return Val;
	}
	inline Key* get_key_leaf(_buffer _buf,int k)
	{
		return reinterpret_cast<Key*>(_buf+(sizeof(Key)+sizeof(off_t))*k);
	}
	inline off_t* get_val(_buffer _buf,int k)
	{
		return reinterpret_cast<off_t*>(_buf+(sizeof(Key)+sizeof(off_t))*k+sizeof(Key));
	}
	inline Key* get_key_nleaf(_buffer _buf,int k)
	{
		return reinterpret_cast<Key*>(_buf+(sizeof(Key)+sizeof(off_t))*k);
	}
	inline off_t* get_to(_buffer _buf,int k)
	{
		return reinterpret_cast<off_t*>(_buf+(sizeof(Key)+sizeof(off_t))*k+sizeof(Key));
	}

// INSERT------------------------------------------------------------------------------------------


	size_t kth_leaf(_buffer _buf,node &p,const Key &key)
	{
		int l=0,r=p.cnt-1; size_t ans=p.cnt;
		while(l<=r)
		{
			int mid=(l+r)>>1;
			Key *k=get_key_leaf(_buf,mid);
			if(!cmp(*k,key)) ans=mid,r=mid-1;
			else l=mid+1;
		}
		return ans;
	}
	size_t kth_nleaf(_buffer _buf,node &p,const Key &key)
	{
		int l=0,r=p.cnt-1; size_t ans=p.cnt;
		while(l<=r)
		{
			int mid=(l+r)>>1;
			Key *k=get_key_nleaf(_buf,mid);
			if(!cmp(*k,key)) ans=mid,r=mid-1;
			else l=mid+1;
		}
		return ans;
	}
	/*void PRINT(node p)
	{
		buffer buf1; LOAD_LEAF_BUF(buf1,p);
			std::cout<<"leafshit:"<<p.self<<"-->"<<p.key<<"  ";
			for(int i=0;i<p.cnt;i++)
				std::cout<<*get_key_leaf(buf1,i)<<"->"<<*get_val(buf1,i)<<" ";
			std::cout<<"\n";
	}
	void PRINT2(node p)
	{
		buffer buf1; LOAD_NLEAF_BUF(buf1,p);
			std::cout<<"nodeshit:"<<p.self<<"-->"<<p.key<<"  ";
			for(int i=0;i<p.cnt;i++)
				std::cout<<*get_key_nleaf(buf1,i)<<"->"<<*get_to(buf1,i)<<" ";
		std::cout<<"\n";
	}*/
	node leaf_split(_buffer _buf,node &p)
	{
		size_t S1=(p.cnt>>1),S2=p.cnt-S1;
		/*if(FLAG)
		{
			std::cout<<"leafshit:";
			for(int i=0;i<p.cnt;i++)
				std::cout<<*get_key_leaf(_buf,i)<<"->"<<*get_val(_buf,i)<<" ";
			std::cout<<"\n";
		}
		if(FLAG) std::cout<<"Leafshit\n";*/
		node p2(DISK.get_pos(),1,p.self,p.nxt);
		assert((p2.self-3*sizeof(off_t)-sizeof(int))%ndsize==0&&p2.self>=3*sizeof(off_t)+sizeof(int));
	//	if(FLAG) std::cout<<"HAHAHA"<<p2.self<<"\n";
		p.cnt=S1;
		p2.cnt=S2;
		p.key=*get_key_leaf(_buf,S1-1);
		p2.key=*get_key_leaf(_buf,S1+S2-1);
		p.nxt=p2.self;
		if(p2.nxt==invalid_pos)
		{
			Tail=p2.self;
			SAVE();
		}
		else{
			node Nxt=LOAD_NODE(p2.nxt);
			Nxt.pre=p2.self;
			SAVE_NODE(Nxt);
		}
		SAVE_NODE(p);
		SAVE_LEAF_BUF(_buf,p);
		SAVE_NODE(p2);
		SAVE_LEAF_BUF(_buf+(sizeof(Key)+sizeof(off_t))*S1,p2);
		//std::cout<<"leafshit:";
		/*if(FLAG)
		{
			PRINT(p);
			PRINT(p2);
		}*/
		return p2;
	}
	node nleaf_split(_buffer _buf,node &p)
	{
		//if(FLAG) std::cout<<"Nodeshit\n";
		size_t S1=(p.cnt>>1),S2=p.cnt-S1;
		node p2(DISK.get_pos(),0);
		assert((p2.self-3*sizeof(off_t)-sizeof(int))%ndsize==0&&p2.self>=3*sizeof(off_t)+sizeof(int));
		p.cnt=S1;
		p2.cnt=S2;
		p.key=*get_key_nleaf(_buf,S1-1);
		p2.key=*get_key_nleaf(_buf,S1+S2-1);
		SAVE_NODE(p);
		SAVE_NLEAF_BUF(_buf,p);
		SAVE_NODE(p2);
		SAVE_NLEAF_BUF(_buf+(sizeof(Key)+sizeof(off_t))*S1,p2);
		return p2;
	}
	node insert_leaf(_buffer _buf,node &p,const Key &key,const off_t &Val)
	{
		//if(FLAG) PRINT2(p);
		size_t k=kth_leaf(_buf,p,key);
		for(size_t i=p.cnt;i>k;i--)
		{
			*get_key_leaf(_buf,i)=*get_key_leaf(_buf,i-1);
			*get_val(_buf,i)=*get_val(_buf,i-1);
		}
		*get_key_leaf(_buf,k)=key;
		*get_val(_buf,k)=Val;
		p.cnt++;
		if(k==p.cnt-1) p.key=key;
		if(p.cnt==entry)
		{
			//if(FLAG) std::cout<<"leaf\n";
			node p2=leaf_split(_buf,p);
			return p2;
		}
		else{
			SAVE_NODE(p);
			SAVE_LEAF_BUF(_buf,p);
			return p;
		}
	}
	node insert_nleaf(_buffer _buf,node &p,const Key &key,const off_t &To,size_t k)
	{
		for(size_t i=p.cnt;i>k;i--)
		{
			*get_key_nleaf(_buf,i)=*get_key_nleaf(_buf,i-1);
			*get_to(_buf,i)=*get_to(_buf,i-1);
		}
		*get_key_nleaf(_buf,k)=key;
		*get_to(_buf,k)=To;
		p.cnt++;
		if(k==p.cnt-1) p.key=key;
		if(p.cnt==order)
		{
			node p2=nleaf_split(_buf,p);
			return p2;
		}
		else{
			SAVE_NODE(p);
			SAVE_NLEAF_BUF(_buf,p);
			return p;
		}
	}
	node dfs_insert(node &p,const Key &key,const off_t &Val)
	{
		if(p.tp==1)
		{
			//if(FLAG) std::cout<<p.self<<"leaf\n";
			//if(FLAG) PRINT(p);
			buffer buf;
			LOAD_LEAF_BUF(buf,p);
			return insert_leaf(buf,p,key,Val);
		}
		else{
			//if(FLAG) std::cout<<p.self<<"node\n";
			//if(FLAG) PRINT2(p);
			buffer buf;
			LOAD_NLEAF_BUF(buf,p);
			int k=kth_nleaf(buf,p,key);
			if(k==p.cnt) k--;
			off_t to0=*get_to(buf,k);
			node t0=LOAD_NODE(to0);
			node t1=dfs_insert(t0,key,Val);
			if(t0.self==t1.self)
			{
				if(k==p.cnt-1) p.key=t1.key;
				fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*k,SEEK_SET);
				fwrite(&t1.key,sizeof(Key),1,file);
				SAVE_NODE(p);
				return p;
			}
			else{
				*get_key_nleaf(buf,k)=t0.key;
				return insert_nleaf(buf,p,t1.key,t1.self,k+1);
			}
		}
	}
	void final_insert(const Key &key,const off_t &Val)
	{
		if(Root==invalid_pos)
		{
			node root(DISK.get_pos(),1);
			assert((root.self-3*sizeof(off_t)-sizeof(int))%ndsize==0&&root.self>=3*sizeof(off_t)+sizeof(int));
			buffer _buf;
			insert_leaf(_buf,root,key,Val);
			Head=Tail=Root=root.self;
			SAVE();
			SAVE_LEAF_BUF(_buf,root);
		}
		else{
			node root=LOAD_NODE(Root);
			node p=dfs_insert(root,key,Val);
			if(p.self==Root) SAVE_NODE(p);
			else{
				node rt2(DISK.get_pos(),0);
				assert((rt2.self-3*sizeof(off_t)-sizeof(int))%ndsize==0&&rt2.self>=3*sizeof(off_t)+sizeof(int));
				Root=rt2.self;
				SAVE();
				rt2.key=p.key;
				rt2.cnt=2;
				SAVE_NODE(rt2);
				buffer _buf;
				*get_key_nleaf(_buf,0)=root.key;
				*get_to(_buf,0)=root.self;
				*get_key_nleaf(_buf,1)=p.key;
				*get_to(_buf,1)=p.self;
				SAVE_NLEAF_BUF(_buf,rt2);
			}
		}
	}

// ERASE-------------------------------------------------------------------------------------------

	void leaf_erase(_buffer _buf,node &p,size_t k)
	{
		//if(FLAG) PRINT(p);
		if(k==p.cnt-1) p.key=*get_key_leaf(_buf,p.cnt-2);
		for(int i=k;i<p.cnt;i++)
		{
			*get_key_leaf(_buf,i)=*get_key_leaf(_buf,i+1);
			*get_val(_buf,i)=*get_val(_buf,i+1);
		}
		p.cnt--;
		SAVE_NODE(p);
		SAVE_LEAF_BUF(_buf,p);
		//if(FLAG) PRINT(p);
	}
	void merge_leaf(node &p,node &p2)
	{
		//if(FLAG) PRINT(p);
		//if(FLAG) PRINT(p2);
		p.nxt=p2.nxt;
		DISK.put_pos(p2.self);
		if(p2.nxt==invalid_pos)
		{
			Tail=p.self;
			SAVE();
		}
		else{
			node Nxt=LOAD_NODE(p2.nxt);
			Nxt.pre=p.self;
			SAVE_NODE(Nxt);
		}
		buffer _buf;
		LOAD_LEAF_BUF(_buf,p2);
		fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*p.cnt,SEEK_SET);
		fwrite(_buf,(sizeof(Key)+sizeof(off_t))*p2.cnt,1,file);
		fflush(file);
		p.cnt+=p2.cnt;
		p.key=p2.key;
		SAVE_NODE(p);
		//if(FLAG) PRINT(p);
	}
	void merge_nleaf(node &p,node &p2)
	{
		DISK.put_pos(p2.self);
		buffer _buf;
		LOAD_LEAF_BUF(_buf,p2);
		fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*p.cnt,SEEK_SET);
		fwrite(_buf,(sizeof(Key)+sizeof(off_t))*p2.cnt,1,file);
		fflush(file);
		p.cnt+=p2.cnt;
		p.key=p2.key;
		SAVE_NODE(p);
	}
	int leaf_operate_left(_buffer _buf,node &p,node &left)
	{
		if(left.cnt>entry/2)
		{
			//if(FLAG) std::cout<<"op1"<<"\n";
			/*if(FLAG)
			{
				PRINT(left);
				PRINT(p);
			}*/
			p.cnt++;
			buffer lbuf;
			LOAD_LEAF_BUF(lbuf,left);
			fseek(file,p.self,SEEK_SET);
			fwrite(&p,sizeof(node),1,file);
			fwrite(lbuf+(sizeof(Key)+sizeof(off_t))*(left.cnt-1),sizeof(Key)+sizeof(off_t),1,file);
			fwrite(_buf,(sizeof(Key)+sizeof(off_t))*(p.cnt-1),1,file);
			fflush(file);
			left.cnt--;
			left.key=*get_key_leaf(lbuf,left.cnt-1);
			SAVE_NODE(left);
			/*if(FLAG)
			{
				PRINT(left);
				PRINT(p);
			}*/
			return -1;
		}
		else{
			merge_leaf(left,p);
			return -2;
		}
	}
	int leaf_operate_right(_buffer _buf,node &p,node &right)
	{
		if(right.cnt>entry/2)
		{
			//if(FLAG) std::cout<<"op2"<<"\n";
			p.cnt++;
			buffer rbuf;
			LOAD_LEAF_BUF(rbuf,right);
			p.key=*get_key_leaf(rbuf,0);
			fseek(file,p.self,SEEK_SET);
			fwrite(&p,sizeof(node),1,file);
			fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*(p.cnt-1),SEEK_SET);
			fwrite(rbuf,sizeof(Key)+sizeof(off_t),1,file);
			fflush(file);
			right.cnt--;
			SAVE_NODE(right);
			SAVE_LEAF_BUF(rbuf+sizeof(Key)+sizeof(off_t),right);
			return 1;
		}
		else{
			merge_leaf(p,right);
			//std::cout<<p.key<<endl;
			return 2;
		}
	}
	int nleaf_operate_left(_buffer _buf,node &p,node &left)
	{
		if(left.cnt>order/2)
		{
			/*if(FLAG)
			{
				std::cout<<"LOOK!\n";
				PRINT2(left);
				PRINT2(p);
				std::cout<<"\n";
			}*/
			p.cnt++;
			buffer lbuf;
			LOAD_NLEAF_BUF(lbuf,left);
			fseek(file,p.self,SEEK_SET);
			fwrite(&p,sizeof(node),1,file);
			fwrite(lbuf+(sizeof(Key)+sizeof(off_t))*(left.cnt-1),sizeof(Key)+sizeof(off_t),1,file);
			fwrite(_buf,(sizeof(Key)+sizeof(off_t))*(p.cnt-1),1,file);
			fflush(file);
			left.cnt--;
			left.key=*get_key_nleaf(lbuf,left.cnt-1);
			SAVE_NODE(left);
			/*if(FLAG)
			{
				PRINT2(left);
				PRINT2(p);
				std::cout<<"\n";
			}*/
			return -1;
		}
		else{
			//if(FLAG) std::cout<<"op4\n";
			merge_nleaf(left,p);
			return -2;
		}
	}
	int nleaf_operate_right(_buffer _buf,node &p,node &right)
	{
		if(right.cnt>order/2)
		{
			p.cnt++;
			buffer rbuf;
			LOAD_NLEAF_BUF(rbuf,right);
			p.key=*get_key_nleaf(rbuf,0);
			fseek(file,p.self,SEEK_SET);
			fwrite(&p,sizeof(node),1,file);
			fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*(p.cnt-1),SEEK_SET);
			fwrite(rbuf,sizeof(Key)+sizeof(off_t),1,file);
			fflush(file);
			right.cnt--;
			SAVE_NODE(right);
			SAVE_NLEAF_BUF(rbuf+sizeof(Key)+sizeof(off_t),right);
			return 1;
		}
		else{
			merge_nleaf(p,right);
			return 2;
		}
	}
	int dfs_erase(node &p,const Key &key,off_t left,off_t right)
	{
		if(p.tp==1)
		{
			//if(FLAG) std::cout<<"leaf"<<p.self<<" "<<p.cnt<<"\n";
			//if(FLAG) std::cout<<"node-> "<<p.self<<" "<<p.key<<'\n';
			//if(FLAG) PRINT(p);
			buffer _buf; LOAD_LEAF_BUF(_buf,p);
			size_t k=kth_leaf(_buf,p,key);
			/*if(FLAG)
			{
				std::cout<<"\n"<<k<<" "<<p.cnt<<"\n";
				for(int i=0;i<p.cnt;i++)
					std::cout<<*get_key_leaf(_buf,i)<<" ";
				std::cout<<"fuck\n";
			}*/
			if(!EQUAL(*get_key_leaf(_buf,k),key)) return -5;
			leaf_erase(_buf,p,k);
			//if(FLAG) PRINT(p);
			/*if(FLAG)
			{
				std::cout<<"\n"<<k<<" "<<p.cnt<<"\n";
				for(int i=0;i<p.cnt;i++)
					std::cout<<*get_key_leaf(_buf,i)<<" ";
				std::cout<<"fuck\n";
			}*/
			if(!p.cnt&&p.self==Root)
			{
				DISK.put_pos(p.self);
				Root=Head=Tail=invalid_pos;
				SAVE();
				return 0;
			}
			else if(p.cnt>=(entry/2)||p.self==Root) return 0;
			else if(left==invalid_pos&&right==invalid_pos) return 0;
			else if(left==invalid_pos)
			{
				node R=LOAD_NODE(right);
				return leaf_operate_right(_buf,p,R);
			}
			else if(right==invalid_pos)
			{
				node L=LOAD_NODE(left);
				return leaf_operate_left(_buf,p,L);
			}
			else{
				node L=LOAD_NODE(left),R=LOAD_NODE(right);
				return L.cnt>R.cnt?leaf_operate_left(_buf,p,L):leaf_operate_right(_buf,p,R);
			}
		}
		else{
			//if(FLAG) std::cout<<"node-> "<<p.self<<" "<<p.key<<'\n';
			//if(FLAG) PRINT2(p);
			buffer _buf; LOAD_NLEAF_BUF(_buf,p);
			size_t k=kth_nleaf(_buf,p,key);
			off_t to0=*get_to(_buf,k);
			off_t to1=(k==0?invalid_pos:*get_to(_buf,k-1)),
			to2=(k==p.cnt-1?invalid_pos:*get_to(_buf,k+1));
			node t0=LOAD_NODE(to0);
			//if(FLAG) std::cout<<t0.self<<"\n";
			int flag=dfs_erase(t0,key,to1,to2);
			//if(FLAG) std::cout<<flag<<" "<<t0.self<<"\n";
			//if(FLAG) std::cout<<flag<<" GAM\n";
			if(flag==-5) return -5;
			else if(flag==-1)
			{
				node t1=LOAD_NODE(to1);
				if(EQUAL(key,p.key))
				{
					//if(FLAG) PRINT2(p)
					p.key=t0.key;
					SAVE_NODE(p);
				}
				fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*(k-1),SEEK_SET);
				fwrite(&t1.key,sizeof(Key),1,file);
				fwrite(&t1.self,sizeof(off_t),1,file);
				fwrite(&t0.key,sizeof(Key),1,file);
				fflush(file);
				return 0;
			}
			else if(flag==1)
			{
				fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*k,SEEK_SET);
				fwrite(&t0.key,sizeof(Key),1,file);
				fflush(file);
				return 0;
			}
			else if(flag==0)
			{
				if(EQUAL(key,p.key))
				{
					//if(FLAG) std::cout<<"GAM\n";
					p.key=t0.key;
					SAVE_NODE(p);
				}
				fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*k,SEEK_SET);
				fwrite(&t0.key,sizeof(Key),1,file);
				fflush(file);
				return 0;
			}
			else if(flag==-2)
			{
				//if(FLAG) std::cout<<"HERE\n";
				//if(FLAG) PRINT2(p);
				node t1=LOAD_NODE(to1);
				if(EQUAL(key,p.key)) p.key=t1.key;
				p.cnt--;
				SAVE_NODE(p);
				fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*(k-1),SEEK_SET);
				fwrite(&t1.key,sizeof(Key),1,file);
				fwrite(&t1.self,sizeof(off_t),1,file);
				fwrite(_buf+(sizeof(Key)+sizeof(off_t))*(k+1),(sizeof(Key)+sizeof(off_t))*(p.cnt-k),1,file);
				fflush(file);
				//if(FLAG) PRINT2(p);
			}
			else if(flag==2)
			{
				//if(FLAG) std::cout<<t0.key<<"\n";
				//if(FLAG) std::cout<<p.key<<" "<<*get_key_nleaf(_buf,p.cnt-1)<<"\n";
				p.cnt--;
				SAVE_NODE(p);
				fseek(file,p.self+sizeof(node)+(sizeof(Key)+sizeof(off_t))*k,SEEK_SET);
				fwrite(&t0.key,sizeof(Key),1,file);
				fwrite(&t0.self,sizeof(off_t),1,file);
				fwrite(_buf+(sizeof(Key)+sizeof(off_t))*(k+2),(sizeof(Key)+sizeof(off_t))*(p.cnt-k-1),1,file);
				fflush(file);
			}
			LOAD_NLEAF_BUF(_buf,p);
			//if(FLAG) std::cout<<p.key<<" "<<*get_key_nleaf(_buf,p.cnt-1)<<"\n";
			/*if(FLAG)
			{
				std::cout<<"info: "<<p.self<<"->";
				for(int i=0;i<p.cnt;i++)
					std::cout<<*get_key_nleaf(_buf,i)<<" ";
				std::cout<<"\n";
			}*/
			if(p.self==Root&&p.cnt==1)
			{
				Root=*get_to(_buf,0);
				DISK.put_pos(p.self);
				SAVE();
				return 0;
			}
			if(p.cnt>=order/2||p.self==Root) return 0;
			else if(left==invalid_pos&&right==invalid_pos) return 0;
			else if(left==invalid_pos)
			{
				node R=LOAD_NODE(right);
				return nleaf_operate_right(_buf,p,R);
			}
			else if(right==invalid_pos)
			{
				node L=LOAD_NODE(left);
				return nleaf_operate_left(_buf,p,L);
			}
			else{
				node L=LOAD_NODE(left),R=LOAD_NODE(right);
				return L.cnt>=R.cnt?nleaf_operate_left(_buf,p,L):nleaf_operate_right(_buf,p,R);
			}
		}
	}
	void final_erase(const Key &key)
	{
		if(Root==invalid_pos) return;
		node root=LOAD_NODE(Root);
		if(cmp(root.key,key)) return;
		int flag=dfs_erase(root,key,invalid_pos,invalid_pos);
	}
	/*int dfs_debug(node p)
	{
		buffer buf;
		if(!p.tp)
		{
			LOAD_NLEAF_BUF(buf,p);
			if(!EQUAL(*get_key_nleaf(buf,p.cnt-1),p.key)) return -1;
			for(int i=1;i<p.cnt;i++)
				if(!cmp(*get_key_nleaf(buf,i-1),*get_key_nleaf(buf,i)))
				{
					return -1;
				}
			for(int i=0;i<p.cnt;i++)
				if(dfs_debug(LOAD_NODE(*get_to(buf,i)))==-1) return -1;
			return 0;
		}
		else{
			LOAD_LEAF_BUF(buf,p);
			if(!EQUAL(*get_key_leaf(buf,p.cnt-1),p.key)) return -1;
			for(int i=1;i<p.cnt;i++)
				if(!cmp(*get_key_leaf(buf,i-1),*get_key_leaf(buf,i)))
				{
					return -1;
				}
			return 0;
		}
	}*/
	
//public:
	Bplustree():
		order((ndsize-sizeof(node))/(sizeof(Key)+sizeof(off_t))),
		entry((ndsize-sizeof(node))/(sizeof(Key)+sizeof(off_t)))
		//order(30),
		//entry(30)
	{
		sz=0;
		fname=NULL;
		Dname=NULL;
		Tdata=NULL;
		file=NULL;
	}
	Bplustree(const char *filename,const char *diskname,const char *dataname):
		order((ndsize-sizeof(node))/(sizeof(Key)+sizeof(off_t))),
		entry((ndsize-sizeof(node))/(sizeof(Key)+sizeof(off_t)))
		//order(30),
		//entry(30)
	{
		fname=new char[strlen(filename)+1];
		strcpy(fname,filename);
		Dname=new char[strlen(diskname)+1];
		strcpy(Dname,diskname);
		DISK.init(Dname);
		file=fopen(fname,"rb+");
		if(!file)
		{
			file=fopen(fname,"wb+");
			sz=0;
			Head=Tail=Root=invalid_pos;
			fseek(file,0,SEEK_SET);
			fwrite(&sz,sizeof(int),1,file);
			fwrite(&Root,sizeof(off_t),1,file);
			fwrite(&Head,sizeof(off_t),1,file);
			fwrite(&Tail,sizeof(off_t),1,file);
			fflush(file);
		}
		else{
			fseek(file,0,SEEK_SET);
			fread(&sz,sizeof(int),1,file);
			fread(&Root,sizeof(off_t),1,file);
			fread(&Head,sizeof(off_t),1,file);
			fread(&Tail,sizeof(off_t),1,file);
		}
		Tdata=fopen(dataname,"rb+");
		if(!Tdata) Tdata=fopen(dataname,"wb+");
	}
	~Bplustree()
	{
		fseek(file,0,SEEK_SET);
		fwrite(&sz,sizeof(int),1,file);
		fflush(file);
		if(fname!=NULL) delete [] fname;
		if(Dname!=NULL) delete [] Dname;
		if(file) fclose(file);
		if(Tdata) fclose(Tdata);
	}
	void init(const char *filename,const char *diskname,const char *dataname)
	{
		fname=new char[strlen(filename)+1];
		strcpy(fname,filename);
		Dname=new char[strlen(diskname)+1];
		strcpy(Dname,diskname);
		DISK.init(Dname);
		file=fopen(fname,"rb+");
		if(!file)
		{
			file=fopen(fname,"wb+");
			sz=0;
			Head=Tail=Root=invalid_pos;
			fseek(file,0,SEEK_SET);
			fwrite(&sz,sizeof(int),1,file);
			fwrite(&Root,sizeof(off_t),1,file);
			fwrite(&Head,sizeof(off_t),1,file);
			fwrite(&Tail,sizeof(off_t),1,file);
			fflush(file);
		}
		else{
			fseek(file,0,SEEK_SET);
			fread(&sz,sizeof(int),1,file);
			fread(&Root,sizeof(off_t),1,file);
			fread(&Head,sizeof(off_t),1,file);
			fread(&Tail,sizeof(off_t),1,file);
		}
		Tdata=fopen(dataname,"rb+");
		if(!Tdata) Tdata=fopen(dataname,"wb+");
	}
	int SIZE()
	{
		return sz;
	}
	void clear()
	{
		sz=0;
		Head=Tail=Root=invalid_pos;
		fseek(file,0,SEEK_SET);
		fwrite(&sz,sizeof(int),1,file);
		fwrite(&Root,sizeof(off_t),1,file);
		fwrite(&Head,sizeof(off_t),1,file);
		fwrite(&Tail,sizeof(off_t),1,file);
		fflush(file);
		DISK.clear();
	}
	bool empty()
	{
		return Head==invalid_pos;
	}
	off_t insert(const Key &key,const T &Val)
	{
		final_insert(key,(off_t(sz))*(off_t(sizeof(T))));
		SAVE_T((off_t(sz))*(off_t(sizeof(T))),Val);
		sz++;
		return (off_t(sz-1))*(off_t(sizeof(T)));
	}
	void erase(const Key &key)
	{
		//if(FLAG) std::cout<<key<<"\n";
		/*if(FLAG)
		{
			node p=LOAD_NODE(Root);
			buffer _buf; LOAD_NLEAF_BUF(_buf,p);
			std::cout<<"info: "<<p.self<<"->";
				for(int i=0;i<p.cnt;i++)
					std::cout<<*get_key_nleaf(_buf,i)<<" ";
				std::cout<<"\n";
		}*/
		final_erase(key);
		/*if(FLAG)
		{
			node p=LOAD_NODE(Root);
			buffer _buf; LOAD_NLEAF_BUF(_buf,p);
			std::cout<<"info: "<<p.self<<"->";
				for(int i=0;i<p.cnt;i++)
					std::cout<<*get_key_nleaf(_buf,i)<<" ";
				std::cout<<"\n";
		}*/
	}
	bool exist(const Key &key)
	{
		if(Root==invalid_pos) return false;
		node p=LOAD_NODE(Root);
		if(cmp(p.key,key)) return false;
		buffer _buf; int k;
		//if(FLAG) std::cout<<" START: "<<key<<"  ";
		while(!p.tp)
		{
			LOAD_NLEAF_BUF(_buf,p);
			k=kth_nleaf(_buf,p,key);
			//if(FLAG) std::cout<<p.self<<"->"<<k<<" ";
			p=LOAD_NODE(*get_to(_buf,k));
		}
		LOAD_LEAF_BUF(_buf,p);
		k=kth_leaf(_buf,p,key);
		//if(FLAG) std::cout<<p.self<<"->"<<k<<"    \n";
		return EQUAL(*get_key_leaf(_buf,k),key);
	}
	T find(const Key &key)
	{
		if(Root==invalid_pos) return T();
		node p=LOAD_NODE(Root);
		if(cmp(p.key,key)) return T();
		buffer _buf; int k;
		while(!p.tp)
		{
			LOAD_NLEAF_BUF(_buf,p);
			k=kth_nleaf(_buf,p,key);
			p=LOAD_NODE(*get_to(_buf,k));
		}
		LOAD_LEAF_BUF(_buf,p);
		k=kth_leaf(_buf,p,key);
		if(EQUAL(*get_key_leaf(_buf,k),key)) return LOAD_T(*get_val(_buf,k));
		else return T();
	}
	off_t find2(const Key &key)
	{
		if(Root==invalid_pos) return invalid_pos;
		node p=LOAD_NODE(Root);
		if(cmp(p.key,key)) return invalid_pos;
		buffer _buf; int k;
		while(!p.tp)
		{
			LOAD_NLEAF_BUF(_buf,p);
			k=kth_nleaf(_buf,p,key);
			p=LOAD_NODE(*get_to(_buf,k));
		}
		LOAD_LEAF_BUF(_buf,p);
		k=kth_leaf(_buf,p,key);
		if(EQUAL(*get_key_leaf(_buf,k),key)) return *get_val(_buf,k);
		else return invalid_pos;
	}
	bool change(const Key &key,T Val)
	{
		if(Root==invalid_pos) return false;
		node p=LOAD_NODE(Root);
		if(cmp(p.key,key)) return false;
		buffer _buf; int k;
		while(!p.tp)
		{
			LOAD_NLEAF_BUF(_buf,p);
			k=kth_nleaf(_buf,p,key);
			p=LOAD_NODE(*get_to(_buf,k));
		}
		LOAD_LEAF_BUF(_buf,p);
		k=kth_leaf(_buf,p,key);
		bool flag=EQUAL(*get_key_leaf(_buf,k),key);
		if(flag)
		{
			SAVE_T(*get_val(_buf,k),Val);
			return true;
		}
		else return false;
	}
	void traverse(std::vector<std::pair<Key,T> >&vec,const Key l,const Key r)
	{
		if(Root==invalid_pos) return;
		node p=LOAD_NODE(Root);
		if(cmp(p.key,l)) return;
		buffer _buf; int k;
		while(!p.tp)
		{
			LOAD_NLEAF_BUF(_buf,p);
			k=kth_nleaf(_buf,p,l);
			p=LOAD_NODE(*get_to(_buf,k));
		}
		while(1)
		{
			LOAD_LEAF_BUF(_buf,p);
			if(!cmp(r,p.key))
			{
				if(!cmp(*get_key_leaf(_buf,0),l))
				{
					for(int i=0;i<p.cnt;i++)
						vec.push_back(std::pair<Key,T>(*get_key_leaf(_buf,i),LOAD_T(*get_val(_buf,i))));
				}
				else{
					int i=0;
					while(cmp(*get_key_leaf(_buf,i),l)) i++;
					for(;i<p.cnt;i++)
						vec.push_back(std::pair<Key,T>(*get_key_leaf(_buf,i),LOAD_T(*get_val(_buf,i))));
				}
				if(p.nxt==invalid_pos) return;
				p=LOAD_NODE(p.nxt);
			}
			else{
				if(!cmp(*get_key_leaf(_buf,0),l))
				{
					for(int i=0;i<p.cnt;i++)
					{
						if(cmp(r,*get_key_leaf(_buf,i))) break;
						vec.push_back(std::pair<Key,T>(*get_key_leaf(_buf,i),LOAD_T(*get_val(_buf,i))));
					}
				}
				else{
					int i=0;
					while(cmp(*get_key_leaf(_buf,i),l)) i++;
					for(;i<p.cnt;i++)
					{
						if(cmp(r,*get_key_leaf(_buf,i))) break;
						vec.push_back(std::pair<Key,T>(*get_key_leaf(_buf,i),LOAD_T(*get_val(_buf,i))));
					}
				}
				return;
			}
		}
	}
	void getall(std::vector<std::pair<Key,T> > & vec)
	{
		if(Root==invalid_pos) return;
		node p=LOAD_NODE(Head);
		buffer _buf;
		while(1)
		{
			LOAD_LEAF_BUF(_buf,p);
			for(size_t i=0;i<p.cnt;i++)
				vec.push_back(std::pair<Key,T>(*get_key_leaf(_buf,i),LOAD_T(*get_val(_buf,i))));
			if(p.nxt==invalid_pos) break;
			p=LOAD_NODE(p.nxt);
		}
	}
	/*void F()
	{
		FLAG^=1;
	}*/
	/*int DEBUG()
	{
		int flag=0;
		if(Root!=invalid_pos)
		{
			flag=dfs_debug(LOAD_NODE(Root));
			if(flag==-1) std::cout<<"ERRRRRRRRRRRRRRRRRRRRRRRR\n";
		}
		return flag;
	}*/
};

#endif