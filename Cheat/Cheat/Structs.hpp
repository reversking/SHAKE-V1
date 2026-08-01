#pragma once
#include "Brand.hpp"
REVERS_STAMP(Structs_hpp)
using namespace DirectX::SimpleMath;

extern struct GameStruct Game;
extern struct OffsetsStruct Offsets;

struct Player {
	int Id;
	std::string Username;
};

std::vector<Player> Players;

struct Target {
	DWORD pID;
	HWND hWnd;
	HANDLE hProcess;
	LPCSTR lpClassName = "grcWindow";
	LPCSTR lpWindowName;
	RECT lpRect;
	POINT lpPoint;
} target;

struct Overlay {
	HWND hWnd;
	WNDCLASSEX wndClassEx;
	LPCSTR lpClassName = "Spotify";
	LPCSTR lpWindowName = "Spotify";
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;
} overlay;

void GetOffsets() {
	while (true) {
		Game.Version = XorString("3751");
		{
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
			uintptr_t GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			uintptr_t GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			uintptr_t GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}
		Game.Version = XorString("3570");
		{
			Offsets.GameWorld = 0x25D7108;
			Offsets.ReplayInterface = 0x1F9A9D8;
			Offsets.ViewPort = 0x20431C0;
			Offsets.Camera = 0x2059778;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.Silent = 0x102D550;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x20440C8;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2047D50;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;
			Offsets.CurrentBuild = 3570;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}
		Game.Version = XorString("3407");
		{
			Offsets.GameWorld = 0x25D7108;
			Offsets.ReplayInterface = 0x1F9A9D8;
			Offsets.ViewPort = 0x20431C0;
			Offsets.Camera = 0x2043DF8;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.Silent = 0x102FF89;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x20440C8;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2047D50;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;
			Offsets.CurrentBuild = 3407;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}

		Game.Version = XorString("3323");
		{
			Offsets.GameWorld = 0x25C15B0;
			Offsets.ReplayInterface = 0x1F85458;
			Offsets.ViewPort = 0x202DC50;
			Offsets.Camera = 0x202E878;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.Silent = 0x1026CAD;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x2002888;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2022DE0;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}

		Game.Version = XorString("3258");
		{
			Offsets.GameWorld = 0x25B14B0;
			Offsets.ReplayInterface = 0x1FBD4F0;
			Offsets.ViewPort = 0x201DBA0;
			Offsets.Camera = 0x201E7D0;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.Silent = 0x101A65D;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x2002FA0;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2023400;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;
			Offsets.CurrentBuild = 3258;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}

		Game.Version = XorString("3095");
		{
			Offsets.GameWorld = 0x2593320;
			Offsets.ReplayInterface = 0x1F58B58;
			Offsets.ViewPort = 0x20019E0;
			Offsets.Camera = 0x20025B8;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.Silent = 0x100F5A4;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x2002888;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x2002FA0;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}

		Game.Version = XorString("2944");
		{
			Offsets.GameWorld = 0x257BEA0;
			Offsets.ReplayInterface = 0x1F42068;
			Offsets.ViewPort = 0x1FEAAC0;
			Offsets.Camera = 0x1FEB968;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.Silent = 0x1003F80;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x1FEB968;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x1FF3130;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}

		Game.Version = XorString("2802");
		{
			Offsets.GameWorld = 0x254D448;
			Offsets.ReplayInterface = 0x1F5B820;
			Offsets.ViewPort = 0x1FBC100;
			Offsets.Camera = 0x1FBCCD8;
			Offsets.BoneList = 0x410;
			Offsets.WeaponManager = 0x10B8;
			Offsets.Silent = 0xFF716C;
			Offsets.VisibleFlag = 0x145C;
			Offsets.BlipList = 0x1FBCFA8;
			Offsets.Vehicle = 0x0D10;
			Offsets.Waypoint = 0x1FBD6E0;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10A8;
			Offsets.Id = 0xE8;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x284;
			Offsets.Armor = 0x150C;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}

		Game.Version = XorString("2699");
		{
			Offsets.GameWorld = 0x26684D8;
			Offsets.ReplayInterface = 0x20304C8;
			Offsets.ViewPort = 0x20D8C90;
			Offsets.Camera = 0x20D9868;
			Offsets.BoneList = 0x430;
			Offsets.WeaponManager = 0x10D8;
			Offsets.Silent = 0xFF9D90;
			Offsets.VisibleFlag = 0x147C;
			Offsets.BlipList = 0x20D9B38;
			Offsets.Vehicle = 0xD30;
			Offsets.Waypoint = 0x20E1420;
			Offsets.LocalPlayer = 0x8;
			Offsets.PlayerInfo = 0x10C8;
			Offsets.Id = 0x88;
			Offsets.Health = 0x280;
			Offsets.MaxHealth = 0x2A0;
			Offsets.Armor = 0x1530;

			auto GameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			auto GameViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			auto GameReplayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
			if (GameWorld != 0 && GameViewPort != 0 && GameReplayInterface != 0) {
				break;
			}
		}
	}
}