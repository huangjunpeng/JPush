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
	//����ͷ���ֶ�
	if (!parseField(strHeader))
		return -1;

	//������һ�е���Ϣ
	if(!parseFirstLine())
		return -1;

	return 0;
}

bool CJPushHttpHeader::parseField(const std::string &strHeader)
{
	//��ȡ��һ��
	std::string::size_type nPos = strHeader.find("\r\n");
	if (nPos == std::string::npos)
		return false;
	m_strFirstLine = strHeader.substr(0, nPos);

	//����ͷ��
	std::string strHeaderNoFirstLine = strHeader.substr(nPos + 2);
	const char* ptr = strHeaderNoFirstLine.c_str();
	while (1)
	{
		//�Ƿ����ı�����ĩβ
		if (memcmp(ptr, "\r\n", 2) == 0)
			break;

		//��ĩ
		char* pLineEnd = const_cast<char*>(strstr(ptr, "\r\n"));
		if (!pLineEnd)
			break;

		CInsertTempZero zLineEnd(pLineEnd);

		//��һ�н��з���
		char* p = const_cast<char*>(strstr(ptr, ": "));
		if (!p)
			break;

		//��ȡͷ������ֵ
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

	//Э��
	std::string::size_type nPos = m_strFirstLine.find(' ');
	if (nPos == std::string::npos)
		return false;
	m_strProto = m_strFirstLine.substr(0, nPos);

	std::string strFirstLine = m_strFirstLine.substr(nPos + 1);
	nPos = strFirstLine.find(' ');
	if (nPos == std::string::npos)
		return false;

	//״̬��
	std::string strCode = strFirstLine.substr(0, nPos);
	m_nCode = atoi(strCode.c_str());

	//״̬
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