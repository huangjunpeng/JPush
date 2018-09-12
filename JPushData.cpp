#include "JPushData.h"

CJPushData_Map::~CJPushData_Map(void)
{
	for (std::map<std::string, CJPushData*>::iterator it = begin(); it != end(); it++)
	{
		CJPushData *pJPushData = it->second;
		if (pJPushData)
		{
			delete pJPushData;
			pJPushData = NULL;
		}
	}
	clear();
}

void CJPushData_Map::ToJson(Json::Value& jOut)
{
	for (std::map<std::string, CJPushData*>::iterator it = begin(); it != end(); it++)
	{
		Json::Value jTemp;
		CJPushData *pJPushData = it->second;
		if (NULL == pJPushData) 
		{
			continue;
		}
		else if (MAP == pJPushData->GetType())
		{
			CJPushData_Map *pMap = (CJPushData_Map*)pJPushData;
			pMap->ToJson(jTemp);
		}
		else if (STRING == pJPushData->GetType())
		{
			CJPushData_String *pString = (CJPushData_String*)pJPushData;
			jTemp = *pString;
		}
		else if (INTER == pJPushData->GetType())
		{
			CJPushData_Int *pInt = (CJPushData_Int*)pJPushData;
			jTemp = pInt->GetValue();
		}
		else
		{
			continue;
		}
		jOut[it->first] = jTemp;
	}
}

CJPushData_Array::~CJPushData_Array(void) 
{
	for (std::vector<CJPushData*>::iterator it = begin(); it != end(); it++)
	{
		CJPushData *pJPushData = *it;
		if (pJPushData)
		{
			delete pJPushData;
			pJPushData = NULL;
		}
	}
	clear();
}
void CJPushData_Array::ToJson(Json::Value& jOut)
{
	for (std::vector<CJPushData*>::iterator it = begin(); it != end(); it++)
	{
		Json::Value jTemp;
		CJPushData *pJPushData = *it;
		if (NULL == pJPushData) 
		{
			continue;
		}
		else if (MAP == pJPushData->GetType() || ARRAY == pJPushData->GetType())
		{
			CToJson *pMap = (CToJson*)pJPushData;
			pMap->ToJson(jTemp);
		}
		else if (STRING == pJPushData->GetType())
		{
			CJPushData_String *pString = (CJPushData_String*)pJPushData;
			jTemp = *pString;
		}
		else if (INTER == pJPushData->GetType())
		{
			CJPushData_Int *pInt = (CJPushData_Int*)pJPushData;
			jTemp = pInt->GetValue();
		}
		else
		{
			continue;
		}
		jOut.append(jTemp);
	}
}

bool operator==(const CJPushData *p, const std::string& v)
{
	if (STRING == p->GetType())
	{
		CJPushData_String *pString = (CJPushData_String*)p;
		if (pString->compare(v) == 0)
		{
			return true;
		}
	}
	return false;
}