#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNPCInfo final:
public CComponent
{
public:
	typedef struct tagNPC {
		_uint npcID = 0;
		_uint hasQuestID = 0;
		_uint boxID = 0;
	}NPCTAG;
private:
	explicit CNPCInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNPCInfo(const CNPCInfo& rhs);
	virtual ~CNPCInfo() = default;

public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void SetNpcInfo(NPCTAG _info) { m_tNPCInfo = _info; }
	void SetNPCID(_uint _id) { m_tNPCInfo.npcID = _id; }
	void SetNPCQuestID(_uint _id) { m_tNPCInfo.hasQuestID = _id; }
	void SetBoxID(_uint _id) { m_tNPCInfo.boxID = _id; }
public:
	NPCTAG GetNPCInfo() { return m_tNPCInfo; }
	_uint GetNPCID() { return m_tNPCInfo.npcID; }
	_uint GetNPCQuestID() { return m_tNPCInfo.hasQuestID; }
	_uint GetBoxID() { return m_tNPCInfo.boxID; }
private:
	NPCTAG m_tNPCInfo{};

public:
	static CNPCInfo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END