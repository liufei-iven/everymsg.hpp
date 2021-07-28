
#pragma once

#include "everymsg.hpp"

using namespace std;

//用户信息
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


//城市信息
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
	listpUserInfo  listpUser;    //用户列表

	DEFINE_PUSHMSG(CityInfo, i.nCityID << i.strCityName << i.listpUser);
	DEFINE_POPMSG(CityInfo, i.nCityID >> i.strCityName >> i.listpUser);
};
typedef std::vector<CityInfo*> listpCityInfo;

//登录消息返回
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
	SBuffer        m_FileData;//二进制数据
	listpCityInfo  listpCity; //城市列表

	DEFINE_PUSHMSG(LoginRsp, i.nErrorNo << i.strError << i.m_FileData << i.listpCity);
	DEFINE_POPMSG(LoginRsp, i.nErrorNo >> i.strError >> i.m_FileData >> i.listpCity);
};


//视频数据包
//struct LoginRsp
class VideoPack
{
public:
	int      m_nSeqNumber = 0;  //序列号
	bool     m_bIsFrameKey = false; //是否为关键帧
	SBuffer  m_Data;                //数据包

	DEFINE_PUSHMSG(VideoPack, i.m_nSeqNumber << i.m_bIsFrameKey << i.m_Data);
	DEFINE_POPMSG(VideoPack, i.m_nSeqNumber >> i.m_bIsFrameKey >> i.m_Data);

};
DEFINE_MAP(int, mappVideoPack, VideoPack)

//视频文件
class VideoMediaFile
{
public:
	int  m_nWidth = 0;
	int  m_nHeight = 0;
	int  m_nCodeID = 0;

	mappVideoPack  m_mappData;  //数据列表

	DEFINE_PUSHMSG(VideoMediaFile, i.m_nWidth << i.m_nHeight << i.m_nCodeID << i.m_mappData);
	DEFINE_POPMSG(VideoMediaFile, i.m_nWidth >> i.m_nHeight >> i.m_nCodeID >> i.m_mappData);
};
