#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"

// General
#include "M2_Part_Bone.h"

SM2_Part_Bone_Wrapper::SM2_Part_Bone_Wrapper(const CM2& M2Object, const std::shared_ptr<IFile>& File, const SM2_Bone& M2Bone)
	: m_M2Object(M2Object)
	, m_M2Bone(M2Bone)
{
	m_TranslateAnimated.Initialize(M2Bone.translation, File, Fix_XZmY);
	m_RotateAnimated.Initialize(M2Bone.rotation, File, Fix_XZmYW);
	m_ScaleAnimated.Initialize(M2Bone.scale, File, Fix_XZY);
}


SM2_Part_Bone_Wrapper::~SM2_Part_Bone_Wrapper()
{
}

glm::mat4 SM2_Part_Bone_Wrapper::calcMatrix(const CM2_Base_Instance* M2Instance, uint32 globalTime) const
{
	glm::mat4 m(1.0f);
	if (const auto& animator = M2Instance->getAnimator())
	{
		if (IsInterpolated(animator->getSequenceIndex()))
		{
			m = glm::translate(m, getPivot());

			if (m_TranslateAnimated.IsUsesBySequence(animator->getSequenceIndex()))
			{
				m = glm::translate(m, m_TranslateAnimated.GetValue(animator->getSequenceIndex(), animator->getCurrentTime(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime));
			}

			if (m_RotateAnimated.IsUsesBySequence(animator->getSequenceIndex()))
			{
				glm::quat q = m_RotateAnimated.GetValue(animator->getSequenceIndex(), animator->getCurrentTime(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime);
				m *= glm::toMat4(q);

				/*
				if (ParentBone != nullptr)
				{
					_ASSERT(ParentBone->IsCalculated());
					m_RotationMatrix = ParentBone->m_RotationMatrix * glm::toMat4(q);
				}
				else
				{
					m_RotationMatrix = glm::toMat4(q);
				}
				*/
			}

			if (m_ScaleAnimated.IsUsesBySequence(animator->getSequenceIndex()))
			{
				m = glm::scale(m, m_ScaleAnimated.GetValue(animator->getSequenceIndex(), animator->getCurrentTime(), m_M2Object.getSkeleton().getGlobalLoops(), globalTime));
			}

			m = glm::translate(m, getPivot() * -1.0f);
		}
	}

	return m;
}

glm::mat4 SM2_Part_Bone_Wrapper::calcBillboardMatrix(cmat4 _viewMatrix, cmat4 _worldMatrix) const
{
	glm::mat4 m(1.0f);
	if (IsBillboard())
	{
		m = glm::translate(m, getPivot());
		{
			mat4 W = _worldMatrix * m;
			mat4 VW = _viewMatrix * W;

			// Set vectors default
			vec3 worldScale = extractScale(W);
			vec3 vRight   = vec3(VW[0][0], VW[1][0], VW[2][0]) / worldScale.x;
			vec3 vUp      = vec3(VW[0][1], VW[1][1], VW[2][1]) / worldScale.y;
			vec3 vForward = vec3(VW[0][2], VW[1][2], VW[2][2]) / worldScale.z;
			vRight *= -1.0f;

			if (m_M2Bone.flags.cylindrical_billboard_lock_x)
			{
				vUp = vec3(VW[0][1], 0, 0);
			}
			else if (m_M2Bone.flags.cylindrical_billboard_lock_y)
			{
				vUp = vec3(0, VW[1][1], 0);
			}
			else if (m_M2Bone.flags.cylindrical_billboard_lock_z)
			{
				vUp = vec3(0, 0, VW[2][1]);
			}

			m[0][0] = vForward.x;
			m[0][1] = vForward.y;
			m[0][2] = vForward.z;

			m[1][0] = vUp.x;
			m[1][1] = vUp.y;
			m[1][2] = vUp.z;

			m[2][0] = vRight.x;
			m[2][1] = vRight.y;
			m[2][2] = vRight.z;
		}
		m = glm::translate(m, getPivot() * -1.0f);
	}

	return m;
}
