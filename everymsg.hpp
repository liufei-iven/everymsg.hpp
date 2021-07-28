/*****************************************************************************************************************************
*   Copyright(c) 2016-2020  ������ -- ����                                                                                   *
*   All rights reserved.                                                                                                     *
*                                                                                                                            *
*  ��  ��: everymsg.hpp   create by liufei    2015.5.28                                                                      *
*  ��  ��: ��Ϣ�ṹ���л�;                                                                                                   *
*  ˵  ��: ���ṹ���������л���һ��������Buf��������;                                                                        *
*  ��  ע: ��ֹʹ��long �� unsigned long���Ͳ�����Ϣ���л�, ��ʹ��int64���ʹ���;                                             *
*                                                                                                                            *
******************************************************************************************************************************/


/********************************************************************************************************
*                                                                                                       *
*   ע�⣺������Ϣ���ݽṹʱ����ֹʹ��long��unsigned long���͵����ݳ�Ա����ʹ��int64���档              *
*   ***** ��Ϊlong������linux64ƽ̨��ռ��64���ֽڣ���Ŀǰ��֪������ƽ̨����32���ֽڡ�****               *
*                                                                                                       *
*********************************************************************************************************/



#pragma once

#include <iostream>
#include <map>
#include <list>
#include <vector>

#include "sbuffer.hpp"

#ifndef USE_TYPE_SAFETY
#define USE_TYPE_SAFETY    //�����������Ͱ�ȫ
#endif // !USE_TYPE_SAFETY


#pragma pack(push,4)      //ָ���ڴ����ϵ��


//��Ϣͷ��������
class MsgHead
{
public:
	inline MsgHead()
	{
		m_nMsgID = 0;
		m_nModeID = 0;
	}

	inline ~MsgHead() {}

	int  m_nMsgID;    //��ϢID
	int  m_nModeID;   //ģ��ID
};
#pragma pack(pop)


class EveryMsg
{
public:
	inline virtual ~EveryMsg() {}

	inline EveryMsg();//����

	inline EveryMsg(int nMsgID, int nModeID = 0);//����2

	inline EveryMsg(const EveryMsg& tMsg);//��������

	inline EveryMsg(EveryMsg&& tMsg);//�ƶ�����
									 //�ƶ�����ζ���޸�,���Բ���const
									 //����������ֵʱ�������Ȱ��ƶ����캯��

	inline EveryMsg& operator = (const EveryMsg& tMsg);//������ֵ

	inline EveryMsg& operator = (EveryMsg&& tMsg);//�ƶ���ֵ


	//����Msg����; ����Դ�����Ǵӵ�ǰ���GetData(...)��GetDataLen()�����л�ȡ;
	inline bool LoadData(const char* pBuf, unsigned int nLen);

	//��ȡ��Ϣͷ��Ϣ;
	inline MsgHead* GetHead()
	{
		return (MsgHead*)m_Buffer.GetBuf();
	}

	//��ȡ���л���ɵ�����;  bIsCopy:������new����;
	inline char* GetData(bool bIsNew = false) const
	{
		return m_Buffer.GetData(0, bIsNew);
	}

	//��ȡ���л�������ݵĳ���;
	inline unsigned int GetDataLen() const
	{
		return m_Buffer.GetDataLen();
	}

	//���MSG����, ͬʱ������Ϣͷ;
	inline void ClearData(int nMsgID = 0, int nModeID = 0);

	//��������
#define INPUT_DATA()                      \
    if (!m_Buffer.AppendData((char*)&value, sizeof(value))) \
    {                                     \
        throw "EveryMsg In Overload !!!"; \
        return *this;                     \
    }                                     \
    return *this;

	//�������
#define OUTPUT_DATA(_TYPE)                 \
    if (m_Buffer.m_ParamEx.unNum + sizeof(_TYPE) > m_Buffer.GetDataLen()) \
    {                                      \
        throw "EveryMsg Out Overload !!!"; \
        return *this;                      \
    }                                      \
    value = *(_TYPE*)(m_Buffer.GetBuf(m_Buffer.m_ParamEx.unNum)); \
    m_Buffer.m_ParamEx.unNum  += sizeof(_TYPE); \
    return *this;

#ifdef USE_TYPE_SAFETY
	//���л�����; ��ٻ�����������;
	inline EveryMsg& operator << (const bool& value) { INPUT_DATA() }

	inline EveryMsg& operator << (const char& value) { INPUT_DATA() }
	inline EveryMsg& operator << (const unsigned char& value) { INPUT_DATA() }

	inline EveryMsg& operator << (const short& value) { INPUT_DATA() }
	inline EveryMsg& operator << (const unsigned short& value) { INPUT_DATA() }

	inline EveryMsg& operator << (const int& value) { INPUT_DATA() }
	inline EveryMsg& operator << (const unsigned int& value) { INPUT_DATA() }

	inline EveryMsg& operator << (const long& value) { INPUT_DATA() }
	inline EveryMsg& operator << (const unsigned long& value) { INPUT_DATA() }

	inline EveryMsg& operator << (const float& value) { INPUT_DATA() }
	inline EveryMsg& operator << (const double& value) { INPUT_DATA() }
#else
	//���л�����; ʹ��ģ�巺�ʹ���
	template<class T> inline EveryMsg& operator << (const T& value) { INPUT_DATA() }
#endif // USE_TYPE_SAFETY


	//std::string���Ի�����;
	inline EveryMsg& operator << (const std::string& value)
	{
		//+1 �����ַ��������һ��0
		if (!m_Buffer.AppendData((char*)value.c_str(), (unsigned int)value.length() + 1))
		{
			throw "EveryMsg In String Overload !!!";
			return *this;
		}
		return *this;
	}
	
	//���л�SBuffer����; �����Ҫ���л������������Ƽ�ʹ��SBuffer����;
	inline EveryMsg& operator << (const SBuffer& value)
	{
		//*this << value.m_ParamEx;
		*this << value.GetDataLen();

		if (!m_Buffer.AppendData(value.GetBuf(), value.GetDataLen()))
		{
			throw "EveryMsg In SBuffer Overload !!!";
			return *this;
		}
		return *this;
	}

	//��׼stl��������
	template<typename T>
	inline EveryMsg& operator << (const std::vector<T>& value);

	template<typename T>
	inline EveryMsg& operator << (const std::vector<T*>& value);

	template<typename T>
	inline EveryMsg& operator << (const std::list<T>& value);

	template<typename T>
	inline EveryMsg& operator << (const std::list<T*>& value);

	template<typename T1, typename T2>
	inline EveryMsg& operator << (const std::map<T1, T2>& value);

	template<typename T1, typename T2>
	inline EveryMsg& operator << (const std::map<T1, T2*>& value);


#ifdef USE_TYPE_SAFETY
	//���л����;��ٻ�����������;
	inline EveryMsg& operator >> (bool& value) { OUTPUT_DATA(bool) }

	inline EveryMsg& operator >> (char& value) { OUTPUT_DATA(char) }
	inline EveryMsg& operator >> (unsigned char& value) { OUTPUT_DATA(unsigned char) }

	inline EveryMsg& operator >> (short& value) { OUTPUT_DATA(short) }
	inline EveryMsg& operator >> (unsigned short& value) { OUTPUT_DATA(unsigned short) }

	inline EveryMsg& operator >> (int& value) { OUTPUT_DATA(int) }
	inline EveryMsg& operator >> (unsigned int& value) { OUTPUT_DATA(unsigned int) }

	inline EveryMsg& operator >> (long& value) { OUTPUT_DATA(int) }
	inline EveryMsg& operator >> (unsigned long& value) { OUTPUT_DATA(unsigned int) }

	inline EveryMsg& operator >> (float& value) { OUTPUT_DATA(float) }
	inline EveryMsg& operator >> (double& value) { OUTPUT_DATA(double) }

#else
	//���л����; ʹ��ģ�巺�ʹ���
	template<class T> inline EveryMsg& operator >> (T& value) { OUTPUT_DATA(T) }
#endif //USE_TYPE_SAFETY

	//std::string���Ի����;
	inline EveryMsg& operator >> (std::string& value)
	{
		if (m_Buffer.m_ParamEx.unNum >= m_Buffer.GetDataLen())
		{
			throw "EveryMsg Out String Overload !!!";
			return *this;
		}
		value = (m_Buffer.GetBuf() + m_Buffer.m_ParamEx.unNum);
		m_Buffer.m_ParamEx.unNum += value.length() + 1;
		return *this;
	}

	//���л�SBuffer���; �����Ҫ���л������������Ƽ�ʹ��SBuffer����;
	inline EveryMsg& operator >> (SBuffer& value)
	{
		//*this >> value.m_ParamEx;
		unsigned int nLen = 0;
		*this >> nLen;

		if ((m_Buffer.m_ParamEx.unNum + nLen) > m_Buffer.GetDataLen())
		{
			throw "EveryMsg Out SBuffer Overload !!!";
			return *this;
		}
		value.ReSetData(m_Buffer.GetData(m_Buffer.m_ParamEx.unNum, false), nLen);
		m_Buffer.m_ParamEx.unNum += nLen;
		return *this;
	}

	//��׼stl�������
	template<typename T>
	inline EveryMsg& operator >> (std::vector<T>& value);

	template<typename T>
	inline EveryMsg& operator >> (std::vector<T*>& value);

	template<typename T>
	inline EveryMsg& operator >> (std::list<T>& value);

	template<typename T>
	inline EveryMsg& operator >> (std::list<T*>& value);

	template<typename T1, typename T2>
	inline EveryMsg& operator >> (std::map<T1, T2>& value);

	template<typename T1, typename T2>
	inline EveryMsg& operator >> (std::map<T1, T2*>& value);


	union ParamEx
	{
		void*   vPtr;
		long    nNum;
		unsigned long unNum;
	};

	ParamEx   m_ParamEx;    //��չ����

private:
	//m_Buffer.m_ParamEx.unNum //���ʱ��ǰ���ݵ�λ��
	SBuffer   m_Buffer; //���ݻ���(�������)
};


inline EveryMsg::EveryMsg()
{
	m_ParamEx.vPtr = NULL;
	ClearData();
}

inline EveryMsg::EveryMsg(int nMsgID, int nModeID)
{
	m_ParamEx.vPtr = NULL;
	ClearData(nMsgID, nModeID);
}

inline EveryMsg::EveryMsg(const EveryMsg& tMsg)
{
	m_ParamEx.vPtr = NULL;
	*this = tMsg;
}

inline EveryMsg::EveryMsg(EveryMsg&& tMsg)
{
	*this = std::move(tMsg);
}

inline EveryMsg& EveryMsg::operator = (const EveryMsg& tMsg)
{
	if (this == &tMsg)
		return *this;

	m_ParamEx = tMsg.m_ParamEx;
	m_Buffer = tMsg.m_Buffer;
	return *this;
}

inline EveryMsg& EveryMsg::operator = (EveryMsg&& tMsg)
{
	if (this == &tMsg)
		return *this;

	m_ParamEx = std::move(tMsg.m_ParamEx);
	m_Buffer = std::move(tMsg.m_Buffer);
	return *this;
}

inline bool EveryMsg::LoadData(const char* pBuf, unsigned int nLen)
{
	if (nLen < sizeof(MsgHead))
	{
		return false;
	}
	m_Buffer.m_ParamEx.unNum = sizeof(MsgHead);
	return m_Buffer.ReSetData(pBuf, nLen);
}

inline void EveryMsg::ClearData(int nMsgID, int nModeID)
{
	m_Buffer.ClearData();
	m_Buffer.AppendData(NULL, sizeof(MsgHead));
	m_Buffer.m_ParamEx.unNum = sizeof(MsgHead);
	GetHead()->m_nMsgID = nMsgID;
	GetHead()->m_nModeID = nModeID;
}



template<typename T>
inline EveryMsg& EveryMsg::operator << (const std::vector<T>& value)
{
	int nCount = (int)value.size();
	*this << nCount;

	for (auto& item : value)
	{
		*this << item;
	}
	return *this;
}

template<typename T>
inline EveryMsg& EveryMsg::operator << (const std::vector<T*>& value)
{
	int nCount = (int)value.size();
	*this << nCount;

	for (auto& item : value)
	{
		*this << *item;
	}
	return *this;
}


template<typename T>
inline EveryMsg& EveryMsg::operator << (const std::list<T>& value)
{
	int nCount = (int)value.size();
	*this << nCount;

	for (auto& item : value)
	{
		*this << item;
	}
	return *this;
}

template<typename T>
inline EveryMsg& EveryMsg::operator << (const std::list<T*>& value)
{
	int nCount = (int)value.size();
	*this << nCount;

	for (auto& item : value)
	{
		*this << *item;
	}
	return *this;
}


template<typename T1, typename T2>
inline EveryMsg& EveryMsg::operator << (const std::map<T1, T2>& value)
{
	int nCount = (int)value.size();
	*this << nCount;

	for (auto& item : value)
	{
		*this << item.first;
		*this << item.second;
	}
	return *this;
}

template<typename T1, typename T2>
inline EveryMsg& EveryMsg::operator << (const std::map<T1, T2*>& value)
{
	int nCount = (int)value.size();
	*this << nCount;

	for (auto& item : value)
	{
		*this << item.first;
		*this << *item.second;
	}
	return *this;
}



template<typename T>
inline EveryMsg& EveryMsg::operator >> (std::vector<T>& value)
{
	value.clear();
	int nCount = 0;
	*this >> nCount;

	while (nCount--)
	{
		T Elem;
		*this >> Elem;
		value.push_back(std::move(Elem));
	}
	return *this;
}

template<typename T>
inline EveryMsg& EveryMsg::operator >> (std::vector<T*>& value)
{
	value.clear();
	int nCount = 0;
	*this >> nCount;

	while (nCount--)
	{
		T* pElem = new T();
		*this >> *pElem;
		value.push_back(std::move(pElem));
	}
	return *this;
}



template<typename T>
inline EveryMsg& EveryMsg::operator >> (std::list<T>& value)
{
	value.clear();
	int nCount = 0;
	*this >> nCount;

	while (nCount--)
	{
		T Elem;
		*this >> Elem;
		value.push_back(std::move(Elem));
	}
	return *this;
}

template<typename T>
inline EveryMsg& EveryMsg::operator >> (std::list<T*>& value)
{
	value.clear();
	int nCount = 0;
	*this >> nCount;

	while (nCount--)
	{
		T* pElem = new T();
		*this >> *pElem;
		value.push_back(std::move(pElem));
	}
	return *this;
}


template<typename T1, typename T2>
inline EveryMsg& EveryMsg::operator >> (std::map<T1, T2>& value)
{
	value.clear();
	int nCount = 0;
	*this >> nCount;

	while (nCount--)
	{
		T1 key;
		*this >> key;
		T2 Elem;
		*this >> Elem;
		value.insert(std::make_pair(std::move(key), std::move(Elem)));
	}
	return *this;
}

template<typename T1, typename T2>
inline EveryMsg& EveryMsg::operator >> (std::map<T1, T2*>& value)
{
	value.clear();
	int nCount = 0;
	*this >> nCount;

	while (nCount--)
	{
		T1 key;
		*this >> key;
		T2* pElem = new T2();
		*this >> *pElem;
		value.insert(std::make_pair(std::move(key), std::move(pElem)));
	}
	return *this;
}


//(*)����EveryMsg������Ԫ����; ����Ƕ��Դ�ṹ�ڲ�, ǿ�з�����ϵ
#define DEFINE_PUSHMSG(classname, Input)        \
friend EveryMsg& operator << (EveryMsg& p, const classname& i) \
{                                               \
    p << Input;                                 \
    return p;                                   \
}

//(*)����EveryMsg�����Ԫ����; ����Ƕ��Դ�ṹ�ڲ�, ǿ�з�����ϵ
#define DEFINE_POPMSG(classname, Output)        \
friend EveryMsg& operator >> (EveryMsg& p,classname& i) \
{                                               \
    try                                         \
    {                                           \
        p >> Output;                            \
    }                                           \
    catch(...)                                  \
    {                                           \
    }                                           \
    return p;                                   \
}



//�ض���map��; ��Ϊ���л�map���͵����ݽṹ
#define DEFINE_MAP(type, classname, baseclass)  \
class classname : public std::map<type, baseclass*>\
{                                               \
public:                                         \
    classname(bool bClearMem = true)            \
    :m_bClearMem(bClearMem)                     \
    {                                           \
        ClearAll();                             \
    }                                           \
    ~classname()                                \
    {                                           \
        if(m_bClearMem)                         \
        {                                       \
            ClearAll();                         \
        }else                                   \
        {   clear();                            \
        }                                       \
    }                                           \
    void ClearAll()                             \
    {                                           \
        iterator iter;                          \
        for(iter = begin();iter != end();iter++)\
        {                                       \
            delete iter->second;                \
        }                                       \
        clear();                                \
    }                                           \
    baseclass* Get(type nID)                    \
    {                                           \
        iterator iter = find(nID);              \
        if(iter == end())                       \
        {                                       \
            return NULL;                        \
        }else                                   \
        {                                       \
            return iter->second;                \
        }                                       \
    }                                           \
    void Add(type nID,baseclass* pAdd)          \
    {                                           \
        (*this)[nID] = pAdd;                    \
    }                                           \
    void Del(type nID)                          \
    {                                           \
        iterator iter = find(nID);              \
        if(iter != end())                       \
        {                                       \
            delete iter->second;                \
            erase(iter);                        \
        }                                       \
    }                                           \
    friend EveryMsg& operator << (EveryMsg& p, const classname& info) \
    {                                           \
        int nCount = (int)info.size();          \
        p << nCount;                            \
        for(auto& item : info)                  \
        {                                       \
            p << item.first;                    \
            p << *item.second;                  \
        }                                       \
        return p;                               \
    }                                           \
    friend EveryMsg& operator >> (EveryMsg& p,classname& info) \
    {                                           \
        try                                     \
        {                                       \
            int nCount;                         \
            p >> nCount;                        \
            type nID; baseclass* pBase;         \
            while(nCount > 0)                   \
            {                                   \
                nCount--;                       \
                pBase = new baseclass();        \
                p >> nID;                       \
                p >> *pBase;                    \
                info[nID] = pBase;              \
            }                                   \
        }                                       \
        catch(...)                              \
        {                                       \
        }                                       \
        return p;                               \
    }                                           \
    bool m_bClearMem;                           \
};

//�ض���vector��; ��Ϊ���л�vector���͵����ݽṹ
#define DEFINE_VECTOR(classname,baseclass)      \
class classname : public std::vector<baseclass*>\
{                                               \
public:                                         \
    classname(bool bClearMem = true)            \
    :m_bClearMem(bClearMem)                     \
    {                                           \
        ClearAll();                             \
    }                                           \
    ~classname()                                \
    {                                           \
        if(m_bClearMem)                         \
        {                                       \
            ClearAll();                         \
        }else                                   \
        {                                       \
            clear();                            \
        }                                       \
    }                                           \
    void ClearAll()                             \
    {                                           \
        iterator iter;                          \
        for(iter = begin();iter != end();iter++)\
        {                                       \
            delete *iter;                       \
        }                                       \
        clear();                                \
    }                                           \
    friend EveryMsg& operator << (EveryMsg& p,const classname& info) \
    {                                           \
        int nCount = (int)info.size();          \
        p << nCount;                            \
        for(auto& item : info)                  \
        {                                       \
            p << *item;                         \
        }                                       \
        return p;                               \
    }                                           \
    friend EveryMsg& operator >> (EveryMsg& p,classname& info)\
    {                                           \
        try                                     \
        {                                       \
            int nCount;                         \
            p >> nCount;                        \
            baseclass* pBase;                   \
            while(nCount > 0)                   \
            {                                   \
                nCount--;                       \
                pBase = new baseclass();        \
                p >> *pBase;                    \
                info.push_back(pBase);          \
            }                                   \
        }                                       \
        catch(...)                              \
        {                                       \
        }                                       \
        return p;                               \
    }                                           \
    bool m_bClearMem;                           \
};    


//�ض���list��; ��Ϊ���л�list���͵����ݽṹ
#define DEFINE_LIST(classname,baseclass)        \
class classname : public std::list<baseclass*>  \
{                                               \
public:                                         \
    classname(bool bClearMem = true)            \
    :m_bClearMem(bClearMem)                     \
    {                                           \
        ClearAll();                             \
    }                                           \
    ~classname()                                \
    {                                           \
        if(m_bClearMem)                         \
        {                                       \
            ClearAll();                         \
        }else                                   \
        {                                       \
            clear();                            \
        }                                       \
    }                                           \
    void ClearAll()                             \
    {                                           \
        iterator iter;                          \
        for(iter = begin();iter != end();iter++)\
        {                                       \
            delete *iter;                       \
        }                                       \
        clear();                                \
    }                                           \
    friend EveryMsg& operator << (EveryMsg& p,const classname& info) \
    {                                           \
        int nCount = (int)info.size();          \
        p << nCount;                            \
        for(auto& item : info)                  \
        {                                       \
            p << *item;                         \
        }                                       \
        return p;                               \
    }                                           \
    friend EveryMsg& operator >> (EveryMsg& p,classname& info) \
    {                                           \
        try                                     \
        {                                       \
            int nCount;                         \
            p >> nCount;                        \
            baseclass* pBase;                   \
            while(nCount > 0)                   \
            {                                   \
                nCount--;                       \
                pBase = new baseclass();        \
                p >> *pBase;                    \
                info.push_back(pBase);          \
            }                                   \
        }                                       \
        catch(...)                              \
        {                                       \
        }                                       \
        return p;                               \
    }                                           \
    bool m_bClearMem;                           \
};

