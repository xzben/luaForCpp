/********************************************************************
*	文件名称:	config.h											*
*	创建时间：	2014/03/17											*
*	作   者 :	xzben												*
*	文件功能:	本文件包含系统中必备的配制							*
*********************************************************************/

#include <cassert>

#ifndef __CONFIG_H__
#define __CONFIG_H__


/*************************************************
** 系统中使用的命名空间宏定义,使用命名空间主要为了 **
** 防止和其它库的命名冲突						    **
**************************************************/
#define BEGIN_NAMESPACE  namespace  XZBEN{
#define END_NAMESPACE	 }
#define USING_NAMESPACE  using namespace XZBEN;


/*************************************************
**	系统中使用的 ASSERT 宏定义					**
**************************************************/
//只在DEBUG下有效，__express 只有在debug下才执行
#define	ASSERT(__express)					assert((__express))
//__express 总是会执行，但只有debug的时候才会产生 assert
#define VERIFY(__express)					if( !(__express) ) { ASSERT(false); }


/*************************************************
**	系统中使用的 DELETE 安全宏定义				**
**************************************************/
#define	SAFE_DELETE(__ptr)					if( nullptr != (__ptr) ) { delete __ptr; __ptr = nullptr; }
#define SAFE_DELETE_ARR(__ptr)				if( nullptr != (__ptr) ) { delete[] __ptr; __ptr = nullptr; }	

/*************************************************
**	系统中使用的 设置状态标记 					**
**************************************************/
#define	SET_BIT(_dwCntrl, _bit)				((_dwCntrl)|(_bit))
#define GET_BIT(_dwCntrl, _bit)				((_dwCntrl)&(_bit))
#define DEL_BIT(_dwCntrl, _bit)				((_dwCntrl)&(~(_bit)))

#define SET_DEL_BIT(_dwCntrl, _bit, _bSet)	((_bSet) ? SET_BIT(_dwCntrl, _bit) : DEL_BIT(_dwCntrl, _bit))
#define QUERY_IS_SET_BIT(_dwCntrl, _bit)	GET_BIT(_dwCntrl, _bit)

#define SET_PTR_VALUE_SAFE(_ptr, _val)		if(nullptr != _ptr) { *_ptr = _val; }


/*************************************************
**	dll 导入导出配置宏 							**
**************************************************/
#ifdef _WIN32
	# define EXPORT_DLL												__declspec (dllexport)
	# define IMPORT_DLL												__declspec (dllimport)
	# define EXPORT_C_DLL											extern "C" __declspec(dllexport)
	# define IMPORT_C_DLL											extern "C" __declspec(dllimport)
#else
	# define EXPORT_DLL												
	# define IMPORT_DLL												
	# define EXPORT_C_DLL											
	# define IMPORT_C_DLL
#endif/_WIN32

#ifdef  _LUAFORCPP_BUILD_
	#define EXP_IMP_DLL			EXPORT_DLL
	#define EXP_IMP_C_DLL		EXPORT_C_DLL
#else//_OPENSOCKET_BUILD_
	#define EXP_IMP_DLL			IMPORT_DLL
	#define	EXP_IMP_C_DLL		IMPORT_C_DLL
#endif//_OPENSOCKET_BUILD_

#endif // !__CONFIG_H__