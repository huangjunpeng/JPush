#include "JPushHttpHeader.h"

CJPushHttpHeader::CJPushHttpHeader(const std::string &strHeader)
{
	if (strHeader.length() > 0)
	{
		parse(strHeader);
	}
}

CJPushHttpHeader::~CJPushHttpHeader(void)
{
}

void CJPushHttpHeader::Add(const std::string &key, const std::string &value)
{
	m_mapHeader[key] = value;
}

std::string CJPushHttpHeader::Get(const std::string &key)
{
	HttpHeader::iterator it = m_mapHeader.find(key);
	if (it == m_mapHeader.end())
	{
		return std::string("");
	}
	return it->second;
}

void CJPushHttpHeader::Del(const std::string &key)
{
	HttpHeader::iterator it = m_mapHeader.find(key);
	if (it != m_mapHeader.end())
	{
		m_mapHeader.erase(it);
	}
}


int CJPushHttpHeader::parse(const std::string &strHeader)
{
	//分离头域字段
	if (!parseField(strHeader))
		return -1;

	//分析第一行的信息
	if(!parseFirstLine())
		return -1;

	return 0;
}

bool CJPushHttpHeader::parseField(const std::string &strHeader)
{
	//获取第一行
	std::string::size_type nPos = strHeader.find("\r\n");
	if (nPos == std::string::npos)
		return false;
	m_strFirstLine = strHeader.substr(0, nPos);

	//解析头域
	std::string strHeaderNoFirstLine = strHeader.substr(nPos + 2);
	const char* ptr = strHeaderNoFirstLine.c_str();
	while (1)
	{
		//是否到了文本区域末尾
		if (memcmp(ptr, "\r\n", 2) == 0)
			break;

		//行末
		char* pLineEnd = const_cast<char*>(strstr(ptr, "\r\n"));
		if (!pLineEnd)
			break;

		CInsertTempZero zLineEnd(pLineEnd);

		//对一行进行分析
		char* p = const_cast<char*>(strstr(ptr, ": "));
		if (!p)
			break;

		//提取头域名和值
		CInsertTempZero zp(p);
		std::string strFieldName = ptr;

		p += strlen(": ");
		std::string strValue = p;

		m_mapHeader[strFieldName] = strValue;

		ptr = pLineEnd + strlen("\r\n");
	}
	return true;
}

bool CJPushHttpHeader::parseFirstLine()
{
	//HTTP/1.1 200 OK

	//协议
	std::string::size_type nPos = m_strFirstLine.find(' ');
	if (nPos == std::string::npos)
		return false;
	m_strProto = m_strFirstLine.substr(0, nPos);

	std::string strFirstLine = m_strFirstLine.substr(nPos + 1);
	nPos = strFirstLine.find(' ');
	if (nPos == std::string::npos)
		return false;

	//状态吗
	std::string strCode = strFirstLine.substr(0, nPos);
	m_nCode = atoi(strCode.c_str());

	//状态
	m_strStatus = strFirstLine.substr(nPos + 1);
	return true;
}

CJPushHttpResponse::CJPushHttpResponse(int nCode, const std::string &strHeader, const std::string &strBody)
	: m_nCode(nCode), m_pHeader(new CJPushHttpHeader(strHeader)), m_strBody(strBody)
{

}

CJPushHttpHeader * CJPushHttpResponse::GetHttpHeader()
{
	return m_pHeader;
}