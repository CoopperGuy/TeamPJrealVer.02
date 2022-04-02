#pragma once
#include "ImGuiWindow.h"
#include "Gizmo.h"
BEGIN(Tool)
class CInspector : public CImGuiWindow
{
public:
	explicit CInspector();
	virtual ~CInspector() = default;
	virtual void Free() override;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	void UpdateGameObject();
	void UpdateMapObject();
	void UpdateEffect();
	void UpdateUI();
	void UpdateCamera();
public:
	void DrawVec3(const string& label, _float3& values);
	void DrawCollider();
	void DrawTransform();
	void DrawInstansModelTransform();
	void DrawBuffer();
	void DrawModel();
	void DrawLight();
	void DrawNavi();
	void DrawInstancing();
	void DrawNPCInfo();
	void DrawBasicCollider();
	void DrawStat();
	void DrawShop();
	void DrawSpawn();
public:
	void DrawRectTransform();
	void DrawRectDesc(const string& label, _float& x, _float& y);
	void DrawImage();
	void DrawEffectImage();
	void DrawEffectSetting();
	void DrawTextUI();
public:
	void DrawCameraSetting();

private:
	//std::shared_ptr<CGizmo>		m_pGizmo;
	CGizmo*		m_pGizmo;
	_bool		m_bOBBSetSize = false;
	_bool		m_bOBBSetCenter = false;
};
END