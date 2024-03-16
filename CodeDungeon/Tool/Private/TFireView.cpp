#include "ToolDefines.h"
#include "TFireView.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UAnimatedParticle.h"
#include "UGameInstance.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UMethod.h"
#include "UTransform.h"
#include "UFire.h"


TFireView::TFireView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "FireView"),
	m_stMainDesc{},
	m_stFireView{},
	m_isInitSetting{ false }
	
{
}

void TFireView::Free()
{
}

HRESULT TFireView::NativeConstruct()
{
	m_stMainDesc = MAINDESC(ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_None,
		ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });
	m_stFireView = DOCKDESC("Fire Viewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);

	return S_OK;
}





HRESULT TFireView::LoadResource()
{
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		UFire::FIREDESC tFireDesc;
		tFireDesc.wstrFireShader = PROTO_RES_2DFIRESHADER;
		m_stFire = std::static_pointer_cast<UFire>(spGameInstance->CloneActorAdd(PROTO_ACTOR_FIRE, { &tFireDesc }));
	}

	

	return S_OK;
}

//================================================================
//**************** 파티클 종류별로 리소스 해제 ****************
//================================================================




HRESULT TFireView::ReleaseResource()
{



	return S_OK;
}


//================================================================
//************ 파티클 종류별로 Rendering 하는 부분 ************
//================================================================

void TFireView::TickActive(const _double& _dTimeDelta)
{
}

void TFireView::LateTickActive(const _double& _dTimeDetla)
{
}

void TFireView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 

	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		//여기부터 내 입력

		FireView();
	}
	ImGui::End();
}

void TFireView::RenderMenu()
{
}

void TFireView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
		m_stFireView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, NULL, &dock_main_id);

		// DockBuild		
		ImGui::DockBuilderDockWindow(m_stFireView.strName.c_str(), m_stFireView.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}



//================================================================
//************ MULTIPLE PARTICLE TOOL VIEW SETTING ************
//================================================================

void TFireView::FireView()
{
	ImGui::Begin(m_stFireView.strName.c_str(), GetOpenPointer(), m_stFireView.imgWindowFlags);
	{
		if (ImGui::CollapsingHeader("Fire Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("Fire View");
			

			if (true == ImGui::Button("Start Multiple Particle")) {
			

				m_stFire->SetActive(true);

			}

			if (true == ImGui::Button("Stop Multiple Particle"))
			{

				m_stFire->SetActive(false);
			}

			FireColorTextureSetting();
			FireNoiseTextureSetting();
			FireAlphaTextureSetting();
			FireScalingSetting();
			FirePosSetting();
		}
	}
	ImGui::End();
}

void TFireView::FireColorTextureSetting() {
	ImGui::Text("Fire Color Texture Select");
	if (ImGui::BeginListBox("Fire Color Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{

		using TEXNAMES = UNORMAP<_wstring, _uint>;
		TEXNAMES m_TextureNames = m_stFire->GetColorTextureGroup()->GetTextureNames();
		for (auto& Texture : m_TextureNames)
		{
			if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
			{
				m_stFire->SetColorTexture(Texture.second);
			}
		}
		ImGui::EndListBox();
	}
}

void TFireView::FireNoiseTextureSetting() {
	ImGui::Text("Fire Noise Texture Select");
	if (ImGui::BeginListBox("Fire Noise Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{

		using TEXNAMES = UNORMAP<_wstring, _uint>;
		TEXNAMES m_TextureNames = m_stFire->GetNoiseTextureGroup()->GetTextureNames();
		for (auto& Texture : m_TextureNames)
		{
			if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
			{
				m_stFire->SetNoiseTexture(Texture.second);
			}
		}
		ImGui::EndListBox();
	}
}

void TFireView::FireAlphaTextureSetting() {
	ImGui::Text("Fire Alpha Texture Select");
	if (ImGui::BeginListBox("Fire Alpha Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{

		using TEXNAMES = UNORMAP<_wstring, _uint>;
		TEXNAMES m_TextureNames = m_stFire->GetAlphaTextureGroup()->GetTextureNames();
		for (auto& Texture : m_TextureNames)
		{
			if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
			{
				m_stFire->SetAlphaTexture(Texture.second);
			}
		}
		ImGui::EndListBox();
	}
}

void TFireView::FireScalingSetting() {

	static float Scale = 50.f;
	ImGui::SliderFloat("Scale", &Scale, 1.f, 100.f, "%.2f");

	_float3 ScaleFloat3 = _float3(Scale, Scale, Scale);
	m_stFire->GetTransform()->SetScale(ScaleFloat3);
}

void TFireView::FirePosSetting() {

	static _float posX = 0;
	static _float posY = 0;
	static _float posZ = 0;

	ImGui::InputFloat("Input Fire Position X", &posX);
	ImGui::InputFloat("Input Fire Position Y", &posY);
	ImGui::InputFloat("Input Fire Position Z", &posZ);

	m_stFire->GetTransform()->SetPos(_float3(posX, posY, posZ));
}