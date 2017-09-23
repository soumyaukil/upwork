#include "stdafx.h"
//#include <time.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <stdio.h>
//#include  <io.h>


/*Selecting a subfolder from a particular folder

LPITEMIDLIST ConvertPathToLpItemIdList(const char *pszPath)
{
    LPITEMIDLIST  pidl;
    LPSHELLFOLDER pDesktopFolder;
    OLECHAR       olePath[MAX_PATH];
    ULONG         chEaten;
    ULONG         dwAttributes;
    HRESULT       hr;

    if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
    {
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszPath, -1, olePath,                             MAX_PATH);
        hr = pDesktopFolder->ParseDisplayName(NULL,NULL,olePath,&chEaten,                                              &pidl,&dwAttributes);
        pDesktopFolder->Release();
    }
    return pidl;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{
    switch(uMsg) 
    {
        case BFFM_SELCHANGED: 
        {
           // Set the status window to the currently selected path.
           if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir)) 
              SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
           break;
        }
        default:
           break;
    }
    return 0;
}

int CFoldersDialog::BrowseFolder(HWND hWnd, CString RootPath)
{
    BROWSEINFO bi;
    TCHAR szDir[MAX_PATH];
    LPITEMIDLIST pidl;
    LPMALLOC pMalloc;
    int nRet;

    if (SUCCEEDED(SHGetMalloc(&pMalloc))) 
    {
        nRet = 0;
        ZeroMemory(&bi,sizeof(bi));
        bi.hwndOwner = hWnd;
        bi.pszDisplayName = 0;
        bi.lpszTitle = 0;
        bi.pidlRoot = ConvertPathToLpItemIdList(RootPath);
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
        bi.lpfn = BrowseCallbackProc;

        pidl = SHBrowseForFolder(&bi);
        if (pidl) 
           SHGetPathFromIDList(pidl,szDir);
        else
            nRet = 1;
        pMalloc->Free(pidl); 
        pMalloc->Release();
    }
    return nRet;
}

*/



static LPCSTR pstartdir;

static int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, 
    LPARAM lParam, LPARAM lpData)
{
	char szDir[300];
	switch(uMsg) 
    {
	case BFFM_INITIALIZED:
		if (pstartdir) {
			SendMessage(hwnd, BFFM_SETSELECTION, 1, (long)pstartdir);
		}
		break;
	case BFFM_SELCHANGED: 
		// Set the status window to the currently selected path.
		if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir)) 
			SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
		break;
	default:
		break;
    }
    return 0;
	
};


//the ret buffer must be big enought to save the path
BOOL BrowseForFolder(char *ret,HWND hwnd, LPCSTR title, LPCSTR start) 
{
	LPMALLOC pMalloc = NULL;
    LPITEMIDLIST pidl = NULL;
    LPITEMIDLIST pidlroot = NULL;
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));
	
    // set the bi's default values
    bi.hwndOwner = hwnd;
    bi.lpszTitle = title;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	pstartdir=start;
	bi.lpfn = BrowseCallbackProc;
    pidl = SHBrowseForFolder(&bi);
    if(pidl != NULL)
    {
        SHGetPathFromIDList(pidl, ret);
		SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);  
		pMalloc->Release(); 
		return TRUE;
	}
	return FALSE;
}



static int dirpullto(LPCSTR src,char *desc, int nu) {
	int i,counter;
	i=0; counter=0;
	while(src[i]){
		if ((src[i]=='\\')||(src[i]=='/')){
			counter++;
			if (counter>nu) break;

		}
		desc[i]=src[i];
		i++;
	}
	return i;
}

////0: not exist, 1:dir; 2: file;
//int pathtype(LPCSTR ppath)
//{
//	int eno;
//	struct _stat st;
//	eno=_stat(ppath,&st);
//	if (eno==0) {
//		if (st.st_mode &_S_IFDIR) return 1;
//		return 2;
//	}else {
//		eno=GetLastError();
//			
//		
//
//		return 0;
//	}
//	return 0;
//}

BOOL CheckAndCreateDir(LPCSTR pdir) 
{
	char dir[300];
	char saveddir[300];
	memset(dir,0,sizeof(dir));
	int i,len;
	i=0;
	if (strncmp(pdir,"\\\\",2)==0) {// We have an UNC name
		len=dirpullto(pdir,dir,3); 
	}else if (pdir[0]=='\\'){
		len=dirpullto(pdir,dir,1);
	}else if (pdir[1]==':') {
		len=dirpullto(pdir,dir,1);
	}else {
		len=dirpullto(pdir,dir,0);
	}

	GetCurrentDirectory(sizeof(saveddir),saveddir); 
	while(len) {
		i+=len;
		if (!SetCurrentDirectory(dir)){
			if(!CreateDirectory(dir,NULL)) return FALSE;
			if (!SetCurrentDirectory(dir)) return FALSE;
		};
		len=dirpullto(pdir+i,dir+i,1);
	}
	SetCurrentDirectory(saveddir);
	return TRUE;
}