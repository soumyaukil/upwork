#ifndef __PREF_COUNTER
#define __PREF_COUNTER
#pragma once

#ifndef PREF_ENABLE

#define Perf_Clear() 
#define Perf_Reset(x,y)
#define Perf_SetName(x, y)
#define Perf_Start(x)
#define Perf_Stop(x)
#define Perf_Show()

#else


struct _perf_item
{
	const char *m_name;
	 _int64 m_ticks;
	 _int64 m_start;
	 _int64 m_counter;
	
};

#define MAX_PERFITEM 500
extern _perf_item _perf_items[500];


inline void Perf_Clear(){
	ZeroMemory(&_perf_items,sizeof(_perf_items)); 
};

inline void Perf_Reset(int start,int end){
	ZeroMemory(&_perf_items[start],(end-start+1)*sizeof(_perf_item)); 
};

inline void Perf_SetName(int index, const char * name)
{
	_perf_items[index].m_name=name;
}


inline void Perf_Start(int indx) {
	QueryPerformanceCounter((LARGE_INTEGER *)&(_perf_items[indx].m_start));
}
inline void Perf_Stop(int indx){
	 _int64 tmp;
	QueryPerformanceCounter((LARGE_INTEGER *)&tmp);
	tmp-=_perf_items[indx].m_start;
	_perf_items[indx].m_ticks+=tmp;
	_perf_items[indx].m_counter++;
}

void Perf_Show();

#endif //PREF_ENABLE
#endif //__PREF_COUNTER