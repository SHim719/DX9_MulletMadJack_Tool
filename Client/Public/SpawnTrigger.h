#pragma once


#include "Client_Defines.h"
#include "TriggerObject.h"

BEGIN(Engine)
class CBoxCollider;
class CGameObject;
END

BEGIN(Client)

class CSpawnTrigger final : public CTriggerObject
{
private:
	CSpawnTrigger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpawnTrigger(const CSpawnTrigger& rhs);
	virtual ~CSpawnTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	HRESULT Add_Components();

protected:
	void TriggerEvent() override;

private:
	static vector<CGameObject*> m_vecEnemies;

	_int	m_iMinIdx = { 0 };
	_int	m_iMaxIdx = { 0 };
public:
	void Add_EnemyObj(CGameObject* pObj) { m_vecEnemies.push_back(pObj); }

	void Set_MinIdx(_int iMin) { m_iMinIdx = iMin; }
	void Set_MaxIdx(_int iMax) { m_iMaxIdx = iMax; }
	_int Get_MinIdx() { return m_iMinIdx; }
	_int Get_MaxIdx() { return m_iMaxIdx; }

public:
	static CSpawnTrigger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg)	override;
 	void Free() override;
};

END

