#include "msgdef.h"

bool initdata(LoginRsp* _out_pLoginRsp)
{
    bool bRet = false;
    _out_pLoginRsp->nErrorNo = 0;
    _out_pLoginRsp->strError = "OK";

    CityInfo* pCity1 = new CityInfo();
    pCity1->nCityID = 1;
    pCity1->strCityName = "����";
    _out_pLoginRsp->listpCity.push_back(pCity1);

    CityInfo* pCity2 = new CityInfo();
    pCity2->nCityID = 2;
    pCity2->strCityName = "����";
    _out_pLoginRsp->listpCity.push_back(pCity2);
    
    string strUName[6] = { "����", "���", "��̩��", "����", "����", "������" };
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

    //���л�;���ṹ�����ݴ��л�(�õ�����������pBuf)
    {
        int nIn = 11;
        string strIn("abc123456");

		EveryMsg Msg;
		Msg.ClearData(999);//������Ϣ����
		Msg << nIn;  //���л�
		Msg << strIn;

		nLen = Msg.GetDataLen();
		memcpy(pBuf, Msg.GetData(), Msg.GetDataLen());
    }

    //�����л�;ͨ������������pBuf, �õ��ṹ������
    {
        int nOut = 0;
        string strOut;

		EveryMsg Msg;
		Msg.LoadData(pBuf, nLen);
		int nMsgID = Msg.GetHead()->m_nMsgID; //��ȡ��Ϣ����
		Msg >> nOut;  //�����л�
		Msg >> strOut;
    }
	delete pBuf;
}

void example2()
{
	char* pBuf = new char[1024];
    int   nLen = 1024;

    //���л�;���ṹ�����ݴ��л�(�õ�����������pBuf)
    {
		LoginRsp LoginRspIn;  //������Ϣ�ṹ
		initdata(&LoginRspIn);

		EveryMsg Msg;
		Msg.ClearData(888);//������Ϣ����
		Msg << LoginRspIn;  //���л�
        
		nLen = Msg.GetDataLen();
		memcpy(pBuf, Msg.GetData(), Msg.GetDataLen());
    }
	
	//�����л�;ͨ������������pBuf, �õ��ṹ������
    {
		EveryMsg Msg;
		Msg.LoadData(pBuf, nLen);
		int nMsgID = Msg.GetHead()->m_nMsgID; //��ȡ��Ϣ����

        LoginRsp loginRspOut; //�����Ϣ�ṹ		
		Msg >> loginRspOut; //�����л�
    }
	delete pBuf;
}

void example3()
{
	char* pBuf = new char[1024];
	int   nLen = 1024;

	//���л�;���ṹ�����ݴ��л�(�õ�����������pBuf)
	{
		VideoMediaFile VideFile;  //������Ϣ�ṹ
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
		Msg.ClearData(888);//������Ϣ����
		Msg << VideFile;  //���л�

		nLen = Msg.GetDataLen();
		memcpy(pBuf, Msg.GetData(), Msg.GetDataLen());
	}

	//�����л�;ͨ������������pBuf, �õ��ṹ������
	{
		EveryMsg Msg;
		Msg.LoadData(pBuf, nLen);
		int nMsgID = Msg.GetHead()->m_nMsgID; //��ȡ��Ϣ����

		VideoMediaFile VideFile; //�����Ϣ�ṹ
		Msg >> VideFile; //�����л�
	}
	delete pBuf;
}

void main()
{
	example1();
	example2();
	example3();
}
