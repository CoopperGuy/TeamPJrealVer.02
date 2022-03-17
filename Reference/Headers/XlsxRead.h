#pragma once

#include "Base.h"

BEGIN(Engine)

class CXlsxRead :
	public CBase
{
	DECLARE_SINGLETON(CXlsxRead)

public:
	CXlsxRead();
	virtual ~CXlsxRead() = default;
public:
	HRESULT Ready_XlsxLoader(string _path);
	pair<string, ITEMINFO> GetItemAsName(string name);
public:
	void SetIsFinish() { m_bIsFinish = true; }
public:
	_bool GetIsFinish() { return m_bIsFinish; }
	list<pair<string,ITEMINFO>> GetItemTypeList(ITEMTYPE _type);
private:
	unordered_map<string, ITEMINFO> m_itemDB;
private:
	_bool	m_bIsFinish = false;
public:
	virtual void Free() override;
};

END