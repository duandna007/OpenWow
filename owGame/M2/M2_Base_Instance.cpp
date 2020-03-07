#include "stdafx.h"

// General
#include "M2_Base_Instance.h"

// Additional
#include "M2_ColliderComponent.h"

CM2_Base_Instance::CM2_Base_Instance(const std::shared_ptr<CM2>& M2Object) 
	: CLoadableObject(M2Object)
	, m_M2(M2Object)
	, m_Attached(nullptr)
	, m_Animator(nullptr)
	, m_Color(vec4(1.0f, 1.0f, 1.0f, 1.0f))
	, m_Alpha(1.0f)
{
	SetType(cM2_NodeType);
}

CM2_Base_Instance::~CM2_Base_Instance()
{
	if (getM2().isAnimated())
	{
		//_Bindings->UnregisterUpdatableObject(this);
	}
}

// CM2_Base_Instance

void CM2_Base_Instance::CreateInstances()
{
	getM2().CreateInsances(shared_from_this());
}

bool CM2_Base_Instance::Load()
{
	if (getM2().isAnimated())
		m_Animator = std::make_shared<CM2_Animator>(GetBaseManager(), getM2());

	if (getM2().getSkeleton().hasBones())
		m_SkeletonComponent = std::make_shared<CM2SkeletonComponent3D>(*this, getM2());

	UpdateLocalTransform();
	CreateInstances();

	return true;
}

const CM2 & CM2_Base_Instance::getM2() const
{
	_ASSERT(m_M2 != nullptr);
	return *m_M2;
}

void CM2_Base_Instance::Attach(std::shared_ptr<const CM2_Part_Attachment> _attachment)
{
	_ASSERT(_attachment != nullptr);
	m_Attached = _attachment;
}
void CM2_Base_Instance::Detach()
{
	m_Attached = nullptr;
}

std::shared_ptr<const CM2_Part_Attachment> CM2_Base_Instance::GetAttachPoint() const
{
    return m_Attached;
}

// Mesh & textures provider
bool CM2_Base_Instance::isMeshEnabled(uint32 _index) const
{
	return true;
}
void CM2_Base_Instance::setSpecialTexture(SM2_Texture::Type _type, const std::shared_ptr<ITexture>& _texture)
{
	//_ASSERT(_texture != nullptr);
	m_SpecialTextures[_type] = _texture;
}
const std::shared_ptr<ITexture>& CM2_Base_Instance::getSpecialTexture(SM2_Texture::Type _type) const
{
	_ASSERT(_type < SM2_Texture::Type::COUNT);
	return m_SpecialTextures[_type];
}


//
//	m_M2->update(_time, _dTime);
//

void CM2_Base_Instance::Initialize()
{
	GetColliderComponent()->SetCullDistance(GetBaseManager().GetManager<ISettings>()->GetGroup("WoWSettings")->GetSettingT<float>("ADT_MDX_Distance")->Get());
	GetColliderComponent()->SetBounds(getM2().GetBounds());
	GetColliderComponent()->SetDebugDrawMode(false);
}

void CM2_Base_Instance::Update(const UpdateEventArgs & e)
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return;

	if (getM2().isAnimated())
	{
		m_Animator->Update(e.TotalTime, e.DeltaTime);
	}

	if (getM2().getSkeleton().hasBones())
		getSkeletonComponent()->Calculate(static_cast<uint32>(e.TotalTime));

	if (m_Attached != nullptr)
		ForceRecalculateLocalTransform();
}

void CM2_Base_Instance::Accept(IVisitor* visitor)
{
	SceneNode3D::Accept(visitor);
}

//
// CTransformComponent
//
void CM2_Base_Instance::UpdateLocalTransform()
{
	if (auto attachPoint = GetAttachPoint())
	{
		uint16 boneIndex = attachPoint->getBone();

		if (auto parent = GetParent().lock())
		{
			auto parentM2Instance = std::dynamic_pointer_cast<CM2_Base_Instance>(parent);
			const auto& bone = parentM2Instance->getSkeletonComponent()->GetBone(boneIndex);

			glm::mat4 relMatrix = glm::translate(bone->GetPivotPoint());
			glm::mat4 absMatrix = GetParentWorldTransform() * bone->GetMatrix() * relMatrix;
			SetWorldTransform(absMatrix);
		}
	}
	else
	{
		SceneNode3D::UpdateLocalTransform();
	}
}

void CM2_Base_Instance::RegisterComponents()
{
	m_Components_Models = AddComponent(std::make_shared<CModelsComponent3D>(*this));
    m_Components_Collider = AddComponent(std::make_shared<CM2_ColliderComponent>(*this));
}
