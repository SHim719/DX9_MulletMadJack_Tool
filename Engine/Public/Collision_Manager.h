#pragma once

#include "Base.h"
#include "Collider.h"


BEGIN(Engine)
class CCollision_Manager : public CBase
{
private:
	union CollisionID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};

		UINT64 id;
	};

	enum CollisionType
	{
		Trigger,
		Collision,
		CT_END,
	};
private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

	HRESULT Initialize();

public:
	void Tick();

private:
	list<RAY_DESC>	m_RayDescs; //�÷��̾ ���̸� ��� GameInstance�� Add_RayDesc�� ȣ���ؼ� ���������� ���⿡ �־���
private:
	void Intersect_Ray();		// �������Ӹ��� ȣ��ż� ���� ���̿� ��ŷ������ Collider ������Ʈ�� On_Ray_Intersectȣ��
								// Collider�� On_Ray_Intersect�� GameObject�� On_Ray_Intersect�� ȣ��
								// On_Ray_Intersect�� �����Լ��̹Ƿ� ��ӹ޾Ƽ� ����ϸ� ��.

public:
	void Add_RayDesc(const RAY_DESC& RayDesc) { m_RayDescs.push_back(RayDesc); }
	_bool Ray_Cast(const RAY_DESC& RayDesc, OUT class CGameObject*& pOutHit, OUT _float3& fHitWorldPos, OUT _float& fDist);

private:
	unordered_map<UINT64, bool> m_CollisionInfo;

private:
	void Collision_Box(_uint iLevel, const wstring& strDstLayer, const wstring& strSrcLayer, CollisionType eCollisionType);

	bool Check_Intersect_AABB(class CBoxCollider* pDstCollider, CBoxCollider* pSrcCollider, OUT _float3& _fDist);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CCollision_Manager* Create();
	void Free() override;

};

END

