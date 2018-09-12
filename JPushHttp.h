#ifndef _JPUSH_HTTP_H_
#define _JPUSH_HTTP_H_
#include <string>
#include "JPushClient.h"
#include "JPushHttpHeader.h"
#include <curl/curl.h>

class CJPushHttp
{
public:
	static CJPushHttpResponse *http_get(const std::string &url);
	static CJPushHttpResponse *http_post(const std::string &url, const std::string &body);
	static CJPushHttpResponse *http_put(const std::string &url, const std::string &body);
	static CJPushHttpResponse *http_delete(const std::string &url);
protected:
	static CJPushHttpResponse *sendRequest(const std::string &url, CJPushHttp_Method method, const std::string& body = "", int times = 1);
	static CJPushHttpResponse *processResp(CURL *pCurl, const std::string &strRes);
};


#endif //_JPUSH_HTTP_H_