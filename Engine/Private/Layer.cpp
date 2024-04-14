#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.emplace_back(pGameObject);

	return S_OK;
}

void CLayer::Destroy_Objects()
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); )
	{
		if (nullptr != *it)
		{
			if ((*it)->Is_Destroyed())
			{
				Safe_Release(*it);
				it = m_GameObjects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

}

void CLayer::PriorityTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
		{
			if (!pGameObject->Is_Destroyed())
				pGameObject->PriorityTick(fTimeDelta);
		}
			
	}
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
		{
			if (!pGameObject->Is_Destroyed())
				pGameObject->Tick(fTimeDelta);
		}
			
	}
}

void CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
		{
			if (!pGameObject->Is_Destroyed())
				pGameObject->LateTick(fTimeDelta);
		}
	}
}

CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
	
}
