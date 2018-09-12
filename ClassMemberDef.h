/****************************************************************************
| Copyright (c) 2018														|
| All rights reserved														|
| 当前版本：1.0.0.1														|
| 作    者：黄俊朋														|
****************************************************************************/

#include <string>

#if !defined(_CLASS_MEMBER_DEF_H_)
#define _CLASS_MEMBER_DEF_H_


/****************************************************************************
| 宏定义功能：定义类成员变量以及GET|SET方法								|
| 说    明：																|
****************************************************************************/
#if !defined(DefineMember)
#define DefineMember(type, prefix, variable)						\
public:																\
	inline void Set##variable(const type& prefix##variable)			\
	{																\
		m_##prefix##variable = prefix##variable;					\
	}																\
	inline type Get##variable() const								\
	{																\
		return m_##prefix##variable;								\
	}																\
private:															\
	type m_##prefix##variable
#endif


/****************************************************************************
| 宏定义功能：定义string成员变量以及GET|SET方法							|
| 说    明：																|
****************************************************************************/
#if !defined(StrMember)
#define StrMember(variable)		DefineMember(std::string, str, variable)
#endif

/****************************************************************************
| 宏定义功能：定义int成员变量以及GET|SET方法							|
| 说    明：																|
****************************************************************************/
#if !defined(Int32Member)
#define Int32Member(variable)		DefineMember(int, n, variable)
#endif

/****************************************************************************
| 宏定义功能：定义double成员变量以及GET|SET方法							|
| 说    明：																|
****************************************************************************/
#if !defined(DoubleMember)
#define DoubleMember(variable) 	DefineMember(double, d, variable)
#endif

/****************************************************************************
| 宏定义功能：定义float成员变量以及GET|SET方法							|
| 说    明：																|
****************************************************************************/
#if !defined(FloatMember)
#define FloatMember(variable) 	DefineMember(float, f, variable)
#endif

/****************************************************************************
| 宏定义功能：定义unsigned int成员变量以及GET|SET方法					|
| 说    明：																|
****************************************************************************/
#if !defined(UInt32Member)
#define UInt32Member(variable)	DefineMember(unsigned int, n, variable)
#endif

/****************************************************************************
| 宏定义功能：定义unsigned double成员变量以及GET|SET方法				|
| 说    明：																|
****************************************************************************/
#if !defined(UDoubleMember)
#define UDoubleMember(variable) DefineMember(unsigned double, d, variable)
#endif

/****************************************************************************
| 宏定义功能：定义unsigned float成员变量以及GET|SET方法					|
| 说    明：																|
****************************************************************************/
#if !defined(UFloatMember)
#define UFloatMember(variable) 	DefineMember(unsigned float, f, variable)
#endif

#if !defined(_delete)
#define _delete(c)  \
	if (NULL != c)  \
	{				\
		delete c;   \
		c = NULL;   \
	}
#endif


#endif //