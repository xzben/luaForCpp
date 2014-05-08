#ifndef __2014_05_08_BASE_TYPE_H__
#define __2014_05_08_BASE_TYPE_H__

#include "config.h"

extern struct lua_State;

BEGIN_NAMESPACE

class LuaWrapper;
typedef int(*luaRegFunction)(LuaWrapper lua);

// �˽ṹ������� luaL_Reg �Ľṹ����һ��
typedef struct luaLib_Reg {
	const char		*name;
	luaRegFunction	func;
}luaLib_Reg;

END_NAMESPACE

#endif//__2014_05_08_BASE_TYPE_H__