

#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Type.h"
#include "Scene.h"
#include "IDTable.h"
#include "Config.h"


/////////////////////////////////////////////////////////////////////////////////
//��������
struct _OBJ_MONSTER_INIT ;
struct FLOAT_POS
{
	FLOAT	 m_fX ;
	FLOAT	 m_fZ ;
};
enum MONSTER_VALUE
{
	MONSTER_VALUE_GUID		= 0 ,
	MONSTER_VALUE_TYPE		= 1 ,
	MONSTER_VALUE_NAME		= 2 ,
	MONSTER_VALUE_POS		= 3 ,
	MONSTER_VALUE_SCRIPTID	= 4 ,
	MONSTER_VALUE_GROUPID	= 5 ,
	MONSTER_VALUE_TEAMID	= 6 ,
	MONSTER_VALUE_BASEAI	= 7 ,
	MONSTER_VALUE_AIFILE	= 8 ,
	MONSTER_VALUE_PATROLID	= 9 ,
	MONSTER_VALUE_TRAND		= 10 ,
	MONSTER_VALUE_GRAND		= 11 ,
};
struct MONSTER_ATTR_VALUE
{
	union
	{
		GUID_t			m_GUID ;
		INT				m_Type ;
		CHAR			m_Name[NPC_NAME_LEN+1] ;
		FLOAT_POS		m_Pos ;
		ScriptID_t		m_ScriptID ;
		GroupID_t		m_GroupID ;
		TeamID_t		m_TeamID ;
		INT				m_BaseAI ;
		INT				m_AIFile ;
		INT				m_PatrolID ;
	};
};
struct _OBJ_MONSTER_OPT
{
	INT					m_SelectType ;
	MONSTER_ATTR_VALUE	m_Value ;
	INT					m_DataDef ;
	INT					m_MaxLeft ;
};
struct _OBJ_MONSTER_DATA
{
	INT					m_DataType ;
	INT					m_DataCount ;
	MONSTER_ATTR_VALUE*	m_pDataValue ;

	_OBJ_MONSTER_DATA()
	{
		m_DataType = 0 ;
		m_DataCount = 0 ;
		m_pDataValue = NULL ;
	};
	~_OBJ_MONSTER_DATA()
	{
		SAFE_DELETE_ARRAY(m_pDataValue) ;
	};
};
struct MONSTER_FILE
{
	CHAR				m_szFileName[_MAX_PATH] ;	//monster.ini�ļ���
	_OBJ_MONSTER_INIT*	m_pMonsterInitData ;		//�����ʼ�����ݽṹ���༭�������������
	BOOL*				m_pAllocFlag ;				//ÿ�������Ƿ����ʹ�õı�־
	INT					m_Count ;					//��������
	_OBJ_MONSTER_INIT*	m_pInitUsing ;				//ʵ��ʹ�õĹ����ʼ������
	INT					m_CountUsing ;				//ʵ��ʹ�õĹ�������
	INT					m_OperateCount ;			//ת����������
	_OBJ_MONSTER_OPT*	m_pOperate ;				//ת���������ݽṹ
	INT					m_DataCount ;				//����������
	_OBJ_MONSTER_DATA*	m_pData ;					//�������ṹ

	MONSTER_FILE( )
	{
		memset( m_szFileName, 0, _MAX_PATH ) ;
		m_pMonsterInitData = NULL ;
		m_pAllocFlag = NULL ;
		m_Count = 0 ;
		m_pInitUsing = NULL ;
		m_CountUsing = 0 ;

		m_OperateCount = 0 ;
		m_pOperate = NULL ;

		m_DataCount = 0 ;
		m_pData = NULL ;
	};
	~MONSTER_FILE( ) ;

	VOID				CleanUp( ) ;

	BOOL				Do( ) ;

protected :
	BOOL				DoOneOperate( INT iIndex ) ;
};

/////////////////////////////////////////////////////////////////////////////////
//Ѳ��·������
struct PatrolPath ;
struct PATROL_FILE
{
	CHAR				m_szFileName[_MAX_PATH] ;	//patrol.ini�ļ���
	PatrolPath*			m_PatrolPathList ;			//·������
	INT					m_PatrolPathCount ;			//·������

	PATROL_FILE()
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_PatrolPathList = NULL ;
		m_PatrolPathCount = 0 ;
	};
	~PATROL_FILE() ;
	VOID CleanUp( ) ;
};

/////////////////////////////////////////////////////////////////////////////////
//����̨����
struct _OBJ_PLATFORM_INIT ;
struct PLATFORM_FILE
{
	CHAR					m_szFileName[_MAX_PATH] ;	//platform.ini�ļ���
	_OBJ_PLATFORM_INIT*		m_pInit ;
	INT						m_Count ;

	PLATFORM_FILE( )
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_pInit = NULL ;
		m_Count = 0 ;
	}
	~PLATFORM_FILE( ) ;
	VOID CleanUp( ) ;
};

/////////////////////////////////////////////////////////////////////////////////
//�¼�������
struct Area ;
struct AREA_FILE
{
	CHAR		m_szFileName[_MAX_PATH] ;	//area.ini�ļ���
	Area*		m_pArea ;
	INT			m_Count ;

	AREA_FILE( )
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_pArea = NULL ;
		m_Count = 0 ;
	};
	~AREA_FILE( ) ;
	VOID CleanUp( ) ;
};

/////////////////////////////////////////////////////////////////////////////////
//����������
struct GROWPOINT_LOAD ;
struct GROWPOINT_FILE
{
	CHAR			m_szFileName[_MAX_PATH] ;	//growpoint.ini�ļ���
	GROWPOINT_LOAD*	m_pGrowpoint ;
	INT				m_Count ;

	GROWPOINT_FILE( )
	{
		memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
		m_pGrowpoint = NULL ;
		m_Count = 0 ;
	}
	~GROWPOINT_FILE( ) ;
	VOID CleanUp( ) ;
};


/////////////////////////////////////////////////////////////////////////////////
// SceneManager
/////////////////////////////////////////////////////////////////////////////////
class SceneManager
{
public :
	SceneManager( ) ;
	~SceneManager( ) ;

	//��ʼ���ӿ�
	BOOL			Init( uint MaxSceneCount = MAX_SCENE ) ;

	//���ݳ�����ȡ�ó�������ָ��
	Scene*			GetScene( SceneID_t SceneID ) ;
	//���ݳ�����ȡ�ó���������Ϣ
	_SCENE_DATA*	GetSceneInfo( SceneID_t SceneID ) ;
	//���ݳ������жϴ˳����Ƿ��ڵ�ǰ�������˳�����
	BOOL			IsInCurServer( SceneID_t SceneID ) ;
	//���ݳ������жϴ˳����Ƿ񱻵�ǰ�������ϵ�ĳ���������˳�����
	BOOL			IsInCurMachine( SceneID_t SceneID ) ;

	//��ϵͳ������һ������
	BOOL			AddScene( Scene* pScene ) ;
	//��һ��������ϵͳ��ɾ��
	BOOL			DelScene( SceneID_t SceneID ) ;

	//����ĳ�������е�����
	VOID			SetScenePlayerCount( SceneID_t SceneID, INT count ){
		if( SceneID < 0 && SceneID>=MAX_SCENE ) return ;
		m_aScenePlayerCount[SceneID] = count ;
	};
	//��ȡĳ�������е�����
	INT				GetScenePlayerCount( SceneID_t SceneID ){
		if( SceneID < 0 && SceneID>=MAX_SCENE ) return 0 ;
		return m_aScenePlayerCount[SceneID] ;
	};

	VOID			SetTotalPlayerCount(INT nCount){ m_nTotalPlayerCount=nCount; } ;
	INT				GetTotalPlayerCount(){ return m_nTotalPlayerCount ; } ;


	MONSTER_FILE*	FindMonsterFile( const CHAR* szMonsterFileName ) ;
	MONSTER_FILE*	FindEmptyMonsterFile( ) ;

	PATROL_FILE*	FindPatrolFile( const CHAR* szPatrolFileName ) ;
	PATROL_FILE*	FindEmptyPatrolFile( ) ;

	PLATFORM_FILE*	FindPlatformFile( const CHAR* szPlatformFileName ) ;
	PLATFORM_FILE*	FindEmptyPlatformFile( ) ;

	AREA_FILE*		FindAreaFile( const CHAR* szAreaFileName ) ;
	AREA_FILE*		FindEmptyAreaFile( ) ;

	GROWPOINT_FILE*	FindGrowpointFile( const CHAR* szGrowpointFileName ) ;
	GROWPOINT_FILE*	FindEmptyGrowpointFile( ) ;

	BOOL			BroadCast_Scene(Packet* pMsg);
	SceneID_t		GetSpecialSceneIDFromCurServer( ) ;//ȡ��һ���ڵ�ǰServer�ϵ���Ϸ�߼�����
private :
	Scene*			m_pScene[MAX_SCENE] ;			//��������ָ��
	INT				m_aScenePlayerCount[MAX_SCENE] ;//����������
	uint			m_Count ;//��ǰϵͳ�еĳ�������

	MONSTER_FILE	m_aMonsterFile[MAX_SCENE] ;//����*_monster.ini�����ݣ�ԭʼ����
	PATROL_FILE		m_aPatrolFile[MAX_SCENE] ;//Ѳ��·��*partrol.ini�����ݣ�ԭʼ����
	PLATFORM_FILE	m_aPlatformFile[MAX_SCENE] ;//����̨*platform.ini�����ݣ�ԭʼ����
	AREA_FILE		m_aAreaFile[MAX_SCENE] ;//�¼���*area.ini�����ݣ�ԭʼ����
	GROWPOINT_FILE	m_aGrowpointFile[MAX_SCENE] ;//������ԭʼ����

	INT				m_nTotalPlayerCount ;//������Ϸ����������ܺ�
};

extern SceneManager* g_pSceneManager ;


#endif