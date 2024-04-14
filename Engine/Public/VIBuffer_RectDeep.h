#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectDeep : public CVIBuffer
{
protected:
	CVIBuffer_RectDeep(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RectDeep(const CVIBuffer_RectDeep& rhs);
	virtual ~CVIBuffer_RectDeep() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	void Reset_Texcoord(vector<_float2>& vecTexcoord);

public:
	static CVIBuffer_RectDeep* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END