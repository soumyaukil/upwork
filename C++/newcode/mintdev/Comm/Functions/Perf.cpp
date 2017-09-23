//#include "windows.h"
#include "Perf.h"
#include "stdio.h"

#ifdef PREF_ENABLE

_perf_item _perf_items[500];

void Perf_Show()
{
	int i;
	char str[5000];
	int len=0;;
	_int64 frequency;
	if (!QueryPerformanceFrequency ((LARGE_INTEGER*)&frequency)) {
		MessageBox(NULL,"Query frequency error!","Perfermence",0); 
	};
	len+=sprintf(str+len,"Frequence:%I64d, Resolution:%gus\r\n",frequency,1000000.0/frequency);
	len+=sprintf(str+len,"id\tcounter\tticker\ttime\tAvgTime\tname\r\n",frequency); 
	for (i=0;i<MAX_PERFITEM;i++)
	{
		LPCSTR pname;
		pname=_perf_items[i].m_name;
		if (pname==NULL) {
			pname="N/A";
		}
		if (_perf_items[i].m_ticks>0) {
			double elapse;
			elapse=(double)(_int64)_perf_items[i].m_ticks*1000000;
			elapse/=frequency;
			len+=sprintf(str+len,"%d\t%I64d\t%I64d\t%.1lfus Avg%g\t%s\r\n",i,_perf_items[i].m_counter, _perf_items[i].m_ticks,elapse,
				elapse/_perf_items[i].m_counter,pname); 
		}
	}
	MessageBox(NULL,str,"Performance",0); 
};

#endif //PREF_ENABLE
