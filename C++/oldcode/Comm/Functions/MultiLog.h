#ifndef __MULTI_LOG
#define __MULTI_LOG
#define ML_MAXSUBLOG 32
#define ML_MAXLOGLEN 4096
#define ML_ALIGN_ID 0x474F4C4D

struct ML_st_header {
	long malignid;
	long mseq;	//the main sequence of this log;
	time_t mtime;  //the log time;
	long msubid; //the sub id of the log;
	long msubseq;// the sub log sequence;
	long mlen;	// the length of this log,	don't incluse header
};

typedef void( *ML_Data_CB)(ML_st_header *phead,char *pdata,long udata);

class MultiLog {
	int mcritical;
public:
	int mfd;		// the file handle for this head file.
	int mlastseq;
	int mlastsubseq[ML_MAXSUBLOG];// the lastsequence number for the sub seq;
protected:
	int ReadRecord(int fd,ML_st_header &st,char *buff);
public:
	inline void SetCritical(int bcritical){mcritical=bcritical;};

	void Flush();
	int GetLastSeq(int subid);

	//return the log number which recovered;
	//if pcb==NULL return 0;
	// -1 means can't open file for write
	int QueryLog(int subid, int startseq, int endseq, ML_Data_CB pcb, long udata);

	//1 success; 0not success
	int AddLog(int subid,char *pdata, int len);
	
	//1 success; 0 not 
	int AddLog(ML_st_header *pheader, char *pdata);
	
	
	//return the log number which recovered;
	//if pcb==NULL return 0;
	// -1 means can't open file for write
	int InitLog(char * pfile, ML_Data_CB pcb,long udata);
	MultiLog();
	virtual ~MultiLog();
};

#endif
