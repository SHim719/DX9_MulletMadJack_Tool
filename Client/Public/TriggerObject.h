#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CBoxCollider;
END

BEGIN(Client)

class CTriggerObject abstract : public CGameObject
{
private:
	CTriggerObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTriggerObject(const CTriggerObject& rhs);
	virtual ~CTriggerObject() = default;

protected:
	CBoxCollider* m_pBoxCollider = { nullptr };
	//HRESULT Add_Components();

protected:
	virtual void TriggerEvent() = 0; // ��ӹ޾Ƽ� �̺�Ʈ ó�����ٰ�.

public:
	CGameObject* Clone(void* pArg) = 0;
	void Free() override;
};

END