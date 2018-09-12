#ifndef _JPUSH_CLIENT_H_
#define _JPUSH_CLIENT_H_
#include <string>


#define jPushClient CJPushClient::Instance()

class CJPushPayload;

class CJPushClient
{
public:
	explicit CJPushClient(std::string appKey, std::string masterSecret, std::string logFile = "./jpush.log", int retryTimes = 3);
	~CJPushClient(void);
public:
	inline std::string getAuthStr() const { return _appKey + ":" + _masterSecret; }
	inline int getRetryTimes() const { return _retryTimes; }
	inline std::string getLogFile() const  { return _logFile; }
	CJPushPayload* push();
	static CJPushClient* Instance() { return _self; }
private:
	std::string		_appKey;
	std::string		_masterSecret;
	int				_retryTimes;
	std::string		_logFile;
	static CJPushClient *_self;
};



#endif //_JPUSH_CLIENT_H_