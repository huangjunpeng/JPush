/****************************************************************************
| Copyright (c) 2018														|
| All rights reserved														|
| ��ǰ�汾��1.0.0.1														|
| ��    �ߣ��ƿ���														|
****************************************************************************/

#include <string>

#if !defined(_CLASS_MEMBER_DEF_H_)
#define _CLASS_MEMBER_DEF_H_


/****************************************************************************
| �궨�幦�ܣ��������Ա�����Լ�GET|SET����								|
| ˵    ����																|
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
| �궨�幦�ܣ�����string��Ա�����Լ�GET|SET����							|
| ˵    ����																|
****************************************************************************/
#if !defined(StrMember)
#define StrMember(variable)		DefineMember(std::string, str, variable)
#endif

/****************************************************************************
| �궨�幦�ܣ�����int��Ա�����Լ�GET|SET����							|
| ˵    ����																|
****************************************************************************/
#if !defined(Int32Member)
#define Int32Member(variable)		DefineMember(int, n, variable)
#endif

/****************************************************************************
| �궨�幦�ܣ�����double��Ա�����Լ�GET|SET����							|
| ˵    ����																|
****************************************************************************/
#if !defined(DoubleMember)
#define DoubleMember(variable) 	DefineMember(double, d, variable)
#endif

/****************************************************************************
| �궨�幦�ܣ�����float��Ա�����Լ�GET|SET����							|
| ˵    ����																|
****************************************************************************/
#if !defined(FloatMember)
#define FloatMember(variable) 	DefineMember(float, f, variable)
#endif

/****************************************************************************
| �궨�幦�ܣ�����unsigned int��Ա�����Լ�GET|SET����					|
| ˵    ����																|
****************************************************************************/
#if !defined(UInt32Member)
#define UInt32Member(variable)	DefineMember(unsigned int, n, variable)
#endif

/****************************************************************************
| �궨�幦�ܣ�����unsigned double��Ա�����Լ�GET|SET����				|
| ˵    ����																|
****************************************************************************/
#if !defined(UDoubleMember)
#define UDoubleMember(variable) DefineMember(unsigned double, d, variable)
#endif

/****************************************************************************
| �궨�幦�ܣ�����unsigned float��Ա�����Լ�GET|SET����					|
| ˵    ����																|
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