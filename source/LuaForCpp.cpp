#include "LuaWrapper.h"
#include <iostream>
using namespace std;

int main()
{
	LuaWrapper lua;
	lua.openBase();
	lua.openString();
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
		lua.callLuaFunc("testFunc", "ii>", 120, 120);
	}

	getchar();
	return 0;
}