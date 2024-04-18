#pragma once

#include "Base.h"

/*클라이언트개발자가 실질적으로 생성해낸 객체타입(Player, Monster)의 부모가 되는 클래스이다. */


BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	map<const wstring, class CComponent*>	m_Components;

public:
	class CComponent* Add_Component(_uint iPrototoypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, void* pArg = nullptr);
	class CComponent* Find_Component(const wstring& strComponentTag);

protected:
	class CTransform* m_pTransform = { nullptr };
public:
	class CTransform* Get_Transform();

protected:
	_uint m_iTexture_Index = 0;

	string m_strTag = "";

public:
	void Set_Texture_Index(_uint iIndex) { m_iTexture_Index = iIndex; }
	_uint Get_Texture_Index() { return m_iTexture_Index; }

	const string& Get_Tag() { return m_strTag; }
public:
	virtual void On_Ray_Intersect(const _float3& fHitWorldPos, const _float& fDist) {}

public:
	virtual void OnCollisionEnter(CGameObject* pOther) {}
	virtual void OnCollisionStay(CGameObject* pOther) {}
	virtual void OnCollisionExit(CGameObject* pOther) {}

	virtual void OnTriggerEnter(CGameObject* pOther) {}
	virtual void OnTriggerStay(CGameObject* pOther) {}
	virtual void OnTriggerExit(CGameObject* pOther) {}

protected:
	_bool m_bDestroyed = { false };

public:
	void Set_Destroy(_bool b) { m_bDestroyed = b; }
	_bool Is_Destroyed() { return m_bDestroyed; }
	
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END