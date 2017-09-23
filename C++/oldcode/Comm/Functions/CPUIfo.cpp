// PUIfo.cpp: implementation of the CPUIfo class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "CPUIfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





/*******************************************************************
Pointer Functions 
 
    These functions are used to walk down the various structures 
    in the perf data block returned by the call to RegQueryValueEx 
*******************************************************************/
static PPERF_OBJECT_TYPE FirstObject (PPERF_DATA_BLOCK pPerfData)
{
    return ((PPERF_OBJECT_TYPE) ((PBYTE) pPerfData + pPerfData->HeaderLength)) ;
}


static PPERF_OBJECT_TYPE NextObject (PPERF_OBJECT_TYPE pObject)
{                                                 // NextObject
    return ((PPERF_OBJECT_TYPE) ((PBYTE) pObject + pObject->TotalByteLength)) ;
}                                                 // NextObject


static PERF_COUNTER_DEFINITION * FirstCounter (PERF_OBJECT_TYPE *pObjectDef)
{
    return (PERF_COUNTER_DEFINITION *) ((PCHAR) pObjectDef + pObjectDef->HeaderLength);
}


static PERF_COUNTER_DEFINITION * NextCounter(PERF_COUNTER_DEFINITION *pCounterDef)
{
    return (PERF_COUNTER_DEFINITION *) ((PCHAR) pCounterDef + pCounterDef->ByteLength);
}


static PERF_INSTANCE_DEFINITION * FirstInstance (PERF_OBJECT_TYPE *pObject)
{
    return (PERF_INSTANCE_DEFINITION *) ((PBYTE) pObject + pObject->DefinitionLength);

}

static PERF_INSTANCE_DEFINITION * NextInstance (PERF_INSTANCE_DEFINITION *pInstance)
{
    // next instance is after
    //    this instance + this instances counter data
    PERF_COUNTER_BLOCK  *pCtrBlk;

    pCtrBlk = (PERF_COUNTER_BLOCK *) ((PBYTE)pInstance + pInstance->ByteLength);

    return (PERF_INSTANCE_DEFINITION *) ((PBYTE)pInstance + pInstance->ByteLength + pCtrBlk->ByteLength);
}

 
static PVOID CounterData (PERF_INSTANCE_DEFINITION* pInst, PERF_COUNTER_DEFINITION* pCount)
{
    PERF_COUNTER_BLOCK* pCounterBlock;

    if (pCount && pInst)
    {
        pCounterBlock = (PPERF_COUNTER_BLOCK)((PCHAR)pInst + pInst->ByteLength);
        return (PVOID)((PCHAR)pCounterBlock + pCount->CounterOffset);
    }
    else
        return NULL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPUIfo::CPUIfo()
{
	hCPULoadKey=NULL;
	pDataBlock    = NULL;
	lpCounterText = NULL;
    mcpuLoad=0; 
	mLastTick=0; 	
	if ((hCPULoadKey = OpenCPUStatus95 ()) != NULL) {
        StartCPULoadProfiling95 ();
	}else  {// NT system
        if (InitCPUCounter () == FALSE){// for visa it will not working!
            //AfxMessageBox("CPUINFO:Unable to obtain NT CPU counter info", MB_ICONINFORMATION | MB_OK);
		}
    }

}

CPUIfo::~CPUIfo()
{
	if (hCPULoadKey != NULL)
		RegCloseKey (hCPULoadKey);                
	else
		CloseCPUCounter ();


}



// open the registry for windows 95.
HKEY CPUIfo::OpenCPUStatus95()
{
	LONG    result;
    LPCSTR  pDir = "PerfStats\\StatData";
    HKEY    statKey;

    if ((result = RegOpenKey (HKEY_DYN_DATA, pDir , &statKey)) != 0)
        return NULL;  // not a windows 95 system!
    return statKey;
}

void CPUIfo::StartCPULoadProfiling95()
{
   LPCSTR  pDir = "PerfStats\\StartStat";
   HKEY    statKey;
   ULONG   type      = REG_DWORD;
   DWORD   cpuLoad   = 1;
   DWORD   cpuSize   = sizeof (cpuLoad);


   if (RegOpenKey (HKEY_DYN_DATA, pDir , &statKey) != 0)
        return;  // not a windows 95 system!

   if (RegQueryValueEx(statKey, "KERNEL\\CPUUsage", NULL, &type, (LPBYTE) &cpuLoad, &cpuSize) != 0)
  
   RegCloseKey (statKey);                

}

BOOL CPUIfo::InitCPUCounter()
{
    lpCounterText = BuildNameTable (HKEY_LOCAL_MACHINE, DefaultLangId, &dwLastElement);
	return FindCounter (TEXT("Processor"), lpCounterText, dwLastElement,  &lpValueString);
//    return FindCounter (TEXT("System"), lpCounterText, dwLastElement,  &lpValueString);

}


/*******************************************************************
BuildNameTable 
 
    Caches the counter names and explain text to accelerate name lookups 
    for display. 
 
Arguments: 
 
    hKeyRegistry 
        Handle to an open registry (this can be local or remote.) and 
        is the value returned by RegConnectRegistry or a default key. 
 
    lpszLangId 
            The unicode id of the language to look up. (default is 009) 
 
    pdwLastItem 
            The last array element 
 
Return Value: 
 
    pointer to an allocated table. 
    (the caller must free it when finished!) 
 
    the table is an array of pointers to zero terminated TEXT strings. 
 
    A NULL pointer is returned if an error occured. (error value is 
    available using the GetLastError function). 
 
    The structure of the buffer returned is: 
 
        Array of pointers to zero terminated strings consisting of 
            pdwLastItem elements 
 
        MULTI_SZ string containing counter id's and names returned from 
            registry for the specified language 
 
        MULTI_SZ string containing explain text id's 
            and explain text strings 
            as returned by the registry for the specified language 
 
    The structures listed above are contiguous so that they may be freed 
    by a single "free" call when finished with them, however only the 
    array elements are intended to be used. 
*******************************************************************/
LPTSTR * CPUIfo::BuildNameTable(HKEY hKeyRegistry,  // handle to registry db with counter names
                        LPTSTR  lpszLangId,       // unicode value of Language subkey
                        PDWORD  pdwLastItem)      // size of array in elements
{

    LPTSTR  *lpReturnValue;                       // returned pointer to buffer

    LPTSTR  *lpCounterId;                         //
    LPTSTR  lpCounterNames;                       // pointer to Names buffer returned by reg.
    LPTSTR  lpHelpText ;                          // pointet to exlpain buffer returned by reg.

    LPTSTR  lpThisName;                           // working pointer

    BOOL    bStatus;                              // return status from TRUE/FALSE fn. calls
    LONG    lWin32Status;                         // return status from fn. calls


    DWORD   dwHelpItems;                          // number of explain text items
    DWORD   dwCounterItems;                       // number of counter text items
    DWORD   dwValueType;                          // value type of buffer returned by reg.
    DWORD   dwArraySize;                          // size of pointer array in bytes
    DWORD   dwBufferSize;                         // size of total buffer in bytes
    DWORD   dwCounterSize;                        // size of counter text buffer in bytes
    DWORD   dwHelpSize;                           // size of help text buffer in bytes
    DWORD   dwThisCounter;                        // working counter


    DWORD   dwLastId;                             // largest ID value used by explain/counter text

    HKEY    hKeyValue;                            // handle to Perflib entry in registry

    TCHAR   CounterNameBuffer[LINE_LENGTH];       // buffer conatining Counter query
    TCHAR   HelpNameBuffer[LINE_LENGTH];          // buffer conatining Help query
    BOOL    bClosePerformanceKey;                 // flag for closing HKEY_PERFORMANCE_DATA key


    //initialize data
    lpReturnValue        = NULL;
    hKeyValue            = NULL;
    bClosePerformanceKey = FALSE;

    // check for null arguments and insert defaults if necessary

    if (!lpszLangId)
        lpszLangId = DefaultLangId;

    // open handle to Perflib key in registry to get number of items
    // used in order to compute array and buffer sizes
    lWin32Status = RegOpenKeyEx (hKeyRegistry,
                                 NamesKey,
                                 RESERVED,
                                 KEY_READ,
                                 &hKeyValue);

    // on error, free buffers, close keys and return NULL pointer
    if (lWin32Status != ERROR_SUCCESS)
        goto BNT_BAILOUT;

    // query registry to get number of Explain text items
    dwBufferSize = sizeof (dwHelpItems);
    lWin32Status = RegQueryValueEx (hKeyValue,
                                    LastHelp,
                                    RESERVED,
                                    &dwValueType,
                                    (LPBYTE)&dwHelpItems,
                                    &dwBufferSize);

    if ((lWin32Status != ERROR_SUCCESS) || (dwValueType != REG_DWORD))
        goto BNT_BAILOUT;

    // query registry to get number of counter and object name items
    dwBufferSize = sizeof (dwCounterItems);
    lWin32Status = RegQueryValueEx (hKeyValue,
                                    LastCounter,
                                    RESERVED,
                                    &dwValueType,
                                    (LPBYTE)&dwCounterItems,
                                    &dwBufferSize);

    if ((lWin32Status != ERROR_SUCCESS) || (dwValueType != REG_DWORD))
        goto BNT_BAILOUT;

    // use the greater of Help items or Counter Items to size array
    if (dwHelpItems >= dwCounterItems)
        dwLastId = dwHelpItems;
    else
        dwLastId = dwCounterItems;

    // array size is # of elements (+ 1, since names are "1" based)
    // times the size of a pointer
    dwArraySize = (dwLastId + 1) * sizeof(LPTSTR);

    // build the Query strings for the specified language ID
    lstrcpy (CounterNameBuffer, Counters);
    lstrcat (CounterNameBuffer, lpszLangId);

    lstrcpy (HelpNameBuffer, Help);
    lstrcat (HelpNameBuffer, lpszLangId);

    // get size of counter names
    dwBufferSize = 0;
    lWin32Status = RegQueryValueEx (HKEY_PERFORMANCE_DATA,
                                    CounterNameBuffer,
                                    RESERVED,
                                    &dwValueType,
                                    NULL,
                                    &dwBufferSize);

    if (lWin32Status != ERROR_SUCCESS)
        goto BNT_BAILOUT;

    bClosePerformanceKey = TRUE;
    dwCounterSize        = dwBufferSize;

    // get size of help text
    dwBufferSize = 0;
    lWin32Status = RegQueryValueEx (HKEY_PERFORMANCE_DATA,
                                    HelpNameBuffer,
                                    RESERVED,
                                    &dwValueType,
                                    NULL,
                                    &dwBufferSize);


    if (lWin32Status != ERROR_SUCCESS)
        goto BNT_BAILOUT;

    dwHelpSize = dwBufferSize;

    // allocate buffer with room for pointer array, counter name
    // strings and help name strings
    lpReturnValue = (LPTSTR*) malloc (dwArraySize + dwCounterSize + dwHelpSize);

    if (!lpReturnValue)
    {
        lWin32Status = ERROR_OUTOFMEMORY;
        goto BNT_BAILOUT;
    }

    // initialize pointers into buffer
    lpCounterId    = lpReturnValue;
    lpCounterNames = (LPTSTR)((LPBYTE)lpCounterId + dwArraySize);
    lpHelpText     = (LPTSTR)((LPBYTE)lpCounterNames + dwCounterSize);

    // initialize the pointers to NULL
    for (dwBufferSize = 0; dwBufferSize <= dwLastId; dwBufferSize++)
        lpCounterId[dwBufferSize] = NULL;

    // read counter names into buffer. Counter names will be stored as
    // a MULTI_SZ string in the format of "###" "Name"
    dwBufferSize = dwCounterSize;
    lWin32Status = RegQueryValueEx (HKEY_PERFORMANCE_DATA,
                                    CounterNameBuffer,
                                    RESERVED,
                                    &dwValueType,
                                    (LPBYTE)lpCounterNames,
                                    (LPDWORD) &dwBufferSize);

    if (lWin32Status != ERROR_SUCCESS) 
        goto BNT_BAILOUT;

    // read explain text into buffer. Counter names will be stored as

    // a MULTI_SZ string in the format of "###" "Text..."
    dwBufferSize = dwHelpSize;
    lWin32Status = RegQueryValueEx (HKEY_PERFORMANCE_DATA,
                                    HelpNameBuffer,
                                    RESERVED,
                                    &dwValueType,
                                    (LPBYTE)lpHelpText,
                                    &dwBufferSize);

    if (lWin32Status != ERROR_SUCCESS)
        goto BNT_BAILOUT;

    // load counter array items, by locating each text string
    // in the returned buffer and loading the
    // address of it in the corresponding pointer array element.

    for (lpThisName = lpCounterNames;  *lpThisName; lpThisName += (lstrlen(lpThisName)+1) )
    {

        // first string should be an integer
        //       (in decimal digit characters)
        // so translate to an integer for use in array
        //       element identification
        bStatus = StringToInt (lpThisName, &dwThisCounter);

        if (!bStatus)
        {
            // error is in GetLastError
            goto BNT_BAILOUT;                     // bad entry
        }

        // point to corresponding counter name which follows the id number
        // string.
        lpThisName += (lstrlen(lpThisName)+1);


        // and load array element with pointer to string
        lpCounterId[dwThisCounter] = lpThisName;
    }

    // repeat the above for the explain text strings

    for (lpThisName = lpHelpText; *lpThisName; lpThisName += (lstrlen(lpThisName)+1) )
    {

        // first string should be an integer (in decimal unicode digits)
        bStatus = StringToInt (lpThisName, &dwThisCounter);

        if (!bStatus)
        {
            // error is in GetLastError
            goto BNT_BAILOUT;                     // bad entry
        }

        // point to corresponding counter name
        lpThisName += (lstrlen(lpThisName)+1);

        // and load array element;
        lpCounterId[dwThisCounter] = lpThisName;
    }

    // if the last item arugment was used,
    // then load the last ID value in it
    if (pdwLastItem) 
        *pdwLastItem = dwLastId;


    // close the registry keys
    RegCloseKey (hKeyValue);
    RegCloseKey (HKEY_PERFORMANCE_DATA);


    // exit returning the pointer to the buffer
    return lpReturnValue;

    BNT_BAILOUT:
    if (lWin32Status != ERROR_SUCCESS)
    {
        // if lWin32Status has error, then set last error value to it,
        // otherwise assume that last error already has value in it
        SetLastError (lWin32Status);
    }

    if (lpReturnValue)
        free ((LPVOID)lpReturnValue);

    if (bClosePerformanceKey == TRUE) 
        RegCloseKey (HKEY_PERFORMANCE_DATA);

    if (hKeyValue) 
        RegCloseKey (hKeyValue);

    return NULL;

} //buildnametable


/*******************************************************************
StringToInt 
 
    Returns the decimal interpretation of the string in the InStr 
    argument. Works for either Unicode or ANSI strings. 
 
Arguments: 
 
    InStr 
        string to translate 
 
    pdwOutInt 
        base 10 version of the number in the InStr argument 
 
Return Value: 
 
    TRUE if conversion was successful 
    FALSE if not 
*******************************************************************/ 
BOOL CPUIfo::StringToInt(LPTSTR InStr, PDWORD pdwOutInt)
{
 
    // check for NULL pointers passed to function  
    if ((!InStr) || (!pdwOutInt)) 
    { 
        SetLastError (ERROR_BAD_ARGUMENTS); 
        return FALSE; 
    } 
 
    if ((_stscanf (InStr, TEXT(" %d"), pdwOutInt)) == 1) 
        return TRUE; 
    else 
        return FALSE; 

}


/*******************************************************************
    ProcessCommandLine 
 
    Reads arguments in command line and determines 
    what program should do. 
 
    Command line syntax is: 
 
        PERFEX [ ? | Object] 
 
    if "?" is in command line, then list of counter objects is displayed 
 
    if a text string (other than "?") then string is translated to a 
    counter object id and the counters for that object are displayed 
 
Arguments 
 
    lpCmdLine 
 
        null terminated string that contains command line arguments 
 
    lppCounterNames 
 
 
        pointer to array of counter names (used for lookup) 
 
    dwLastCounter 
 
        number of elements in CounterNames array 
 
    lpValueString 
 
        value string to send to ReqQueryValue in order to return counter 
        data for specified counter 
 
ReturnValue 
 
    TRUE    if valid data is returned 
    FALSE   if unable to process command line 
            (command syntax help should be displayed). 
*******************************************************************/

BOOL CPUIfo::FindCounter(LPTSTR lpCmdLine, LPTSTR *lppCounterNames, DWORD dwLastCounter, LPTSTR *lpValueString)
{
    BOOL    rtnValue = FALSE;
    DWORD   dwElem;       
//	return true;
    //compare to counters
    for (dwElem = 0; dwElem <= dwLastCounter; dwElem++)
    {
        // compare counter name to command line
        if (lppCounterNames && lppCounterNames[dwElem])
        {
            if ((lstrcmpi(lppCounterNames[dwElem], lpCmdLine)) == 0)
            {
                // match found
                // make element Number a string and exit
                _stprintf ((LPTSTR)&cValueName[0], TEXT("%d"), dwElem); 
 
                *lpValueString = (LPTSTR)&cValueName[0]; 
                rtnValue = TRUE;
                break;                    // out of for loop
            }                             // endif command line matches string
        }                                 // endif lppCounterNames[dwElem] not equal NULL
    }                                     // end for loop that searches table

    return rtnValue;

}

void CPUIfo::CloseCPUCounter()
{
    // Free memory
    free (pDataBlock);
    pDataBlock  = NULL;

    free (lpCounterText);                     
    lpCounterText = NULL;   

}

void CPUIfo::GetMemDetails(int &realMem, int &pageFile, int &memLoad)
{
    MEMORYSTATUS ms;
    ULONG        type      = REG_DWORD;
//    DWORD        cpuSize   = sizeof (cpuLoad);
    GlobalMemoryStatus(&ms);
    memLoad  = ms.dwMemoryLoad;
    realMem  = 100 - (int) (((float)ms.dwAvailPhys / (float)ms.dwTotalPhys) * 100.0);
    pageFile = 100 - (int) (((float)ms.dwAvailPageFile / (float)ms.dwTotalPageFile) * 100.0);
}

int CPUIfo::GetCPUPrecent()
{
    DWORD dwStatus;                   // return status of fn. calls

    if (lpCounterText == NULL)
        return -1;
//	NdisGetCurrentProcessorCpuUsage();
    dwStatus   = GetSystemPerfData (HKEY_PERFORMANCE_DATA, lpValueString, &pDataBlock);

    if (dwStatus != ERROR_SUCCESS)
    {
        MessageBox (NULL, TEXT("ERROR: Unable to obtain data for System//% Total Processor Time counter"), 
                    TEXT("Query Counter ERROR"), MB_OK | MB_ICONSTOP);

        free   (lpCounterText);
        lpCounterText = NULL;
        return -1;
    }
    
    return CounterData (pDataBlock, lpCounterText, TEXT("System"), TEXT("% Total Processor Time"));

}


/*******************************************************************
GetSystemPerfData 
 
    Allocates data buffer as required and queries performance data 
    specified in pValue from registry. 
 
Arguments 
 
    hKeySystem 
        Handle to performance data in registry 
 
 
    pValue 
        Value string to return from registry 
 
    pPerfData 
        address of pointer to allocated perf data block that is 
        filled in by call to RegQueryValue. 
 
Return Value 
 
    ERROR_SUCCESS if data returned 
    WIN 32 ERROR if error encountered 
*******************************************************************/
DWORD CPUIfo::GetSystemPerfData(HKEY hKeySystem, LPTSTR pValue, PERF_DATA_BLOCK **pPerfData)
{
    LONG     lError ;                             // Win 32 Error returned by fn. calls
    DWORD    size;                                // size of data buffer passed to fn. call
    DWORD    type;                                // type of data buffer returned by fn. call


    // allocate initial buffer if one is not passed into this routine
    if (*pPerfData == NULL)
    {
        if ((*pPerfData = (PERF_DATA_BLOCK*) malloc (INITIAL_SIZE)) == NULL)
            return ERROR_OUTOFMEMORY;
    }

    for(;;)
    {
        size = _msize (*pPerfData);               // query the size of the data buffer

        // get performance data from reg.
        lError = RegQueryValueEx (hKeySystem,
                                  pValue,
                                  RESERVED,
                                  &type,
                                  (LPBYTE)*pPerfData,
                                  &size);

        // check for success and valid perf data block signature
        if ((!lError) && (size > 0) &&
            (*pPerfData)->Signature[0] == (WCHAR)'P' &&
            (*pPerfData)->Signature[1] == (WCHAR)'E' &&
            (*pPerfData)->Signature[2] == (WCHAR)'R' &&
            (*pPerfData)->Signature[3] == (WCHAR)'F' )
        {

            return (ERROR_SUCCESS) ;
        }

        // if buffer is not big enough, reallocate and try again
        if (lError == ERROR_MORE_DATA)
        {	
            *pPerfData = (PERF_DATA_BLOCK*) realloc (*pPerfData, _msize (*pPerfData)+EXTEND_SIZE) ;
            if (*pPerfData == NULL)
                return (lError) ;
        }
        else
            return (lError) ;
    }

}

/*******************************************************************
DisplayCounterData 
 
    Scans the perf data and prints the name of counter and 
    instance found in the list 
 
Arguments 
 
    pPerf 
        pointer to the perf data block returned by the call to 
        RegQueryValueEx 
 
    pNames 
        pointer to the name table containing the array of counter names 
        and explain text pointers. 
 
Return Value 
 
    NONE 
*******************************************************************/
int CPUIfo::CounterData(PERF_DATA_BLOCK *pPerf, LPTSTR *pNames, LPTSTR pSysObject, LPTSTR pCounterRef)
{
    DWORD                       dwThisObject;             // count of objects
    PPERF_OBJECT_TYPE           pThisObject;              // pointer to object structure
	PPERF_INSTANCE_DEFINITION   PerfInst;
    DWORD                       dwThisCounter;            // count of counters
    PPERF_COUNTER_DEFINITION    pThisCounter;
	PPERF_COUNTER_BLOCK PtrToCntr;
//	DWORD i,j;
	int k;
    static DWORD counterDiff = 0;
	static DWORD counterDiff1=0;
	static DWORD LastTick=0;
	static DWORD LastTick1=0;
	DWORD   CurrTick;
	DWORD   TickDiff;
	
// for each object in buffer, display each counter and instance
//	swprintf(st1,TEXT("%d"),pPerf->NumObjectTypes);
//	MessageBox(NULL,st1,TEXT("  "),0);
    for (dwThisObject = 0, pThisObject = FirstObject (pPerf); dwThisObject < pPerf->NumObjectTypes; dwThisObject++, pThisObject = NextObject(pThisObject)) {         
        // if there are instances, then loop through all instances
//	  swprintf(st1,TEXT("%d"),dwThisObject);
//      MessageBox(NULL,pNames[pThisObject->ObjectNameTitleIndex],st1,0);
 
		if ((lstrcmpi(pNames[pThisObject->ObjectNameTitleIndex],TEXT("Processor"))) == 0)  {
			//MessageBox(NULL,TEXT("Find Processer item"),TEXT("tmp"),0) ;
			if (pThisObject->NumInstances >0 ) { // has more than one instince
				PerfInst=FirstInstance(pThisObject);
				for (k=0;k<pThisObject->NumInstances;k++, PerfInst=NextInstance(PerfInst)) {
					PtrToCntr=(PPERF_COUNTER_BLOCK)((PBYTE)PerfInst+PerfInst->ByteLength); 
					if (lstrcmpi((LPTSTR)((PBYTE)PerfInst+PerfInst->NameOffset),TEXT("0"))==0 ) { // the first processor 's CPU Time
						
						pThisCounter = FirstCounter (pThisObject);
						for (dwThisCounter = 0;  dwThisCounter < pThisObject->NumCounters;  dwThisCounter++, pThisCounter = NextCounter(pThisCounter)) {            
							if ((lstrcmpi(pNames[pThisCounter->CounterNameTitleIndex], TEXT("% Processor Time")) == 0)){
								//PPERF_COUNTER_BLOCK PtrToCntr = (PPERF_COUNTER_BLOCK) ((BYTE*)pThisObject + (pThisObject->DefinitionLength));                                          
								DWORD               counter;
								int                 cpuLoad;
								
								counter     = *(DWORD*) ((BYTE*)PtrToCntr + pThisCounter->CounterOffset);
								CurrTick=GetTickCount();
								TickDiff=CurrTick- LastTick;
								if (TickDiff> 500) { 
									cpuLoad     = 100 - (int)((counter -counterDiff)/TickDiff/100);
									counterDiff1=counterDiff;
									counterDiff = counter;
									LastTick1 =LastTick;
									LastTick=CurrTick;
								} else {
									cpuLoad     = 100 - (int)((counter -counterDiff1)/(CurrTick - LastTick1)/100);
								//	return -1;	
								}
								return cpuLoad;
							}
						}
					}
				}
			}// has no instince
        }
	}

    return -1;
/*
    DWORD                       dwThisObject;             // count of objects
    PPERF_OBJECT_TYPE           pThisObject;              // pointer to object structure
    DWORD                       dwThisCounter;            // count of counters
    PPERF_COUNTER_DEFINITION    pThisCounter;
    static DWORD counterDiff = 0;
	static DWORD counterDiff1=0;
	static DWORD LastTick=0;
	static DWORD LastTick1=0;
	DWORD   CurrTick;
	DWORD   TickDiff;
    // for each object in buffer, display each counter and instance

    for (dwThisObject = 0, pThisObject = FirstObject (pPerf); dwThisObject < pPerf->NumObjectTypes; dwThisObject++, pThisObject = NextObject(pThisObject))
    {        
        // if there are instances, then loop through all instances
        if ((lstrcmpi(pNames[pThisObject->ObjectNameTitleIndex],pSysObject)) == 0)
        {
            for (dwThisCounter = 0,  pThisCounter = FirstCounter (pThisObject);  dwThisCounter < pThisObject->NumCounters;  dwThisCounter++, pThisCounter = NextCounter(pThisCounter))
            {            
                if ((lstrcmpi(pNames[pThisCounter->CounterNameTitleIndex], pCounterRef) == 0))
                {
                    PPERF_COUNTER_BLOCK PtrToCntr = (PPERF_COUNTER_BLOCK) ((BYTE*)pThisObject + (pThisObject->DefinitionLength));                                          
                    DWORD               counter;
                    int                 cpuLoad;
					
                    counter     = *(DWORD*) ((BYTE*)PtrToCntr + pThisCounter->CounterOffset);
					CurrTick=GetTickCount();
					TickDiff=CurrTick- LastTick;
					if (TickDiff> 500) { 
						cpuLoad     = 100 - (int)((counter -counterDiff)/TickDiff/100);
						counterDiff1=counterDiff;
						counterDiff = counter;
						LastTick1 =LastTick;
						LastTick=CurrTick;
					} else {
						cpuLoad     = 100 - (int)((counter -counterDiff1)/(CurrTick - LastTick1)/100);
					//	return -1;	
					}
                    return cpuLoad;
                }
            }
        }
    }
    return -1;
*/
}

/*
PERF_OBJECT_TYPE CPUIfo::FirstObject (PPERF_DATA_BLOCK pPerfData)
{
    return ((PPERF_OBJECT_TYPE) ((PBYTE) pPerfData + pPerfData->HeaderLength)) ;
}


PPERF_OBJECT_TYPE CPUIfo::NextObject (PPERF_OBJECT_TYPE pObject)
{                                                 // NextObject
    return ((PPERF_OBJECT_TYPE) ((PBYTE) pObject + pObject->TotalByteLength)) ;
}                                                 // NextObject


PERF_COUNTER_DEFINITION * CPUIfo::FirstCounter (PERF_OBJECT_TYPE *pObjectDef)
{
    return (PERF_COUNTER_DEFINITION *) ((PCHAR) pObjectDef + pObjectDef->HeaderLength);
}


PERF_COUNTER_DEFINITION * CPUIfo::NextCounter(PERF_COUNTER_DEFINITION *pCounterDef)
{
    return (PERF_COUNTER_DEFINITION *) ((PCHAR) pCounterDef + pCounterDef->ByteLength);
}


PERF_INSTANCE_DEFINITION * CPUIfo::FirstInstance (PERF_OBJECT_TYPE *pObject)
{
    return (PERF_INSTANCE_DEFINITION *) ((PBYTE) pObject + pObject->DefinitionLength);

}

PERF_INSTANCE_DEFINITION * CPUIfo::NextInstance (PERF_INSTANCE_DEFINITION *pInstance)
{
    // next instance is after
    //    this instance + this instances counter data
    PERF_COUNTER_BLOCK  *pCtrBlk;

    pCtrBlk = (PERF_COUNTER_BLOCK *) ((PBYTE)pInstance + pInstance->ByteLength);

    return (PERF_INSTANCE_DEFINITION *) ((PBYTE)pInstance + pInstance->ByteLength + pCtrBlk->ByteLength);
}
*/
/*
PVOID CounterData (PERF_INSTANCE_DEFINITION* pInst, PERF_COUNTER_DEFINITION* pCount)
{
    PERF_COUNTER_BLOCK* pCounterBlock;

    if (pCount && pInst)
    {
        pCounterBlock = (PPERF_COUNTER_BLOCK)((PCHAR)pInst + pInst->ByteLength);
        return (PVOID)((PCHAR)pCounterBlock + pCount->CounterOffset);
    }
    else
        return NULL;
}
*/

int CPUIfo::GetMemPrecent()
{
    MEMORYSTATUS ms;
	int ret;
//    DWORD        cpuSize   = sizeof (cpuLoad);
    GlobalMemoryStatus(&ms);
    ret= 100 - (int) (((float)ms.dwAvailPhys / (float)ms.dwTotalPhys) * 100.0);
    return ret;
}
