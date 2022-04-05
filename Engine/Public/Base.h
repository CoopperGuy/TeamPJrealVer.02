#pragma once

#include "Engine_Defines.h"

/* 모든 클래스들의 부모가 된다. */
/* 레퍼런스 카운트를 관리한다. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;
public:
	_uint AddRef();
	_uint Release();
#ifdef _DEBUG

public:
	const _uint&	getRefCnt() { return m_dwRefCnt; }

#endif // DEBUG

private:
	std::atomic<_uint>			m_dwRefCnt = 0;
public:
	/* 해당 클래스의 멤버를 정리한다. */
	virtual void Free() = 0;
};

END