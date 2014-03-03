#include "LuaWrapper.h"
#include <iostream>
using namespace std;

void TestBase()
{
	LuaWrapper lua;
	lua.openAllLib();

	lua.loadFile("test.lua");

	{
		int			intValue	= lua.getGloabal<int>("intValue");
		double		doubleValue = lua.getGloabal<double>("doubleValue");
		std::string strValue	= lua.getGloabal<std::string>("stringValue");
		bool		bValue		= lua.getGloabal<bool>("boolValue");
		lua.callLuaFunc("testFunc", "ii>", 100, 100);
	}

	lua.setGloabal("intValue", 123);
	lua.setGloabal("doubleValue", 0.123456);
	lua.setGloabal<char*>("stringValue", "stringLua");
	lua.setGloabal("boolValue", false);

	{
		int			intValue	= lua.getGloabal<int>("intValue");
		double		doubleValue = lua.getGloabal<double>("doubleValue");
		std::string strValue	= lua.getGloabal<std::string>("stringValue");
		bool		bValue		= lua.getGloabal<bool>("boolValue");
		lua.callLuaFunc("testFunc", "is>", 120, "120");
	}
	
	{
		int a = lua.getTableField<int>("tbl", "a");
		int b = lua.getTableField<int>("tbl", "b");
		std::string c = lua.getTableField<std::string>("tbl", "c");
		std::string d = lua.getTableField<std::string>("tbl", "d");
		bool e = lua.getTableField<bool>("tbl", "e");
		lua.callLuaFunc("testTablePrint", "siisss>", "%d -- %d -- %s -- %s -- %s", a, b, c.c_str(), d.c_str() , e?"true":"false");
	}
	
}

static int l_myprint(lua_State *L)
{
	int a = lua_tonumber(L, 1);
	const char* str = lua_tostring(L, 2);
	printf("%d --- %s ", a, str);
	return 0;
}

int main()
{
	/*
	
	lua.loadFile("test.lua");*/
	//TestBase();
	
	LuaWrapper lua;
	
	lua.registerFunc("myPrint", l_myprint);

	lua.loadString("myPrint(1000, '11111');");

// 	luaL_loadfile(L, "test.lua");
// 	lua_pcall(L, 0, 0, 0);
// 
// 	lua_getglobal(L, "tbl");
// 
// 	/* table is in the stack at index 't' */
// 	int t = lua_gettop(L);
// 	lua_pushnil(L);  /* first key */
// 	while (lua_next(L, t) != 0) {
// 		/* uses 'key' (at index -2) and 'value' (at index -1) */
// 		printf("%s - %s\n",
// 			lua_typename(L, lua_type(L, -2)),
// 			lua_typename(L, lua_type(L, -1)));
// 		/* removes 'value'; keeps 'key' for next iteration */
// 		lua_pop(L, 1);
// 	}

	
	getchar();
	return 0;
}