#include "LuaWrapper.h"
#include <iostream>
#include <string>
using namespace std;

#pragma comment(lib, "LuaForCpp.lib")

USING_NAMESPACE;

int regsCFunction(LuaWrapper lua)
{
	int a = lua.toNumber(-2);
	int b = lua.toNumber(-1);

	lua.pushNumber(a + b);

	return 1;
}

void TestLuaWrapper()
{
	printf("<----------------------TestLuaWrapper---------------------->\n");
	LuaWrapper lua;
	lua.openAllLib();
	lua.loadFile("loadfile.lua");
	lua.loadString("print('hello world, this is call from loadstring');");
	
	printf("<----------------------get data from luaData---------------------->\n");
	lua.loadFile("luaData.lua");
	cout << lua.getGloabal<int>("luaInt") << endl;
	cout << lua.getGloabal<string>("luaString") << endl;
	cout << lua.getGloabal<double>("luaDouble") << endl;
	cout << lua.getGloabal<bool>("luaBool") << endl;
	printf("<----------------------set data to luaData---------------------->\n");
	cout << lua.setGloabal<int>("luaInt", 111) << endl;
	cout << lua.setGloabal<string>("luaString", "this is value set from luaForCpp") << endl;
	cout << lua.setGloabal<double>("luaDouble", 2345.67890) << endl;
	cout << lua.setGloabal<bool>("luaBool", true) << endl;
	printf("<----------------------get data from luaData again---------------------->\n");
	cout << lua.getGloabal<int>("luaInt") << endl;
	cout << lua.getGloabal<string>("luaString") << endl;
	cout << lua.getGloabal<double>("luaDouble") << endl;
	cout << lua.getGloabal<bool>("luaBool") << endl;

	int result;
	lua.loadFile("luafunction.lua");
	lua.callLuaFunc("luaGetMax", "ii>i", 1000, 20, &result);
	cout << "luaGetMax Function result:"<< result << endl;

	lua.registerFunc("CGetMax", regsCFunction);
	lua.loadString("print('CGetMax function result:',CGetMax(11, 22));");
	printf("<----------------------TestLuaWrapper---------------------->\n");
}

int main()
{
	TestLuaWrapper();

	getchar();
	return 0;
}