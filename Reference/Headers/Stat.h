#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStat :
	public CComponent
{
public:
	typedef struct tagStat
	{
		_float hp;
		_float maxHp;
		_float boHp;
		_float hpRegeneration;
		_float atk;
		_float boAtk;
		_float armor;
		_float boArmor;
		_float exp;
		_float maxExp;
		_float levelHp;
		_float levelArmor;
		_float levelExp;
		_float levelAtk;
		_float levelRegen;
		_float critical = 0.1f;
		_int   level;
		_int   gold = 0;
		_bool  isDead = false;
		_bool  isImmortal = false;
		char   padding[6];
	}STAT;
	enum STATES{STATES_IDEL, STATES_ATK, STATES_HITTED, STATES_END};
	enum STATUSEFFECT{
		STATUSEFFCT_NONE = 0,
		BLOOD = (1 << 0)
		};
private:
	explicit CStat(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CStat(const CStat& rhs);
	virtual ~CStat() = default;
public:
	void SetStatInfo(STAT _info) { m_tStat = _info; }
	void SetStaFullHP() { m_tStat.hp = m_tStat.maxExp; }
	void SetSTATE(STATES _state) { m_eState = _state; }
	void SetStamina(_float _stamina) { m_fStamina += _stamina; }
	void SetDMGRatio(_float _ratio) { m_fDMGRatio = _ratio; }
	void SetCritical(_float critical) { m_tStat.critical = critical; }
	void SetStatusEffect(STATUSEFFECT _effect) { m_StatusEffect = STATUSEFFECT(m_StatusEffect | _effect); }
	void ShutStatusEffect(STATUSEFFECT _effect) { m_StatusEffect &= ~_effect; }
	void SetSlow(_bool _isSlow) { m_bisSlow = _isSlow; }
public:
	_bool GetIsImmortal() { return m_tStat.isImmortal; }
	_bool GetIsSlow() { return m_bisSlow; }
	STAT GetStatInfo() { return m_tStat; }
	STATES GetSTATES() { return m_eState; }
	_float GetHpPercentage() { return m_tStat.hp / m_tStat.maxHp; }
	_float GetStaPercentage() { return m_fStamina / m_fStaMax; }
	_float GetExpPercentage() { return m_tStat.exp / m_tStat.maxExp; }
	_float GetDMGRatio() { return m_fDMGRatio; }
	_float GetCritical() { return m_tStat.critical; }
	_int GetStatusEffect() { return m_StatusEffect; }
public:
	_bool UseStamina(_float stamina);
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	HRESULT Update(_double deltatime);
public:
	void HpHeal(_float healPoint);
	_bool Damaged(CStat* enemyStat,_bool printDmg = false);
	_bool DamagedAtk(_float enemyStatAtk, _bool printDmg = false);
	void EarnExp(CStat* enemyStat);
	void EarnGold(CStat* enemyStat);
	void EarnGold(_int _gold);
	void Immortal(_bool _immortal) {
		m_tStat.isImmortal = _immortal;
	}
private:
	void SetLevelStat();
	void CorrectMaxHp();
	void HpRegeneration(_double deltaTime);
	void StaminaRegeneration(_double deltaTime);
	void CorrecetMaxStamina();
private:
	class CTransform*			m_pTransform = nullptr;
private:
	STAT	m_tStat{};
	STATES	m_eState = STATES::STATES_END;
	_int	m_StatusEffect = STATUSEFFECT::STATUSEFFCT_NONE;
private:
	_float	m_fStamina = 0.f;
	_float	m_fStaMax = 100.f;
	_float	m_fStaRegen = 1.f;
private:
	_float	m_fmagnification = 1.f;
	_float	m_fDMGRatio = 1.f;
private:
	_bool	m_bisSlow = false;
	_double	m_slowDelta = 0;
	_double m_slowTime = 0.2;
private:
	string	m_strItemName;
public:
	static CStat*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent * Clone(void * pArg) override;
	virtual void Free() override;
};

END