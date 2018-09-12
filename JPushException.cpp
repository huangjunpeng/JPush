#include "JPushException.h"

CJPushException::CJPushException(const std::string& _what)
	: exception(_what.c_str())
{
}

CJPushException::~CJPushException(void)
{
}
