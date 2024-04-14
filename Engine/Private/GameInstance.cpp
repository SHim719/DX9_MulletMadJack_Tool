#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Collision_Manager.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	/* ������ ����ϱ����� �ʿ��� ��Ÿ �ʱ�ȭ �۾����� ��ģ��. */

	/* �׷��� ����̽� �ʱ�ȭ. */
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* ��ǲ ����̽� �ʱ�ȭ. */
	m_pKey_Manager = CKey_Manager::Create();
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* ���� ����̽� �ʱ�ȭ. */

	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* ���� �Ŵ����� �غ��س��´�. */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;


	/* ������Ʈ �Ŵ��� ����� �غ�. . */
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* ������Ʈ �Ŵ��� ����� �غ�. . */
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pCollision_Manager = CCollision_Manager::Create();
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	/* ��Ÿ �����. */
	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager ||
		nullptr == m_pCollision_Manager)
		return;

	m_pKey_Manager->Update();

	m_pObject_Manager->Destroy_Objects();

	m_pObject_Manager->PriorityTick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);

	m_pCollision_Manager->Tick();

	m_pLevel_Manager->Tick(fTimeDelta);

	m_pRenderer->Sort_AlphaBlendObj();
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw();	

	return m_pLevel_Manager->Render();
}
void CGameInstance::Clear(_uint iLevelIndex)
{
	m_pRenderer->Clear();
	m_pObject_Manager->Clear(iLevelIndex);	
	m_pComponent_Manager->Clear(iLevelIndex);
}

void CGameInstance::Clear_Objects(_uint iLevelIndex)
{
	m_pRenderer->Clear();
	m_pObject_Manager->Clear(iLevelIndex);
}

#pragma region GRAPHIC_DEVICE
void CGameInstance::Render_Begin()
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Render_End(hWnd);
}
#pragma endregion

#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Change_Level(CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Change_Level(pNewLevel);
}
#pragma endregion

#pragma region TIMER_MANAGER
_float CGameInstance::Get_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}
HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}
void CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return ;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
	
}
CGameObject* CGameInstance::Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Add_Clone(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

CLayer* CGameInstance::Find_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
}
#pragma endregion

#pragma region COMPONENT_MANAGER
HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region COLLISION_MANAGER
void CGameInstance::Add_RayDesc(const RAY_DESC& RayDesc)
{
	if (nullptr == m_pCollision_Manager)
		return;

	m_pCollision_Manager->Add_RayDesc(RayDesc);
}

#pragma endregion

#pragma region RENDERER
HRESULT CGameInstance::Add_RenderObjects(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObjects(eRenderGroup, pRenderObject);	
}
#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Free();

	CGameInstance::Get_Instance()->Destroy_Instance();	
}

void CGameInstance::Free()
{	
	Safe_Release(m_pRenderer);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pGraphic_Device);
}
