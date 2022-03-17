#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
	//�׺���̼��� �������ִ� �� ��ü���� � index�����ִ��� ���� �����ϱ����ؼ� ������ش�.
	typedef struct tagNavigationDesc
{
	_uint		iCurrentCellIndex = 0;  //�׺���̼��� �������ִ� �� ��ü���� � index�����ִ��� ���� �����ϱ����ؼ� ������ش�. �̷��� ����ϴ� ��ü�� ��� Ž�� X
	_float3		iCurrentNormal = {};
}NAVIDESC;

private:
	explicit CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT InitializePrototype(const _tchar* pDataFiles);
	virtual HRESULT InitializePrototype_Tool ();
	virtual HRESULT Initialize(void* pArg) override;

public:
	//void AddPoint(_fvector fpoint);
	void AddPoint(_float3 fpoint);
	HRESULT PushBackCell();
public:
	_float3 NowLine = {};
	_bool isMove(_fvector vPoint);
	_float Get_SlidingAngle() { return angle; }
	void  Sliding(_float3 & vPos);

public:
	void Delete_Last();
	void Delete_All();

#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	_bool CCW(_float3 Point_1, _float3 Point_2, _float3 Point_3);

private:
	vector<class CCell*>				m_Cells;
	typedef vector<class CCell*>		CELLS;

public:
	vector<class CCell*>  Get_Cells() { return m_Cells; }

private:
	NAVIDESC	m_NaviDesc;
	_uint		m_iNumCells = 0;

	_vector		m_Sliding = {};

private:
	//������ �����������ҿ뵵�Դϴ�.
	_vector			vPoints[3] = {};
	_float3			fTemp[3] = {};
	_int			num = 0;
	_int			CellCount = 0;
	_float			angle = 0;

public:
	HRESULT SetUp_NeighborCell();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pDataFiles);
	static CNavigation* Create_ForTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

