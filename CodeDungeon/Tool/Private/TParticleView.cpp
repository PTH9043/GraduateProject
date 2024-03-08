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
		tDesc.wstrParticleTextureName[0] = L"bubble";
		tDesc.wstrParticleTextureName[1] = L"Flare_Sparks_TexS";

		tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 5.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 10.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 20.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.5f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 10.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 50.f;
		tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 300;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 0.f;
		tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(1.f,1.f,1.f);
		m_spParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
	}
	
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
	ImGui::Begin(m_stParticleView.strName.c_str(), GetOpenPointer(), m_stParticleView.imgWindowFlags);
	{
		ImGui::Text("ParticleView");
		if (true == ImGui::Button("Show Particle"))
		{
			m_spParticle->SetActive(true);
		}
		float& endScale = m_spParticle->GetParticleParam()->stGlobalParticleInfo.fEndScaleParticle;

		if (ImGui::CollapsingHeader("Particle Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::DragFloat("EndScale", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fEndScaleParticle, 0.5f, 1.f, 20.f, "%.2f"))
			{
				
			}
			if (ImGui::DragFloat("StartScale", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fStartScaleParticle, 0.5f, 1.f, 20.f, "%.2f"))
			{

			}
			if (ImGui::DragFloat("MaxLifeTime", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fMaxLifeTime, 0.5f, 10.f, 20.f, "%.2f"))
			{

			}
			if (ImGui::DragFloat("MaxSpeed", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fMaxSpeed, 0.5f, 10.f, 100.f, "%.2f"))
			{

			}
			if (ImGui::DragFloat("MinSpeed", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fMinSpeed, 0.5f, 10.f, 100.f, "%.2f"))
			{

			}	
			if (ImGui::DragFloat("DirectionX", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fParticleDirection.x, 0.01f, -1.f, 1.f, "%.2f"))
			{

			}
			if (ImGui::DragFloat("DirectionY", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fParticleDirection.y, 0.01f, -1.f, 1.f, "%.2f"))
			{

			}
			if (ImGui::DragFloat("DirectionZ", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fParticleDirection.z, 0.01f, -1.f, 1.f, "%.2f"))
			{

			}
			if (ImGui::DragFloat("Thickness", &m_spParticle->GetParticleParam()->stGlobalParticleInfo.fParticleThickness, 0.2f, 0.f, 50.f, "%.2f"))
			{

			}
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
			
		}
	}
	ImGui::End();
}
