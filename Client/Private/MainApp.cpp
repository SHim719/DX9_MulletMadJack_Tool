#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "ToolCamera.h"
#include "Level_Tool.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	GRAPHIC_DESC		GraphicDesc{};

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;
	GraphicDesc.isWindowed = true;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObjects()))
		return E_FAIL;

	if (FAILED(CImGui_Manager::Get_Instance()->Initialize(m_pGraphic_Device)))
		return E_FAIL;

	CToolCamera::Get_Instance()->Initialize(m_pGraphic_Device);
	
	if (FAILED(Open_Level(LEVEL_TOOL)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	CToolCamera::Get_Instance()->Tick(fTimeDelta);
	m_pGameInstance->Tick_Engine(fTimeDelta);

	CToolCamera::Get_Instance()->Update_ViewMatrix();
	CToolCamera::Get_Instance()->Set_Transform_View();
	CToolCamera::Get_Instance()->Set_Transform_Proj();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();

	m_pGameInstance->Draw();

	CImGui_Manager::Get_Instance()->Render();

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eStartLevelID)
{
	/*if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, eStartLevelID))))
		return E_FAIL;*/

	if (FAILED(m_pGameInstance->Change_Level(CLevel_Tool::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObjects()
{
	return S_OK; 
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Transform_Default"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Rect_Deep_Default"),
		CVIBuffer_RectDeep::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_RectX_Default"),
		CVIBuffer_RectX::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_RectXY_Default"),
		CVIBuffer_RectXY::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Box_Default"),
		CVIBuffer_Box::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region Model
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_SodaMachine_Default"),
		CVIBuffer_Mesh::Create(m_pGraphic_Device, L"../Bin/Resources/Models/SodaMachine.obj"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Floor_Border_Default"),
		CVIBuffer_Floor_Border::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("VIBuffer_Door_Default"),
		CVIBuffer_Door::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Box_Collider_Default"),
		CBoxCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Rigidbody_Default"),
		CRigidbody::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Insert_Default_GameObjects()
{
	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{	
	__super::Free();

	Safe_Release(m_pGraphic_Device);

	Safe_Release(m_pGameInstance);	

	CImGui_Manager::Get_Instance()->Free();
	CImGui_Manager::Destroy_Instance();

	CToolCamera::Get_Instance()->Free();
	CToolCamera::Destroy_Instance();

	CGameInstance::Release_Engine();

}

