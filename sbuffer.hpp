/*****************************************************************************************************************************
*   Copyright(c) 2016-2020  自由人 -- 刘飞                                                                                   *
*   All rights reserved.                                                                                                     *
*                                                                                                                            *
*  文  件: sbuffer.hpp   create by liufei    2015.5.20                                                                       *
*  功  能: 聪明的缓冲区;                                                                                                     *
*  说  明: 无;                                                                                                               *
*  备  注: 无;                                                                                                               *
*                                                                                                                            *
******************************************************************************************************************************/

#pragma once

#include <iostream>
#include <string.h>

#define SBUFFER_DEF_SIZE  512           //默认缓冲区大小
#define SBUFFER_MAX_SIZE  1024 * 1024 * 64  //最大支持64M



class SBuffer
{
public:
	virtual inline ~SBuffer();//析构

	inline SBuffer(unsigned int nLen = 0);//构造

	inline SBuffer(const SBuffer& tBuffer);//拷贝构造

	inline SBuffer(SBuffer&& tBuffer);//移动构造
									  //移动就意味着修改,所以不加const
									  //当发现是右值时，会优先绑定移动构造函数

	inline SBuffer& operator = (const SBuffer& tBuffer);//拷贝赋值

	inline SBuffer& operator = (SBuffer&& tBuffer);//移动赋值

												   //重置数据
	inline bool ReSetData(const char* pData, unsigned int nLen);

	//追加数据; 当pData为NULL时数据的长度依然会递增
	inline bool AppendData(const char* pData, unsigned int nLen);

	//删除数据;
	inline bool EraseData(unsigned int nBeginPos, unsigned int nCount);

	//增加Buffer的长度; 如果已有数据,数据不会丢失
	inline bool AddBufferLen(unsigned int nAddLen = 128);

	//获取数据; nBeginPos: 数据的开始位置; bIsCopy:是否new返回
	inline char* GetData(unsigned int nBeginPos = 0, bool bIsNew = false) const;

	//获取数据长度
	inline unsigned int GetDataLen() const { return m_nDataLen; }

	//获取Buf; nBeginPos: Buf的开始位置
	inline char* GetBuf(unsigned int nBeginPos = 0) const { return m_pBuf + nBeginPos; }

	//获取Buf长度
	inline unsigned int GetBufLen() const { return m_nLen; }

	//清除数据
	inline void ClearData() { SetDataLen(); }

	//设置数据长度, 默认清除数据
	inline void SetDataLen(unsigned int nLen = 0);

	//是否自动释放内存
	inline void AutoFreeMem(bool bisAuto);

	union ParamEx
	{
		void*    vPtr;
		long     nNum;
		unsigned long unNum;
	};

	ParamEx         m_ParamEx;    //扩展参数

protected:

private:
	char*           m_pBuf;       //缓冲区;
	unsigned int    m_nLen;       //缓冲区长度;
	unsigned int    m_nDataLen;   //数据长度;
	bool            m_bIsFree;    //是否释放内存;
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
	//重置原先数据长度
	m_nDataLen = 0;

	//保证Buf长度足够
	AddBufferLen(nLen); //增加Buffer定会成功
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
		//没有数据
		return false;
	}

	if (nBeginPos >= m_nDataLen || nCount > m_nDataLen)
	{
		//坐标或者长度越界
		return false;
	}

	if (nBeginPos > 0 && nBeginPos + nCount >= m_nDataLen)
	{
		//如果是全删除, nBeginPos必须是0
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
	if (m_nDataLen + nAddLen > SBUFFER_MAX_SIZE) //超过最大限制
	{
		return false;
	}
	else if (m_nDataLen + nAddLen <= m_nLen)     //剩余的空间足够
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
