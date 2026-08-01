#pragma once
#include "Brand.hpp"
REVERS_STAMP(GameSDK_hpp)
#include "SimpleMath.h"

template<typename T>
T ReadMemory(uintptr_t address);

namespace sdk {
	namespace c_ped {
		namespace fx_draw_data {
			inline uint64_t get_fx_draw_data(uint64_t c_ped)
			{
				return ReadMemory<uint64_t>(c_ped + 0x48);
			}

			inline uint32_t get_query_id(uint64_t fx_draw_data)
			{
				return static_cast<uint32_t>(ReadMemory<uint8_t>(fx_draw_data + 0x2B)) & 0xFF;
			}

			inline bool is_visible(uint32_t query_id, uint64_t offset)
			{
				return ReadMemory<int>(offset + ((query_id - 1) * 0x80) + 0x78) > 100;
			}
		}
	}
}

enum class ePedTask : uint8_t
{
	TASK_NONE = 0,
	TASK_DRIVING = 1 << 0,
	TASK_WALKING = 1 << 1,
	TASK_SWIMMING = 1 << 2,
	TASK_FLYING = 1 << 3
};

bool vec3Empty(const Vector3& value) {
	return value == Vector3(0.f, 0.f, 0.f);
}

Vector3 vec3Transform(Vector3* vIn, Matrix* mIn) {
	Vector3 vOut{};
	vOut.x = vIn->x * mIn->_11 + vIn->y * mIn->_21 + vIn->z * mIn->_31 + 1.f * mIn->_41;
	vOut.y = vIn->x * mIn->_12 + vIn->y * mIn->_22 + vIn->z * mIn->_32 + 1.f * mIn->_42;
	vOut.z = vIn->x * mIn->_13 + vIn->y * mIn->_23 + vIn->z * mIn->_33 + 1.f * mIn->_43;
	return vOut;
};

void normalizeAngles(Vector3& angle) {
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

float getDistance(Vector3 value1, Vector3 value2) {
	float num = value1.x - value2.x;
	float num2 = value1.y - value2.y;
	float num3 = value1.z - value2.z;
	return sqrt(num * num + num2 * num2 + num3 * num3);
}

Vector3 calcAngle(Vector3 localCam, Vector3 toPoint) {
	Vector3 vOut{};
	float distance = getDistance(localCam, toPoint);
	vOut.x = (toPoint.x - localCam.x) / distance;
	vOut.y = (toPoint.y - localCam.y) / distance;
	vOut.z = (toPoint.z - localCam.z) / distance;
	return vOut;
}

bool worldToScreen(const Matrix& viewMatrix, const Vector3& vWorld, Vector2& vOut) {
	Matrix v = viewMatrix.Transpose();
	Vector4 vec = Vector4(v._21, v._22, v._23, v._24), vecY = Vector4(v._31, v._32, v._33, v._34), vecZ = Vector4(v._41, v._42, v._43, v._44);
	Vector3 vecOut = Vector3((vec.x * vWorld.x) + (vec.y * vWorld.y) + (vec.z * vWorld.z) + vec.w,
		(vecY.x * vWorld.x) + (vecY.y * vWorld.y) + (vecY.z * vWorld.z) + vecY.w,
		(vecZ.x * vWorld.x) + (vecZ.y * vWorld.y) + (vecZ.z * vWorld.z) + vecZ.w);

	if (vecOut.z <= 0.1f)
		return false;

	vecOut.z = 1.0f / vecOut.z;
	vecOut.x *= vecOut.z;
	vecOut.y *= vecOut.z;

	const float width = (float)Game.lpRect.right;
	const float height = (float)Game.lpRect.bottom;
	float xTemp = width / 2;
	float yTemp = height / 2;

	vOut.x = xTemp + (0.5f * vecOut.x * width);
	vOut.y = yTemp - (0.5f * vecOut.y * height);
	return true;
}

static std::string getWeaponName(DWORD hash) {
	static auto dagger = std::string("Dagger");
	static auto bat = std::string("Bat");
	static auto bottle = std::string("Bottle");
	static auto crowbar = std::string("Crowbar");
	static auto unarmed = std::string("Unarmed");
	static auto flashlight = std::string("Flashlight");
	static auto golfclub = std::string("Golfclub");
	static auto hammer = std::string("Hammer");
	static auto hatchet = std::string("Hatchet");
	static auto knuckle = std::string("Knuckle");
	static auto knife = std::string("Knife");
	static auto machete = std::string("Machete");
	static auto switchblade = std::string("Switchblade");
	static auto nightstick = std::string("Nightstick");
	static auto wrench = std::string("Wrench");
	static auto battleaxe = std::string("Battleaxe");
	static auto poolcue = std::string("Poolcue");
	static auto pistol = std::string("Pistol");
	static auto pistolmk2 = std::string("Pistol Mk2");
	static auto combatpistol = std::string("Combat Pistol");
	static auto appistol = std::string("Ap Pistol");
	static auto stungun = std::string("Stun Gun");
	static auto pistol50 = std::string("Pistol 50");
	static auto snspistol = std::string("SNS Pistol");
	static auto snspistolmk2 = std::string("SNS Pistol Mk2");
	static auto heavypistol = std::string("Heavy Pistol");
	static auto vintagepistol = std::string("Vintage Pistol");
	static auto flaregun = std::string("Flare Gun");
	static auto marksmanpistol = std::string("Marksman Pistol");
	static auto revolver = std::string("Revolver");
	static auto revolvermk2 = std::string("Revolver Mk2");
	static auto doubleaction = std::string("Double Action");
	static auto microsmg = std::string("Micro SMG");
	static auto smg = std::string("SMG");
	static auto smgmk2 = std::string("SMG Mk2");
	static auto assaultsmg = std::string("Assault SMG");
	static auto combatpdw = std::string("Combat PDW");
	static auto machinepistol = std::string("Machine Pistol");
	static auto minismg = std::string("Mini SMG");
	static auto pumpshotgun = std::string("Pump Shotgun");
	static auto pumpshotgun_mk2 = std::string("Pump Shotgun Mk2");
	static auto sawnoffshotgun = std::string("Sawed Off Shotgun");
	static auto assaultshotgun = std::string("Assault Shotgun");
	static auto bullpupshotgun = std::string("Bullpup Shotgun");
	static auto musket = std::string("Musket");
	static auto heavyshotgun = std::string("Heavy Shotgun");
	static auto dbshotgun = std::string("DB Shotgun");
	static auto autoshotgun = std::string("Auto Shotgun");
	static auto assaultrifle = std::string("Assault Rifle");
	static auto assaultrifle_mk2 = std::string("Assault Rifle Mk2");
	static auto carbinerifle = std::string("Carbine Rifle");
	static auto carbinerifle_mk2 = std::string("Carbine Rifle Mk2");
	static auto advancedrifle = std::string("Advanced Rifle");
	static auto specialcarbine = std::string("Special Carbine");
	static auto specialcarbine_mk2 = std::string("Special Carbine Mk2");
	static auto bullpuprifle = std::string("Bullpup Rifle");
	static auto bullpuprifle_mk2 = std::string("Bullpup Rifle Mk2");
	static auto compactrifle = std::string("Compact Rifle");
	static auto machine_gun = std::string("Machine Gun");
	static auto combatmg = std::string("Combat MG");
	static auto combatmg_mk2 = std::string("Combat MG Mk2");
	static auto gusenberg = std::string("Gusenberg");
	static auto sniperrifle = std::string("Sniper Rifle");
	static auto heavysniper = std::string("Heavy Sniper");
	static auto heavysniper_mk2 = std::string("Heavy Sniper Mk2");
	static auto marksmanrifle = std::string("Marksman Rifle");
	static auto marksmanrifle_mk2 = std::string("Marksman Rifle Mk2");
	static auto rpg = std::string("RPG");
	static auto grenadelauncher = std::string("Grenade Launcher");
	static auto grenadelauncher_smoke = std::string("Grenade Launcher Smoke");
	static auto minigun = std::string("Minigun");
	static auto firework = std::string("Firework");
	static auto railgun = std::string("Railgun");
	static auto hominglauncher = std::string("Homing Launcher");
	static auto compactlauncher = std::string("Compact Launcher");
	static auto grenade = std::string("Grenade");
	static auto bzgas = std::string("BZ Gas");
	static auto smokegrenade = std::string("Smoke Grenade");
	static auto flare = std::string("Flare");
	static auto molotov = std::string("Molotov");
	static auto stickybomb = std::string("Sticky Bomb");
	static auto proxmine = std::string("Prox Mine");
	static auto snowball = std::string("Snowball");
	static auto pipebomb = std::string("Pipe Bomb");
	static auto ball = std::string("Ball");
	static auto petrolcan = std::string("Petrol Can");
	static auto fireextinguisher = std::string("Fire Extinguisher");
	static auto parachute = std::string("Parachute");
	switch (hash) {
	case 0x92A27487:
		return dagger;
		break;
	case 0x958A4A8F:
		return bat;
		break;
	case 0xF9E6AA4B:
		return bottle;
		break;
	case 0x84BD7BFD:
		return crowbar;
		break;
	case 0xA2719263:
		return unarmed;
		break;
	case 0x8BB05FD7:
		return flashlight;
		break;
	case 0x440E4788:
		return golfclub;
		break;
	case 0x4E875F73:
		return hammer;
		break;
	case 0xF9DCBF2D:
		return hatchet;
		break;
	case 0xD8DF3C3C:
		return knuckle;
		break;
	case 0x99B507EA:
		return knife;
		break;
	case 0xDD5DF8D9:
		return machete;
		break;
	case 0xDFE37640:
		return switchblade;
		break;
	case 0x678B81B1:
		return nightstick;
		break;
	case 0x19044EE0:
		return wrench;
		break;
	case 0xCD274149:
		return battleaxe;
		break;
	case 0x94117305:
		return poolcue;
		break;
	case 0x1B06D571:
		return pistol;
		break;
	case 0xBFE256D4:
		return pistolmk2;
		break;
	case 0x5EF9FEC4:
		return combatpistol;
		break;
	case 0x22D8FE39:
		return appistol;
		break;
	case 0x3656C8C1:
		return stungun;
		break;
	case 0x99AEEB3B:
		return pistol50;
		break;
	case 0xBFD21232:
		return snspistol;
		break;
	case 0x88374054:
		return snspistolmk2;
		break;
	case 0xD205520E:
		return heavypistol;
		break;
	case 0x83839C4:
		return vintagepistol;
		break;
	case 0x47757124:
		return flaregun;
		break;
	case 0xDC4DB296:
		return marksmanpistol;
		break;
	case 0xC1B3C3D1:
		return revolver;
		break;
	case 0xCB96392F:
		return revolvermk2;
		break;
	case 0x97EA20B8:
		return doubleaction;
		break;
	case 0x13532244:
		return microsmg;
		break;
	case 0x2BE6766B:
		return smg;
		break;
	case 0x78A97CD0:
		return smgmk2;
		break;
	case 0xEFE7E2DF:
		return assaultsmg;
		break;
	case 0xA3D4D34:
		return combatpdw;
		break;
	case 0xDB1AA450:
		return machinepistol;
		break;
	case 0xBD248B55:
		return minismg;
		break;
	case 0x1D073A89:
		return pumpshotgun;
		break;
	case 0x555AF99A:
		return pumpshotgun_mk2;
		break;
	case 0x7846A318:
		return sawnoffshotgun;
		break;
	case 0xE284C527:
		return assaultshotgun;
		break;
	case 0x9D61E50F:
		return bullpupshotgun;
		break;
	case 0xA89CB99E:
		return musket;
		break;
	case 0x3AABBBAA:
		return heavyshotgun;
		break;
	case 0xEF951FBB:
		return dbshotgun;
		break;
	case 0x12E82D3D:
		return autoshotgun;
		break;
	case 0xBFEFFF6D:
		return assaultrifle;
		break;
	case 0x394F415C:
		return assaultrifle_mk2;
		break;
	case 0x83BF0278:
		return carbinerifle;
		break;
	case 0xFAD1F1C9:
		return carbinerifle_mk2;
		break;
	case 0xAF113F99:
		return advancedrifle;
		break;
	case 0xC0A3098D:
		return specialcarbine;
		break;
	case 0x969C3D67:
		return specialcarbine_mk2;
		break;
	case 0x7F229F94:
		return bullpuprifle;
		break;
	case 0x84D6FAFD:
		return bullpuprifle_mk2;
		break;
	case 0x624FE830:
		return compactrifle;
		break;
	case 0x9D07F764:
		return machine_gun;
		break;
	case 0x7FD62962:
		return combatmg;
		break;
	case 0xDBBD7280:
		return combatmg_mk2;
		break;
	case 0x61012683:
		return gusenberg;
		break;
	case 0x5FC3C11:
		return sniperrifle;
		break;
	case 0xC472FE2:
		return heavysniper;
		break;
	case 0xA914799:
		return heavysniper_mk2;
		break;
	case 0xC734385A:
		return marksmanrifle;
		break;
	case 0x6A6C02E0:
		return marksmanrifle_mk2;
		break;
	case 0xB1CA77B1:
		return rpg;
		break;
	case 0xA284510B:
		return grenadelauncher;
		break;
	case 0x4DD2DC56:
		return grenadelauncher_smoke;
		break;
	case 0x42BF8A85:
		return minigun;
		break;
	case 0x7F7497E5:
		return firework;
		break;
	case 0x6D544C99:
		return railgun;
		break;
	case 0x63AB0442:
		return hominglauncher;
		break;
	case 0x781FE4A:
		return compactlauncher;
		break;
	case 0x93E220BD:
		return grenade;
		break;
	case 0xA0973D5E:
		return bzgas;
		break;
	case 0xFDBC8A50:
		return smokegrenade;
		break;
	case 0x497FACC3:
		return flare;
		break;
	case 0x24B17070:
		return molotov;
		break;
	case 0x2C3731D9:
		return stickybomb;
		break;
	case 0xAB564B93:
		return proxmine;
		break;
	case 0x787F0BB:
		return snowball;
		break;
	case 0xBA45E8B8:
		return pipebomb;
		break;
	case 0x23C9F95C:
		return ball;
		break;
	case 0x34A67B97:
		return petrolcan;
		break;
	case 0x60EC506:
		return fireextinguisher;
		break;
	case 0xFBAB5776:
		return parachute;
		break;
	default:
		return unarmed;
		break;
	}
}