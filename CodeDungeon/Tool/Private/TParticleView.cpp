#include "ToolDefines.h"
#include "TParticleView.h"
#include "UParticle.h"
#include "UParticleSystem.h"
#include "UAnimatedParticle.h"
#include "UGameInstance.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UMethod.h"

TParticleView::TParticleView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "ParticleView"),
	m_stMainDesc{},
	m_stParticleView{},
	m_isInitSetting{ false }
	, m_SingleParticle{nullptr}, m_SingleParticleParam{nullptr}, m_SingleParticleType{nullptr}
	, m_MultipleParticle{nullptr}, m_MultipleParticleParam{nullptr}, m_MultipleParticleType{nullptr}
	, m_AnimParticle{nullptr}, m_AnimParticleParam{nullptr}, m_AnimParticleType{nullptr}
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

//================================================================
//**** 파티클 종류별로 관리하는 컨테이너(벡터) 크기 미리 할당 ****
//================================================================
void TParticleView::ResizeSingleParticleVector(_uint _resizeAmount)
{
	m_iSingleParticleSize = _resizeAmount;
	m_SingleParticle.resize(m_iSingleParticleSize);
	m_SingleParticleParam.resize(m_iSingleParticleSize);
	m_SingleParticleType.resize(m_iSingleParticleSize);
}


void TParticleView::ResizeMultipleParticleVector(_uint _resizeAmount)
{
	m_iMultipleParticleSize = _resizeAmount;
	m_MultipleParticle.resize(m_iMultipleParticleSize);
	m_MultipleParticleParam.resize(m_iMultipleParticleSize);
	m_MultipleParticleType.resize(m_iMultipleParticleSize);
}

void TParticleView::ResizeAnimParticleVector(_uint _resizeAmount)
{
	m_iAnimParticleSize = _resizeAmount;
	m_AnimParticle.resize(m_iAnimParticleSize);
	m_AnimParticleParam.resize(m_iAnimParticleSize);
	m_AnimParticleType.resize(m_iAnimParticleSize);
}

//================================================================
//**************** 파티클 종류별로 리소스 할당 ****************
//================================================================

void TParticleView::LoadSingleParticleResource()
{
	for (int i = 0; i < m_iSingleParticleSize; i++) {
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
			tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 1000;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 25.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.5f, 0.5f, 0.5f);
			m_SingleParticle[i] = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
		}
		m_SingleParticleParam[i] = m_SingleParticle[i]->GetParticleSystem()->GetParticleParam();
		m_SingleParticleType[i] = m_SingleParticle[i]->GetParticleSystem()->GetParticleTypeParam();
		m_SingleParticleType[i]->fParticleType = PARTICLE_TYPE_AUTO;
	}
}

void TParticleView::LoadMultipleParticleResource()
{
	for (int i = 0; i < m_iMultipleParticleSize; i++) {
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			UParticle::PARTICLEDESC tDesc;
			tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
			tDesc.wstrParticleShader = PROTO_RES_PARTICLE2DSHADER;
			

			tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
			//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 1.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 10.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 0.8f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 0.3f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 50.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 100.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 500 * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 25.f * (i + 1);
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.5f - i, 0.5f - i, 0.5f - i);
			m_MultipleParticle[i] = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE, { &tDesc }));
		}
		m_MultipleParticleParam[i] = m_MultipleParticle[i]->GetParticleSystem()->GetParticleParam();
		m_MultipleParticleType[i] = m_MultipleParticle[i]->GetParticleSystem()->GetParticleTypeParam();
		m_MultipleParticleType[i]->fParticleType = PARTICLE_TYPE_AUTO;
		m_MultipleParticle[i]->SetTexture(4);
		//m_MultipleParticle[i]->SetActive(true);
	}

}

void TParticleView::LoadAnimParticleResource()
{
	for (int i = 0; i < m_iAnimParticleSize; i++) {
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		{
			UParticle::PARTICLEDESC tDesc;
			tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
			tDesc.wstrParticleShader = PROTO_RES_2DANIMATEPARTICLESHADER;
		

			tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 0.f;
			//tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 10.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 30.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 5.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMinLifeTime = 3.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 50.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 100.f;
			tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 1000;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleThickness = 25.f;
			tDesc.ParticleParam.stGlobalParticleInfo.fParticleDirection = _float3(0.0f, 0.0f, 0.1f);

			UAnimatedParticle::ANIMPARTICLEDESC tAnimDesc;
			tAnimDesc.vTextureSize = _float2{4.f, 4.f };
			tAnimDesc.fNextAnimTime = 0.05f;
			m_AnimParticle[i] = std::static_pointer_cast<UAnimatedParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ANIMATEPARTICLE, { &tDesc,&tAnimDesc }));
		}
		m_AnimParticleParam[i] = m_AnimParticle[i]->GetParticleSystem()->GetParticleParam();
		m_AnimParticleType[i] = m_AnimParticle[i]->GetParticleSystem()->GetParticleTypeParam();
		m_AnimParticleType[i]->fParticleType = PARTICLE_TYPE_DEFAULT;
		m_AnimParticle[i]->SetTexture(6);
		*m_AnimParticle[i]->GetParticleSystem()->GetAddParticleAmount() = 1;
		m_AnimParticle[i]->SetActive(true);
	}

}

HRESULT TParticleView::LoadResource()
{
	ResizeSingleParticleVector(1);
	LoadSingleParticleResource();

	ResizeMultipleParticleVector(2);
	LoadMultipleParticleResource();

	ResizeAnimParticleVector(1);
	LoadAnimParticleResource();

	return S_OK;
}

//================================================================
//**************** 파티클 종류별로 리소스 해제 ****************
//================================================================

void TParticleView::ReleaseSingleParticleResource()
{
	for (auto& _particle : m_SingleParticle) {
		_particle.reset();
		GetGameInstance()->RemoveActor(_particle);
	}//문제 유발될 수 있는 코드 RemoveActor의 상세 구현을 살펴보아야함.
	
}

void TParticleView::ReleaseMultipleParticleResource()
{
	for (auto& _particle : m_MultipleParticle) {
		_particle.reset();
		GetGameInstance()->RemoveActor(_particle);
	}//문제 유발될 수 있는 코드 RemoveActor의 상세 구현을 살펴보아야함.
}

void TParticleView::ReleaseAnimParticleResource()
{
	for (auto& _particle : m_AnimParticle) {
		_particle.reset();
		GetGameInstance()->RemoveActor(_particle);
	}//문제 유발될 수 있는 코드 RemoveActor의 상세 구현을 살펴보아야함.
}


HRESULT TParticleView::ReleaseResource()
{
	ReleaseSingleParticleResource();
	ReleaseMultipleParticleResource();
	ReleaseAnimParticleResource();

	return S_OK;
}


//================================================================
//************ 파티클 종류별로 Rendering 하는 부분 ************
//================================================================

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
		//여기부터 내 입력
		SingleParticleView();
		MultipleParticleView();
		AnimParticleView();
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

//================================================================
//************ SINGLE PARTICLE TOOL VIEW SETTING ************
//================================================================

void TParticleView::SingleParticleView()
{
	

	ImGui::Begin(m_stParticleView.strName.c_str(), GetOpenPointer(), m_stParticleView.imgWindowFlags);
	{
		ImGui::Text("ParticleView");
		if (true == ImGui::Button("Show Particle"))
		{
			m_SingleParticle[0]->SetActive(true);
		}
		if (true == ImGui::Button("Stop Particle"))
		{
			m_SingleParticle[0]->SetActive(false);
		}

		SingleParticleCountSetting();
		SingleParticleTimeSetting();
		SingleParticleTexSetting();
		DefaultSingleParticleSetting();
		AutomaticSingleParticleSetting();
	
	/*	if (true == ImGui::Button("Random Particle")) {
			particleType->fParticleType = 1;
			particleParam->stGlobalParticleInfo.fParticleThickness = 25;
			particleParam->stGlobalParticleInfo.fEndScaleParticle = 1;
			particleParam->stGlobalParticleInfo.fStartScaleParticle = 10;
			particleParam->stGlobalParticleInfo.fMinLifeTime = 0.3f;
			particleParam->stGlobalParticleInfo.fMaxLifeTime = 0.8f;
			particleParam->stGlobalParticleInfo.fMaxSpeed = 50.f;
			particleParam->stGlobalParticleInfo.fMinSpeed = 100.f;
		}*/
		
	}
	ImGui::End();
}



void TParticleView::SingleParticleCountSetting()
{
	//파티클 한 사이클 생성 갯수.
	if (ImGui::CollapsingHeader("Particle Count Setting", ImGuiTreeNodeFlags_DefaultOpen)) {
		_uint increment = 1;
		ImGui::InputScalar("Enter Create Amount\n Min:1  Max :1000", ImGuiDataType_U32, m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount(), &increment, &increment);
		if (*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() < 1) {
			*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1;
		}
		else if (*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() > 1000) {
			*m_SingleParticle[0]->GetParticleSystem()->GetAddParticleAmount() = 1000;
		}
	}
}


void TParticleView::SingleParticleTimeSetting()
{
	//파티클의 생성 시간 정하기
	if (ImGui::CollapsingHeader("Particle Create Time Interval", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::InputFloat("Enter Time Interval\n Min:0.f  Max :6.f", m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval(), 0.1f, 1.0f, "%.2f", ImGuiInputTextFlags_CharsDecimal);
		if (*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() <= 0) {
			*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() = 0.1;
		}
		else if (*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() > 6) {
			*m_SingleParticle[0]->GetParticleSystem()->GetCreateInterval() = 6;
		}
	}
}

void TParticleView::SingleParticleTexSetting()
{
	if (ImGui::CollapsingHeader("Particle Settings", ImGuiTreeNodeFlags_DefaultOpen)) {

		//파티클의 텍스쳐 지정
		ImGui::Text("Particle Texture Select");
		if (ImGui::BeginListBox("Texture List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{

			using TEXNAMES = UNORMAP<_wstring, _uint>;
			TEXNAMES m_TextureNames = m_SingleParticle[0]->GetTextureGroup()->GetTextureNames();
			for (auto& Texture : m_TextureNames)
			{
				if (ImGui::Selectable(UMethod::ConvertWToS(Texture.first)))
				{
					m_SingleParticle[0]->SetTexture(Texture.second);
				}
			}
			ImGui::EndListBox();
		}
	}
}


void TParticleView::DefaultSingleParticleSetting()
{
	_bool Default = false;
	if (ImGui::Selectable("Set Particle Type : Default", Default)) {
		m_SingleParticleType[0]->fParticleType = PARTICLE_TYPE_DEFAULT;
		ImGui::OpenPopup("Default Particle");
	}

	if (ImGui::BeginPopup("Default Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Default");

		ImGui::SliderFloat("EndScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxLifeTime", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxLifeTime, 10.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("DirectionX", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleDirection.x, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionY", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleDirection.y, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("DirectionZ", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleDirection.z, -1.f, 1.f, "%.2f");
		ImGui::SliderFloat("Thickness", &m_SingleParticleParam[0]->stGlobalParticleInfo.fParticleThickness, 0.f, 50.f, "%.2f");
		ImGui::EndPopup();
	}
}

void TParticleView::AutomaticSingleParticleSetting()
{
	_bool Automatic = false;
	if (ImGui::Selectable("Set Particle Type : Automatic", Automatic)) {
		m_SingleParticleType[0]->fParticleType = PARTICLE_TYPE_AUTO;
		ImGui::OpenPopup("Automatic Particle");
	}



	if (ImGui::BeginPopup("Automatic Particle")) {
		ImGui::SeparatorText("Particle Settings");
		ImGui::SeparatorText("Current Particle Type : Automatic");

		ImGui::SliderFloat("EndScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fEndScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("StartScale", &m_SingleParticleParam[0]->stGlobalParticleInfo.fStartScaleParticle, 1.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxLifeTime", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxLifeTime, 10.f, 20.f, "%.2f");
		ImGui::SliderFloat("MaxSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMaxSpeed, 10.f, 100.f, "%.2f");
		ImGui::SliderFloat("MinSpeed", &m_SingleParticleParam[0]->stGlobalParticleInfo.fMinSpeed, 10.f, 100.f, "%.2f");
		

		ImGui::EndPopup();
	}
}

//================================================================
//************ MULTIPLE PARTICLE TOOL VIEW SETTING ************
//================================================================

void TParticleView::MultipleParticleView()
{
}

//================================================================
//************ ANIM PARTICLE TOOL VIEW SETTING ************
//================================================================

void TParticleView::AnimParticleView()
{
}