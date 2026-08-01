#pragma once
#include "Brand.hpp"
REVERS_STAMP(Game_hpp)

struct GameStruct {
	DWORD pID;
	HANDLE hProcess;
	HWND hWnd;
	LPCSTR lpClassName = "grcWindow";
	std::string Version;
	RECT lpRect;
	POINT lpPoint;
	std::string Path;
	uintptr_t World;
	uintptr_t ViewPort;
	uintptr_t ReplayInterface;
};

extern GameStruct Game;

namespace game
{
	inline HWND hwnd;
	inline uintptr_t base;
	inline uintptr_t world;
	inline uintptr_t local_player;
	inline Vector3 local_position;
	inline uintptr_t view_angles;
}

struct OffsetsStruct {
	uintptr_t GameBase;
	uintptr_t GameWorld;
	uintptr_t ReplayInterface;
	uintptr_t ViewPort;
	uintptr_t LocalPlayer;
	uintptr_t Camera;
	uintptr_t BlipList;
	uintptr_t PlayerInfo;
	uintptr_t Id;
	uintptr_t Health;
	uintptr_t MaxHealth;
	uintptr_t Armor;
	uintptr_t WeaponManager;
	uintptr_t BoneList;
	uintptr_t Silent;
	uint64_t Waypoint;
	uintptr_t Vehicle;
	uintptr_t VisibleFlag;

	uintptr_t SpeedSigAddr;
	uintptr_t PlayerSpeed;
	uintptr_t RagDollSigAddr;
	uintptr_t RagDoll;
	uintptr_t SeatBealtAddr;
	uintptr_t SeatBealt;

	uintptr_t VehicleDoorsLockState;

	uintptr_t VehicleModelHash;

	uintptr_t m_Invisible;

	uintptr_t m_GetFinalRenderedCamCord;
	uintptr_t m_GetCurrentPedWeapon;
	uintptr_t m_GetSelectedPedWeapon;
	uintptr_t bulletHandler;
	uintptr_t bulletPos;

	int CurrentBuild;

	uintptr_t NetObject = 0x820;
	uintptr_t Handling = 0x960;
};

extern OffsetsStruct Offsets;

inline DWORD FindGame() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hSnapshot, &pe32)) {
		do {
			std::string processName = pe32.szExeFile;
			if (processName.find("GTAProcess.exe") != std::string::npos) {
				CloseHandle(hSnapshot);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}
	CloseHandle(hSnapshot);
	return 0;
}

inline void ReadOffsets() {
	if (Game.Version.empty()) {
		return;
	}

	Offsets.VehicleDoorsLockState = 0x13C0;

	Offsets.VehicleModelHash = 0x20;

	Offsets.Handling = 0x960;

	if (Game.Version == "3751") {
		Offsets.GameWorld = 0x2603908;
		Offsets.ReplayInterface = 0x1FC38A8;
		Offsets.ViewPort = 0x206C060;
		Offsets.Camera = 0x206D1C0;
		Offsets.BoneList = 0x410;
		Offsets.WeaponManager = 0x10B8;
		Offsets.Silent = 0x102B560;
		Offsets.VisibleFlag = 0x145C;
		Offsets.BlipList = 0x206D600;
		Offsets.Vehicle = 0x0D10;
		Offsets.Waypoint = 0x2047D50;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.m_Invisible = 0x6AE372;
		Offsets.CurrentBuild = 3751;
	}

	if (Game.Version == "3570") {
		Offsets.GameWorld = 0x25EC580;
		Offsets.ReplayInterface = 0x1FB0418;
		Offsets.ViewPort = 0x2058BA0;
		Offsets.Camera = 0x2059778;
		Offsets.BoneList = 0x410;
		Offsets.WeaponManager = 0x10B8;
		Offsets.Silent = 0x102D550;
		Offsets.VisibleFlag = 0x145C;
		Offsets.BlipList = 0x2061870;
		Offsets.Vehicle = 0x0D10;
		Offsets.Waypoint = 0x2047D50;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.m_Invisible = 0x6AE372;
		Offsets.CurrentBuild = 3570;
	}

	if (Game.Version == "3407") {
		Offsets.GameWorld = 0x25D7108;
		Offsets.ReplayInterface = 0x1F9A9D8;
		Offsets.ViewPort = 0x20431C0;
		Offsets.Camera = 0x2043DF8;
		Offsets.BlipList = 0x20440C8;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.WeaponManager = 0x10B8;
		Offsets.BoneList = 0x410;
		Offsets.Silent = 0x102FF89;
		Offsets.Vehicle = 0x0D10;
		Offsets.VisibleFlag = 0x145C;
		Offsets.Waypoint = 0x2047D50;
		Offsets.m_Invisible = 0x6AC2DA;
		Offsets.CurrentBuild = 3407;
	}

	if (Game.Version == "3323") {
		Offsets.GameWorld = 0x25C15B0;
		Offsets.ReplayInterface = 0x1F85458;
		Offsets.ViewPort = 0x202DC50;
		Offsets.Camera = 0x202E878;
		Offsets.BlipList = 0x2002888;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.WeaponManager = 0x10B8;
		Offsets.BoneList = 0x410;
		Offsets.Silent = 0x1026CAD;
		Offsets.Vehicle = 0x0D10;
		Offsets.VisibleFlag = 0x145C;
		Offsets.Waypoint = 0x2022DE0;
		Offsets.m_Invisible = 0x6A51DA;
		Offsets.CurrentBuild = 3323;
	}

	if (Game.Version == "3258") {
		Offsets.GameWorld = 0x25B14B0;
		Offsets.ReplayInterface = 0x1FBD4F0;
		Offsets.ViewPort = 0x201DBA0;
		Offsets.Camera = 0x201E7D0;
		Offsets.BlipList = 0x2002FA0;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.WeaponManager = 0x10B8;
		Offsets.BoneList = 0x410;
		Offsets.Silent = 0x101A65D;
		Offsets.Vehicle = 0x0D10;
		Offsets.VisibleFlag = 0x145C;
		Offsets.Waypoint = 0x2023400;
		Offsets.m_Invisible = 0x6A46EE;
		Offsets.CurrentBuild = 3258;
	}

	if (Game.Version == "3095") {
		Offsets.GameWorld = 0x2593320;
		Offsets.ReplayInterface = 0x1F58B58;
		Offsets.ViewPort = 0x20019E0;
		Offsets.Camera = 0x20025B8;
		Offsets.BlipList = 0x2002888;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.WeaponManager = 0x10B8;
		Offsets.BoneList = 0x410;
		Offsets.Silent = 0x100F5A4;
		Offsets.Vehicle = 0x0D10;
		Offsets.VisibleFlag = 0x145C;
		Offsets.Waypoint = 0x2002FA0;
		Offsets.m_Invisible = 0x6A46EE;
		Offsets.CurrentBuild = 3095;
	}

	if (Game.Version == "2944") {
		Offsets.GameWorld = 0x257BEA0;
		Offsets.ReplayInterface = 0x1F42068;
		Offsets.ViewPort = 0x1FEAAC0;
		Offsets.Camera = 0x1FEB968;
		Offsets.BlipList = 0x1FEB968;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.WeaponManager = 0x10B8;
		Offsets.BoneList = 0x410;
		Offsets.Silent = 0x1003F80;
		Offsets.VisibleFlag = 0x145C;
		Offsets.Waypoint = 0x1FF3130;
		Offsets.m_Invisible = 0x6A46EE;
		Offsets.CurrentBuild = 2944;
		Offsets.Handling = 0x918;
	}

	if (Game.Version == "2802") {
		Offsets.GameWorld = 0x254D448;
		Offsets.ReplayInterface = 0x1F5B820;
		Offsets.ViewPort = 0x1FBC100;
		Offsets.Camera = 0x1FBCCD8;
		Offsets.BlipList = 0x1FBCFA8;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10A8;
		Offsets.Id = 0xE8;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x284;
		Offsets.Armor = 0x150C;
		Offsets.WeaponManager = 0x10B8;
		Offsets.BoneList = 0x410;
		Offsets.Silent = 0xFF716C;
		Offsets.VisibleFlag = 0x145C;
		Offsets.Waypoint = 0x1FBD6E0;
		Offsets.m_Invisible = 0x6A46EE;
		Offsets.CurrentBuild = 2802;
	}

	if (Game.Version == "2699") {
		Offsets.GameWorld = 0x26684D8;
		Offsets.ReplayInterface = 0x20304C8;
		Offsets.ViewPort = 0x20D8C90;
		Offsets.Camera = 0x20D9868;
		Offsets.BlipList = 0x20D9B38;
		Offsets.LocalPlayer = 0x8;
		Offsets.PlayerInfo = 0x10C8;
		Offsets.Id = 0x88;
		Offsets.Health = 0x280;
		Offsets.MaxHealth = 0x2A0;
		Offsets.Armor = 0x1530;
		Offsets.WeaponManager = 0x10D8;
		Offsets.BoneList = 0x430;
		Offsets.Silent = 0xFF9D90;
		Offsets.VisibleFlag = 0x147C;
		Offsets.Waypoint = 0x20E1420;
		Offsets.m_Invisible = 0x6A46EE;
		Offsets.CurrentBuild = 2699;
	}

	if (Offsets.CurrentBuild == 3751) {
		Offsets.bulletPos = 0x0F41F3FEFD2E83E9;
		Offsets.bulletHandler = 0x102B560;
	}
	else if (Offsets.CurrentBuild == 2699) {
		Offsets.bulletPos = 0xF41F3FF006643E9;
		Offsets.bulletHandler = 0xFF9D90;
	}
	else if (Offsets.CurrentBuild == 2802) {
		Offsets.bulletPos = 0x0F41F3FF009267E9;
		Offsets.bulletHandler = 0x0FF716C;
	}
	else if (Offsets.CurrentBuild == 2944) {
		Offsets.bulletPos = 0x0F41F3FEFFC453E9;
		Offsets.bulletHandler = 0x1003F80;
	}
	else if (Offsets.CurrentBuild == 3095) {
		Offsets.bulletPos = 0x0F41F3FEFF0E2FE9;
		Offsets.bulletHandler = 0x100F5A4;
	}
	else if (Offsets.CurrentBuild == 3258) {
		Offsets.bulletPos = 0x0F41F3FEFE5D73E9;
		Offsets.bulletHandler = 0x101A660;
	}
	else if (Offsets.CurrentBuild == 3323) {
		Offsets.bulletPos = 0x0F41F3FEFD9723E9;
		Offsets.bulletHandler = 0x1026CB0;
	}
	else if (Offsets.CurrentBuild == 3407) {
		Offsets.bulletPos = 0x0F41F3FEFD0447E9;
		Offsets.bulletHandler = 0x102FF8C;
	}
	else if (Offsets.CurrentBuild == 3570) {
		Offsets.bulletPos = 0x0F41F3FEFD2E83E9;
		Offsets.bulletHandler = 0x102D550;
	}
	else {
		Offsets.bulletPos = 0x0F41F3FEFD0447E9;
		Offsets.bulletHandler = 0x102FF8C;
	}
}

inline std::string GetGamePath() {
	CHAR processPath[MAX_PATH] = { 0 };
	DWORD size = MAX_PATH;
	if (!QueryFullProcessImageName(Game.hProcess, 0, processPath, &size)) {
		return "";
	}

	std::string fullPath = std::string(processPath);
	std::string targetPath = "FiveM.app\\";
	size_t position = fullPath.find(targetPath);
	if (position != std::string::npos) {
		return fullPath.substr(0, position + targetPath.size());
	}
	return fullPath;
}