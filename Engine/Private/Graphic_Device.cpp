#include "../Public/Graphic_Device.h"

CGraphic_Device::CGraphic_Device()
	: m_pSDK{ nullptr }
	, m_pDevice{ nullptr }
	, m_pSprite{ nullptr }
	, m_pFont{ nullptr }
{	

}


HRESULT CGraphic_Device::Initialize(const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	//return E_FAIL;
	
	// 1. ��ġ�� �����ϴ� ��ü ����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. ��ġ ����(���� ����)

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : ��ġ�� ���� ����(���� ����)�� ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī�带 �ǹ�

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSG_BOX(L"GetDeviceCaps Failed");
		return E_FAIL;
	}
	
	// 3. ��ġ�� ������ �� �ִ� ��ü ����
	// ���ؽ� ���μ��� = ���� ��ȯ + ���� ����

	DWORD		vp(0);

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(GraphicDesc, d3dpp);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GraphicDesc.hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSG_BOX(L"CreateDevice Failed");
		return E_FAIL;
	}

	// ��������Ʈ ��ü ����

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSG_BOX(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// ��Ʈ ��ü ����
	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");

	// D3DXCreateFont : ���� ��Ʈ �ڵ��� ���� ��Ʈ ��ü�� �����ϴ� �Լ�
	// D3DXCreateFontIndirect : ��Ʈ ����ü ������ ���� ��Ʈ ��ü�� �����ϴ� �Լ�

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSG_BOX(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);
	
	return S_OK;
}

// �ĸ� ������ ���� ����

void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0, // ��Ʈ�� ����
					nullptr, // ��Ʈ �迭�� ù ��° �ּ�
					D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	// �׸���(Ÿ��, �ĸ�), ���ٽ�, ���̹����� ���� ��� ��� 
					D3DCOLOR_ARGB(0, 0, 0, 0), // �ĸ� ���� �ؽ�ó�� ����
					1.f,	// ��� ���� �ʱ�ȭ ��
					0);		// ���ٽ� ���� �ʱ�ȭ ��

	m_pDevice->BeginScene();	
}

void CGraphic_Device::Render_End(HWND hWnd)
{	
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);

	// Present : �ĸ� ���۰� ���� ���۷� �ؽ�ó�� ��ȯ���� ȭ�鿡 ����� �� �ְ� �������
	// 1, 2 ���� : ��Ʈ �ּ�, ����ü�� D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ��� NULL
	// 4���� : ����ü�� D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ��� NULL
}


void CGraphic_Device::Set_Parameters(const GRAPHIC_DESC& GraphicDesc, _Out_ D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth = GraphicDesc.iWinSizeX;
	d3dpp.BackBufferHeight = GraphicDesc.iWinSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : ���� ü�� ���
	// D3DSWAPEFFECT_FLIP : ���� �ϳ��� �������鼭 ����ϴ� ���
	// D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// ��ġ�� ����� ������ �ڵ� ���� 
	d3dpp.hDeviceWindow = GraphicDesc.hWnd;

	// â ��� ���� or ��ü ȭ�� ��� ����
	d3dpp.Windowed = GraphicDesc.isWindowed;		// â ���� ����

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// �����
	// ��ü ȭ�� ����� ��� ����� ����� ������ ��� �� ���ΰ�
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// ������� �ÿ��� ���� ����
	// D3DPRESENT_INTERVAL_IMMEDIATE : ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT : ������ ������ DX�� �˾Ƽ� ����(���� ����� ������� ����)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

CGraphic_Device * CGraphic_Device::Create(const GRAPHIC_DESC & GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	CGraphic_Device*	pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Initialize(GraphicDesc, ppOut)))
	{
		MSG_BOX(TEXT("Failed to Created : CGraphic_Device"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CGraphic_Device::Free()
{
	if (nullptr != m_pFont)
		m_pFont->Release();

	if (nullptr != m_pSprite)
		m_pSprite->Release();

	unsigned int iRefCnt = { 0 };

	if (nullptr != m_pDevice)
	{
		iRefCnt = m_pDevice->Release();

		if (0 != iRefCnt)
			MSG_BOX(TEXT("Failed to Deleted : IDirect3DDevice9"));
	}

	if (nullptr != m_pSDK)
		m_pSDK->Release();
}
