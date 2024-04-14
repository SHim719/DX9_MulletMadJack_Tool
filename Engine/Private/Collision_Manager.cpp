#include "Collision_Manager.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "BoxCollider.h"


CCollision_Manager::CCollision_Manager()
	: m_pGameInstance { CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCollision_Manager::Initialize()
{
	return S_OK;
}

void CCollision_Manager::Tick()
{
	//Collision_Box(4, L"Player", L"Wall");
	//Collision_Box(4, L"Player", L"Floor");
	//Collision_Box(4, L"Player", L"Door");

	Collision_Box(4, L"Soda", L"Wall");
	Collision_Box(4, L"Soda", L"Floor");
	Collision_Box(4, L"Soda", L"Soda");
	Intersect_Ray();
}

void CCollision_Manager::Intersect_Ray()
{
	for (auto& RayDesc : m_RayDescs)
	{
		CLayer* pLayer = m_pGameInstance->Find_Layer(RayDesc.iLevel, RayDesc.strDstLayer);
		if (pLayer == nullptr)
			continue;

		auto& gameObjects = pLayer->Get_GameObjects();

		Safe_AddRef(pLayer);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>((*it)->Find_Component(L"VIBuffer"));
			if (nullptr == pVIBuffer)
				continue;

			_float3 fHitWorldPos; 	
			_float fDist;
			if (pVIBuffer->Intersect_Ray((*it)->Get_Transform(), RayDesc.vRayWorldPos, RayDesc.vRayDir, &fHitWorldPos, &fDist))
				(*it)->On_Ray_Intersect(fHitWorldPos, fDist);
			
		}

		Safe_Release(pLayer);
	}

	m_RayDescs.clear();
}

void CCollision_Manager::Collision_Box(_uint iLevel, const wstring& strDstLayer, const wstring& strSrcLayer)
{
	CLayer* pDstLayer = m_pGameInstance->Find_Layer(iLevel, strDstLayer);
	CLayer* pSrcLayer = m_pGameInstance->Find_Layer(iLevel, strSrcLayer);

	if (nullptr == pDstLayer || nullptr == pSrcLayer)
		return;

	auto& DstObjects = pDstLayer->Get_GameObjects();
	auto& SrcObjects = pSrcLayer->Get_GameObjects();

	for (auto DstIt = DstObjects.begin(); DstIt != DstObjects.end(); ++DstIt)
	{
		if ((*DstIt)->Is_Destroyed())
			continue;
		CTransform* pDstTransform = (*DstIt)->Get_Transform();
		CBoxCollider* pDstCollider = dynamic_cast<CBoxCollider*>((*DstIt)->Find_Component(L"Collider"));
		if (nullptr == pDstCollider || false == pDstCollider->IsActive())
			continue;

		for (auto SrcIt = SrcObjects.begin(); SrcIt != SrcObjects.end(); ++SrcIt)
		{
			if (*SrcIt == *DstIt || (*SrcIt)->Is_Destroyed())
				continue;

			CTransform* pSrcTransform = (*SrcIt)->Get_Transform();
			CBoxCollider* pSrcCollider = dynamic_cast<CBoxCollider*>((*SrcIt)->Find_Component(L"Collider"));
			if (nullptr == pSrcCollider || false == pSrcCollider->IsActive())
				continue;

			CollisionID id;
			id.left = pDstCollider->Get_CollisionID();
			id.right = pSrcCollider->Get_CollisionID();

			auto it = m_CollisionInfo.find(id.id);
			if (m_CollisionInfo.end() == it)
				m_CollisionInfo.insert({ id.id, false });

			it = m_CollisionInfo.find(id.id);

			_float3 fDist;
			if (Check_Intersect_AABB(pDstCollider, pSrcCollider, fDist))
			{
				if (false == it->second)
				{
					if (pDstCollider->IsTrigger())
					{
						(*DstIt)->OnTriggerEnter(*SrcIt);
					}
						
					else
					{
						pDstTransform->Add_Pos(fDist);
						(*DstIt)->OnCollisionEnter(*SrcIt);
					}
						
					
					if (pSrcCollider->IsTrigger())
					{
						(*SrcIt)->OnTriggerEnter(*DstIt);
					}
					else
					{
						(*SrcIt)->OnCollisionEnter(*DstIt);
					}
						

					it->second = true;
				}
				else
				{
					if (pDstCollider->IsTrigger())
					{
						(*DstIt)->OnTriggerStay(*SrcIt);
					}
					else
					{
						(*DstIt)->OnCollisionStay(*SrcIt);
						pDstTransform->Add_Pos(fDist);
					}
						

					if (pSrcCollider->IsTrigger())
					{
						(*SrcIt)->OnTriggerStay(*DstIt);
					}	
					else
					{
						(*SrcIt)->OnCollisionStay(*DstIt);
					}		
				}	
			}

			else if (it->second)
			{
				if (pDstCollider->IsTrigger())
				{
					(*DstIt)->OnTriggerExit(*SrcIt);
				}
				else
				{
					(*DstIt)->OnCollisionExit(*SrcIt);
				}


				if (pSrcCollider->IsTrigger())
				{
					(*SrcIt)->OnTriggerExit(*DstIt);
				}
				else
				{
					(*SrcIt)->OnCollisionExit(*DstIt);
				}
				it->second = false;
			}
		}
	}
}

bool CCollision_Manager::Check_Intersect_AABB(CBoxCollider* pDstCollider, CBoxCollider* pSrcCollider, OUT _float3& _fDist)
{
	_float3 vDstMin = pDstCollider->Get_MinCoord();
	_float3 vDstMax = pDstCollider->Get_MaxCoord();
	
	_float3 vSrcMin = pSrcCollider->Get_MinCoord();
	_float3 vSrcMax = pSrcCollider->Get_MaxCoord();

	if (min(vDstMax.x, vSrcMax.x) < max(vDstMin.x, vSrcMin.x))
		return false;
	if (min(vDstMax.y, vSrcMax.y) < max(vDstMin.y, vSrcMin.y))
		return false; 
	if (min(vDstMax.z, vSrcMax.z) < max(vDstMin.z, vSrcMin.z))
		return false;

	_float3 vDstCenter = (vDstMin + vDstMax) * 0.5f;
	_float3 vSrcCenter = (vSrcMin + vSrcMax) * 0.5f;
	

	if (vDstCenter.x < vSrcCenter.x)
		_fDist.x = -(min(vSrcMax.x, vDstMax.x) - max(vSrcMin.x, vDstMin.x));
	else
		_fDist.x = (min(vSrcMax.x, vDstMax.x) - max(vSrcMin.x, vDstMin.x));

	if (vDstCenter.y < vSrcCenter.y)
		_fDist.y = -(min(vSrcMax.y, vDstMax.y) - max(vSrcMin.y, vDstMin.y));
	else
		_fDist.y = (min(vSrcMax.y, vDstMax.y) - max(vSrcMin.y, vDstMin.y));

	if (vDstCenter.z < vSrcCenter.z)
		_fDist.z = -(min(vSrcMax.z, vDstMax.z) - max(vSrcMin.z, vDstMin.z));
	else
		_fDist.z = (min(vSrcMax.z, vDstMax.z) - max(vSrcMin.z, vDstMin.z));

	if (fabsf(_fDist.x) <= fabsf(_fDist.y))
	{
		_fDist.y = 0.f;
		if (fabsf(_fDist.x) <= fabsf(_fDist.z))
			_fDist.z = 0.f;
		
		else
			_fDist.x = 0.f;
	}
	else
	{
		_fDist.x = 0.f;
		if (fabsf(_fDist.y) <= fabsf(_fDist.z))
			_fDist.z = 0.f;

		else
			_fDist.y = 0.f;
	}
		

	return true;
}

CCollision_Manager* CCollision_Manager::Create()
{
	CCollision_Manager* pInstance = new CCollision_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollision_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
