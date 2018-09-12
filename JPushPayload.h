#ifndef _JPUSH_PAYLOAD_H_
#define _JPUSH_PAYLOAD_H_
#include <string>
#include <vector>
#include <algorithm>
#include <json/json.h>
#include "JPushHttp.h"
#include "JPushClient.h"
#include "JPushException.h"

const int max_rand = 4294967294;
const int min_rand = 100000;

class CJPushPayload
{
public:
	explicit CJPushPayload(void);
	explicit CJPushPayload(CJPushClient *pJpushClient);
	~CJPushPayload(void);
public:
	CJPushPayload *setPlatform(const Json::Value &platform);

	inline CJPushPayload *addTag(const Json::Value &tag) { return add(_tags, tag, "Invalid tag value"); }
	inline CJPushPayload *addTagAnd(const Json::Value &tag) { return add(_tagAnds, tag, "Invalid tag_and value"); }
	inline CJPushPayload *addAlias(const Json::Value &alias) { return add(_alias, alias, "Invalid alias value"); }
	inline CJPushPayload *addRegistrationId(const Json::Value &registrationId) 
	{ return add(_registrationIds, registrationId, "Invalid registration_id value"); }

	CJPushPayload *setAudience(const std::string& all);
	inline CJPushPayload *addAllAudience() { _audience = "all"; return this; }

	inline CJPushPayload *setNotificationAlert(const std::string& alert) { _notificationAlert = alert; return this; }
	inline CJPushPayload *addWinPhoneNotification(const std::string& alert, const std::string& title, const std::string& _open_page, const Json::Value& extras);
	CJPushPayload *setSmsMessage(const std::string& content, int delay_time = 0);
	std::string toJSON();
	void printJSON(){}
	CJPushHttpResponse *sendRequest();
	CJPushHttpResponse *validateRequest();
	void clearAudience();
	void clearNotification();
	void clearPlatform() { _platform = Json::Value(NULL); }
	void clearMessage() { _message = ""; }
	void clearSmsMessage() { _smsMessage = Json::Value(NULL); }
	void clearOptions() { _options = Json::Value(NULL); }
	void clearAll();
private:
	int generateSendno();
public:
	CJPushPayload *iosNotification(const std::string &alert, const Json::Value &notification);
	CJPushPayload *androidNotification(const std::string &alert, const Json::Value &notification);
	CJPushPayload *message(const std::string &msg_content, const Json::Value &msg);
	CJPushPayload *options(const Json::Value &opts);
private:
	Json::Value build();
	CJPushPayload * add(Json::Value &jValue, const Json::Value &tag, const std::string& errmsg);
private:
	static std::vector<std::string> _effectiveDeviceTypes;
	static std::string _pushUrl;
	static std::string _pushValidateUrl;

	CJPushClient *_pJpushClient;
	Json::Value _platform;

	std::string _audience;
	Json::Value _tags;
	Json::Value _tagAnds;
	Json::Value _alias;
	Json::Value _registrationIds;

	std::string _notificationAlert;
	Json::Value _iosNotification;
	Json::Value _androidNotification;
	Json::Value _winPhoneNotification;
	Json::Value _smsMessage;
	Json::Value _message;
	Json::Value _options;
};

#endif //_JPUSH_PAYLOAD_H_