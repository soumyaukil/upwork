// MDVersion.h: interface for the CMDVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDVERSION_H__D28BF392_0A29_11D4_BC84_00105A82D2D1__INCLUDED_)
#define AFX_MDVERSION_H__D28BF392_0A29_11D4_BC84_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*//-------------------------------------------

#ifndef DLLVERSIONINFO
// following is from shlwapi.h, in November 1997 release of the Windows SDK
typedef struct _DllVersionInfo 
{ 
    uint32_t cbSize; 
    uint32_t dwMajorVersion;                   // Major version 
    uint32_t dwMinorVersion;                   // Minor version  
    uint32_t dwBuildNumber;                    // Build number  
    uint32_t dwPlatformID;                     // DLLVER_PLATFORM_* 
} DLLVERSIONINFO;

// Platform IDs for DLLVERSIONINFO
#define DLLVER_PLATFORM_WINDOWS      0x00000001      // Windows 95
#define DLLVER_PLATFORM2_NT           0x00000002      // Windows NT

#endif // DLLVERSIONINFO*/

class CMDVersion: public VS_FIXEDFILEINFO {
protected:
   uint8_t* m_pVersionInfo;   // all version info

   struct TRANSLATION {
      WORD langID;         // language ID
      WORD charset;        // character set (code page)
   } m_translation;

public:
   CMDVersion();
   virtual ~CMDVersion();

   BOOL     GetFileVersionInfo(LPCTSTR modulename);
   CString  GetValue(LPCTSTR lpKeyName);
   static BOOL DllGetVersion(LPCTSTR modulename, DLLVERSIONINFO& dvi);
};

#endif // !defined(AFX_MDVERSION_H__D28BF392_0A29_11D4_BC84_00105A82D2D1__INCLUDED_)
