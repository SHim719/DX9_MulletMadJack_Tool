#pragma once

#include "Base.h"

/* ȭ�鿡 �׷������� ��ü���� �׸��� ������� ��Ƽ� �����Ѵ�. */
/* �����ϰ� �ִ� ��ü���� Render�Լ��� ȣ���Ѵ�. */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	/* �׸��� ������� ����ü�� �����ߴ�. */
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND, RENDER_BLEND, RENDER_UI, RENDER_END };
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObjects(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);	
	HRESULT Draw();
	void Clear();


	void Sort_AlphaBlendObj();

private:
	LPDIRECT3DDEVICE9					m_pGraphic_Device = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RENDER_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();


public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END