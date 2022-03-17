#include "stdafx.h"
#include "..\Public\Item.h"

USING(Client)

CItem::CItem()
{
}

CItem::CItem(ITEMINFO _info)
	:m_tinfo(_info)
{
}

CItem::CItem(CItem* rhs)
	:m_tinfo(rhs->m_tinfo)
{
}


CItem::~CItem()
{
}
