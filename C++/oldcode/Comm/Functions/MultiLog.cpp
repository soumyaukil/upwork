#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "io.h"
#include "fcntl.h"
#include "time.h"
#include "SYS\STAT.H"

#ifdef DLL_EXPORTS
class __declspec(dllexport) MultiLog;
#endif

#include "MultiLog.h"

MultiLog::MultiLog()
{
	mcritical=0;
	mfd=0;
	mlastseq=0;
	memset(&mlastsubseq,0,sizeof(mlastsubseq));  
}

MultiLog::~MultiLog(){
	if (mfd>0) {
		close(mfd);
		mfd=0;
	}
}

int MultiLog::InitLog(char *pfile, ML_Data_CB pcb, long udata)
{
	int count=0;
	ML_st_header st1;
	char buff[ML_MAXLOGLEN];
	if (mfd!=0) {
		_close(mfd);
		mfd=0;
		mlastseq=0;
		memset(&mlastsubseq,0,sizeof(mlastsubseq));  
	}
	mfd=_open(pfile,O_CREAT|O_BINARY|O_RDWR,_S_IREAD|_S_IWRITE);
	if (mfd<=NULL) return -1;

	if (pcb==NULL){// assume the there are broken recordes.
		lseek(mfd,0,SEEK_END);
		return 0;
	}


	int pos=0;
	int ret;
	while((ret=read(mfd,&st1,sizeof(st1)))>0) {
		if (ret<sizeof(st1)){
			fprintf(stderr,"wrong log head id!");
			lseek(mfd,pos,SEEK_SET);
			break;
		}
		if (st1.malignid!=ML_ALIGN_ID) {
			fprintf(stderr,"wrong log align id!");
			lseek(mfd,pos,SEEK_SET);
			break;
		}
		if (st1.mlen>ML_MAXLOGLEN) {
			fprintf(stderr,"wrong log content len!");
			lseek(mfd,pos,SEEK_SET);
			break;
		}
		if (st1.mlen>0) ret=read(mfd,buff,st1.mlen); 
		mlastseq=st1.mseq;
		if ((st1.msubid>=0)&&(st1.msubid<ML_MAXSUBLOG)) {
			mlastsubseq[st1.msubid]=st1.msubseq;
		}
		pos=pos+sizeof(st1)+st1.mlen;
		if (pcb) pcb(&st1,buff,udata);
		count++;
		
	}
	if (ret<0) {
		fprintf(stderr,"read file error!\n");
	}
	return count;
}

int MultiLog::AddLog(ML_st_header *pheader, char *pdata)
{
//	pheader->malignid=ML_ALIGN_ID;
	if (mfd<=0) return 0;
	if (write(mfd,pheader,sizeof(ML_st_header))<=0) {
		return 0;
	};
	if (pheader->mlen>0) {
		if (write(mfd,pdata,pheader->mlen)<=0) {
			return 0;
		}
	}
	mlastseq=pheader->mseq;
	if (pheader->msubid<ML_MAXSUBLOG){
		mlastsubseq[pheader->msubid]=pheader->msubseq;  
	}
	if (mcritical) _commit(mfd);
	return 1;
}

int MultiLog::AddLog(int subid, char *pdata, int len)
{
	if (mfd<=0) return 0;
	ML_st_header st1;
	st1.malignid=ML_ALIGN_ID;
	st1.mseq=mlastseq+1;
	st1.msubid=subid;
	if ((subid>=0)&&(subid<ML_MAXSUBLOG)) {
		st1.msubseq=mlastsubseq[st1.msubid]+1; 
	}else {
		st1.msubseq=0;
	}
	if ((len>=0)&&(len<ML_MAXLOGLEN)) {
		st1.mlen=len;
	}else {
		st1.mlen=0;
	}
	time(&st1.mtime);
	if (write(mfd,&st1,sizeof(st1))<=0) {
		return 0;
	};
	if (st1.mlen>0) {
		if (write(mfd,pdata,st1.mlen)<=0) {
			return 0;
		}
	}
//  success, incress the counter.
	mlastseq++;
	if (st1.msubid<ML_MAXSUBLOG){
		mlastsubseq[st1.msubid]++;  
	}
	if (mcritical) _commit(mfd); 
	return 1;
}


//this function is a little bit slow, maybe we can use buffer to make it faster.

int MultiLog::QueryLog(int subid, int startseq, int endseq, ML_Data_CB pcb, long udata)
{
	int count=0;
	ML_st_header st1;
	char buff[ML_MAXLOGLEN];
	if (mfd<=0) return -1;
	int fpos;

	fpos=tell(mfd);
	lseek(mfd,0,SEEK_SET);
//	int pos=0;
	
	while(1) {
		int send=0;
		int quit=0;
		int ret=read(mfd,&st1,sizeof(st1));
		if (ret>0) {
			if (ret<sizeof(st1)){
				fprintf(stderr,"wrong log head id!");
				break;;
			}
			if (st1.malignid!=ML_ALIGN_ID) {
				fprintf(stderr,"wrong log align id!");
				break;
			}
			if (st1.mlen>ML_MAXLOGLEN) {
				fprintf(stderr,"wrong log content len!");
				break;;
			}
		}else if (ret==0){
			fprintf(stderr,"---return 0!\n");
			break;
		}else if (ret<0) {
			fprintf(stderr,"---return %d!\n",ret);
			break;
		}
	
		if (subid<0) {
			if (st1.mseq>=startseq){
				if (st1.mseq<=endseq)send=1;
				if (st1.mseq>=endseq)quit=1;
			}
		}else if (subid==st1.msubid){
			if (st1.msubseq>=startseq){
				if (st1.msubseq<=endseq) send=1;
				if (st1.msubseq>=endseq) quit=1;
			}
		}

		if (st1.mlen>0){
//			if (send) { // seek is slow than read.
				ret=read(mfd,buff,st1.mlen); 
				if (ret<st1.mlen){
					fprintf(stderr,"not enought data!");
					break;
				}else {
					if (pcb) pcb(&st1,buff,udata);
					count++;
				}
/*			}else {
				if (lseek(mfd,st1.mlen,SEEK_CUR)<0) {
					fprintf(stderr,"seek error!");
					break;
				};
			}
*/
		}
		
		if (quit) break;

	}
	lseek(mfd,fpos,SEEK_SET);

	return count;
}

int MultiLog::ReadRecord(int fd, ML_st_header &st, char *buff)
{
	int ret=read(mfd,&st,sizeof(st));
	if (ret>0) {
		if (ret<sizeof(st)){
			fprintf(stderr,"wrong log head id!");
			return -1;
		}
		if (st.malignid!=ML_ALIGN_ID) {
			fprintf(stderr,"wrong log align id!");
			return -1;
		}
		if (st.mlen>ML_MAXLOGLEN) {
			fprintf(stderr,"wrong log content len!");
			return -1;
		}
		if (st.mlen>0) ret=read(mfd,buff,st.mlen); 
		if (ret<st.mlen){
			fprintf(stderr,"not enought data!");
			return -1;
		}
		return (sizeof(st)+st.mlen); 
	}else if (ret==0){
		return 0;
	}else {
		return -1;
	}
}

int MultiLog::GetLastSeq(int subid)
{
	if (subid<0) return mlastseq;
	if (subid<ML_MAXSUBLOG) 
		return mlastsubseq[subid];
	else 
		return 0;
}

void MultiLog::Flush()
{
	if (mfd>=0)	_commit(mfd);
}


///-----------------test---------------------
/*
static void  paa (ML_st_header  *phead,char *pdata,long udata){
	printf("seq:%d,subid:%d,subseq:%d,time %d,len:%d\n",
		phead->mseq,phead->msubid,phead->msubseq,phead->mtime,phead->mlen);
}

int main (){
	MultiLog l1;
	l1.InitLog("abcde",paa,0);
//	ML_st_header t1;
	char st[100];
	sprintf(st,"abv");
	fprintf(stderr,"query\n");
	l1.QueryLog(0,0,100,paa,1);
	fprintf(stderr,"query end\n");
//	l1.AddLog(2,st,strlen(st));
//	l1.AddLog(1,st,strlen(st));
//	l1.AddLog(0,st,strlen(st));
//	l1.AddLog(0,st,strlen(st));
	fprintf(stderr,"mast query\n");
	l1.QueryLog(-1,1,100,paa,1);
	fprintf(stderr,"mast query end\n");
	return 0;
}
*/
