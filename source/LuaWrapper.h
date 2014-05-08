#ifndef __2013_01_25_LUAWRAPPER_H__
#define __2013_01_25_LUAWRAPPER_H__


#include <string>
#include "config.h"
#include "baseType.h"

extern struct lua_State;

BEGIN_NAMESPACE

class EXP_IMP_DLL NoCopyable
{
protected:
	NoCopyable(){};
	~NoCopyable(){};
private:
	NoCopyable(const NoCopyable& );
	NoCopyable& operator=(const NoCopyable&);
};

class EXP_IMP_DLL LuaWrapper : public NoCopyable
{
public:
	LuaWrapper();//创建一个lua 栈，然后打开 luaBase 库
	LuaWrapper(lua_State* pLuaState);
	LuaWrapper& operator=(lua_State* pLuaState);
	operator lua_State*(){ return m_luaState; }
	~LuaWrapper();

	inline bool isValid();
	bool close();
	/*打开需要使用的库*/
	inline	bool openBase();
	inline	bool openCoroutine();
	inline	bool openTable();
	inline	bool openIO();
	inline	bool openOS();
	inline	bool openString();
	inline	bool openBit32();
	inline	bool openMath();
	inline	bool openDebug();
	inline	bool openPackage();
	inline	bool openAllLib();
	inline  bool openLib(char const* szLibName, const luaLib_Reg* libTable, int nup);

	bool loadString(const char* szLuaScript, bool bCall = true);
	bool loadFile(const char* szLuaFile, bool bCall = true);
	
	template<typename VariableType>
	VariableType getGloabal(const char* szGlobalVariableName);
	
	template<typename VariableType>
	bool setGloabal(const char* szGlobalVariableName, VariableType Value);

	template<typename VariableType>
	VariableType getTableField(const char* szTableName, const char*	szFieldName);

	template<typename VariableType>
	bool setTableField(const char* szTableName, const char*	szFieldName, VariableType Value);
	

	/*
	*	szFunctionName	: 要 call 的 lua 函数的名字
	*	sig				: lua 函数描述  可以接受数据类型：
						  int (i) 
						  double (d) 
						  char* (s) 
						  bool (b)
						  格式为  传递参数描述 > 返回值描述：  ii>i (传递两个int 返回一个int)
		根据 sig 描述要传递对应的参数
	*/
	bool callLuaFunc(const char* szFuncName, const char* sig, ...);
	bool callLuaFunc(const char* szTableName, const char* szFuncName, const char* sig, ...);

	bool registerFunc(const char* regFuncName, luaRegFunction regCFunc);
	bool unregisterFunc(const char* unregFuncName);
	inline void pushNil();
	inline void pushBoolean(bool bValue);
	inline void pushNumber(double dValue);
	inline void pushNumber(int nValue);
	/*
	*	可以是任意类型的字符串，可以包含 '\0', 
	*/
	inline void pushlString(char const* szStr, int nLen);
	/*
	*	获取指定索引字符串的长度，因为lua中字符串可能中间也有 '\0'
	*	用此函数能够获取 字符串真正的长度
	*/
	inline int	  strlen(int nIndex);
	/*
	*	压入以 '\0' 结束的字符串
	*/
	inline void pushString(char const* szStr);
	/*
	*	检查栈上空间,是否够 nSize
	*/
	inline bool  checkStack(int nSize);

	/*
	*	lua 栈索引规则， 从 1 开始是 从栈底开始， -1 是从栈顶开始
	*/
	/*
	*	检测栈中对应索引元素的类型
	*/
	inline bool  isNumber(int nIndex);
	inline bool  isString(int nIndex);
	inline bool  isBoolean(int nIndex);
	inline bool  isNil(int nIndex);
	inline bool  isFunction(int nIndex);
	inline bool  isUserData(int nIndex);
	inline bool  isTable(int nIndex);
	inline bool  isThread(int nIndex);

	inline int			checkint(int nIndex); //检查索引index对应的是否是一个数字，并返回一个int值
	inline double		checknumber(int nIndex);
	inline long			checklong(int nIndex);
	inline char const * checklstring(int nIndex, size_t *pnLen);
	inline void*		checkudata(int nIndex, char const* tname);//检查索引index对应的元素是否是类型为tname的userdata，并返回userdata的地址
	inline void			argcheck(bool bCondition, int nArgs, const char* szError);
	/*
	*	获取对应索引元素的类型ID
	*	LUA_TNIL
	*	LUA_TBOOLEAN
	*	LUA_TNUMBER
	*	LUA_TSTRING
	*	LUA_TTABLE
	*	LUA_TFUNCTION
	*	LUA_TUSERDATA
	*	LUA_TTHREAD
	*/
	inline int	 type(int nIndex);
	/*
	*	检测栈中对应索引元素的类型
	*/
	inline double			toNumber(int nIndex);
	inline std::string		toString(int nIndex);
	inline bool				toBoolean(int nIndex);
	inline void*			touserdata(int nIndex);
	/*
	*	栈操作接口
	*/
	/*
	*	获取栈中元素个数
	*/
	inline int				gettop();
	/*
	*	设置栈中元素个数，如果为 负数，则此负数代表 栈的索引，则表示设置栈顶到这个元素索引处
	*/
	inline void			settop(int nIndex);
	/*
	*  从栈中弹出指定个数的元素
	*/
	inline void			pop(int nNum);
	/*
	*	将对应 索引的元素复制一份压入栈顶
	*/
	inline void			pushValue(int nIndex);
	/*
	*	将指定索引的元素删除
	*/
	inline void			remove(int nIndex);
	/*
	*	将栈顶元素弹出，然后插入到指定索引位置
	*/
	inline void			insert(int nIndex);
	/*
	*	将栈顶元素弹出，然后设置给指定索引位置的元素值
	*/
	inline void			replace(int nIndex);

	/* UserData 相关的接口 */
	inline void*		newuserdata(int nBytes);

	/* metatable 相关接口 */
	inline int			newmetatable(char const* szTName);
	inline void			getmetatable(char const* szTName);
	inline void			setmetatable(int nIndex);
protected:
	bool loadTable(const char* szTableName);
	bool doCallFunction(const char* sig, va_list vl);
private:
	lua_State*	m_luaState;
	bool		m_bDupicate;
};

END_NAMESPACE
#endif/*__2013_01_25_LUAWRAPPER_H__*/