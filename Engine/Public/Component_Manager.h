#pragma once

#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_RectX.h"
#include "VIBuffer_RectXY.h"
#include "VIBuffer_Box.h"
#include "VIBuffer_Mesh.h"
#include "VIBuffer_RectDeep.h"
#include "Texture.h"
#include "BoxCollider.h"


/* ��üȭ�� ������Ʈ(����)�� �������� �����Ѵ�. <-> �纻��ü�� �������� �ʴ´�. �纻��ü�� �� ��ü(�÷��̾�, ����, ���� ���)���� �����Ѵ�.  */
/* ���� ������ ������ü�� ã�Ƽ� ����(�纻�� �����Ѵ�)�Ͽ� �������ش�. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	_uint											m_iNumLevels = { 0 };
	map<const wstring, class CComponent*>*			m_pPrototypes = { nullptr };
	typedef map<const wstring, class CComponent*>	PROTOTYPES;

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	static CComponent_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END

/* �� - ���� ��ġ, ����, �ƾ���*/