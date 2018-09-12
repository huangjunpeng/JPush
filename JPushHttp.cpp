#include "JPushHttp.h"
#include "JPushClient.h"
#include "JPushException.h"
#include <iostream>

CJPushHttpResponse *CJPushHttp::http_get(const std::string &url)
{
	return sendRequest(url, HTTP_GET);
}

CJPushHttpResponse *CJPushHttp::http_post(const std::string &url, const std::string &body)
{
	return sendRequest(url, HTTP_POST, body);
}

CJPushHttpResponse *CJPushHttp::http_put(const std::string &url, const std::string &body)
{
	return sendRequest(url, HTTP_PUT, body);
}

CJPushHttpResponse *CJPushHttp::http_delete(const std::string &url)
{
	return sendRequest(url, HTTP_DELETE);
}

static size_t reslut(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	std::string *version = (std::string*)userdata;
	version->append((char*)ptr, size * nmemb);
	return (size * nmemb);
}

void FreeCurlHandel(CURL *pCurl, curl_slist *pHeaders)
{
	if (pHeaders != NULL)
	{
		curl_slist_free_all(pHeaders);
	}

	if (pCurl != NULL)
	{
		curl_easy_cleanup(pCurl);
	}
}

#ifndef CURL_HTTP_VERSION_2_0
#define CURL_HTTP_VERSION_2_0 3
#endif

CJPushHttpResponse *CJPushHttp::sendRequest(const std::string& url, CJPushHttp_Method method, const std::string& body, int times /*= 1*/)
{
	CURL *pCurl = curl_easy_init();
	curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
	//curl_easy_setopt(pCurl, CURLOPT_RETURNTRANSFER, true);
	curl_easy_setopt(pCurl, CURLOPT_HEADER, true);
	curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "JPush-API-PHP-Client");
	curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 20);  // 连接建立最长耗时
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 120);  // 请求最长耗时

	// 设置SSL版本
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

	// 设置Basic认证
	curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_easy_setopt(pCurl, CURLOPT_USERPWD, jPushClient->getAuthStr().c_str());
	curl_easy_setopt(pCurl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

	//数据读取设置
	std::string strResult;
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, reslut);	//设置处理数据的函数
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &strResult);	

	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);

	// 设置Post参数
	if (method == HTTP_POST) 
	{
		curl_easy_setopt(pCurl, CURLOPT_POST, true);
	} 
	else if (method == HTTP_DELETE) 
	{
		curl_easy_setopt(pCurl, CURLOPT_CUSTOMREQUEST, "DELETE");
	}
	else if (method == HTTP_PUT)
	{
		curl_easy_setopt(pCurl, CURLOPT_CUSTOMREQUEST, "PUT");
	}
	if (!body.empty()) {
		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, body.c_str());
	}

	curl_slist *pHeaders = NULL;
	pHeaders = curl_slist_append(pHeaders, "Content-Type: application/json");
	pHeaders = curl_slist_append(pHeaders, "Connection: Keep-Alive");
	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pHeaders);

	//响应信息
	CJPushHttpResponse *pJPushHttpResponse = NULL;

	//执行
	CURLcode code = curl_easy_perform(pCurl);
	if(CURLE_OK != code){
		if (times < jPushClient->getRetryTimes())
		{
			pJPushHttpResponse = sendRequest(url, method, body, ++times);
		}
		else
		{
			FreeCurlHandel(pCurl, pHeaders);
			if (code == 28) 
			{
				throw CJPushException("Response timeout. Your request has probably be received by JPush Server,please check that whether need to be pushed again.");
			} 
			else if (code == 56) 
			{
				throw CJPushException("Response timeout, maybe cause by old CURL version. Your request has probably be received by JPush Server, please check that whether need to be pushed again.");
			} 
			else 
			{
				char chBuf[1024] = {0};
				sprintf(chBuf, "curl_easy_perform failed, code=%d, url=%s", code, url.c_str());
				throw CJPushException(chBuf);
			}
		}
	}
	else
	{
		pJPushHttpResponse = processResp(pCurl, strResult);
	}
	FreeCurlHandel(pCurl, pHeaders);
	return pJPushHttpResponse;
}

CJPushHttpResponse * CJPushHttp::processResp(CURL *pCurl, const std::string &strRes)
{
	long nCode = 0;
	CURLcode code = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE , &nCode);
	if((CURLE_OK == code) && nCode)
	{
		std::cout << "状态吗:" << nCode << std::endl;
	}

	long headerSize = 0;
	code = curl_easy_getinfo(pCurl, CURLINFO_HEADER_SIZE , &headerSize);
	if((CURLE_OK == code) && headerSize)
	{
		std::cout << "响应头大小:" << headerSize << "字节" << std::endl;
	}
	
	std::string strHeader = strRes.substr(0, headerSize);
	{
		std::cout << "响应头:\r\n" << strHeader << std::endl;
	}

	std::string strBody = strRes.substr(headerSize);
	{
		std::cout << "响应：\r\n" << strBody << std::endl;
	}

	return new CJPushHttpResponse(nCode, strHeader, strBody);
}
