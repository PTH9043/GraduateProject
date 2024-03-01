#include "ToolDefines.h"
#include "TParticleView.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UGameInstance.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UMethod.h"

TParticleView::TParticleView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "ParticleView"),
	m_stMainDesc{},
	m_stParticleView{},
	m_isInitSetting{ false }
{
}

void TParticleView::Free()
{
}

HRESULT TParticleView::NativeConstruct()
{
	m_stMainDesc = MAINDESC(ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_None,
		ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });
	m_stParticleView = DOCKDESC("ParticleViewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	return S_OK;
}

HRESULT TParticleView::LoadResource()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	{
		UParticle::PARTICLEDESC tDesc;
		tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
		tDesc.wstrParticleShader = PROTO_RES_PARTICLE2DSHADER;
		

		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
		//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 1.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 10.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 0.8f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.3f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 50.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 100.f;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount =1000;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 25.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.5f,0.5f,0.5f);
		m_spParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	m_spParticle->GetParticleSystem()->GetParticleTypeParam()->fParticleType = 1;
	
	return S_OK;
}

HRESULT TParticleView::ReleaseResource()
{
	m_spParticle.reset();

	GetGameInstance()->RemoveActor(m_spParticle);

	return S_OK;
}

void TParticleView::TickActive(const _double& _dTimeDelta)
{
}

void TParticleView::LateTickActive(const _double& _dTimeDetla)
{
}

void TParticleView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		ParticleView();
	}
	ImGui::End();
}

void TParticleView::RenderMenu()
{
}

void TParticleView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
		m_stParticleView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 1.f, NULL, &dock_main_id);
		// DockBuild
		ImGui::DockBuilderDockWindow(m_stParticleView.strName.c_str(), m_stParticleView.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}

void TParticleView::ParticleView()
{
	PARTICLEPARAM* particleParam = m_spParticle->GetParticleSystem()->GetParticleParam();
	ComputeParticleType* particleType = m_spParticle->GetParticleSystem()->GetParticleTypeParam();

	ImGui::Begin(m_stParticleView.strName.c_str(), GetOpenPointer(), m_stParticleView.imgWindowFlags);
	{
		ImGui::Text("ParticleView");
		if (true == ImGui::Button("Show Particle"))
		{
			m_spParticle->SetActive(true);
		}
		if (true == ImGui::Button("Stop Particle"))
		{
			m_spParticle->SetActive(false);
		}

		if (ImGui::CollapsingHeader("Particle Count Setting", ImGuiTreeNodeFlags_DefaultOpen)) {
			_uint increment = 1;
			ImGui::InputScalar("Enter Create Amount\n Min:1  Max :1000", ImGuiDataType_U32, m_spParticle->GetParticleSystem()->GetAddParticleAmount(), &increment, &increment);
			if (*m_spParticle->GetParticleSystem()->GetAddParticleAmount() < 1) {
				*m_spParticle->GetParticleSystem()->GetAddParticleAmount() = 1;
			}
			else if (*m_spParticle->GetParticleSystem()->GetAddParticleAmount() > 1000) {
				*m_spParticle->GetParticleSystem()->GetAddParticleAmount() = 1000;
			}
		}

		//파티클의 생성 시간 정하기
		if (ImGui::CollapsingHeader("Particle Create Time Interval", ImGuiTreeNodeFlags_DefaultOpen)) {

			ImGui::InputFloat("Enter Time Interval\n Min:0.f  Max :6.f", m_spParticle->GetParticleSystem()->GetCreateInterval(), 0.1f, 1.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
			if (*m_spParticle->GetParticleSystem()->GetCreateInterval() <= 0) {
				*m_spParticle->GetParticleSystem()->GetCreateInterval() = 0.1;
			}
			else if (*m_spParticle->GetParticleSystem()->GetCreateInterval() > 6) {
				*m_spParticle->GetParticleSystem()->GetCreateInterval() = 6;
			}
		}
		if (ImGui::CollapsingHeader("Particle Settings", ImGuiTreeNodeFlags_DefaultOpen)) {

			//파티클의 한 타임 생성 갯수.
			

			//파티클의 텍스쳐 지정
			ImGui::Text("Particle Texture Select");
			if (ImGui::BeginListBox("Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{

				using TEXNAMES = UNORMAP<_wstring, _uint>;
				TEXNAMES m_TextureNames = m_spParticle->GetTextureGroup()->GetTextureNames();
				for (auto& Texture : m_TextureNames)
				{
					if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
					{
						m_spParticle->SetTexture(Texture.second);
					}
				}
				ImGui::EndListBox();
			}
			
			if (ImGui::SliderFloat("EndScale", &particleParam->stGlobalParticleInfo.fEndScaleParticle, 1.f, 20.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("StartScale", &particleParam->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("MaxLifeTime", &particleParam->stGlobalParticleInfo.fMaxLifeTime, 10.f, 20.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("MaxSpeed", &particleParam->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("MinSpeed", &particleParam->stGlobalParticleInfo.fMinSpeed,  10.f, 100.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("DirectionX", &particleParam->stGlobalParticleInfo.fParticleDirection.x, -1.f, 1.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("DirectionY", &particleParam->stGlobalParticleInfo.fParticleDirection.y,  -1.f, 1.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("DirectionZ", &particleParam->stGlobalParticleInfo.fParticleDirection.z,  -1.f, 1.f, "%.2f"))
			{

			}
			if (ImGui::SliderFloat("Thickness", &particleParam->stGlobalParticleInfo.fParticleThickness,  0.f, 50.f, "%.2f"))
			{

			}
			
			
		}

		if (true == ImGui::Button("Normal Particle"))
		{
			particleType->fParticleType = 0;
		
		
		}
		if (true == ImGui::Button("Random Particle")) {
			particleType->fParticleType = 1;
			particleParam->stGlobalParticleInfo.fParticleThickness = 25;
			particleParam->stGlobalParticleInfo.fEndScaleParticle = 1;
			particleParam->stGlobalParticleInfo.fStartScaleParticle = 10;
			particleParam->stGlobalParticleInfo.fMinLifeTime = 0.3f;
			particleParam->stGlobalParticleInfo.fMaxLifeTime = 0.8f;
			particleParam->stGlobalParticleInfo.fMaxSpeed = 50.f;
			particleParam->stGlobalParticleInfo.fMinSpeed = 100.f;
		}
		
	}
	ImGui::End();
}
