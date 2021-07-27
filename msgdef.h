
#pragma once

#include "everymsg.hpp"

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
	SBuffer        m_data;
	listpCityInfo  listpCity; //城市列表

	DEFINE_PUSHMSG(LoginRsp, i.nErrorNo << i.strError << i.m_data << i.listpCity);
	DEFINE_POPMSG(LoginRsp, i.nErrorNo >> i.strError >> i.m_data >> i.listpCity);
};
