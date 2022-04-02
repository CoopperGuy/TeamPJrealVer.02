#include "stdafx.h"
#include "..\Public\Gizmo.h"
#include "ToolManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "EmptyGameObject.h"
#include "EmptyUI.h"
#include "Log.h"
#include "Navigation.h"
#include "EmptyMapObject.h"
#include "EmptyCamera.h"

CGameObject* g_pObjFocused = nullptr;
static ImGuizmo::OPERATION m_CurrentGizmoOperation(ImGuizmo::TRANSLATE);
static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
static bool useSnap = false;
static float snap[3] = { 1.f, 1.f, 1.f };
static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
static bool boundSizing = false;
static bool boundSizingSnap = false;

static const float identityMatrix[16] =
{ 1.f, 0.f, 0.f, 0.f,
0.f, 1.f, 0.f, 0.f,
0.f, 0.f, 1.f, 0.f,
0.f, 0.f, 0.f, 1.f };

float objectMatrix[4][16] = {
	{ 1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f },

	{ 1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	2.f, 0.f, 0.f, 1.f },

	{ 1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	2.f, 0.f, 2.f, 1.f },

	{ 1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 2.f, 1.f }
};
USING(Tool)
CGizmo::CGizmo()
{
	Initialize();
}

void CGizmo::Initialize()
{
	useWindow = true;
	gizmoCount = 1;
	camDistance = 8.f;
	ZeroMemory(&m_tGizmoMatrix, sizeof(GIZMOMATRIX));
	ZeroMemory(&m_tNewGizmoMatrix, sizeof(GIZMOMATRIX));
	m_tGizmoMatrix.matScale[0] = 1;
	m_tGizmoMatrix.matScale[1] = 1;
	m_tGizmoMatrix.matScale[2] = 1;
	m_tNewGizmoMatrix.matScale[0] = 1;
	m_tNewGizmoMatrix.matScale[1] = 1;
	m_tNewGizmoMatrix.matScale[2] = 1;
	XMFLOAT4X4 fMatIdentity;
	XMStoreFloat4x4(&fMatIdentity, XMMatrixIdentity());
	memcpy(_objMat, &fMatIdentity, sizeof(XMFLOAT4X4));

	/* Set up UI View*/
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(Eye, At, Up);
	memcpy(_viewUI, &view, sizeof(XMMATRIX));

}

void CGizmo::Update()
{

	CToolManager::SetImGuizmoStyle();
	CToolManager::SetImGuiColor();

	if (nullptr == g_pObjFocused)
		return;


	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);

	if (CEngine::GetInstance()->IsKeyDown('E'))
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (CEngine::GetInstance()->IsKeyDown('R'))
		m_CurrentGizmoOperation = ImGuizmo::ROTATE;
	if (CEngine::GetInstance()->IsKeyDown('T'))
		m_CurrentGizmoOperation = ImGuizmo::SCALE;

	XMMATRIX viewMatrix = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW);
	XMFLOAT4X4 fView;
	XMStoreFloat4x4(&fView, viewMatrix);
	memcpy(_view, &fView, sizeof(XMFLOAT4X4));

	XMMATRIX projMatrix = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);
	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, projMatrix);
	memcpy(_projection, &projection, sizeof(XMFLOAT4X4));


}

void CGizmo::LateUpdate()
{
	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.0f, 0.0f, 0.0f, 0.f));

	bool pOpen = false;
	ImGui::Begin("Gizmo", &pOpen, window_flags);
	//ImGuizmo::SetDrawlist();

	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();

	imageRect = ImGui::GetContentRegionAvail();
	winPos = ImGui::GetWindowPos();

	if (!ImGui::IsWindowCollapsed())
	{
		// TODO: Need To Change Camera proj
		// imgui화면에 랜더링하기
		ImVec2 textureRect = { imageRect.x, imageRect.y/* - ImGui::GetFontSize() * 2*/ };
		//imageRect.y -= ImGui::GetFontSize() * 2;
		CEngine::GetInstance()->ChangeProj(textureRect.x, textureRect.y);
		ImGui::Image((ImTextureID)(CEngine::GetInstance()->GetShaderResourceView("Target_EditorWindow")),
			imageRect, ImVec2(0, 0), ImVec2(1, 1));
		CEngine::GetInstance()->ChangeProj(imageRect.x, imageRect.y);
	}

	if (g_pObjFocused)
	{
		if (dynamic_cast<CEmptyGameObject*>(g_pObjFocused))
			ManipulateGameObject();
		else if (dynamic_cast<CEmptyMapObject*>(g_pObjFocused))
			ManipulateGameObject();
		else if (dynamic_cast<CEmptyEffect*>(g_pObjFocused))
			ManipulateGameObject();
		else if (dynamic_cast<CEmptyCamera*>(g_pObjFocused))
			ManipulateGameObject();
		else
			ManipulateUI();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const char* s = (const char*)(payload->Data);
			string fullPath = s;
			FILESYSTEM::path filePath = FILESYSTEM::directory_entry(fullPath).path();
			if (filePath.extension().string() == ".prefab")
				CEngine::GetInstance()->SpawnPrefab(filePath.stem().string());

		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();
	ImGui::PopStyleColor(1);

	CToolManager::SetImGuiStyle();
	CToolManager::SetImGuiColor();
}

void CGizmo::ManipulateGameObject()
{
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(winPos.x, winPos.y + ImGui::GetFontSize(), imageRect.x, imageRect.y);
	
	ImGuizmo::Manipulate(_view, _projection, m_CurrentGizmoOperation, mCurrentGizmoMode, _objMat, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
	XMFLOAT4X4 objMat;
	memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

	// TODO : FIX HERE
	CComponent* pObjTransform = nullptr;
	if (g_pObjFocused->GetParent() != nullptr) {
		if (!(pObjTransform = g_pObjFocused->GetComponent("Com_LocalTransform")))
			MSG_BOX("Failed to Get Transform");
	}
	else {
		if (!(pObjTransform = g_pObjFocused->GetComponent("Com_Transform")))
			MSG_BOX("Failed to Get Transform");
	}

	dynamic_cast<CTransform*>(pObjTransform)->SetMatrix(objMat);
}

void CGizmo::ManipulateUI()
{
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(winPos.x, winPos.y + ImGui::GetFontSize(), imageRect.x, imageRect.y);

	/* TODO : Get WinSize */
	_float2 winSize = CEngine::GetInstance()->GetCurrentWindowSize();
	XMMATRIX projMatrix = XMMatrixOrthographicLH(winSize.x, winSize.y, 0.0f, 1.f);
	//XMMATRIX projMatrix = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);
	XMFLOAT4X4 projection;
	XMStoreFloat4x4(&projection, projMatrix);
	memcpy(_projection, &projection, sizeof(XMFLOAT4X4));

	ImGuizmo::Manipulate(_viewUI, _projection, m_CurrentGizmoOperation, mCurrentGizmoMode, _objMat, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	XMFLOAT4X4 objMat;
	memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

	CComponent* pObjTransform = nullptr;
	if (!(pObjTransform = g_pObjFocused->GetComponent("Com_Transform")))
		MSG_BOX("Failed to Get Transform");

	dynamic_cast<CRectTransform*>(pObjTransform)->SetTransformMat(objMat);
	g_pObjFocused->LinkTranformWithParent();
}

void CGizmo::SetObjMat(float* mat)
{
	memcpy(_objMat, mat, sizeof(float) * 16);
}

void CGizmo::SetNewGizmoMatrix(const GIZMOMATRIX & tMat)
{
	m_tNewGizmoMatrix = tMat;
}

_bool CGizmo::RayCast(_float3 & out, HWND hWnd, const _matrix & matWorld)
{
	ImGuizmo::SetRect(winPos.x, winPos.y + ImGui::GetFontSize(), imageRect.x, imageRect.y);
	_float2 winSize = CEngine::GetInstance()->GetCurrentWindowSize();
	//ImGuizmo::Manipulate(_view, _projection, m_CurrentGizmoOperation, mCurrentGizmoMode, _objMat, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
	
	ImGuizmo::Manipulate(_view, _projection, m_CurrentGizmoOperation, mCurrentGizmoMode, _objMat, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
	XMFLOAT4X4 objMat;
	memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

	_float3 mousePos = CEngine::GetInstance()->GetMousePosition();
	 
	POINT pt;
	//pt.x = mousePos.x;
	//pt.y = mousePos.y;

	//기즈모 창 크기 
	_float GizmoWinx = imageRect.x - winPos.x;
	_float GizmoWiny = imageRect.y - winPos.y;

	pt.x = mousePos.x;
	pt.y = mousePos.y;

	// 뷰포트 -> 투영스페이스
	_float3 vMouse = _float3(0.f, 0.f, 0.f);
	vMouse.x = pt.x / (1280 * 0.5f) - 1.f;
	vMouse.y = 1.f - pt.y / (720 * 0.5f);

	// 투영스페이스 -> 뷰스페이스
	_matrix matProj;
	matProj = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ));
	XMStoreFloat3(&vMouse, (XMVector3TransformCoord(XMLoadFloat3(&vMouse), matProj)));
	//XMLoadFloat3(&vMouse) = XMVector3TransformCoord(XMLoadFloat3(&vMouse), matProj);

	// 뷰스페이스에서 광선(ray)의 정보를 설정한다.
	_vector vRayPos = XMVectorSet(0, 0, 0, 1.f);
	_vector vRayDir = XMLoadFloat3(&vMouse) - vRayPos;

	// 뷰스페이스 -> 월드스페이스
	_matrix matView;
	matView = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	// 월드스페이스 -> 로컬스페이스
	_matrix matInverse;
	matInverse = XMMatrixInverse(nullptr, matWorld);
	vRayPos = XMVector3TransformCoord(vRayPos, matInverse);
	vRayDir = XMVector3TransformNormal(vRayDir, matInverse);

	//평면방정식
	_vector Plane = XMPlaneFromPoints(_vector{ 0.f, 0.f ,0.f }, _vector{ 0.f, 0.f ,1.f }, _vector{ 1.f, 0 ,0.f });
	//위치 (방향으로 얼만큼 쏠건지 정하고 위치를 옮겨주자)
	_vector point2 = (vRayPos + vRayDir * 1000.f);

	_vector RayResult = XMPlaneIntersectLine(Plane, vRayPos, point2);
	if (!XMVector4IsNaN(RayResult)) //평면 // 시작위치// 끝위치
	{
		RayResult = XMVector3TransformCoord(RayResult, matWorld);
		XMStoreFloat3(&out, RayResult);
		return true;
	}
	return false;
}

void CGizmo::Free()
{
}

