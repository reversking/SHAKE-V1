#pragma once
#include "Brand.hpp"
REVERS_STAMP(Ped_hpp)
#include "SimpleMath.h"

template<typename T>
T ReadMemory(uintptr_t address);

template<typename T>
void WriteMemory(uintptr_t address, T value);

struct Bones {
	Vector3 head;
	char padding0[0x4]{};
	Vector3 leftFoot;
	char padding1[0x4]{};
	Vector3 rightFoot;
	char padding2[0x4]{};
	Vector3 leftAnkle;
	char padding3[0x4]{};
	Vector3 rightAnkle;
	char padding4[0x4]{};
	Vector3 leftHand;
	char padding5[0x4]{};
	Vector3 rightHand;
	char padding6[0x4]{};
	Vector3 neck;
	char padding7[0x4]{};
	Vector3 hip;
};

enum Bone : int {
	Head,
	LeftFoot,
	RightFoot,
	LeftHand,
	RightHand,
	Neck,
	Hip,

	LeftArm,
	RightArm,
	LeftElbow,
	RightElbow,
	LeftKnee,
	RightKnee,

	LeftWrist,
	RightWrist,
	LeftCalf,
	RightCalf,

	BONE_COUNT
};

typedef enum eAnimBoneTag
{
	BONETAG_UNKNOWN = 0,
	BONETAG_SKEL_ROOT = 1,
	BONETAG_PELVIS = 3,
	BONETAG_SPINE = 4,
	BONETAG_SPINE1 = 5,
	BONETAG_SPINE2 = 6,
	BONETAG_SPINE3 = 7,
	BONETAG_NECK = 8,
	BONETAG_HEAD = 9,
	BONETAG_L_CLAVICLE = 10,
	BONETAG_L_UPPERARM = 11,
	BONETAG_L_FOREARM = 12,
	BONETAG_L_HAND = 13,
	BONETAG_L_FINGER0 = 14,
	BONETAG_L_FINGER1 = 15,
	BONETAG_L_FINGER2 = 16,
	BONETAG_L_FINGER3 = 17,
	BONETAG_L_FINGER4 = 18,
	BONETAG_L_FINGER01 = 19,
	BONETAG_L_FINGER11 = 20,
	BONETAG_L_FINGER21 = 21,
	BONETAG_L_FINGER31 = 22,
	BONETAG_L_FINGER41 = 23,
	BONETAG_L_FINGER02 = 24,
	BONETAG_L_FINGER12 = 25,
	BONETAG_L_FINGER22 = 26,
	BONETAG_L_FINGER32 = 27,
	BONETAG_L_FINGER42 = 28,
	BONETAG_R_CLAVICLE = 29,
	BONETAG_R_UPPERARM = 30,
	BONETAG_R_FOREARM = 31,
	BONETAG_R_HAND = 32,
	BONETAG_R_FINGER0 = 33,
	BONETAG_R_FINGER1 = 34,
	BONETAG_R_FINGER2 = 35,
	BONETAG_R_FINGER3 = 36,
	BONETAG_R_FINGER4 = 37,
	BONETAG_R_FINGER01 = 38,
	BONETAG_R_FINGER11 = 39,
	BONETAG_R_FINGER21 = 40,
	BONETAG_R_FINGER31 = 41,
	BONETAG_R_FINGER41 = 42,
	BONETAG_R_FINGER02 = 43,
	BONETAG_R_FINGER12 = 44,
	BONETAG_R_FINGER22 = 45,
	BONETAG_R_FINGER32 = 46,
	BONETAG_R_FINGER42 = 47,
	BONETAG_L_THIGH = 48,
	BONETAG_L_CALF = 49,
	BONETAG_L_FOOT = 50,
	BONETAG_L_TOE = 51,
	BONETAG_R_THIGH = 52,
	BONETAG_R_CALF = 53,
	BONETAG_R_FOOT = 54,
	BONETAG_R_TOE = 55,
	BONETAG_L_IK_HAND = 56,
	BONETAG_R_IK_HAND = 57,
	BONETAG_L_IK_FOOT = 58,
	BONETAG_R_IK_FOOT = 59,
	BONETAG_FACE = 60,
	BONETAG_IK_HEAD = 61,
	BONETAG_MAX = 62,
} eAnimBoneTag;

class Ped {
public:
	uintptr_t pointer;
	uintptr_t playerInfo;
	uintptr_t weaponManager;
	uintptr_t currentWeapon;

	float armor;
	float health;
	Vector3 position;
	Matrix boneMatrix;
	Vector3 boneList[BONE_COUNT]{};

	bool getPlayer(uintptr_t& base) {
		pointer = base;
		return pointer != NULL;
	}

	uintptr_t getWeapon() {
		return ReadMemory<uintptr_t>(ReadMemory<uintptr_t>(ReadMemory<uintptr_t>(pointer + Offsets.WeaponManager) + 0x20) + 0x10);
	}

	int getID() {
		return ReadMemory<int>(ReadMemory<uint64_t>(pointer + Offsets.PlayerInfo) + Offsets.Id);
	}


	int cachedId = -1;
	void cacheId() {
		if (playerInfo) cachedId = ReadMemory<int>(playerInfo + Offsets.Id);
		else cachedId = -1;
	}

	bool isPlayer() {
		return playerInfo != NULL;
	}

	bool isDead() {
		return position == Vector3(0.f, 0.f, 0.f);
	}

	bool isVisible() {
		enum VisibilityFlags : BYTE {
			InvisibleFlag1 = 36,
			InvisibleFlag2 = 0,
			InvisibleFlag3 = 4,
			IntersectMissionEntityAndTrain = 2,
			IntersectPeds1 = 4,
			IntersectVehicles = 10,
			IntersectVegetation = 256,
			FrustumCulling = 512,
			OcclusionCulling = 1024,
			DistanceCulling = 2048
		};

		BYTE VisibilityFlag = ReadMemory<BYTE>(pointer + Offsets.VisibleFlag);
		if (VisibilityFlag == InvisibleFlag1 ||
			VisibilityFlag == InvisibleFlag2 ||
			VisibilityFlag == InvisibleFlag3 ||
			VisibilityFlag == IntersectMissionEntityAndTrain ||
			VisibilityFlag == IntersectPeds1 ||
			VisibilityFlag == IntersectVehicles ||
			VisibilityFlag == IntersectVegetation ||
			(VisibilityFlag & FrustumCulling) ||
			(VisibilityFlag & OcclusionCulling) ||
			(VisibilityFlag & DistanceCulling))
		{
			return false;
		}

		return true;
	}

	bool update() {
		if (!pointer) return false;

		playerInfo = ReadMemory<uintptr_t>(pointer + Offsets.PlayerInfo);
		currentWeapon = getWeapon();
		health = ReadMemory<float>(pointer + Offsets.Health);
		position = ReadMemory<Vector3>(pointer + 0x90);
		armor = ReadMemory<float>(pointer + Offsets.Armor);
		boneMatrix = ReadMemory<Matrix>(pointer + 0x60);
		cacheId();
		updateBones();
		return true;
	}


	bool updateEsp() {
		if (!pointer) return false;
		playerInfo = ReadMemory<uintptr_t>(pointer + Offsets.PlayerInfo);
		position = ReadMemory<Vector3>(pointer + 0x90);
		health = ReadMemory<float>(pointer + Offsets.Health);
		armor = ReadMemory<float>(pointer + Offsets.Armor);
		boneMatrix = ReadMemory<Matrix>(pointer + 0x60);
		cacheId();
		updateBones();
		return true;
	}

	void updateBones() {
		Bones bones = ReadMemory<Bones>(pointer + Offsets.BoneList);

		boneList[Head] = vec3Transform(&bones.head, &boneMatrix);
		boneList[LeftFoot] = vec3Transform(&bones.leftFoot, &boneMatrix);
		boneList[RightFoot] = vec3Transform(&bones.rightFoot, &boneMatrix);
		boneList[LeftHand] = vec3Transform(&bones.leftHand, &boneMatrix);
		boneList[RightHand] = vec3Transform(&bones.rightHand, &boneMatrix);
		boneList[Neck] = vec3Transform(&bones.neck, &boneMatrix);
		boneList[Hip] = vec3Transform(&bones.hip, &boneMatrix);


		boneList[LeftKnee] = Vector3(
			boneList[Hip].x + (boneList[LeftFoot].x - boneList[Hip].x) * 0.55f,
			boneList[Hip].y + (boneList[LeftFoot].y - boneList[Hip].y) * 0.55f,
			boneList[Hip].z + (boneList[LeftFoot].z - boneList[Hip].z) * 0.55f);
		boneList[RightKnee] = Vector3(
			boneList[Hip].x + (boneList[RightFoot].x - boneList[Hip].x) * 0.55f,
			boneList[Hip].y + (boneList[RightFoot].y - boneList[Hip].y) * 0.55f,
			boneList[Hip].z + (boneList[RightFoot].z - boneList[Hip].z) * 0.55f);
	}

	Vector3 getExtendedBonePosition(eAnimBoneTag boneTag) {
		uintptr_t boneCollection = ReadMemory<uintptr_t>(pointer + 0x48);
		if (!boneCollection) return Vector3(0, 0, 0);

		uintptr_t bone = ReadMemory<uintptr_t>(boneCollection + (uintptr_t)boneTag * 0x10);
		if (!bone) return Vector3(0, 0, 0);

		return ReadMemory<Vector3>(bone + 0x10);
	}

	uint32_t getModelHash() {
		if (!pointer) return 0;

		uintptr_t modelInfo = ReadMemory<uintptr_t>(pointer + 0x20);
		if (!modelInfo) return 0;
		return ReadMemory<uint32_t>(modelInfo + 0x18);
	}

	bool isMale() {
		uint32_t hash = getModelHash();
		return hash == 0x705E61F2;
	}

	bool isFemale() {
		uint32_t hash = getModelHash();
		return hash == 0x9C9EFFD8;
	}
};