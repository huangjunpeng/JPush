#include <iosfwd>
#include <string>
#include <map>
#include "ClassMemberDef.h"

#if !defined(_JPUSH_HTTP_HEADER_H_)
#define _JPUSH_HTTP_HEADER_H_

enum CJPushHttp_Method
{
	HTTP_GET = 1,
	HTTP_POST,
	HTTP_PUT,
	HTTP_DELETE
};

class CInsertTempZero
{
public:
	CInsertTempZero(char* pSrc)
	{
		m_szOld = *pSrc;
		m_pSrc = pSrc;
		*m_pSrc = 0;
	}
	virtual ~CInsertTempZero()
	{
		*m_pSrc = m_szOld;
	}
protected:
private:
	char* m_pSrc;
	char  m_szOld;
};

class CJPushHttpHeader
{
public:
	typedef std::map<std::string, std::string> HttpHeader;
	CJPushHttpHeader(const std::string &strHeader);
	virtual ~CJPushHttpHeader(void);
	void Add(const std::string &key, const std::string &value);
	std::string Get(const std::string &key);
	void Del(const std::string &key);
private:
	int parse(const std::string &strHeader);
	bool parseField(const std::string &strHeader);
	bool parseFirstLine();
private:
	std::string m_strFirstLine;
	HttpHeader m_mapHeader;
	Int32Member(Code);
	StrMember(Proto);
	StrMember(Status);
};

class CJPushHttpResponse
{
public:
	CJPushHttpResponse(int nCode, const std::string &strHeader, const std::string &strBody);
	CJPushHttpHeader *GetHttpHeader();
	Int32Member(Code);
	StrMember(Body);
private:
	CJPushHttpHeader *m_pHeader;
};


#endif //_JPUSH_HTTP_HEADER_H_