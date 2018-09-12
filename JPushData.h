#ifndef _JPUSH_DATA_H_
#define _JPUSH_DATA_H_
#include <string>
#include <map>
#include <utility>
#include <json/value.h>

class CToJson
{
public:
	virtual void ToJson(Json::Value& jOut) = 0;
};


enum CJPushDataType {STRING = 1, MAP, INTER, ARRAY, UNDEFINE};

class CJPushData
{
public:
	explicit CJPushData(void) : _type(UNDEFINE) {}
	explicit CJPushData(const CJPushDataType& type) : _type(type) {}
	virtual ~CJPushData(void){}
	inline void SetType(const CJPushDataType& type) { _type = type; }
	inline CJPushDataType GetType() const { return _type; }
protected:
	CJPushDataType _type;
};

class CJPushData_String : public CJPushData, public std::string
{
public:
	explicit CJPushData_String(void) : CJPushData(STRING), std::string("") {}
	explicit CJPushData_String(const std::string& value) : CJPushData(STRING), std::string(value) {}
	virtual ~CJPushData_String(void) {}
};

class CJPushData_Map : public CJPushData, public std::map<std::string, CJPushData*>, public CToJson
{
public:
	explicit CJPushData_Map(void) : CJPushData(MAP) {}
	explicit CJPushData_Map(const std::map<std::string, CJPushData*>& value) 
		: CJPushData(MAP), std::map<std::string, CJPushData*>(value) {}
	virtual ~CJPushData_Map(void);
	virtual void ToJson(Json::Value& jOut);
};

class CJPushData_Int : public CJPushData
{
public:
	explicit CJPushData_Int(void) : CJPushData(INTER), _value(0) {}
	explicit CJPushData_Int(int value) 
		: CJPushData(INTER), _value(value) {}
	virtual ~CJPushData_Int(void) {}
	inline void SetValue(int value) { _value = value; }
	inline int GetValue() const { return _value; } 
private:
	int _value;
};

class CJPushData_Array : public CJPushData, public std::vector<CJPushData*>, public CToJson
{
public:
	explicit CJPushData_Array(void) : CJPushData(ARRAY) {}
	explicit CJPushData_Array(const std::vector<CJPushData*>& value) 
		: CJPushData(ARRAY), std::vector<CJPushData*>(value) {}
	virtual ~CJPushData_Array(void);
	virtual void ToJson(Json::Value& jOut);
};

bool operator==(const CJPushData *p, const std::string& v);
#endif