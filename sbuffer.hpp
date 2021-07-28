/*****************************************************************************************************************************
*   Copyright(c) 2016-2020  ������ -- ����                                                                                   *
*   All rights reserved.                                                                                                     *
*                                                                                                                            *
*  ��  ��: sbuffer.hpp   create by liufei    2015.5.20                                                                       *
*  ��  ��: �����Ļ�����;                                                                                                     *
*  ˵  ��: ��;                                                                                                               *
*  ��  ע: ��;                                                                                                               *
*                                                                                                                            *
******************************************************************************************************************************/

#pragma once

#include <iostream>

#define SBUFFER_DEF_SIZE  512           //Ĭ�ϻ�������С
#define SBUFFER_MAX_SIZE  1024 * 1024 * 64  //���֧��64M



class SBuffer
{
public:
	virtual inline ~SBuffer();//����

	inline SBuffer(unsigned int nLen = 0);//����

	inline SBuffer(const SBuffer& tBuffer);//��������

	inline SBuffer(SBuffer&& tBuffer);//�ƶ�����
									  //�ƶ�����ζ���޸�,���Բ���const
									  //����������ֵʱ�������Ȱ��ƶ����캯��

	inline SBuffer& operator = (const SBuffer& tBuffer);//������ֵ

	inline SBuffer& operator = (SBuffer&& tBuffer);//�ƶ���ֵ

												   //��������
	inline bool ReSetData(const char* pData, unsigned int nLen);

	//׷������; ��pDataΪNULLʱ���ݵĳ�����Ȼ�����
	inline bool AppendData(const char* pData, unsigned int nLen);

	//ɾ������;
	inline bool EraseData(unsigned int nBeginPos, unsigned int nCount);

	//����Buffer�ĳ���; �����������,���ݲ��ᶪʧ
	inline bool AddBufferLen(unsigned int nAddLen = 128);

	//��ȡ����; nBeginPos: ���ݵĿ�ʼλ��; bIsCopy:�Ƿ�new����
	inline char* GetData(unsigned int nBeginPos = 0, bool bIsNew = false) const;

	//��ȡ���ݳ���
	inline unsigned int GetDataLen() const { return m_nDataLen; }

	//��ȡBuf; nBeginPos: Buf�Ŀ�ʼλ��
	inline char* GetBuf(unsigned int nBeginPos = 0) const { return m_pBuf + nBeginPos; }

	//��ȡBuf����
	inline unsigned int GetBufLen() const { return m_nLen; }

	//�������
	inline void ClearData() { SetDataLen(); }

	//�������ݳ���, Ĭ���������
	inline void SetDataLen(unsigned int nLen = 0);

	//�Ƿ��Զ��ͷ��ڴ�
	inline void AutoFreeMem(bool bisAuto);

	union ParamEx
	{
		void*    vPtr;
		long     nNum;
		unsigned long unNum;
	};

	ParamEx         m_ParamEx;    //��չ����

protected:

private:
	char*           m_pBuf;       //������;
	unsigned int    m_nLen;       //����������;
	unsigned int    m_nDataLen;   //���ݳ���;
	bool            m_bIsFree;    //�Ƿ��ͷ��ڴ�;
};

inline SBuffer::SBuffer(unsigned int nLen)
{
	m_ParamEx.vPtr = NULL;

	m_pBuf = NULL;
	m_nLen = 0;
	m_nDataLen = 0;
	m_bIsFree = true;

	if (AddBufferLen(nLen))
	{
		if (nLen) m_pBuf[nLen - 1] = '\0';
	}
}

inline SBuffer::SBuffer(const SBuffer& tBuffer)
{
	m_pBuf = NULL;
	m_nLen = 0;
	m_nDataLen = 0;
	m_bIsFree = true;

	m_ParamEx = tBuffer.m_ParamEx;

	if (tBuffer.GetDataLen() > 0)
	{
		AppendData(tBuffer.GetData(), tBuffer.GetDataLen());
	}
	else
	{
		AddBufferLen(tBuffer.GetBufLen());
	}

}

inline SBuffer::SBuffer(SBuffer&& tBuffer)
{
	m_ParamEx.vPtr = NULL;

	m_pBuf = NULL;
	m_nLen = 0;
	m_nDataLen = 0;
	m_bIsFree = true;

	*this = std::move(tBuffer);
}

inline SBuffer& SBuffer::operator = (const SBuffer& tBuffer)
{
	if (this == &tBuffer)
		return *this;

	m_ParamEx = tBuffer.m_ParamEx;
	ClearData();
	ReSetData(tBuffer.GetBuf(), tBuffer.GetDataLen());
	return *this;
}

inline SBuffer& SBuffer::operator = (SBuffer&& tBuffer)
{
	if (this == &tBuffer)
		return *this;

	delete[] m_pBuf;

	m_ParamEx = tBuffer.m_ParamEx;
	m_pBuf = tBuffer.m_pBuf;
	m_nLen = tBuffer.m_nLen;
	m_nDataLen = tBuffer.m_nDataLen;
	m_bIsFree = true;

	tBuffer.m_ParamEx.vPtr = NULL;
	tBuffer.m_pBuf = NULL;
	tBuffer.m_nLen = 0;
	tBuffer.m_nDataLen = 0;
	tBuffer.m_bIsFree = true;
	return *this;
}

inline SBuffer::~SBuffer()
{
	if (m_pBuf && m_bIsFree)
	{
		delete[]m_pBuf;
	}
	m_pBuf = NULL;
}

inline bool SBuffer::ReSetData(const char* pData, unsigned int nLen)
{
	if (pData == NULL || nLen > SBUFFER_MAX_SIZE)
	{
		return false;
	}
	//����ԭ�����ݳ���
	m_nDataLen = 0;

	//��֤Buf�����㹻
	AddBufferLen(nLen); //����Buffer����ɹ�
	memcpy(m_pBuf, pData, nLen);

	m_nDataLen = nLen;
	return true;
}

inline bool SBuffer::AppendData(const char* pData, unsigned int nLen)
{
	if (!AddBufferLen(nLen))
	{
		return false;
	}
	if (pData != NULL)
	{
		memcpy((m_pBuf + m_nDataLen), pData, nLen);
	}
	m_nDataLen = (m_nDataLen + nLen);
	return true;
}

inline bool SBuffer::EraseData(unsigned int nBeginPos, unsigned int nCount)
{
	if (m_nDataLen <= 0)
	{
		//û������
		return false;
	}

	if (nBeginPos >= m_nDataLen || nCount > m_nDataLen)
	{
		//������߳���Խ��
		return false;
	}

	if (nBeginPos > 0 && nBeginPos + nCount >= m_nDataLen)
	{
		//�����ȫɾ��, nBeginPos������0
		return false;
	}
	if (nCount)
	{
		char* pData = m_pBuf + nBeginPos;
		char* pData2 = pData + nCount;

		int nMoveCount = m_nDataLen - nBeginPos - nCount;
		while (nMoveCount-- > 0)
		{
			*pData++ = *pData2++;
		}
		m_nDataLen -= nCount;
	}
	return true;
}

inline char* SBuffer::GetData(unsigned int nBeginPos, bool bIsNew) const
{
	char* pTempBuf = NULL;
	if (m_nDataLen <= 0 || nBeginPos > m_nDataLen)
	{
		return pTempBuf;
	}

	if (bIsNew)
	{
		pTempBuf = new char[m_nDataLen - nBeginPos];
		memcpy(pTempBuf, (m_pBuf + nBeginPos), (m_nDataLen - nBeginPos));
	}
	else
	{
		pTempBuf = m_pBuf + nBeginPos;
	}
	return pTempBuf;
}

inline void SBuffer::SetDataLen(unsigned int nLen)
{
	/*
	if (m_pBuf != NULL && nLen == 0)
	{
	memset(m_pBuf, 0, m_nLen);
	}
	*/
	m_nDataLen = nLen;
}

inline void SBuffer::AutoFreeMem(bool bisAuto)
{
	m_bIsFree = bisAuto;
}

inline bool SBuffer::AddBufferLen(unsigned int nAddLen)
{
	if (m_nDataLen + nAddLen > SBUFFER_MAX_SIZE) //�����������
	{
		return false;
	}
	else if (m_nDataLen + nAddLen <= m_nLen)     //ʣ��Ŀռ��㹻
	{
		return true;
	}

	m_nLen = m_nLen + (m_nLen * 2) + nAddLen;
	m_nLen = m_nLen > SBUFFER_MAX_SIZE ? SBUFFER_MAX_SIZE : m_nLen;

	char* pbuf = new char[m_nLen];
	memset(pbuf, 0, m_nLen);
	if (m_nDataLen > 0)
	{
		memcpy(pbuf, m_pBuf, m_nDataLen);
	}
	if (m_pBuf)
	{
		delete[] m_pBuf;
	}
	m_pBuf = pbuf;
	return true;
}
