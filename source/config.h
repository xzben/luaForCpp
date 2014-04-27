/********************************************************************
*	�ļ�����:	config.h											*
*	����ʱ�䣺	2014/03/17											*
*	��   �� :	xzben												*
*	�ļ�����:	���ļ�����ϵͳ�бر�������							*
*********************************************************************/

#include <cassert>

#ifndef __CONFIG_H__
#define __CONFIG_H__


/*************************************************
** ϵͳ��ʹ�õ������ռ�궨��,ʹ�������ռ���ҪΪ�� **
** ��ֹ���������������ͻ						    **
**************************************************/
#define BEGIN_NAMESPACE  namespace  XZBEN{
#define END_NAMESPACE	 }
#define USING_NAMESPACE  using namespace XZBEN;


/*************************************************
**	ϵͳ��ʹ�õ� ASSERT �궨��					**
**************************************************/
//ֻ��DEBUG����Ч��__express ֻ����debug�²�ִ��
#define	ASSERT(__express)					assert((__express))
//__express ���ǻ�ִ�У���ֻ��debug��ʱ��Ż���� assert
#define VERIFY(__express)					if( !(__express) ) { ASSERT(false); }


/*************************************************
**	ϵͳ��ʹ�õ� DELETE ��ȫ�궨��				**
**************************************************/
#define	SAFE_DELETE(__ptr)					if( nullptr != (__ptr) ) { delete __ptr; __ptr = nullptr; }
#define SAFE_DELETE_ARR(__ptr)				if( nullptr != (__ptr) ) { delete[] __ptr; __ptr = nullptr; }	

/*************************************************
**	ϵͳ��ʹ�õ� ����״̬��� 					**
**************************************************/
#define	SET_BIT(_dwCntrl, _bit)				((_dwCntrl)|(_bit))
#define GET_BIT(_dwCntrl, _bit)				((_dwCntrl)&(_bit))
#define DEL_BIT(_dwCntrl, _bit)				((_dwCntrl)&(~(_bit)))

#define SET_DEL_BIT(_dwCntrl, _bit, _bSet)	((_bSet) ? SET_BIT(_dwCntrl, _bit) : DEL_BIT(_dwCntrl, _bit))
#define QUERY_IS_SET_BIT(_dwCntrl, _bit)	GET_BIT(_dwCntrl, _bit)

#define SET_PTR_VALUE_SAFE(_ptr, _val)		if(nullptr != _ptr) { *_ptr = _val; }


/*************************************************
**	dll ���뵼�����ú� 							**
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