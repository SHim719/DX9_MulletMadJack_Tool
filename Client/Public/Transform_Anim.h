#pragma once

#include "Client_Defines.h"
#include "Component.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

typedef struct
{
	_float3 vPosition = _float3(0.f, 0.f, 0.f);
	_float3 vRotation = _float3(0.f, 0.f, 0.f);
	_float3 vScale = _float3(1.f, 1.f, 1.f);
	_bool	bAnimPos = false;
	_bool	bAnimRot = false;
	_bool	bAnimScale = false;
	_uint	iKeyFrame;
} KEYFRAME_DESC;

class CTransform_Anim : public CComponent
{
private:
	CTransform_Anim(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform_Anim(const CTransform_Anim& rhs);
	virtual ~CTransform_Anim() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;

	void Update(_float fTimeDelta);

private:
	CTransform* m_pTransform = { nullptr };

	_uint m_iNowFrame = { 0 };
	_uint m_iNowIdx = { 0 };
	_bool m_bLoop = { false };

	map<wstring, vector<KEYFRAME_DESC>> m_Animations;
	vector<KEYFRAME_DESC>* m_pNowPlayingAnim = { nullptr };
public:
	HRESULT Insert_Animations(const wstring& strAnimKey, const vector<KEYFRAME_DESC>& animations);
	HRESULT Play(const wstring& strAnimKey, _bool bLoop);

public:
	static CTransform_Anim* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CComponent* Clone(void* pArg) override;
	void Free() override;
};

END

