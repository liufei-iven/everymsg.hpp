#include "msgdef.h"

bool initdata(LoginRsp* _out_pLoginRsp)
{
    bool bRet = false;
    _out_pLoginRsp->nErrorNo = 0;
    _out_pLoginRsp->strError = "OK";

    CityInfo* pCity1 = new CityInfo();
    pCity1->nCityID = 1;
    pCity1->strCityName = "深圳";
    _out_pLoginRsp->listpCity.push_back(pCity1);

    CityInfo* pCity2 = new CityInfo();
    pCity2->nCityID = 2;
    pCity2->strCityName = "杭州";
    _out_pLoginRsp->listpCity.push_back(pCity2);
    
    string strUName[6] = { "马化腾", "李开复", "郭泰铭", "马云", "许仙", "白娘子" };
    int nUserID[6] = { 100, 101, 102, 200, 201, 202 };

    for (int i = 0; i < (sizeof(nUserID) / sizeof(int)); i++)
    {
        UserInfo* pUser = new UserInfo();
        pUser->nUserID = nUserID[i];
        pUser->strUName = strUName[i];
        if(i < 3)
        {
            pUser->nCityID = pCity1->nCityID;
            pCity1->listpUser.push_back(pUser);
        }
        else
        {
            pUser->nCityID = pCity2->nCityID;
            pCity2->listpUser.push_back(pUser);
        }
    }
    return bRet;
}

void example1()
{
	char* pBuf = new char[1024];
	int   nLen = 1024;

    //序列化;将结构化数据串行化(得到二进制数据pBuf)
    {
        int nIn = 11;
        string strIn("abc123456");

		EveryMsg Msg;
		Msg.ClearData(999);//设置消息类型
		Msg << nIn;  //序列化
		Msg << strIn;

		nLen = Msg.GetDataLen();
		memcpy(pBuf, Msg.GetData(), Msg.GetDataLen());
    }

    //反序列化;通过二进制数据pBuf, 得到结构化数据
    {
        int nOut = 0;
        string strOut;

		EveryMsg Msg;
		Msg.LoadData(pBuf, nLen);
		int nMsgID = Msg.GetHead()->m_nMsgID; //获取消息类型
		Msg >> nOut;  //反序列化
		Msg >> strOut;
    }
	delete pBuf;
}

void example2()
{
	char* pBuf = new char[1024];
    int   nLen = 1024;

    //序列化;将结构化数据串行化(得到二进制数据pBuf)
    {
		LoginRsp LoginRspIn;  //输入消息结构
		initdata(&LoginRspIn);

		EveryMsg Msg;
		Msg.ClearData(888);//设置消息类型
		Msg << LoginRspIn;  //序列化
        
		nLen = Msg.GetDataLen();
		memcpy(pBuf, Msg.GetData(), Msg.GetDataLen());
    }
	
	//反序列化;通过二进制数据pBuf, 得到结构化数据
    {
		EveryMsg Msg;
		Msg.LoadData(pBuf, nLen);
		int nMsgID = Msg.GetHead()->m_nMsgID; //获取消息类型

        LoginRsp loginRspOut; //输出消息结构		
		Msg >> loginRspOut; //反序列化
    }
	delete pBuf;
}

void example3()
{
	char* pBuf = new char[1024];
	int   nLen = 1024;

	//序列化;将结构化数据串行化(得到二进制数据pBuf)
	{
		VideoMediaFile VideFile;  //输入消息结构
		VideFile.m_nWidth = 1920;
		VideFile.m_nHeight = 1080;
		VideFile.m_nCodeID = 1;

		for (int i = 0; i < 10; i++)
		{
			VideoPack* pPack = new VideoPack();
			pPack->m_nSeqNumber = i;
			pPack->m_Data.AppendData("abcdefg", 8);
			VideFile.m_mappData.Add(pPack->m_nSeqNumber, pPack);
		}
		
		EveryMsg Msg;
		Msg.ClearData(888);//设置消息类型
		Msg << VideFile;  //序列化

		nLen = Msg.GetDataLen();
		memcpy(pBuf, Msg.GetData(), Msg.GetDataLen());
	}

	//反序列化;通过二进制数据pBuf, 得到结构化数据
	{
		EveryMsg Msg;
		Msg.LoadData(pBuf, nLen);
		int nMsgID = Msg.GetHead()->m_nMsgID; //获取消息类型

		VideoMediaFile VideFile; //输出消息结构
		Msg >> VideFile; //反序列化
	}
	delete pBuf;
}

void main()
{
	example1();
	example2();
	example3();
}
