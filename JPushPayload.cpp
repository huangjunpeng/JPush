#include "JPushPayload.h"
#include <ctime>

//#ifdef WIN32
//	#pragma warning(disable:4996)
//#endif


static std::string _type[3] = {
	"ios",
	"android",
	"winphone"
};

std::string CJPushPayload::_pushUrl = "https://api.jpush.cn/v3/push";
std::string CJPushPayload::_pushValidateUrl = "https://api.jpush.cn/v3/push/validate";
std::vector<std::string> CJPushPayload::_effectiveDeviceTypes(_type, _type + 3);


#define _device CJPushPayload::_effectiveDeviceTypes

static std::string JPushToLower(const std::string& str)
{
	std::string _ptf = str;
	transform(_ptf.begin(), _ptf.end(), _ptf.begin(), ::tolower);
	return _ptf;
}

static bool in_array(const Json::Value& _needle, const Json::Value& _haystack)
{
	for (int i = 0; i < _haystack.size(); i++)
	{
		if (_needle == _haystack[i])
		{
			return true;
		}
	}
	return false;
}
	

CJPushPayload::CJPushPayload(void)
{
	_pJpushClient = NULL;
}

CJPushPayload::CJPushPayload(CJPushClient *pJpushClient)
{
	if (NULL == pJpushClient)
	{
		throw CJPushException("Invalid client value");
	}
	_pJpushClient = pJpushClient;
}

CJPushPayload::~CJPushPayload(void)
{
}

CJPushPayload *CJPushPayload::setPlatform(const Json::Value &platform)
{
	if (platform.isString())
	{
		std::string _ptf = platform.asString();
		_ptf = JPushToLower(_ptf);
		if (_ptf.compare("all") == 0)
		{
			_platform = Json::Value("all");
		}
		else if (std::find(_device.begin(), _device.end(), _ptf) != _device.end())
		{
			_platform.append(Json::Value(_ptf));
		}
	}
	else if (platform.isArray())
	{
		for (int i = 0; i < platform.size(); i++)
		{
			if (!in_array(platform[i], _platform))
			{
				std::string _ptf = JPushToLower(platform[i].asString());
				_platform.append(Json::Value(_ptf));
			}
		}
	}
	return this;
}

CJPushPayload *CJPushPayload::add(Json::Value &jValue, const Json::Value &tag, const std::string& errmsg)
{
	if (tag.isArray())
	{
		for (int i = 0; i < tag.size(); i++)
		{
			if (!tag[i].isString())
			{
				throw CJPushException(errmsg);
			}
			if (!in_array(tag[i], jValue))
			{
				jValue.append(tag[i]);
			}
		}
	}
	else if (tag.isString())
	{
		Json::Value jTag = Json::Value(tag);
		if (!in_array(jTag, jValue))
		{
			jValue.append(jTag);
		}
	}
	else
	{
		throw CJPushException(errmsg);
	}
	return this;
}

CJPushPayload *CJPushPayload::setAudience(const std::string& all)
{
	std::string _all = JPushToLower(all);
	if (_all.compare("all") == 0)
	{
		return addAllAudience();
	}
	else
	{
		throw CJPushException("Invalid audience value");
	}
}

CJPushPayload *CJPushPayload::addWinPhoneNotification(const std::string& alert, const std::string& title, const std::string& _open_page, const Json::Value& extras)
{
	Json::Value winPhone;

	if (alert.empty()) 
	{
		throw CJPushException("Invalid winphone notification");
	}
	winPhone["alert"] = Json::Value(alert);

	if (title.empty() || title.size() <= 0)
	{
		throw CJPushException("Invalid winphone title notification");
	}
	winPhone["title"] = Json::Value(title);

	if (_open_page.empty() || _open_page.size() <= 0)
	{
		throw CJPushException("Invalid winphone _open_page notification");
	}
	winPhone["_open_page"] = Json::Value(_open_page);

	if (!extras.empty())
	{
		if (!extras.isArray())
		{
			throw CJPushException("Invalid winphone extras notification");
		}
		if (extras.size() > 0)
		{
			winPhone["extras"] = extras;
		}
	}

	if (winPhone.size() <= 0)
	{
		throw CJPushException("Invalid winphone notification");
	}

	_winPhoneNotification = winPhone;
	return this;
}

CJPushPayload *CJPushPayload::setSmsMessage(const std::string& content, int delay_time /*= 0*/)
{
	if (content.length() < 480)
	{
		_smsMessage["content"] = Json::Value(content);
	}
	else
	{
		throw CJPushException("Invalid sms content, sms content's length must in [0, 480]");
	}
	_smsMessage["delay_time"] = Json::Value((delay_time == 0 || (delay_time > 0 && delay_time <= 86400)) ? delay_time : 0);
	return this;
}

std::string CJPushPayload::toJSON()
{
	Json::Value payload = build();
	return payload.toStyledString();
}

CJPushHttpResponse *CJPushPayload::sendRequest()
{
	std::string url = _pushUrl;
	return CJPushHttp::http_post(url, toJSON());
}

CJPushHttpResponse *CJPushPayload::validateRequest()
{
	std::string url = _pushValidateUrl;
	return CJPushHttp::http_post(url, toJSON());
}

void CJPushPayload::clearAudience()
{
	_audience = "";
	_tags = Json::Value(NULL);
	_tagAnds = Json::Value(NULL);
	_alias = Json::Value(NULL);
	_registrationIds = Json::Value(NULL);
}

void CJPushPayload::clearNotification()
{
	_notificationAlert = "";
	_iosNotification = Json::Value(NULL);
	_androidNotification = Json::Value(NULL);
	_winPhoneNotification = Json::Value(NULL);
}

void CJPushPayload::clearAll()
{
	clearPlatform();
	clearAudience();
	clearNotification();
	clearMessage();
	clearSmsMessage();
	clearOptions();
}

CJPushPayload *CJPushPayload::iosNotification(const std::string& alert, const Json::Value &notification)
{
	//required_keys = array('sound', 'badge', 'content-available', 'mutable-content', category', 'extras');
	Json::Value ios;
	ios["alert"] = alert;
	if (!notification.empty())
	{
		if (notification.isMember("sound") && notification["sound"].isString())
		{
			ios["sound"] = notification["sound"];
		}
		if (notification.isMember("badge") && notification["badge"].isInt())
		{
			ios["badge"] = notification["badge"];
		}
		if (notification.isMember("content-available") && notification["content-available"].isBool() && notification["content-available"].asBool())
		{
			ios["content-available"] = notification["content-available"];
		}
		if (notification.isMember("mutable-content") && notification["mutable-content"].isBool() && notification["mutable-content"].asBool())
		{
			ios["mutable-content"] = notification["mutable-content"];
		}
		if (notification.isMember("category") && notification["category"].isString())
		{
			ios["category"] = notification["category"];
		}
		if (notification.isMember("extras") && notification["extras"].isObject()  && !notification["extras"].empty())
		{
			ios["extras"] = notification["extras"];
		}
	}
	if (!ios.isMember("sound"))
	{
		ios["sound"] = Json::Value("");
	}
	if (!ios.isMember("badge"))
	{
		ios["badge"] = Json::Value("+1");
	}
	_iosNotification = ios;
	return this;
}


CJPushPayload *CJPushPayload::androidNotification(const std::string& alert, const Json::Value &notification)
{
	//required_keys = array('title', 'build_id', 'extras');
	Json::Value android;
	android["alert"] = alert;
	if (!notification.empty())
	{
		if (notification.isMember("title") && notification["title"].isString())
		{
			android["title"] = notification["title"];
		}
		if (notification.isMember("build_id") && notification["build_id"].isInt())
		{
			android["build_id"] = notification["build_id"];
		}
		if (notification.isMember("extras") && notification["extras"].isObject() && !notification["extras"].empty())
		{
			android["extras"] = notification["extras"];
		}
		if (notification.isMember("priority") && notification["priority"].isInt())
		{
			android["priority"] = notification["priority"];
		}
		if (notification.isMember("category") && notification["category"].isString())
		{
			android["category"] = notification["category"];
		}
		if (notification.isMember("style") && notification["style"].isInt())
		{
			android["style"] = notification["style"];
		}
		if (notification.isMember("big_text") && notification["big_text"].isString())
		{
			android["big_text"] = notification["big_text"];
		}
		if (notification.isMember("inbox") && notification["inbox"].isArray())
		{
			android["inbox"] = notification["inbox"];
		}
		if (notification.isMember("big_pic_path") && notification["big_pic_path"].isString())
		{
			android["big_pic_path"] = notification["big_pic_path"];
		}
	}
	_androidNotification = android;
	return this;
}

CJPushPayload * CJPushPayload::message(const std::string &msg_content, const Json::Value &msg)
{
	//required_keys = array('title', 'content_type', 'extras');
	Json::Value message;
	message["msg_content"] = msg_content;
	if (!msg.empty())
	{
		if (msg.isMember("title") && msg["title"].isString())
		{
			message["title"] = msg["title"];
		}
		if (msg.isMember("content_type") && msg["content_type"].isString())
		{
			message["content_type"] = msg["content_type"];
		}
		if (msg.isMember("extras") && msg["extras"].isArray() && !msg["extras"].empty())
		{
			message["extras"] = msg["extras"];
		}
		_message = message;
	}
	return this;
}

CJPushPayload * CJPushPayload::options(const Json::Value &opts)
{
	//required_keys = array('sendno', 'time_to_live', 'override_msg_id', 'apns_production', 'big_push_duration');
	if (!opts.empty())
	{
		Json::Value options;
		if (opts.isMember("sendno") && opts["sendno"].isInt())
		{
			options["sendno"] = opts["sendno"];
		}
		if (opts.isMember("time_to_live") && opts["time_to_live"].isInt())
		{
			int time_to_live = opts["time_to_live"].asInt();
			if (time_to_live <= 864000 && time_to_live >= 0)
			{
				options["time_to_live"] = opts["time_to_live"];
			}
		}
		if (opts.isMember("override_msg_id") && opts["override_msg_id"].isInt()) {
			options["override_msg_id"] = opts["override_msg_id"];
		}
		if (opts.isMember("apns_production") && opts["apns_production"].isBool()) {
			options["apns_production"] = opts["apns_production"];
		} else {
			options["apns_production"] = false;
		}
		if (opts.isMember("big_push_duration") && opts["big_push_duration"].isInt())
		{
			int big_push_duration = opts["big_push_duration"].asInt();
			if (big_push_duration <= 1400 && big_push_duration >= 0)
			{
				options["timebig_push_duration_to_live"] = opts["big_push_duration"];
			}
		}
		_options = options;
	}
	return this;
}

int CJPushPayload::generateSendno()
{
	srand(time(0));
	return rand() % (max_rand - min_rand) + min_rand;
}

Json::Value CJPushPayload::build()
{
	Json::Value payload;

	// validate platform
	payload["platform"] = Json::Value(_platform);

	// validate audience
	Json::Value audience;
	if (_tags.isArray() && _tags.size() > 0){
		audience["tag"] = _tags;
	}
	if (_tagAnds.isArray() && _tagAnds.size() > 0){
		audience["tag_and"] = _tagAnds;
	}
	if (_alias.isArray() && _alias.size() > 0){
		audience["alias"] = _alias;
	}
	if (_registrationIds.isArray() && _registrationIds.size() > 0){
		audience["tag"] = _registrationIds;
	}
	if (_audience.empty() && audience.size() <= 0){
		throw CJPushException("audience must be set");
	}
	else if (!_audience.empty() && audience.size() > 0){
		throw CJPushException("you can't add tags/alias/registration_id/tag_and when audience='all'");
	}
	else if (_audience.empty()){
		payload["audience"] = audience;
	}
	else{
		payload["audience"] = Json::Value(_audience);
	}

	// validate notification
	Json::Value notification;
	if (!_notificationAlert.empty()) {
		notification["alert"] = Json::Value(_notificationAlert);
	}

	if (!_androidNotification.empty()) {
		notification["android"] = _androidNotification;
		if (!_androidNotification.isMember("alert") || _androidNotification["alert"].isNull()) {
			if (_notificationAlert.empty()) {
				throw CJPushException("Android alert can not be null");
			} else {
				notification["android"]["alert"] = Json::Value(_notificationAlert);
			}
		}
	}

	if (!_iosNotification.empty()) {
		notification["ios"] = _iosNotification;
		if (!_iosNotification.isMember("alert") || _iosNotification["alert"].isNull()) {
			if (_notificationAlert.empty()) {
				throw CJPushException("Android alert can not be null");
			} else {
				notification["ios"]["alert"] = Json::Value(_notificationAlert);
			}
		}
	}

	if (!_winPhoneNotification.empty()) {
		notification["winphone"] = _winPhoneNotification;
		if (!_winPhoneNotification.isMember("alert") || _winPhoneNotification["alert"].isNull()) {
			if (_notificationAlert.empty()) {
				throw CJPushException("Android alert can not be null");
			} else {
				notification["winphone"]["alert"] = Json::Value(_notificationAlert);
			}
		}
	}

	if (notification.size() > 0) {
		payload["notification"] = notification;
	}

	if (_message.size() > 0) {
		payload["message"] = _message;
	}

	if (!payload.isMember("notification") && !payload.isMember("message")) {
		throw CJPushException("notification and message can not all be null");
	}

	if (_smsMessage.size() > 0) {
		payload["sms_message"] = _smsMessage;
	}

	if (_options.size() <= 0) {
		_options["apns_production"] = Json::Value(false);
	}

	payload["options"] = _options;
	return payload;
}