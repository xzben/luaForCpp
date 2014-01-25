#ifndef __2013_01_25_LUAWRAPPER_H__
#define __2013_01_25_LUAWRAPPER_H__

extern "C"{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};
#pragma comment(lib, "lua.lib")

#include <string>
#include "cfg-win32.h"

class NoCopyable
{
protected:
	NoCopyable(){};
	~NoCopyable(){};
private:
	NoCopyable(const NoCopyable& );
	NoCopyable& operator=(const NoCopyable&);
};


class LuaWrapper : public NoCopyable
{
public:
	LuaWrapper();
	~LuaWrapper();

	bool isValid()						{	return NULL != m_luaState;		}
	/*打开需要使用的库*/
	inline	bool openBase()			{	luaopen_base(m_luaState);		return true;	}
	inline	bool openCoroutine()	{	luaopen_coroutine(m_luaState);	return true;	}
	inline	bool openTable()		{	luaopen_table(m_luaState);		return true;	}
	inline	bool openIO()			{	luaopen_io(m_luaState);			return true;	}
	inline	bool openOS()			{	luaopen_os(m_luaState);			return true;	}
	inline	bool openString()		{	luaopen_string(m_luaState);		return true;	}
	inline	bool openBit32()		{	luaopen_bit32(m_luaState);		return true;	}
	inline	bool openMath()			{	luaopen_math(m_luaState);		return true;	}
	inline	bool openDebug()		{	luaopen_debug(m_luaState);		return true;	}
	inline	bool openPackage()		{	luaopen_package(m_luaState);	return true;	}
	inline	bool openAllLib()		{	luaL_openlibs(m_luaState);		return true;	}

	bool loadString(const char* szLuaScript, bool bCall = true);
	bool loadFile(const char* szLuaFile, bool bCall = true);
	
	template<typename VariableType>
	VariableType getGloabal(const char* szGlobalVariableName);
	
	template<typename VariableType>
	bool setGloabal(const char* szGlobalVariableName, VariableType Value);
	
	bool callLuaFunc(const char* szFuncName, const char* sig, ...);

private:
	lua_State*	m_luaState;
};

#endif/*__2013_01_25_LUAWRAPPER_H__*/