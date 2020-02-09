#include "stdafx.h"

// Gerenal
#include "SceneWoW.h"

CSceneWoW::CSceneWoW(IBaseManager * BaseManager)
	: SceneBase(BaseManager)
{}

CSceneWoW::~CSceneWoW()
{
	OutputDebugStringW(L"Destroyed.");
}


//
// IGameState
//
void CSceneWoW::Initialize()
{
	SceneBase::Initialize();

	ISceneNode3D* cameraNode = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(*cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, 1.0f/*GetRenderWindow()->GetWindowWidth() / GetRenderWindow()->GetWindowHeight()*/, 0.5f, 10000.0f);

	Load3D();
	LoadUI();

	cameraNode->SetTranslate(vec3(-50, 160, 170));
	GetCameraController()->GetCamera()->SetYaw(-51);
	GetCameraController()->GetCamera()->SetPitch(-38);
}

void CSceneWoW::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CSceneWoW::OnRayIntersected(const glm::vec3& Point)
{

}



//
//
//
void CSceneWoW::OnPreRender(RenderEventArgs& e)
{
	SceneBase::OnPreRender(e);
}



//
// Keyboard events
//
bool CSceneWoW::OnWindowKeyPressed(KeyEventArgs & e)
{

	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneWoW::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
//
//
void CSceneWoW::Load3D()
{
	CWMO_Base_Instance* wmoInstance = GetRootNode3D()->CreateSceneNode<CWMO_Base_Instance>("World\\wmo\\Azeroth\\Buildings\\Stormwind\\Stormwind.wmo");
	wmoInstance->Load(GetRenderDevice());

	std::shared_ptr<BuildRenderListPassTemplated<CWMO_Group_Instance>> wmoListPass = std::make_shared<BuildRenderListPassTemplated<CWMO_Group_Instance>>(GetRenderDevice(), shared_from_this());
	
	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_Technique3D.AddPass(wmoListPass);
	//m_Technique3D.AddPass(std::make_shared<CRenderPass_Sky>(GetRenderDevice(), m_Scene3D)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport()));
	//m_Technique3D.AddPass(std::make_shared<CRenderPass_WDL>(GetRenderDevice(), m_Scene3D)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport()));
	//m_Technique3D.AddPass(std::make_shared<CRenderPass_ADT_MCNK>(GetRenderDevice(), m_Scene3D)->CreatePipeline(GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport()));
	//m_Technique3D.AddPass(std::make_shared<CRenderPass_WMO>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(std::make_shared<CRenderPass_WMO2>(GetRenderDevice(), wmoListPass, shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(std::make_shared<CRenderPass_M2>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("LiquidPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
}

void CSceneWoW::LoadUI()
{
	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}