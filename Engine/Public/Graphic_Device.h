#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{	
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT Initialize(const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	void Render_Begin();
	void Render_End(HWND hWnd = 0);

private:
	LPDIRECT3D9			m_pSDK;		// ��ġ�� �����ϴ� ��ü
	LPDIRECT3DDEVICE9	m_pDevice;	// �׷��� ��ġ�� ���� �������� �����ϴ� ��ü
	LPD3DXSPRITE		m_pSprite;	// dx �󿡼� 2d �̹��� ����� ����ϴ� ��ü
	LPD3DXFONT			m_pFont;	// dx �󿡼� ��Ʈ�� ����� ����ϴ� ��ü

private:
	void		Set_Parameters(const GRAPHIC_DESC& GraphicDesc, _Out_ D3DPRESENT_PARAMETERS& d3dpp);

public:
	static CGraphic_Device* Create(const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	virtual void Free() override;
};

END