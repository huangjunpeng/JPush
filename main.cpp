#include <iostream>
#include "JPushClient.h"
#include "JPushPayload.h"

CJPushClient g_JPushClient("appKey", "masterSecret");

void main()
{
	Json::Value alias, ios, android, options, content;

	//alias
	alias.append(Json::Value("137"));

	//content
	content["title"] = Json::Value("time: 2018-09-11 I love you[20170810161701]");
	content["t_id"]  = Json::Value("101");
	content["type"]  = Json::Value(4);
	content["number"] = Json::Value(100);


	//ios
	ios["sound"]				= Json::Value(1107);
	ios["badge"]				= Json::Value(100);
	ios["category"]				= Json::Value("jiguang");
	ios["extras"];
	ios["extras"]["work"]		= content;

	//android
	android["title"]			= Json::Value("I love you");
	android["extras"];
	android["extras"]["badge"]	= Json::Value(100);
	android["extras"]["work"]	= content;

	//options
	options["time_to_live"]		= Json::Value(10 * 24 * 60 * 60);
	options["apns_production"]	= Json::Value(0);

	try 
	{
		CJPushPayload *pJPushPayload = jPushClient->push()
			->setPlatform(Json::Value("all"))
			->addAlias(alias);

		std::string alert = "I love you";
		pJPushPayload->setNotificationAlert(alert)
			->iosNotification(alert, ios)
			->androidNotification(alert, android)
			->options(options);
		std::string szJson = pJPushPayload->toJSON();
		std::cout << szJson.c_str() << std::endl;
		CJPushHttpResponse* pJPushHttpResponse = pJPushPayload->sendRequest();
		_delete(pJPushHttpResponse);
		_delete(pJPushPayload);
		Sleep(1000);
	}
	catch(CJPushException e)
	{
		std::cout << e.what() << std::endl;
	}
}