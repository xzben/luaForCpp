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

	lua.pushNumber(a > b ? a : b);

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
	lua.loadString("print('CGetMax function result:', CGetMax(11, 22));");

	lua.callLuaFunc("luaTable", "tableField", ">");
	printf("<----------------------TestLuaWrapper---------------------->\n");
}

typedef struct NumArray{
	int			size;
	double		value[1];
}NumArray;

static int newarray(LuaWrapper lua)
{
	int n = lua.checkint(1);
	size_t nbytes = sizeof(NumArray)+n*sizeof(double);
	NumArray *a = (NumArray*)lua.newuserdata(nbytes);
	
	lua.getmetatable("LuaBook.array");
	lua.setmetatable(-2);

	a->size = n;

	return 1;
}
static NumArray* checkarray(LuaWrapper& lua)
{
	void *ud = lua.checkudata(1, "LuaBook.array");
	lua.argcheck(ud != nullptr, 1, "array' expected");

	return (NumArray*)ud;
}
static double* getelem(LuaWrapper &lua)
{
	NumArray *a = checkarray(lua);
	int index = lua.checkint(2);

	lua.argcheck(1 <= index && index <= a->size, 2, "index out of range");

	return &a->value[index - 1];
}



static int setarray(LuaWrapper lua)
{
	double newvalue = lua.checknumber(3);
	*getelem(lua) = newvalue;
	
	return 0;
}

static int getarray(LuaWrapper lua)
{
	lua.pushNumber(*getelem(lua));
	return 1;
}


static int getsize(LuaWrapper lua)
{
	NumArray *a = checkarray(lua);

	lua.pushNumber(a->size);

	return 1;
}

static const struct luaLib_Reg arrlib[] = {
	{ "new", newarray},
	{ "set", setarray },
	{ "get", getarray },
	{ "size", getsize },
	{ nullptr, nullptr }
};

int luaopen_array(LuaWrapper& lua)
{
	lua.newmetatable("LuaBook.array");
	lua.openLib("array", arrlib, 0);

	return 1;
}
void TestLuaUserData()
{
	LuaWrapper lua;
	lua.openAllLib();
	luaopen_array(lua);
	lua.loadFile("luaArray.lua");
}

int main()
{
	//TestLuaWrapper();
	TestLuaUserData();
	getchar();
	return 0;
}