#include <iostream>
#include <cstring>
#include <cassert>

#ifndef _DISK_MANAGER
#define _DISK_MANAGER

const off_t invalid_pos=0xdeadbeef;

class disk_manager
{
private:
	static const off_t ndsize=4096;
	FILE *file;
	char *fname;
	size_t cnt;
	off_t fend;
	void SAVE()
	{
		fseek(file,0,SEEK_SET);
		fwrite(&cnt,sizeof(size_t),1,file);
		fwrite(&fend,sizeof(off_t),1,file);
		fflush(file);
	}
	void SAVE_CNT()
	{
		fseek(file,0,SEEK_SET);
		fwrite(&cnt,sizeof(size_t),1,file);
		fflush(file);
	}
	void SAVE_FEND()
	{
		fseek(file,sizeof(size_t),SEEK_SET);
		fwrite(&fend,sizeof(off_t),1,file);
		fflush(file);
	}
	void LOAD()
	{
		fseek(file,0,SEEK_SET);
		fread(&cnt,sizeof(size_t),1,file);
		fread(&fend,sizeof(off_t),1,file);
	}
public:
	disk_manager()
	{
		file=NULL;
		fname=NULL;
		cnt=0;
		fend=invalid_pos;
	}
	disk_manager(char *filename)
	{
		fname=new char[strlen(filename)+1];
		strcpy(fname,filename);
		file=fopen(fname,"rb+");
		if(!file)
		{
			file=fopen(fname,"wb+");
			cnt=0;
			fend=3*sizeof(off_t)+sizeof(int);
			SAVE();
		}
		else LOAD();
	}
	~disk_manager()
	{
		if(file) fclose(file);
		if(fname!=NULL) delete [] fname;
	}
	void init(char *filename)
	{
		fname=new char[strlen(filename)+1];
		strcpy(fname,filename);
		file=fopen(fname,"rb+");
		if(!file)
		{
			file=fopen(fname,"wb+");
			cnt=0;
			fend=3*sizeof(off_t)+sizeof(int);
			SAVE();
		}
		else LOAD();
	}
	void clear()
	{
		cnt=0;
		fend=3*sizeof(off_t)+sizeof(int);
		SAVE();
	}
	off_t get_pos()
	{
		off_t p;
		if(cnt)
		{
			fseek(file,sizeof(size_t)+sizeof(off_t)*cnt,SEEK_SET);
			fread(&p,sizeof(off_t),1,file);
			cnt--;
			SAVE_CNT();
		}
		else{
			p=fend;
			fend+=ndsize;
			SAVE_FEND();
		}
		return p;
	}
	void put_pos(off_t p)
	{
		cnt++;
		fseek(file,(off_t)sizeof(size_t)+sizeof(off_t)*cnt,SEEK_SET);
		fwrite(&p,sizeof(off_t),1,file);
		SAVE_CNT();
	}
};

#endif
