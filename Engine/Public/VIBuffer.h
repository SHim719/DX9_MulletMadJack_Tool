#pragma once

#include "Component.h"

/* �������� ��, �ε������� �Ҵ��ϴ� �۾�. */
/* ����(Vertex)�� �ε���(Index)�� ���� �����Ѵ�. */
/* ����,�ε������� Ȱ���Ͽ� ȭ�鿡 �׷��ش�. */
/* Rect, Cube, Terrain�� �θ�μ��� ��Ȱ�� �����ϳ�. */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

protected:
	vector<_float3>					m_vecPositions;
	LPDIRECT3DVERTEXBUFFER9			m_pVB = { nullptr };

	/* ���� �ϳ��� ũ��. */
	_uint							m_iVertexStride = { 0 };
	_uint							m_iNumVertices = { 0 };
	_ulong							m_FVF = { 0 };
	D3DPRIMITIVETYPE				m_ePrimitiveType;
	_uint							m_iNumPrimitives = { 0 };
protected:
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_uint							m_iIndexSizeofPrimitive = 0;
	D3DFORMAT						m_eIndexFormat;

public:
	const vector<_float3>& Get_Positions() const { return m_vecPositions; }

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	// Ray�� World ������ǥ, World ������ Input, ���� ������ ��ŷ�� ��ġ�� ���� ��ǥ�� Ray�� ���������κ��� �Ÿ� ���� ����
	virtual bool Intersect_Ray(class CTransform* pTransform, const _float3& vRayWorldPos, const _float3& vRayDir, OUT _float3* pHitWorldPos, OUT _float* pDist);

public:
	virtual void Scaling_Texcoord(const _float3& vScale) {}


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END