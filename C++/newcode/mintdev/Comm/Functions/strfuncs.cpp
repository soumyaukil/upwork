//#include "stdafx.h"
#include "strfuncs.h"
#include "math.h"
#include <cstring>
#include <cassert>

bool CStrFunc::IsTestSec(const char* secsym)
{
	const char *pdest=strstr(secsym,"TEST");
	
	if (pdest==NULL) {
		int len;
		len=strlen(secsym);
		if (len==5) {
			if (strcmp(secsym+2,"ZZT")==0) return true;
		}
		return false;
	}else {
		return true;
	}
}

char * CStrFunc::priceD2Str(double price,char * str)
{	long intpart;
	double fractpart;
	long numerator;
	long denominator;
	if ((price <0.000001)&&(price >-0.000001)){
		sprintf(str,"0");
	}else {
		intpart=(int)price;
		fractpart=(price -intpart)*256;
		if (fractpart<0) fractpart=-fractpart;
		numerator=(int)fractpart;
		if ((fractpart-numerator)!=0) {
			sprintf(str,"%.10g",price);
		}else {
			if (numerator==0) {
				sprintf(str,"%d",intpart);
			}else {
				denominator=256;
				while((numerator & 1)==0) {
					denominator/=2;
					numerator /=2;
				}
				if (denominator>8) sprintf(str,"%d %d/%d",intpart,numerator,denominator);
				else sprintf(str,"%.10g",price);
			}
		}
	}
	return str;
}

uint32_t CStrFunc::GetASCIINumber(const char* numberbuff, int len)
{
	register uint32_t ret;
	register char cc;
	ret=0;
	for (int i=0;i<len;i++){
//		ret=(ret<<3)+(ret<<1) ; // After test,this can't improve speed! sign;
		ret=ret*10;
		cc=numberbuff[i];
		if ((cc>'0')&&(cc<='9')) {
			cc-='0';
			ret+=cc;
		}
	}
	return ret;
}

void CStrFunc::GetSpaceFilledStr(char * dest, const char * src, int len)
{
	int i;
	for (i=len-1;i>=0;i--){
		if (src[i]!=' ') break;
	}
	i++;
	if (i>0) memcpy(dest,src,i);
	dest[i]=0;
}

char * CStrFunc::priceD2Str(double price)
{
	static char str[100];
	return(priceD2Str(price,str));
}

double CStrFunc::priceStr2Dbl(char *str)
{
	double ret;
	ret=0;
	if (StrIsFraction(str)) {
		int a,b,c;
		a=0;b=c=1;
		if (sscanf(str,"%d %d/%d",&a,&b,&c)==3) {
			ret= a + ((double)b)/c;
		}
	}else {
		ret=0;
		sscanf(str,"%lf",&ret);
	}
	return ret;
}

bool CStrFunc::StrIsFraction(char *str)
{
	int i;
	int len;
	len=strlen(str);
	bool bFraction;
	bFraction=false;
	for (i=0;i<len;i++) {
		if (str[i]=='/') {
			bFraction=true;
			break;
		}
	}
	return bFraction;
}

//this function format the integer to easy to read format.
//such like 1,000,000.

char * CStrFunc::CommaIntVal(long nval, char *str)
{
	long val;
	int i,j;
	bool bFirst;
	i=0;
	int part[10];
	if (nval<0){
		str[i]='-';
		i++;
		val=-nval;
	}else {
		val=nval;
	};
	str[i]=0;
	j=-1;
	while(val){
		j++;
		part[j]=val % 1000;
		val=val/1000;
	}
	bFirst=true;
	while(j>=0) {
		while(str[i]) i++;
		if (bFirst){
			sprintf(str+i,"%d",part[j]);
			bFirst=false;
		}else {
			sprintf(str+i,",%03d",part[j]);
		}

		j--;
	}
	return str;
}

//this function translate the string to int value;
int CStrFunc::GetCommaIntVal(const char* pstr)
{
	int ret;
	int sign;
	char c;
	ret=0;
	sign=0;
	for (int i=0;c=pstr[i];i++) {
		if (c==0) break;
		if ((c>='0')&&(c<='9')) {
			ret*=10;
			ret+=c-'0';
			continue;
		}
		if (c=='-'){
			if (ret==0) sign=1;
			else break;
		}
	}
	if (sign){
		return -ret;
	}else {
		return ret;
	}
}




//this function get the  HH:MM:SS from the string,
//return the second of the day.
int CStrFunc::GetTime_HMS(const char* pstr)
{
	int h,m,s;
	h=m=s=0;
	sscanf(pstr,"%d:%d:%d",&h,&m,&s);
	return h*3600+m*60+s;
}

char * CStrFunc::FormatTime_HMS(int nseconds, char *pbuff)
{
	int h,m,s;
	h=nseconds /3600;
	m=nseconds /60 % 60;
	s=nseconds % 60;

	sprintf(pbuff,"%d:%d:%d", h,m,s);
	return pbuff;
}


/*bool CStrFunc::FillFont(LOGFONT *pfont, const char* lpstr)
{
	int i;
	memset(pfont,0,sizeof(LOGFONT));
	char str[100];
	if (strlen(lpstr)==0) {
		strcpy(str,"Arial,-11,0,400,0,0,1240048");
	}else {
		memccpy(str,lpstr,0,sizeof(str));
	}
	str[sizeof(str)-1]=0;
	i=0;
	char *pstr;
	pstr=strtok(str,",");
	
	if (pstr) {
		memccpy(pfont->lfFaceName,pstr,0,LF_FACESIZE);
		pstr=strtok(NULL,",");
	}else return false;
	
	if (pstr) {
		pfont->lfHeight=atoi(pstr);
		pstr=strtok(NULL,",");
	}else return false;

	if (pstr) {
		pfont->lfWidth=atoi(pstr);
		pstr=strtok(NULL,",");
	}else return false;

	if (pstr){
		pfont->lfWeight=atoi(pstr);
		pstr=strtok(NULL,",");
	}else return false;

	if(pstr){
		pfont->lfItalic=atoi(pstr);
		pstr=strtok(NULL,",");
	}else return false;

	if (pstr){
		pfont->lfUnderline=atoi(pstr);
		pstr=strtok(NULL,",");
	}else return false;

	return true;
}

bool CStrFunc::GetFontString(LOGFONT *pfont, char *str, int size)
{
	char str1[100];
	sprintf(str1,"%.32s,%d,%d,%d,%d,%d,%d",pfont->lfFaceName,
		pfont->lfHeight,pfont->lfWidth,pfont->lfWeight,
		pfont->lfItalic,pfont->lfUnderline);
	memccpy(str,str1,0,size);
	return true;
}*/

//this function break the pstr to sub strings with the tokenchar.
//static function;
bool CStrFunc::BreakStr(char *pstr,char tokenchar, char *substr[], int *size)
{
	int i;
	int nitems;
	int maxsize;
	maxsize=*size;
	assert(maxsize>0);
	nitems=1;
	substr[0]=pstr;
	i=0;
	while (pstr[i]){
		if (pstr[i]==tokenchar) {
			pstr[i]=0;
			substr[nitems]=pstr+i+1;
			nitems++;
			if (nitems>=maxsize) break;
		}
		i++;
	}
	*size=nitems;
	return true;
}

char * CStrFunc::CommaDblVal(double dval, char *str)
{
/*	long val;
	int i,j;
	long nval;
	double fract;
	if (dval>0){
		nval=(long) floor(dval);
		
	}else {
		nval=(long) ceil(dval);
	}
	bool bFirst;
	i=0;
	int part[10];
	if (nval<0){
		str[i]='-';
		i++;
		val=-nval;
	}else {
		val=nval;
	};
	str[i]=0;
	j=-1;
	while(val){
		j++;
		part[j]=val % 1000;
		val=val/1000;
	}
	bFirst=true;
	while(j>=0) {
		while(str[i]) i++;
		if (bFirst){
			sprintf(str+i,"%d",part[j]);
			bFirst=false;
		}else {
			sprintf(str+i,",%03d",part[j]);
		}

		j--;
	}
	double fract;
	fract=nval-dval;
	if (fract!=0) {
		i=strlen(str);
		sprintf(str+i,"%g",fract);
	}
	return str;
*/
	return "????";
}

char* CStrFunc::Trim(char *buf)
{
	char *tmp;
	int  i;

	tmp = buf;
	for( i=strlen(buf)-1; i>=0 && (*(buf+i)==' '||*(buf+i)=='	'||*(buf+i)==13||*(buf+i)==10); i-- ){
		*(buf+i) = 0;
	}

	tmp = buf;
	for( ; (*tmp==' '||*tmp=='	'||*tmp==13||*tmp==10) && strlen(tmp)>0 ; ) {
		tmp++;
	}

	i = strlen(tmp);

	strncpy( buf,tmp,i ); buf[i]=0;

	return buf;
}


char * CStrFunc::TrimFlolatNum(char *buff)
{
	int len;
	len=strlen(buff)-1;
	int i=0;
	bool bfindpoint=false;
	for (i=0;i<=len;i++) {
		if (buff[i]=='.'){
			bfindpoint=true;
			break;
		}
	}
	if (bfindpoint) {
		while(len>=0){
		   if (buff[len]=='.') {
			   buff[len]=0;
			   break;
		   }else if (buff[len]=='0'){
			   buff[len]=0;
			   len--;
		   }else {
			   break;
		   }
		}
	}
	return buff;
}

std::string CStrFunc::FormatFloat(double dbl)
{
	char str[100];
	std::string ret;
	sprintf(str,"%lf",dbl);
	TrimFlolatNum(str);
	ret=str;
	return ret;
}


// ntype =1; change;
// ntype =2; percent;
// ntype =3: change(percent);
int CStrFunc::FormatYClose(double yclose, double last, int ntype, char *pbuff)
{
	int ret;
	pbuff[0]=0;ret=0;
	if ((last>0.00000001)&&(yclose>0.00000001)) {
		if (last>yclose) {
			ret=1;
		}else if (last<yclose) {
			ret=-1;
		}else {
		}
		double changeprecent;
		changeprecent=((last-yclose)/(yclose))*100;
		char strpct[20],strp[20];
		if ((changeprecent>1)||(changeprecent<-1)){
			sprintf(strpct,"%.1f",changeprecent);
		}else {
			sprintf(strpct,"%.2f",changeprecent);
		}

		CStrFunc::TrimFlolatNum(strpct);
		sprintf(strp,"%.3f",last-yclose);
		CStrFunc::TrimFlolatNum(strp);
		switch (ntype){
		case 1:
			sprintf(pbuff,"%s",strp);	
			break;
		case 2:
			sprintf(pbuff,"%s%%",strpct);	
			break;
		case 3:
			sprintf(pbuff,"%s(%s%%)",strp,strpct);	
			break;
		}
	}else {
	}
	return ret;
}


//this function translate option symbol by add + in front.
//if already have plus, then ignore.
bool CStrFunc::TransOptSymb(const char* sfrom, char *sto, bool btoplus)
{
	int i,j;
	if (btoplus) {
		i=0;j=0;
		sto[j]='+'; j++;
		while (sfrom[i]!=0){
			if (sfrom[i]!=' '){
				sto[j]=sfrom[i];
				j++;
			}
			i++;
			sto[j]=0;
		}
	}else {
		for (i=0;sfrom[i];i++);
		i--;
		memset(sto,' ',6);
		sto[6]=0;
		sto[5]=sfrom[i];i--;
		sto[4]=sfrom[i];i--;

		j=i-1;
		while(j>=0){
			sto[j]=sfrom[i];
			j--;i--;
		}
	}
	return true;

}

bool CStrFunc::IsOptionSymb(const char* secsym)
{
	if (secsym[0]=='+') return true;
	return false;

}

char* CStrFunc::price2StrDec(double price, int nfmt)
{
	static char buff[40];
	if (nfmt<=0){
		sprintf(buff,"%.10g",price);
		return buff;
	}
	switch(nfmt) {
	case 3:
		sprintf(buff,"%.3f",price);
		break;
	case 2:
		sprintf(buff,"%.2f",price);
		break;
	case 4:
		sprintf(buff,"%.4f",price);
		break;
	case 1:
		sprintf(buff,"%.1f",price);
		break;
	case 5:
		sprintf(buff,"%.5f",price);
		break;
	default:
		sprintf(buff,"%.10g",price);
		break;
	}	
	return buff;

}

int CStrFunc::price2StrFmt(char *buff,double price, int nfmt)
{

	if (nfmt<=0){
		return sprintf(buff,"%.10g",price);
	}
	switch(nfmt) {
	case 3:
		return sprintf(buff,"%.3f",price);
	case 2:
		return sprintf(buff,"%.2f",price);
	case 4:
		return sprintf(buff,"%.4f",price);
	case 1:
		return sprintf(buff,"%.1f",price);
	case 5:
		return sprintf(buff,"%.5f",price);
	default:
		return sprintf(buff,"%.10g",price);
	}	
	return 0;
}


// this function extract a string from long string and add an 0 char after.
char * CStrFunc::ExtractString(const char* psrc, int len,char *pdesc)
{
	static char retbuff[1024];
	if (pdesc==NULL) 
		pdesc=retbuff;
	int i;
	for (i=0;i<len;i++){
		if (psrc[i]==0) break;
		pdesc[i]=psrc[i];
	}
	pdesc[i]=0;
	return pdesc;
}
// pstr should be big enought.
void CStrFunc::PackIntToStr(int *pint, int num, char *pstr)
{	char *p=pstr;
	*p=0;
	for (int i=0;i<num;i++) {
		sprintf(p,"%d,",pint[i]);
		while(*p) p++;
	}
}
// return how many int already unpacked. pint should be big enought
int CStrFunc::UnpackStrToInt(char *pstr, int *pint)
{
	char *p=pstr;
	int i=0;
	while(*p) {
		sscanf(p,"%d",&pint[i]);
		i++;
		while(*p) {
			if (*p==',') {
				p++;
				break;
			}else {
				p++;
			}
		}
	}
	return i;

}

char * CStrFunc::FormatIPAddr(uint32_t ip, char *pbuff)
{
	if (pbuff==NULL) {
		return "";
	}else {
		sprintf(pbuff,"%d.%d.%d.%d",ip&0xff,(ip>>8)&0xff,(ip>>16)&0xff,(ip>>24)&0xff);
	}
	return pbuff;
}


int CStrFunc::Replace(char *pstr,char oldchar, char newchar)
{
	int i,ret;
	i=0;
	ret=0;
	while(1) {
		char ch=*pstr;
		if (ch==0) break;
		if (ch==oldchar) {
			*pstr=newchar;
			ret++;
		}
		pstr++;
	}
	return ret;
}

//---------------------member functons------------------------------

//this function used to connect strings 
void CStrFunc::InitToSet(char *pbuff)
{
	assert(pbuff!=NULL);
	mpbuff=pbuff;
	mpbuff[0]=0;
	mstrlen=0;
}
//
void CStrFunc::AppendStr(char *pstr)
{
	strcpy(mpbuff+mstrlen,pstr);
	mstrlen+=strlen(pstr);
}


char * CStrFunc::SeekToEnd()
{
	while(mpbuff[mstrlen]) mstrlen++;
	return (mpbuff+mstrlen);
}

void CStrFunc::CutLastChar()
{
	assert(mstrlen!=0);
	mstrlen--;
	mpbuff[mstrlen]=0;
}


//this function used as strtok first string;
void CStrFunc::InitToGet(char *pbuff)
{
	mpbuff  =pbuff;
	mstrlen =0;
}

char * CStrFunc::GetStr(char cDelimit)
{
	if (mpbuff[mstrlen]==0) return NULL;
	char *ret;
	int i;
	i=0;
	ret=mpbuff+mstrlen;
	while(mpbuff[mstrlen]) {
		if (mpbuff[mstrlen]==cDelimit) {
			mpbuff[mstrlen]=0;
			mstrlen++;
			break;
		} 
		mstrlen++;
	}
	return ret;
}

char * CStrFunc::GetLine()
{
	char *ret;
	if (mpbuff[mstrlen]==0) return NULL;
	ret=mpbuff+mstrlen;

	while(mpbuff[mstrlen]) {
		if (mpbuff[mstrlen]=='\xd') {
			mpbuff[mstrlen]=0;
			mstrlen++;
			if (mpbuff[mstrlen]=='\xa') mpbuff++;
			break;
		}
		mstrlen++;
	}
	return ret;
}

#define NCHAR 255
static uint8_t m_ctType[NCHAR+1];
#define CT_END          1
#define CT_WHITESPACE	2
#define CT_DIGIT	3
#define CT_ALPHA	4
#define CT_QUOTE	5
#define CT_COMMENT	6

static void InitParser()
{
	int i;
	for (i=0;i<NCHAR+1;i++) {
		m_ctType[i]=0;
	}
	m_ctType[0]=CT_END;
//Alpha;
	for (i='a';i<='z';i++) m_ctType[i]=CT_ALPHA;
	for (i='A';i<='Z';i++) m_ctType[i]=CT_ALPHA;
	for (i=128;i<=255;i++) m_ctType[i]=CT_ALPHA;
	m_ctType['_']=CT_ALPHA;
//Quote;
	m_ctType['"'] = CT_QUOTE;
	m_ctType['\\'] = CT_QUOTE;
//WhiteSpace;
	for (i=1;i<=' ';i++) m_ctType[i]=CT_WHITESPACE;
//Numbers;
	for (i = '0'; i<= '9'; i++)	m_ctType[i] = CT_DIGIT;
	m_ctType['.']= CT_DIGIT;
};

static int inited=0;


enTokenType CStrFunc::GetToken(char* pret)
{
	if (inited==0){
		InitParser();
		inited=1;
	}

	uint8_t ntype;
	int j;
	j=0;
	
	enTokenType rettoken=TT_Nothing;
	while(1){ // remove the leading spaces;
		ntype =m_ctType[(unsigned char)mpbuff[mstrlen]];
		if (ntype==CT_END){
			pret[0]=0;
			return rettoken;
		}
		if (ntype==CT_WHITESPACE) {
			mstrlen++; 
			continue;
		}
		break;
	};

	while(1) {// get the tokens.
		char c=mpbuff[mstrlen];
		ntype =m_ctType[(unsigned char) c];
		switch(ntype) {
		case CT_ALPHA:
			rettoken=TT_Word;
			pret[j]=mpbuff[mstrlen];
			j++;mstrlen++;
			continue;
		case CT_DIGIT:
			if (rettoken==TT_Nothing) {
				rettoken=TT_Number;
			}
			pret[j]=mpbuff[mstrlen];
			j++;mstrlen++;
			continue;
		case CT_WHITESPACE:
		case CT_END:
			pret[j]=0;
			break;
		default:
			if (j==0) {
				pret[j]=mpbuff[mstrlen];
				mstrlen++;j++;
				pret[j]=0;
				rettoken=TT_Char;
			}else {
				pret[j]=0;
			}
			break;
		}
		break;
	};
	return rettoken;
}

enTokenType CStrFunc::GetToken_Str(char* pret)
{
	if (inited==0){
		InitParser();
		inited=1;
	}

	uint8_t ntype;
	int j;
	j=0;
	
	bool bInQuote=false;
	bool bInSpace=false;
	

	enTokenType rettoken=TT_Nothing;
	while(1) {// get the tokens.
		char c=mpbuff[mstrlen];
		ntype =m_ctType[(unsigned char) c];
		if (bInQuote) {
			if(c == '\"') {
				bInQuote = false;
				mstrlen++;
				pret[j]=0;
				break;
			} else if (c=='\\') {
				mstrlen++;
				pret[j]=mpbuff[mstrlen];
				j++;mstrlen++;
				continue;
			} else {
				pret[j]=c;
				j++;mstrlen++;
				continue;
			}
		}else {
			switch(ntype) {
			case CT_ALPHA:
				rettoken=TT_Word;
				pret[j]=mpbuff[mstrlen];
				j++;mstrlen++;
				continue;
			case CT_DIGIT:
				if (rettoken==TT_Nothing) {
					rettoken=TT_Number;
				}
				pret[j]=mpbuff[mstrlen];
				j++;mstrlen++;
				continue;
			case CT_WHITESPACE:
				if (rettoken==TT_Nothing) {
					mstrlen++;
					continue;
				}else {
					pret[j]=0;
				}
				break;
			case CT_END:
				pret[j]=0;
				break;
			case CT_QUOTE:
				if (c=='"') {
					if (rettoken==TT_Nothing){
						bInQuote=true;
						mstrlen++;
						rettoken=TT_String;
						continue;
					}
				}else if (c=='\\') {
					mstrlen++;
					pret[j]=mpbuff[mstrlen];
					j++;mstrlen++;
				}
				continue;
			default:// for this function , treat everything with out space as one word.
				rettoken=TT_Word;
				pret[j]=mpbuff[mstrlen];
				j++;mstrlen++;
				continue;
/*
				if (j==0) {
					pret[j]=mpbuff[mstrlen];
					mstrlen++;j++;
					pret[j]=0;
					rettoken=TT_Char;
				}else {
					pret[j]=0;
				}
				break;
*/
			}
		}
		break;
	};
	return rettoken;

}

//this function is for TTS, insert space into a string,
//to make it pronounce better.
char* CStrFunc::InsertSpace(const char* psrc, char *pdest)
{
	int i;
	int j=0;
	for (i=0;psrc[i];i++){
		pdest[j]=psrc[i];
		j++;
		pdest[j]=' ';
		j++;
	};
	if (j==0) {
		pdest[j]=0;
	}else {
		pdest[j-1]=0;
	}
	return pdest;

}
