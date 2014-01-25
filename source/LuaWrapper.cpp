#include "LuaWrapper.h"
#include <cassert>

LuaWrapper::LuaWrapper()
{
	m_luaState = luaL_newstate();
	assert(NULL != m_luaState);
}

LuaWrapper::~LuaWrapper()
{
	  
}



bool LuaWrapper::loadString(const char* szLuaScript, bool bCall /*= true*/)
{
	int nErr;
	if(LUA_OK != (nErr = luaL_loadstring(m_luaState, szLuaScript)))
	{
		assert(false);
		luaL_error (m_luaState, "load string( %s ) failed error( %d ).", szLuaScript, nErr);
		return false;
	}
	if( bCall && lua_pcall(m_luaState, 0, 0, 0))
	{
		assert(false);
		luaL_error (m_luaState, "call string( %s ) chuck failed.", szLuaScript);
		return false;
	}
	return true;
}

bool LuaWrapper::loadFile(const char* szLuaFile, bool bCall /*= true*/)
{
	if (luaL_loadfile(m_luaState, szLuaFile))
	{
		assert(false);
		luaL_error (m_luaState, "load file( %s ) failed.", szLuaFile);
		return false;
	}
	if( bCall && lua_pcall(m_luaState, 0, 0, 0))
	{
		assert(false);
		luaL_error (m_luaState, "call file( %s ) chuck failed.", szLuaFile);
		return false;
	}
	return true;
}


template<>
bool LuaWrapper::getGloabal<>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if(lua_isboolean(m_luaState, -1))
	{
		bool bRet = (bool)lua_toboolean(m_luaState, -1);
		lua_pop(m_luaState, 1);
		return bRet;
	}

	return false;
}

template<>
int LuaWrapper::getGloabal<int>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if(lua_isnumber(m_luaState, -1))
	{
		int nRet = (int)lua_tonumber(m_luaState, -1);
		lua_pop(m_luaState, 1);
		return nRet;
	}

	return 0;
}

template<>
double LuaWrapper::getGloabal<double>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if(lua_isnumber(m_luaState, -1))
	{
		double dRet = (double)lua_tonumber(m_luaState, -1);
		lua_pop(m_luaState, 1);
		return dRet;
	}

	return 0.0;
}

template<>
std::string LuaWrapper::getGloabal<std::string>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if(lua_isstring(m_luaState, -1))
	{
		std::string strRet = lua_tostring(m_luaState, -1);
		lua_pop(m_luaState, 1);
		return strRet;
	}

	return "";
}

template<>
bool LuaWrapper::setGloabal<bool>(const char* szGlobalVariableName, bool Value)
{
	lua_pushboolean(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);
	
	return false;
}

template<>
bool LuaWrapper::setGloabal<int>(const char* szGlobalVariableName, int Value)
{
	lua_pushnumber(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);
	return false;
}

template<>
bool LuaWrapper::setGloabal<double>(const char* szGlobalVariableName, double Value)
{
	lua_pushnumber(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);
	return false;
}

template<>
bool LuaWrapper::setGloabal<char*>(const char* szGlobalVariableName, char* Value)
{
	lua_pushstring(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);
	return false;
}

template<>
bool LuaWrapper::setGloabal<std::string>(const char* szGlobalVariableName, std::string Value)
{
	lua_pushstring(m_luaState, Value.c_str());
	lua_setglobal(m_luaState, szGlobalVariableName);
	return false;
}

bool LuaWrapper::callLuaFunc(const char* szFuncName, const char* sig, ...)
{
	va_list vl;
	int narg, nres;	/* 记录函数参数个数和返回值个数 */
	bool bRet = false;
	int nTopIndex = lua_gettop(m_luaState); //记住call 函数前栈中元素index，call结束后恢复
	va_start(vl, sig);

	lua_getglobal(m_luaState, szFuncName); /* 将函数压入栈 */

	if( !lua_isfunction(m_luaState, -1))
		goto endcall;
	
	/* push arguments */
	narg = 0;
	while(*sig)
	{
		switch(*sig++) /* push arguments */
		{
		case 'd': /* double argument */
			lua_pushnumber(m_luaState, va_arg(vl, double));
			break;
		case 'i': /* int argument */
			lua_pushnumber(m_luaState, va_arg(vl, int));
			break;
		case 's': /* string argument */
			lua_pushstring(m_luaState, va_arg(vl, char *));
			break;
		case '>':
			goto endwhile;
		default:
			assert(false);
			goto endcall;
		}
		narg++;
		luaL_checkstack(m_luaState, 1, "too many arguments");
	} endwhile:

	/* do the call */
	nres = strlen(sig); /* number of expected results */
	if (lua_pcall(m_luaState, narg, nres, 0) != 0) /* do the call */
	{
		goto endcall;
	}
	/* retrieve results */
	nres = -nres; /* stack index of first result */
	while (*sig) { /* get results */
		switch (*sig++) {
		case 'd': /* double result */
			if (!lua_isnumber(m_luaState, nres))
			{
				goto endcall;
			}
			*va_arg(vl, double *) = lua_tonumber(m_luaState, nres);
			break;
		case 'i': /* int result */
			if (!lua_isnumber(m_luaState, nres))
			{
				goto endcall;
			}
			*va_arg(vl, int *) = (int)lua_tonumber(m_luaState, nres);
			break;
		case 's': /* string result */
			if (!lua_isstring(m_luaState, nres))
			{
				goto endcall;
			}
			*va_arg(vl, const char **) = lua_tostring(m_luaState, nres);
			break;

		default:
			goto endcall;
		}
		nres++;
	}
	bRet = true;

endcall:
	va_end(vl);
	lua_settop(m_luaState, nTopIndex);
	return bRet;
}