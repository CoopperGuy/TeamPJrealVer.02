#include "EnginePCH.h"
#include "..\Public\Stat.h"
#include "Transform.h"
#include "DmgVIBuffer.h"
#include "Engine.h"
USING(Engine)


CStat::CStat(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CStat::CStat(const CStat & rhs)
	: CComponent(rhs),
	m_tStat(rhs.m_tStat)
{
}

_bool CStat::UseStamina(_float stamina)
{
	if (m_fStamina >= stamina) {
		m_fStamina -= stamina;
		return true;
	}
	return false;
}

HRESULT CStat::InitializePrototype()
{
	return S_OK;
}

HRESULT CStat::Initialize(void * pArg)
{
	if (nullptr != pArg)
		m_pTransform = (CTransform*)pArg;
	return S_OK;
}

HRESULT CStat::Update(_double deltatime)
{
	SetLevelStat();
	HpRegeneration(deltatime);
	StaminaRegeneration(deltatime);
	CorrecetMaxStamina();
	return S_OK;
}

void CStat::HpHeal(_float healPoint)
{
	m_tStat.hp += healPoint;
	CorrectMaxHp();
}

_bool CStat::Damaged(CStat * enemyStat, _bool printDmg)
{
	STAT enemyStatus = enemyStat->GetStatInfo();
	_int ratio = 80 + rand() % 20;
	_int crit = rand() % 100;

	_float balance = ratio * 0.01f;
	_float criRatio = 1.f;
	_float dmgRation = enemyStat->GetDMGRatio();

	_bool isCrit = false;
	_bool isEFfect = false;

	if (crit < enemyStatus.critical * 100.f) {
		criRatio = 1.5f;
		isCrit = true;
	}
	if (dmgRation > 1.f)
		isEFfect = true;

	_float dmg = (enemyStatus.atk * (1 - (m_tStat.armor / (m_tStat.armor + 1000)))
		* dmgRation * balance) * criRatio;

	_matrix cameraMat = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float3 fCameraRight, fDmgPosition;
	_float4x4 matrix;
	XMStoreFloat4x4(&matrix, cameraMat);
	memcpy(&fCameraRight, matrix.m[0], sizeof(_float3));
	_vector right = XMVectorSetY(XMVector3Normalize(XMLoadFloat3(&fCameraRight)), 2.f);
	_int randPos = 2 + rand() % 2;
	_float magnification = (_float)randPos / 10.f;
	XMStoreFloat3(&fDmgPosition, m_pTransform->GetState(CTransform::STATE_POSITION) + right * magnification);
	CDmgVIBuffer::Create(nullptr, fDmgPosition, dmg, isCrit, isEFfect, printDmg);

	m_tStat.hp -= dmg;
	if (m_tStat.hp <= 0.f) {
		m_tStat.isDead = true;
		return true;
	}
	return false;
}

_bool CStat::DamagedAtk(_float enemyStatAtk, _bool printDmg)
{
	//don't use

	_float dmg = enemyStatAtk * (1 - (m_tStat.armor / (m_tStat.armor + 1000)));
	//
	if (printDmg) {
		_matrix cameraMat = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
		_float3 fCameraRight, fDmgPosition;
		_float4x4 matrix;
		XMStoreFloat4x4(&matrix, cameraMat);
		memcpy(&fCameraRight, matrix.m[0], sizeof(_float3));
		_vector right = XMVectorSetY(XMVector3Normalize(XMLoadFloat3(&fCameraRight)), 2.f);
		_int randPos = 2 + rand() % 2;
		_float magnification = (_float)randPos / 10.f;
		XMStoreFloat3(&fDmgPosition, m_pTransform->GetState(CTransform::STATE_POSITION) + right * magnification);
		CDmgVIBuffer::Create(nullptr, fDmgPosition, dmg);
	}
	//
	m_tStat.hp -= dmg;

	if (m_tStat.hp <= 0.f) {
		m_tStat.isDead = true;
		return true;
	}

	return false;
}

void CStat::EarnGold(CStat * enemyStat)
{
	STAT enemyStatus = enemyStat->GetStatInfo();
	m_tStat.gold += enemyStatus.gold;
}

void CStat::EarnGold(_int _gold)
{
	m_tStat.gold += _gold;
}

void CStat::EarnExp(CStat * enemyStat)
{
	STAT enemyStatus = enemyStat->GetStatInfo();
	m_tStat.exp += enemyStatus.exp;
	while (m_tStat.exp >= m_tStat.maxExp) {
		m_tStat.level++;
		m_tStat.exp -= m_tStat.maxExp;
	}
}



void CStat::SetLevelStat()
{
	m_tStat.maxHp = m_tStat.levelHp * m_tStat.level + m_tStat.boHp;
	m_tStat.atk = m_tStat.levelAtk * m_tStat.level + m_tStat.boAtk;
	m_tStat.armor = m_tStat.levelArmor * m_tStat.level + m_tStat.boArmor;
	m_tStat.hpRegeneration = m_tStat.levelRegen * m_tStat.level;
	m_tStat.maxExp = m_tStat.levelExp * m_tStat.level;
}

void CStat::CorrectMaxHp()
{
	if (m_tStat.hp >= m_tStat.maxHp) {
		m_tStat.hp = m_tStat.maxHp;
	}
}

void CStat::HpRegeneration(_double deltaTime)
{
	m_tStat.hp += m_tStat.hpRegeneration*(_float)deltaTime;
	CorrectMaxHp();
}

void CStat::StaminaRegeneration(_double deltaTime)
{
	m_fStamina += m_fStaRegen*deltaTime;
}

void CStat::CorrecetMaxStamina()
{
	if (m_fStamina > m_fStaMax)
		m_fStamina = m_fStaMax;
}

CStat * CStat::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CStat*	pInstance = new CStat(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Create CStat");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CStat::Clone(void * pArg)
{
	CComponent*	pInstance = new CStat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CStat");
		SafeRelease(pInstance);
	}


	return pInstance;
}

void CStat::Free()
{
}
