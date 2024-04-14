#include "Level_Tool.h"
#include "ImGui_Manager.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Wall.h"
#include "Floor.h"
#include "Door.h"
#include "MapObject.h"
#include "SodaMachine.h"
#include "SodaMachine_Banner.h"
#include "Soda.h"
#include "Border.h"
#include "Player.h"

CLevel_Tool::CLevel_Tool(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Tool::Initialize()
{
    m_iLevelID = LEVEL_TOOL;

    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Wall_Textures",
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
            L"../Bin/Resources/Textures/Wall/Albedo/Wall%d.png", 17))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Floor_Textures",
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
            L"../Bin/Resources/Textures/Floor/Albedo/Floor%d.png", 6))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Object_Textures",
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
            L"../Bin/Resources/Textures/Objects/Object%d.png", 16))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Door_Textures",
        CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
            L"../Bin/Resources/Textures/Door/Door%d.png", 5))))
        return E_FAIL;

   if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Soda_Textures",
       CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
           L"../Bin/Resources/Textures/SodaMachine/SodaMachine%d.png", 2))))
       return E_FAIL;

   if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Border_Textures",
       CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
           L"../Bin/Resources/Textures/Border/Border%d.png", 1))))
       return E_FAIL;

   if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"SodaCan_Texture",
       CTexture::Create(m_pGraphic_Device, CTexture::TYPE_TEXTURE2D,
           L"../Bin/Resources/Textures/Soda/Soda0.png"))))
       return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Wall"),
        CWall::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Floor"),
        CFloor::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_MapObject"),
        CMapObject::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Door"),
        CDoor::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_SodaMachine"),
        CSodaMachine::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_SodaMachine_Banner"),
        CSodaMachine_Banner::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Border"),
        CBorder::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Soda"),
        CSoda::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Player"),
        CPlayer::Create(m_pGraphic_Device))))
        return E_FAIL;

    if (nullptr == m_pGameInstance->Add_Clone(LEVEL_TOOL, L"Player", TEXT("Prototype_Player")))
        return E_FAIL;
       
   
    return S_OK;
}

void CLevel_Tool::Tick(_float fTimeDelta)
{

}

HRESULT CLevel_Tool::Render()
{
    SetWindowText(g_hWnd, TEXT("툴레벨입니다."));

    return S_OK;
}


CLevel_Tool* CLevel_Tool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CLevel_Tool* pInstance = new CLevel_Tool(pGraphic_Device);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed to Created : CLevel_Tool"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Tool::Free()
{
    __super::Free();
}
