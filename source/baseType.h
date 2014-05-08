#ifndef __2014_05_08_BASE_TYPE_H__
#define __2014_05_08_BASE_TYPE_H__

#include "config.h"

extern struct lua_State;

BEGIN_NAMESPACE

class LuaWrapper;
typedef int(*luaRegFunction)(LuaWrapper lua);

// 此结构体必须与 luaL_Reg 的结构保持一致
typedef struct luaLib_Reg {
	const char		*name;
	luaRegFunction	func;
}luaLib_Reg;

END_NAMESPACE

#endif//__2014_05_08_BASE_TYPE_H__