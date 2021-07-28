
#pragma once

#include "everymsg.hpp"

using namespace std;

//�û���Ϣ
//struct UserInfo
class UserInfo
{
public:
	UserInfo() { nUserID = 0; nCityID = 0; }

	int      nUserID;
	string   strUName;
	int      nCityID;

	DEFINE_PUSHMSG(UserInfo, i.nUserID << i.strUName << i.nCityID);
	DEFINE_POPMSG(UserInfo, i.nUserID >> i.strUName >> i.nCityID);
};
typedef std::vector<UserInfo*> listpUserInfo;


//������Ϣ
//struct CityInfo
class CityInfo
{
public:
	CityInfo() { nCityID = 0; }
	~CityInfo()
	{
		for (auto& item : listpUser)
		{
			delete item;
		}
		listpUser.clear();
	}
	int            nCityID;
	string         strCityName;
	listpUserInfo  listpUser;    //�û��б�

	DEFINE_PUSHMSG(CityInfo, i.nCityID << i.strCityName << i.listpUser);
	DEFINE_POPMSG(CityInfo, i.nCityID >> i.strCityName >> i.listpUser);
};
typedef std::vector<CityInfo*> listpCityInfo;

//��¼��Ϣ����
//struct LoginRsp
class LoginRsp
{
public:
	LoginRsp() { nErrorNo = 0; }
	~LoginRsp()
	{
		for (auto& item : listpCity)
		{
			delete item;
		}
		listpCity.clear();
	}
	int            nErrorNo;
	string         strError;
	SBuffer        m_FileData;//����������
	listpCityInfo  listpCity; //�����б�

	DEFINE_PUSHMSG(LoginRsp, i.nErrorNo << i.strError << i.m_FileData << i.listpCity);
	DEFINE_POPMSG(LoginRsp, i.nErrorNo >> i.strError >> i.m_FileData >> i.listpCity);
};


//��Ƶ���ݰ�
//struct LoginRsp
class VideoPack
{
public:
	int      m_nSeqNumber = 0;  //���к�
	bool     m_bIsFrameKey = false; //�Ƿ�Ϊ�ؼ�֡
	SBuffer  m_Data;                //���ݰ�

	DEFINE_PUSHMSG(VideoPack, i.m_nSeqNumber << i.m_bIsFrameKey << i.m_Data);
	DEFINE_POPMSG(VideoPack, i.m_nSeqNumber >> i.m_bIsFrameKey >> i.m_Data);

};
DEFINE_MAP(int, mappVideoPack, VideoPack)

//��Ƶ�ļ�
class VideoMediaFile
{
public:
	int  m_nWidth = 0;
	int  m_nHeight = 0;
	int  m_nCodeID = 0;

	mappVideoPack  m_mappData;  //�����б�

	DEFINE_PUSHMSG(VideoMediaFile, i.m_nWidth << i.m_nHeight << i.m_nCodeID << i.m_mappData);
	DEFINE_POPMSG(VideoMediaFile, i.m_nWidth >> i.m_nHeight >> i.m_nCodeID >> i.m_mappData);
};
