extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};
#pragma comment(lib, "lua.lib")
#include "LuaWrapper.h"

BEGIN_NAMESPACE

LuaWrapper::LuaWrapper()
{
	m_luaState = luaL_newstate();
	m_bDupicate = false;
	ASSERT(NULL != m_luaState);
	VERIFY(openBase());
}

LuaWrapper::LuaWrapper(lua_State* pLuaState)
{
	ASSERT(NULL != m_luaState);
	m_luaState = pLuaState;
	m_bDupicate = true;
}

LuaWrapper::~LuaWrapper()
{
	if ( !m_bDupicate )
		close();
}

LuaWrapper& LuaWrapper::operator=(lua_State* pLuaState)
{
	close();
	m_luaState = pLuaState;
	m_bDupicate = true;
	return *this;
}

bool LuaWrapper::close()
{
	if ( NULL != m_luaState)
		lua_close(m_luaState);

	m_luaState = NULL;
	return true;
}
bool LuaWrapper::isValid(){ return NULL != m_luaState; }

/********************************打开需要使用的库************************************/
bool LuaWrapper::openBase()			{ luaopen_base(m_luaState);			return true; }
bool LuaWrapper::openCoroutine()	{ luaopen_coroutine(m_luaState);	return true; } //协同库
bool LuaWrapper::openTable()		{ luaopen_table(m_luaState);		return true; }
bool LuaWrapper::openIO()			{ luaopen_io(m_luaState);			return true; }
bool LuaWrapper::openOS()			{ luaopen_os(m_luaState);			return true; }
bool LuaWrapper::openString()		{ luaopen_string(m_luaState);		return true; }
bool LuaWrapper::openBit32()		{ luaopen_bit32(m_luaState);		return true; }
bool LuaWrapper::openMath()			{ luaopen_math(m_luaState);			return true; }
bool LuaWrapper::openDebug()		{ luaopen_debug(m_luaState);		return true; }
bool LuaWrapper::openPackage()		{ luaopen_package(m_luaState);		return true; }
bool LuaWrapper::openAllLib()		{ luaL_openlibs(m_luaState);		return true; }

/******************************向栈中压入元素*************************************/
void	LuaWrapper::pushNil()									{	lua_pushnil(m_luaState);					}
void	LuaWrapper::pushBoolean(bool bValue)					{	lua_pushboolean(m_luaState, bValue);		}
void	LuaWrapper::pushNumber(double dValue)					{	lua_pushnumber(m_luaState, dValue);			}
void	LuaWrapper::pushNumber(int nValue)						{	lua_pushnumber(m_luaState, nValue);			}
void	LuaWrapper::pushlString(char const* szStr, int nLen)	{	lua_pushlstring(m_luaState, szStr, nLen);	}
void	LuaWrapper::pushString(char const* szStr)				{	lua_pushstring(m_luaState, szStr);			}
int		LuaWrapper::strlen(int nIndex)							{	return lua_strlen(m_luaState, nIndex);		}
bool	LuaWrapper::checkStack(int nSize)						{	return lua_checkstack(m_luaState, nSize);	}
/******************************栈中元素类型检测*************************************/
bool  LuaWrapper::isNumber(int nIndex)						{	return lua_isnumber(m_luaState, nIndex);	}
bool  LuaWrapper::isString(int nIndex)						{	return lua_isstring(m_luaState, nIndex);	}
bool  LuaWrapper::isBoolean(int nIndex)						{	return lua_isboolean(m_luaState, nIndex);	}
bool  LuaWrapper::isNil(int nIndex)							{	return lua_isnil(m_luaState, nIndex);		}
bool  LuaWrapper::isFunction(int nIndex)					{	return lua_isfunction(m_luaState, nIndex);	}
bool  LuaWrapper::isUserData(int nIndex)					{	return lua_isuserdata(m_luaState, nIndex);	}
bool  LuaWrapper::isTable(int nIndex)						{	return lua_istable(m_luaState, nIndex);		}
bool  LuaWrapper::isThread(int nIndex)						{	return lua_isthread(m_luaState, nIndex);	}
int	 LuaWrapper::type(int nIndex)							{	return lua_type(m_luaState, nIndex);		}

double			LuaWrapper::toNumber(int nIndex)			{	return lua_tonumber(m_luaState, nIndex);	}
std::string		LuaWrapper::toString(int nIndex)			{	return lua_tostring(m_luaState, nIndex);	}
bool			LuaWrapper::toBoolean(int nIndex)			{	return lua_toboolean(m_luaState, nIndex);	}

int				LuaWrapper::gettop()						{	return lua_gettop(m_luaState);				}
void			LuaWrapper::settop(int nIndex)				{	lua_settop(m_luaState, nIndex);				}
void			LuaWrapper::pushValue(int nIndex)			{	lua_pushvalue(m_luaState, nIndex);			}
void			LuaWrapper::remove(int nIndex)				{	lua_remove(m_luaState, nIndex);				}
void			LuaWrapper::replace(int nIndex)				{	lua_replace(m_luaState, nIndex);			}

bool  LuaWrapper::loadString(const char* szLuaScript, bool bCall /*= true*/)
{
	int nErr;
	if(LUA_OK != (nErr = luaL_loadstring(m_luaState, szLuaScript)))
	{
		luaL_error (m_luaState, "load string( %s ) failed error( %d ).", szLuaScript, nErr);
		return false;
	}
	if( bCall && lua_pcall(m_luaState, 0, 0, 0))
	{
		luaL_error (m_luaState, "call string( %s ) chuck failed.", szLuaScript);
		return false;
	}
	return true;
}

bool LuaWrapper::loadFile(const char* szLuaFile, bool bCall /*= true*/)
{
	if (luaL_loadfile(m_luaState, szLuaFile))
	{
		luaL_error (m_luaState, "load file( %s ) failed.", szLuaFile);
		return false;
	}
	if( bCall && lua_pcall(m_luaState, 0, 0, 0))
	{
		luaL_error (m_luaState, "call file( %s ) chuck failed.", szLuaFile);
		return false;
	}
	return true;
}


template<>
bool EXP_IMP_DLL LuaWrapper::getGloabal<bool>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if( !lua_isboolean(m_luaState, -1) )
		luaL_error(m_luaState, "global variable[ %s ] isn't an boolean.", szGlobalVariableName);

	bool bRet = (bool)lua_toboolean(m_luaState, -1);
	lua_pop(m_luaState, 1);

	return bRet;
}

template<>
int EXP_IMP_DLL LuaWrapper::getGloabal<int>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if(!lua_isnumber(m_luaState, -1))
		luaL_error(m_luaState, "global variable[ %s ] isn't an int.", szGlobalVariableName);

	int nRet = (int)lua_tonumber(m_luaState, -1);
	lua_pop(m_luaState, 1);
	return nRet;
	
}

template<>
double EXP_IMP_DLL LuaWrapper::getGloabal<double>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if(!lua_isnumber(m_luaState, -1))
		luaL_error(m_luaState, "global variable[ %s ] isn't an double.", szGlobalVariableName);

	double dRet = (double)lua_tonumber(m_luaState, -1);
	lua_pop(m_luaState, 1);
	return dRet;
	

	return 0.0;
}

template<>
std::string EXP_IMP_DLL LuaWrapper::getGloabal<std::string>(const char* szGlobalVariableName)
{
	lua_getglobal(m_luaState, szGlobalVariableName);
	if(!lua_isstring(m_luaState, -1))
		luaL_error(m_luaState, "global variable[ %s ] isn't an string.", szGlobalVariableName);

	std::string strRet = lua_tostring(m_luaState, -1);
	lua_pop(m_luaState, 1);
	return strRet;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setGloabal<bool>(const char* szGlobalVariableName, bool Value)
{
	lua_pushboolean(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);
	
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setGloabal<int>(const char* szGlobalVariableName, int Value)
{
	lua_pushnumber(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);

	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setGloabal<double>(const char* szGlobalVariableName, double Value)
{
	lua_pushnumber(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setGloabal<char*>(const char* szGlobalVariableName, char* Value)
{
	lua_pushstring(m_luaState, Value);
	lua_setglobal(m_luaState, szGlobalVariableName);
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setGloabal<std::string>(const char* szGlobalVariableName, std::string Value)
{
	lua_pushstring(m_luaState, Value.c_str());
	lua_setglobal(m_luaState, szGlobalVariableName);
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setGloabal<luaRegFunction>(const char* szGlobalVariableName, luaRegFunction regFunction)
{
	return registerFunc(szGlobalVariableName, regFunction);
}

bool EXP_IMP_DLL LuaWrapper::loadTable(const char* szTableName)
{
	lua_getglobal(m_luaState, szTableName);

	if (!lua_istable(m_luaState, -1))
	{
		luaL_error(m_luaState, "global variable[ %s ] isn't an Table.", szTableName);
		lua_pop(m_luaState, 1);
		return false;
	}
	return true;
}


template<>
bool EXP_IMP_DLL LuaWrapper::getTableField<bool>(const char* szTableName, const char*	szFieldName)
{
	bool bRet = false;

	if( !loadTable(szTableName) )
	{
		return bRet;
	}
	lua_pushstring(m_luaState, szFieldName);
	lua_gettable(m_luaState, -2);// key从栈弹出，并获取 table[key] 的值压入栈
	
	if(!lua_isboolean(m_luaState, -1))
		luaL_error(m_luaState, "table [%s] field [ %s ] isn't an boolean.", szTableName, szFieldName);

	bRet = lua_toboolean(m_luaState, -1);

	lua_pop(m_luaState, 2); /* remove result and table*/
	return bRet;
}

template<>
int EXP_IMP_DLL LuaWrapper::getTableField<int>(const char* szTableName, const char*	szFieldName)
{
	int nRet = 0;

	if( !loadTable(szTableName) )
	{
		return nRet;
	}
	lua_pushstring(m_luaState, szFieldName);
	lua_gettable(m_luaState, -2);// key从栈弹出，并获取 table[key] 的值压入栈

	if( !lua_isnumber(m_luaState, -1))
		luaL_error(m_luaState, "table [%s] field [ %s ] isn't an int.", szTableName, szFieldName);
	nRet = (int)lua_tonumber(m_luaState, -1);

	lua_pop(m_luaState, 2);/* remove result and table*/
	return nRet;
}

template<>
double EXP_IMP_DLL LuaWrapper::getTableField<double>(const char* szTableName, const char*	szFieldName)
{
	double dRet = 0.0;

	if( !loadTable(szTableName) )
	{
		return dRet;
	}
	lua_pushstring(m_luaState, szFieldName);
	lua_gettable(m_luaState, -2);// key从栈弹出，并获取 table[key] 的值压入栈

	if(!lua_isnumber(m_luaState, -1))
		luaL_error(m_luaState, "table [%s] field [ %s ] isn't an double.", szTableName, szFieldName);

	dRet = (double)lua_tonumber(m_luaState, -1);
	lua_pop(m_luaState, 2); /* remove result and table*/
	return dRet;
}

template<>
std::string EXP_IMP_DLL LuaWrapper::getTableField<std::string>(const char* szTableName, const char*	szFieldName)
{
	std::string strRet = "";

	if( !loadTable(szTableName) )
	{
		return strRet;
	}
	lua_pushstring(m_luaState, szFieldName);
	lua_gettable(m_luaState, -2);// key从栈弹出，并获取 table[key] 的值压入栈

	if( !lua_isstring(m_luaState, -1))
		luaL_error(m_luaState, "table [%s] field [ %s ] isn't an string.", szTableName, szFieldName);

	strRet = lua_tostring(m_luaState, -1);
	
	lua_pop(m_luaState, 2); /* remove result and table*/
	return strRet;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setTableField<bool>(const char* szTableName, const char*	szFieldName, bool Value)
{
	if( !loadTable(szTableName) )  return false;
	
	lua_pushstring(m_luaState, szFieldName);
	lua_pushboolean(m_luaState, Value);
	lua_settable(m_luaState, -3);

	lua_pop(m_luaState, 1);
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setTableField<int>(const char* szTableName, const char*	szFieldName, int Value)
{
	if( !loadTable(szTableName) )  return false;

	lua_pushstring(m_luaState, szFieldName);
	lua_pushnumber(m_luaState, Value);
	lua_settable(m_luaState, -3);

	lua_pop(m_luaState, 1);
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setTableField<double>(const char* szTableName, const char*	szFieldName, double Value)
{
	if( !loadTable(szTableName) )  return false;

	lua_pushstring(m_luaState, szFieldName);
	lua_pushnumber(m_luaState, Value);
	lua_settable(m_luaState, -3);

	lua_pop(m_luaState, 1);
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setTableField<std::string>(const char* szTableName, const char*	szFieldName, std::string Value)
{
	if( !loadTable(szTableName) )  return false;

	lua_pushstring(m_luaState, szFieldName);
	lua_pushstring(m_luaState, Value.c_str());
	lua_settable(m_luaState, -3);

	lua_pop(m_luaState, 1);
	return true;
}

template<>
bool EXP_IMP_DLL LuaWrapper::setTableField<char*>(const char* szTableName, const char*	szFieldName, char* Value)
{
	if( !loadTable(szTableName) )  return false;

	lua_pushstring(m_luaState, szFieldName);
	lua_pushstring(m_luaState, Value);
	lua_settable(m_luaState, -3);

	lua_pop(m_luaState, 1);
	return true;
}

bool  LuaWrapper::callLuaFunc(const char* szTableName, const char* szFuncName, const char* sig, ...)
{
	int nIndexTop = gettop();
	va_list vl;
	va_start(vl, sig);
	bool bRet = true;

	if (!loadTable(szTableName))
	{
		luaL_error(m_luaState, "load table[%s] failed.", szTableName);
		bRet = false;
	}
	lua_pushstring(m_luaState, szFuncName);
	lua_gettable(m_luaState, -2);// key从栈弹出，并获取 table[key] 的值压入栈

	if (!lua_isfunction(m_luaState, -1))
	{
		luaL_error(m_luaState, "table [%s] field [ %s ] isn't an function.", szTableName, szFuncName);
		bRet = false;
	}
	else
	{

		bRet = doCallFunction(sig, vl);
	}

	va_end(vl);
	settop(nIndexTop);

	return bRet;
}

bool LuaWrapper::callLuaFunc(const char* szFuncName, const char* sig, ...)
{
	int nTopIndex = lua_gettop(m_luaState); //记住call 函数前栈中元素index，call结束后恢复
	bool bRet = false;
	va_list vl;
	va_start(vl, sig);

	lua_getglobal(m_luaState, szFuncName); /* 将函数压入栈 */

	if (!lua_isfunction(m_luaState, -1))
	{
		bRet = false;
		luaL_error(m_luaState, "function name [%s] isn't an function.", szFuncName);
	}
	else
	{
		bRet = doCallFunction(sig, vl);
	}

	va_end(vl);
	lua_settop(m_luaState, nTopIndex);
	return bRet;
}

bool LuaWrapper::doCallFunction(const char* sig, va_list vl)
{
	int narg = 0;
	int nres;
	/* push arguments */

	while (*sig)
	{
		switch (*sig++) /* push arguments */
		{
		case 'd': /* double argument */
			lua_pushnumber(m_luaState, va_arg(vl, double));
			break;
		case 'i': /* int argument */
			lua_pushnumber(m_luaState, va_arg(vl, int));
			break;
		case 's': /* string argument */
			lua_pushstring(m_luaState, va_arg(vl, const char *));
			break;
		case 'b':
			lua_pushboolean(m_luaState, va_arg(vl, bool));
			break;
		case '>':
			goto endwhile;
		default:
			assert(false);
			goto failed;
		}
		narg++;
		luaL_checkstack(m_luaState, 1, "too many arguments");
	} endwhile:

	/* do the call */
	nres = ::strlen(sig); /* number of expected results */
	if (lua_pcall(m_luaState, narg, nres, 0) != 0) /* do the call */
	{
		printf("%s", lua_tostring(m_luaState, -1));
		goto failed;
	}
	/* retrieve results */
	nres = -nres; /* stack index of first result */
	while (*sig) { /* get results */
		switch (*sig++) {
		case 'd': /* double result */
			if (!lua_isnumber(m_luaState, nres))
			{
				goto failed;
			}
			*va_arg(vl, double *) = lua_tonumber(m_luaState, nres);
			break;
		case 'i': /* int result */
			if (!lua_isnumber(m_luaState, nres))
			{
				goto failed;
			}
			*va_arg(vl, int *) = (int)lua_tonumber(m_luaState, nres);
			break;
		case 's': /* string result */
			if (!lua_isstring(m_luaState, nres))
			{
				goto failed;
			}
			*va_arg(vl, const char **) = lua_tostring(m_luaState, nres);
			break;
		case 'b':
			if (!lua_isboolean(m_luaState, nres))
			{
				goto failed;
			}
			*va_arg(vl, bool*) = lua_toboolean(m_luaState, nres);
			break;
		default:
			goto failed;
		}
		nres++;
	}

	return true;
failed:
	return false;
}



bool LuaWrapper::registerFunc(const char* regFuncName, luaRegFunction regCFunc)
{
	lua_pushcfunction(m_luaState, (lua_CFunction)regCFunc);
	lua_setglobal(m_luaState, regFuncName);

	return true;
}
bool LuaWrapper::unregisterFunc(const char* unregFuncName)
{
	lua_pushnil(m_luaState);
	lua_setglobal(m_luaState, unregFuncName);

	return true;
}

END_NAMESPACE