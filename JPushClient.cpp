#include "JPushClient.h"
#include "JPushPayload.h"
CJPushClient* CJPushClient::_self = NULL;

CJPushClient::CJPushClient(std::string appKey, std::string masterSecret, std::string logFile, int retryTimes)
					:_appKey(appKey), _masterSecret(masterSecret), _logFile(logFile), _retryTimes(retryTimes)
{
	if (_self)
	{
		return;
	}
	_self = this;
}

CJPushClient::~CJPushClient(void)
{
}

CJPushPayload* CJPushClient::push()
{
	return new CJPushPayload(jPushClient);
}
