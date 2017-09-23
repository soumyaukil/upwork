// PUIfo.h: interface for the CPUIfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUIFO_H__8C1B853E_8E03_11D3_B6B6_00105A82D2D1__INCLUDED_)
#define AFX_PUIFO_H__8C1B853E_8E03_11D3_B6B6_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <malloc.h>
//#include <winperf.h>

//  Local constants
//
#define RESERVED        0L                        // reserved argument in fn. calls
#define INITIAL_SIZE    40960L                    // initial perf data buff size
#define EXTEND_SIZE     4096L                     // extend size of perf data buff
#define LINE_LENGTH     80                        // display line length

#define WRAP_POINT       LINE_LENGTH-12           // wrap point for explain text
#define OBJECT_TEXT_INDENT  4                     // indent of object explain text

//
//  Text string Constant definitions
//
const LPTSTR NamesKey      = TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib"); 
const LPTSTR DefaultLangId = TEXT("009"); 
const LPTSTR Counters      = TEXT("Counter "); 
const LPTSTR Help          = TEXT("Explain "); 
const LPTSTR LastHelp      = TEXT("Last Help"); 
const LPTSTR LastCounter   = TEXT("Last Counter"); 
const LPTSTR Slash         = TEXT("\\"); 
const LPTSTR Global        = TEXT("Global"); 

class CPUIfo  
{

	HKEY    hCPULoadKey;
	PERF_DATA_BLOCK *  pDataBlock   ;  // pointer to perfdata block
	LPTSTR*           lpCounterText;  // array of counter name strings
	LPTSTR            lpValueString;         // value string to pass to registry for data
	DWORD             dwLastElement;         // number of elements in lpCounterText

	TCHAR   cValueName[LINE_LENGTH];  // used to store counter value string 
	CHAR    cExeName[LINE_LENGTH*2];  // used in processing command line 
	CHAR    cCtrName[LINE_LENGTH];    // used in processing command line 
    
	DWORD  mcpuLoad; // the cpuload information
	DWORD  mLastTick; // the last Tick we get
public:
	CPUIfo();
	virtual ~CPUIfo();
	void GetMemDetails (int& realMem, int& pageFile, int& memLoad);
	int GetCPUPrecent (void);
	int GetMemPrecent();

private:
	int CounterData (PERF_DATA_BLOCK* pPerf,      LPTSTR *pNames, LPTSTR       pSysObject, LPTSTR pCounterRef);
	DWORD GetSystemPerfData (IN HKEY hKeySystem, IN LPTSTR pValue, IN PERF_DATA_BLOCK** pPerfData);
	void CloseCPUCounter (void);
	BOOL FindCounter (IN LPTSTR  lpCmdLine,     IN  LPTSTR  *lppCounterNames,                   IN DWORD   dwLastCounter, OUT LPTSTR  *lpValueString);
	BOOL StringToInt (IN LPTSTR InStr, OUT PDWORD pdwOutInt);
	LPTSTR * BuildNameTable (HKEY    hKeyRegistry,  LPTSTR  lpszLangId,PDWORD  pdwLastItem);
	BOOL InitCPUCounter (void);
	HKEY OpenCPUStatus95 (void);
	void StartCPULoadProfiling95 (void);
/*
	PERF_OBJECT_TYPE FirstObject (PPERF_DATA_BLOCK pPerfData);
	PPERF_OBJECT_TYPE NextObject (PPERF_OBJECT_TYPE pObject);
	PERF_COUNTER_DEFINITION * FirstCounter (PERF_OBJECT_TYPE *pObjectDef);
	PERF_COUNTER_DEFINITION * NextCounter(PERF_COUNTER_DEFINITION *pCounterDef);
	PERF_INSTANCE_DEFINITION * FirstInstance (PERF_OBJECT_TYPE *pObject);
	PERF_INSTANCE_DEFINITION * NextInstance (PERF_INSTANCE_DEFINITION *pInstance);
*/
  //PVOID CounterData (PERF_INSTANCE_DEFINITION* pInst, PERF_COUNTER_DEFINITION* pCount);





};

#endif // !defined(AFX_PUIFO_H__8C1B853E_8E03_11D3_B6B6_00105A82D2D1__INCLUDED_)
