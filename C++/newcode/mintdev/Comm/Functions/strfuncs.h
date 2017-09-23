#pragma once

#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <string>

//	this class is just for package all the globe functions
//	most of the functions is static

enum enTokenType{
	TT_Nothing=0,
	TT_Word,
	TT_Number,
	TT_Char,
	TT_String,
};

class CStrFunc{
public:
	//no constructor and destructor
	char *mpbuff;//the buffer of the string  
	int  mstrlen;//the length of the string
public:
	void InitToSet(char *pbuff);
	char * SeekToEnd();
	void AppendStr(char *pstr);
	void CutLastChar();
public:
	
	void InitToGet(char *pbuff);
	char * GetStr( char cDelimit);
	char * GetLine();
	enTokenType GetToken(char* pret);
	enTokenType GetToken_Str(char* pret);

public:
	//this function transfer ASCIINumber to number;
	static uint32_t GetASCIINumber(const char* numberbuff,int len);
	//this function extract a substring from src, trim the following spaces.
	static void GetSpaceFilledStr(char * dest, const char * src, int len);
	static char * FormatIPAddr(uint32_t ip, char* pbuff);
	static int FormatYClose(double yclose,double last,int ntype,char *pbuff);
	static std::string FormatFloat(double dbl);

	static char * FormatTime_HMS(int nseconds, char *pbuff);
	static int	  GetTime_HMS(const char* pstr);
	
public:
	//this function insert space into a string, for TTS(Text to speach)
	static char* InsertSpace(const char* psrc, char *pdest);
	static int UnpackStrToInt(char *pstr,int *pint);
	static void PackIntToStr(int *pint, int num, char *pstr);


	static char * ExtractString(const char* psrc, int len,char *pdesc=NULL);
	static char* price2StrDec(double price,int nfmt);
	static int   price2StrFmt(char *buff,double price, int nfmt);
	static bool IsOptionSymb(const char* secsym);
	static bool TransOptSymb(const char* sfrom, char *sto, bool btoplus);
	static char * TrimFlolatNum(char *buff);
	static char* Trim(char *buf);
	static char * CommaIntVal(long nval, char * str);
	static char * CommaDblVal(double dval,char *str);
	static int GetCommaIntVal(const char* pstr);//this function translate the string to int value;
	static bool BreakStr(char *pstr,char tokenchar,char *substr[], int *size);
	//static bool GetFontString(LOGFONT *pfont, char *str, int size);
	//static bool FillFont(LOGFONT *font,const char* lpstr);
	static bool StrIsFraction(char *str);
	static double priceStr2Dbl(char *str);
	static char * priceD2Str( double price);
	static char * priceD2Str(double price,char * str);
	static bool IsTestSec(const char* secsym);

	static int Replace(char *pstr,char oldchar, char newchar);
};
