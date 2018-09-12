#ifndef _JPUSH_EXECPTION_H_
#define _JPUSH_EXECPTION_H_
#include <exception>
#include <string>
using namespace std;

class CJPushException : public exception
{
public:
	CJPushException(const std::string& _what);
	virtual ~CJPushException(void);
};

#endif //_JPUSH_EXECPTION_H_