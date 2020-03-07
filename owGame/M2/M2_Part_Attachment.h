#pragma once

#include "M2_Types.h"
#include "M2_Part_Bone.h"

// FORWARD BEGIN
class CM2;
// FORWARD END

enum class M2_AttachmentType : uint32
{
	Shield = 0,
	MountMain = 0,
	ItemVisual0 = 0,

	HandRight = 1,
	ItemVisual1 = 1,

	HandLeft = 2,
	ItemVisual2 = 2,

	ElbowRight = 3,
	ItemVisual3 = 3,

	ElbowLeft = 4,
	ItemVisual4 = 4,

	ShoulderRight = 5,
	ShoulderLeft = 6,

	KneeRight = 7,
	KneeLeft = 8,

	HipRight = 9,
	HipLeft = 10,

	Helm = 11,
	Back = 12,

	ShoulderFlapRight = 13,
	ShoulderFlapLeft = 14,

	ChestBloodFront = 15,
	ChestBloodBack = 16,

	Breath = 17,
	PlayerName = 18,
	Base = 19,
	Head = 20,

	SpellLeftHand = 21,
	SpellRightHand = 22,

	Special1 = 23,
	Special2 = 24,
	Special3 = 25,

	SheathMainHand = 26,
	SheathOffHand = 27,
	SheathShield = 28,

	PlayerNameMounted = 29,

	LargeWeaponLeft = 30,
	LargeWeaponRight = 31,

	HipWeaponLeft = 32,
	HipWeaponRight = 33,

	Chest = 34,
	HandArrow = 35,

	Bullet = 36,
	SpellHandOmni = 37,
	SpellHandDirected = 38,

	VehicleSeat1 = 39,
	VehicleSeat2 = 40,
	VehicleSeat3 = 41,
	VehicleSeat4 = 42,
	VehicleSeat5 = 43,
	VehicleSeat6 = 44,
	VehicleSeat7 = 45,
	VehicleSeat8 = 46,

	LeftFoot = 47,
	RightFoot = 48,

	ShieldNoGlove = 49,
	SpineLow = 50,

	AlteredShoulderR = 51,
	AlteredShoulderL = 52,

	Count = 53
};


class CM2_Part_Attachment
{
public:
	CM2_Part_Attachment(const CM2& M2Object, const std::shared_ptr<IFile>& File, const SM2_Attachment& M2Attachment);
	virtual ~CM2_Part_Attachment();

	const uint16 getBone() const { return m_BoneIndex; }
	const cvec3 getPosition() const { return m_Position; }

private:
	M2_AttachmentType				    m_Type;
	uint16                              m_BoneIndex;
	glm::vec3						    m_Position;
	M2_Animated<uint8>					m_IsAnimateAttached;

private:
	const CM2& m_M2Object;
};