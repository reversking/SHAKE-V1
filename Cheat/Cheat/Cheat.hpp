#pragma once
#include "Brand.hpp"
REVERS_STAMP(Cheat_hpp)

#include "../Overlay/OverlayDraw.hpp"
#include "../Overlay/Overlay.hpp"
#include "GameSDK.hpp"
#include "Ped.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <map>
#include <unordered_set>
#include <windows.h>
#include <atomic>
#include <mutex>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "../Gui/menu/defs/textures.h"

#include "Globals.hpp"
#include "Sounds.hpp"
#include "Particles.hpp"
#include "Print.hpp"
#include "EntityList.hpp"
#include "Features.hpp"
#include <array>

using namespace hitlog;
using namespace features;
using namespace entity_list;

extern OverlayStruct Overlay;

class Vehicle {
public:
	uintptr_t pointer;

	Vehicle(uintptr_t vehiclePtr) : pointer(vehiclePtr) {}

	void DoorState(bool unlock) {
		if (!pointer) return;

		uint32_t lockValue = unlock ? 1 : 2;
		WriteMemory<uint32_t>(pointer + Offsets.VehicleDoorsLockState, lockValue);

		std::string action = unlock ? "unlocked" : "locked";
		printa<hit>("Vehicle " + action + " successfully!");
	}

	bool IsLocked() {
		if (!pointer) return false;
		uint32_t lockStatus = ReadMemory<uint32_t>(pointer + Offsets.VehicleDoorsLockState);
		return (lockStatus == 2);
	}

	uint64_t get_handling() {
		if (!pointer) return 0;
		return ReadMemory<uintptr_t>(pointer + Offsets.Handling);
	}

	void set_acceleration(float acceleration) {
		uint64_t handling = get_handling();
		if (!handling) return;
		WriteMemory<float>(handling + 0x90, acceleration);
		WriteMemory<float>(handling + 0x4C, acceleration);
		WriteMemory<float>(handling + 0x6C, acceleration);
		WriteMemory<float>(handling + 0x14, acceleration);
	}
};

class Utils {
public:
	static bool KeyPressedWithDelay(int key, int delay) {
		static std::map<int, DWORD> lastPressTime;

		if (GetAsyncKeyState(key) & 0x8000) {
			DWORD currentTime = GetTickCount();
			if (lastPressTime.find(key) == lastPressTime.end() ||
				(currentTime - lastPressTime[key]) >= delay) {
				lastPressTime[key] = currentTime;
				return true;
			}
		}
		return false;
	}
};

Ped FindBestTarget(int aimFov, bool isAimbot = false, bool isSilent = false, bool isTrigger = false);

Ped localPlayer;
uintptr_t gameWorld;
uintptr_t viewPort;
std::vector<Ped> pedList;
std::mutex pedListMutex;
bool TPVehicle = false;
uintptr_t TPModelInfo = NULL;
uintptr_t TPNavigation = NULL;
Vector3 TPPosition = Vector3(0, 0, 0);

uintptr_t currentTargetPointer = 0;

struct ClosestBoneData {
	Vector3 position;
	uintptr_t targetPointer;
	std::mutex mutex;
};
ClosestBoneData globalClosestBone;

bool hasGlobalTarget = false;
Vector3 globalTargetBone = Vector3();

float currentDynamicFov = 0.0f;

bool semiGodModeActive = false;

bool infiniteAmmoApplied = false;
bool noRecoilApplied = false;
bool noSpreadApplied = false;
bool noReloadApplied = false;
bool noRangeApplied = false;
bool damageBoostApplied = false;

float oldAmmoValue = 0.0f;
float oldRecoilValue = 0.0f;
float oldSpreadValue = 0.0f;
float oldReloadValue = 0.0f;
float oldRangeValue = 0.0f;
uint32_t oldDamageValue = 1;
uint32_t oldDamageValue2 = 1;
float oldDamageValue3 = 1.0f;
uintptr_t lastWeaponInfo = 0;

float GetCrosshairDistance(Vector2 screenPos) {
	float x = (Game.lpRect.right / 2.f) - screenPos.x;
	float y = (Game.lpRect.bottom / 2.f) - screenPos.y;
	return sqrt((x * x) + (y * y));
}

bool IsFriend(int pedId) {
	return friendStatus.count(pedId) && friendStatus[pedId];
}

bool ShouldFire(int missChanceValue) {
	int olasilik = (10 - missChanceValue) * 100 / 9;
	int sans = rand() % 100 + 1;
	return sans <= olasilik;
}

struct PedBarFix {
	int id;
	float health;
	float armor;
};

std::map<int, PedBarFix> pedBarFix;

std::map<uintptr_t, bool> wasDeadMap;

static float savedPosX = 0.0f;
static float savedPosY = 0.0f;
static float savedPosZ = 0.0f;
static bool peekAssistActive = false;
static bool hasValidPosition = false;

Vector3 GetPositionByID(int targetID) {
	std::lock_guard<std::mutex> lock(pedListMutex);
	for (auto& ped : pedList) {
		if (!ped.update()) continue;
		if (ped.cachedId == targetID) {
			return ped.position;
		}
	}
	return Vector3(0, 0, 0);
}
int loopDelay = 10;

extern std::atomic<bool> isMenuVisible;

namespace Cheats {
	namespace Misc {
		extern bool christmasEnabled;
		extern bool showInfoBox;
	}
}

namespace AimWarning {
	struct AimerInfo {
		std::string name;
		int playerId;
		float distance;
		float lastAimTime;
		bool isAiming;
	};

	std::vector<AimerInfo> aimers;
	float updateTimer = 0.0f;
	const float UPDATE_INTERVAL = 0.05f;

	void UpdateAimWarning() {
		if (!localPlayer.pointer || !globals::toggles::aim_warning) {
			aimers.clear();
			return;
		}

		updateTimer += ImGui::GetIO().DeltaTime;
		if (updateTimer < UPDATE_INTERVAL) {
			return;
		}
		updateTimer = 0.0f;

		float currentTime = ImGui::GetTime();

		aimers.erase(
			std::remove_if(aimers.begin(), aimers.end(),
				[currentTime](const AimerInfo& aimer) {
					return (currentTime - aimer.lastAimTime) > 0.5f;
				}),
			aimers.end()
		);

		int localPlayerIndex = 0;
		for (const auto& p : entity_list::ped_list) {
			if (p.origin == game::local_position) {
				localPlayerIndex = p.index;
				break;
			}
		}

		for (const auto& ped : entity_list::ped_list) {
			if (ped.index == localPlayerIndex || ped.index == 0) continue;
			if (ped.player_name.empty() || ped.player_name == "NPC") continue;
			if (ped.health <= 0.0f) continue;

			float distance = getDistance(ped.origin, game::local_position);
			if (distance > 500.0f) continue;

			bool isAimingOrLooking = false;

			Vector3 theirHead = ped.v_bones[0];
			Vector3 theirRightHand = ped.v_bones[6];
			Vector3 theirLeftHand = ped.v_bones[5];
			Vector3 theirNeck = ped.v_bones[7];

			if (theirHead.x == 0 && theirHead.y == 0 && theirHead.z == 0) {
				continue;
			}

			bool isAiming = false;

			if ((theirRightHand.x != 0 || theirRightHand.y != 0 || theirRightHand.z != 0) &&
				(theirLeftHand.x != 0 || theirLeftHand.y != 0 || theirLeftHand.z != 0)) {

				float rightHandHeight = theirRightHand.z - ped.origin.z;
				float leftHandHeight = theirLeftHand.z - ped.origin.z;

				bool handsRaised = (rightHandHeight > 0.3f && leftHandHeight > 0.3f);

				if (handsRaised) {

					Vector3 toUs = game::local_position - ped.origin;
					float distToUs = sqrt(toUs.x * toUs.x + toUs.y * toUs.y + toUs.z * toUs.z);

					if (distToUs > 0.1f && distToUs < 200.0f) {
						toUs.x /= distToUs;
						toUs.y /= distToUs;
						toUs.z /= distToUs;

						Vector3 handMidpoint;
						handMidpoint.x = (theirRightHand.x + theirLeftHand.x) / 2.0f;
						handMidpoint.y = (theirRightHand.y + theirLeftHand.y) / 2.0f;
						handMidpoint.z = (theirRightHand.z + theirLeftHand.z) / 2.0f;

						Vector3 aimDir = handMidpoint - ped.origin;
						aimDir.z = 0;
						float aimDirLength = sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);

						if (aimDirLength > 0.1f) {
							aimDir.x /= aimDirLength;
							aimDir.y /= aimDirLength;

							float dotProduct = aimDir.x * toUs.x + aimDir.y * toUs.y;

							if (dotProduct > 0.7f) {
								isAiming = true;
							}
						}
					}
				}
			}

			bool isLooking = false;
			if (!isAiming) {

				Vector3 lookDirection = theirHead - ped.origin;
				lookDirection.z = 0;
				float lookLength = sqrt(lookDirection.x * lookDirection.x + lookDirection.y * lookDirection.y);

				if (lookLength > 0.1f) {
					lookDirection.x /= lookLength;
					lookDirection.y /= lookLength;

					Vector3 toUsFlat = game::local_position - ped.origin;
					toUsFlat.z = 0;
					float toUsLength = sqrt(toUsFlat.x * toUsFlat.x + toUsFlat.y * toUsFlat.y);

					if (toUsLength > 0.1f) {
						toUsFlat.x /= toUsLength;
						toUsFlat.y /= toUsLength;

						float lookDot = lookDirection.x * toUsFlat.x + lookDirection.y * toUsFlat.y;
						if (lookDot > 0.85f) {
							isLooking = true;
						}
					}
				}
			}

			isAimingOrLooking = isAiming || isLooking;

			if (isAimingOrLooking) {
				bool found = false;
				for (auto& aimer : aimers) {
					if (aimer.playerId == ped.index) {
						aimer.lastAimTime = currentTime;
						aimer.distance = distance;
						aimer.isAiming = isAiming;
						found = true;
						break;
					}
				}

				if (!found) {
					aimers.push_back({ ped.player_name, ped.index, distance, currentTime, isAiming });
				}
			}
		}
	}

	void DrawAimWarning() {
		if (aimers.empty()) return;

		std::vector<AimerInfo> filteredAimers;
		for (const auto& aimer : aimers) {
			if ((globals::toggles::aim_warning_show_aimers && aimer.isAiming) ||
				(globals::toggles::aim_warning_show_lookers && !aimer.isAiming)) {
				filteredAimers.push_back(aimer);
			}
		}

		if (filteredAimers.empty()) return;

		static bool firstTime = true;
		static ImVec2 savedPos;

		if (firstTime) {
			savedPos = ImVec2((Game.lpRect.right - 350.0f) / 2.0f, 100.0f);
			firstTime = false;
		}

		ImDrawList* drawList = ImGui::GetForegroundDrawList();

		ImVec2 windowSize(350, 120);
		ImVec2 windowPos = savedPos;

		static bool isDragging = false;
		ImVec2 mousePos = ImGui::GetMousePos();

		if (!globals::toggles::aim_warning_locked) {
			if (ImGui::IsMouseClicked(0)) {
				if (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
					mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + 35) {
					isDragging = true;
				}
			}

			if (ImGui::IsMouseReleased(0)) {
				isDragging = false;
			}

			if (isDragging && ImGui::IsMouseDragging(0)) {
				savedPos.x += ImGui::GetIO().MouseDelta.x;
				savedPos.y += ImGui::GetIO().MouseDelta.y;
				windowPos = savedPos;
			}
		}

		drawList->AddRectFilled(windowPos, windowPos + windowSize,
			IM_COL32(25, 25, 30, 240), 5.0f);

		drawList->AddRect(windowPos, windowPos + windowSize,
			IM_COL32(60, 60, 65, 255), 5.0f, 0, 1.0f);

		ImVec2 headerPos = windowPos;
		ImVec2 headerSize(windowSize.x, 35);

		std::string headerText = "Aim Warning System";
		ImVec2 headerTextSize = ImGui::CalcTextSize(headerText.c_str());
		drawList->AddText(
			ImVec2(headerPos.x + 15, headerPos.y + 10),
			IM_COL32(255, 128, 0, 255),
			headerText.c_str()
		);
		ImGui::PopFont();

		drawList->AddLine(
			ImVec2(windowPos.x + 10, windowPos.y + 35),
			ImVec2(windowPos.x + windowSize.x - 10, windowPos.y + 35),
			IM_COL32(60, 60, 65, 255), 1.0f
		);

		ImVec2 buttonSize(150, 25);
		ImVec2 button1Pos(windowPos.x + 15, windowPos.y + 45);
		ImVec2 button2Pos(windowPos.x + windowSize.x - buttonSize.x - 15, windowPos.y + 45);

		bool button1Hovered = mousePos.x >= button1Pos.x && mousePos.x <= button1Pos.x + buttonSize.x &&
			mousePos.y >= button1Pos.y && mousePos.y <= button1Pos.y + buttonSize.y;

		ImU32 button1Color = globals::toggles::aim_warning_show_aimers ?
			IM_COL32(255, 128, 0, 255) :
			(button1Hovered ? IM_COL32(50, 50, 55, 255) : IM_COL32(35, 35, 40, 255));

		drawList->AddRectFilled(button1Pos, button1Pos + buttonSize, button1Color, 3.0f);
		drawList->AddRect(button1Pos, button1Pos + buttonSize, IM_COL32(60, 60, 65, 255), 3.0f);

		std::string button1Text = "Show Aimers";
		ImVec2 text1Size = ImGui::CalcTextSize(button1Text.c_str());
		drawList->AddText(
			ImVec2(button1Pos.x + (buttonSize.x - text1Size.x) / 2, button1Pos.y + (buttonSize.y - text1Size.y) / 2),
			IM_COL32(255, 255, 255, 255),
			button1Text.c_str()
		);

		bool button2Hovered = mousePos.x >= button2Pos.x && mousePos.x <= button2Pos.x + buttonSize.x &&
			mousePos.y >= button2Pos.y && mousePos.y <= button2Pos.y + buttonSize.y;

		ImU32 button2Color = globals::toggles::aim_warning_show_lookers ?
			IM_COL32(255, 128, 0, 255) :
			(button2Hovered ? IM_COL32(50, 50, 55, 255) : IM_COL32(35, 35, 40, 255));

		drawList->AddRectFilled(button2Pos, button2Pos + buttonSize, button2Color, 3.0f);
		drawList->AddRect(button2Pos, button2Pos + buttonSize, IM_COL32(60, 60, 65, 255), 3.0f);

		std::string button2Text = "Show Lookers";
		ImVec2 text2Size = ImGui::CalcTextSize(button2Text.c_str());
		drawList->AddText(
			ImVec2(button2Pos.x + (buttonSize.x - text2Size.x) / 2, button2Pos.y + (buttonSize.y - text2Size.y) / 2),
			IM_COL32(255, 255, 255, 255),
			button2Text.c_str()
		);
		ImGui::PopFont();

		if (ImGui::IsMouseClicked(0)) {
			if (button1Hovered) {
				globals::toggles::aim_warning_show_aimers = !globals::toggles::aim_warning_show_aimers;
			}
			if (button2Hovered) {
				globals::toggles::aim_warning_show_lookers = !globals::toggles::aim_warning_show_lookers;
			}
		}

		std::string countText;
		int aimerCount = 0;
		int lookerCount = 0;

		for (const auto& aimer : filteredAimers) {
			if (aimer.isAiming) aimerCount++;
			else lookerCount++;
		}

		if (globals::toggles::aim_warning_show_aimers && globals::toggles::aim_warning_show_lookers) {
			countText = std::to_string(aimerCount) + " aiming, " + std::to_string(lookerCount) + " looking";
		}
		else if (globals::toggles::aim_warning_show_aimers) {
			countText = std::to_string(aimerCount) + (aimerCount == 1 ? " player is aiming at you" : " players are aiming at you");
		}
		else if (globals::toggles::aim_warning_show_lookers) {
			countText = std::to_string(lookerCount) + (lookerCount == 1 ? " player is looking at you" : " players are looking at you");
		}

		ImVec2 countTextSize = ImGui::CalcTextSize(countText.c_str());
		drawList->AddText(
			ImVec2(windowPos.x + (windowSize.x - countTextSize.x) / 2, windowPos.y + 80),
			IM_COL32(200, 200, 200, 255),
			countText.c_str()
		);
		ImGui::PopFont();

		if (!globals::toggles::aim_warning_locked) {

			drawList->AddText(
				ImVec2(windowPos.x + windowSize.x - 80, windowPos.y + 5),
				IM_COL32(150, 150, 150, 200),
				"[DRAG]"
			);

		}
	}
}

void DrawFriendList() {
	if (friendStatus.empty()) return;

	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	std::vector<std::string> friendNames;
	for (auto const& [id, isFriend] : friendStatus) {
		if (isFriend) {
			if (playerNames.count(id)) {
				if (!playerNames[id].empty() && playerNames[id] != "Unknown") {
					friendNames.push_back(playerNames[id]);
				}
			}
		}
	}

	if (friendNames.empty()) return;


	ImVec2 startPos = ImVec2(20, 20);
	float padding = 10.f;
	float itemHeight = 22.f;
	float iconSize = 8.f;
	float headerHeight = 28.f;
	float maxWidth = 180.f;


	ImFont* font = ImGui::GetIO().Fonts->Fonts[0];
	float fontSize = 13.f;
	for (const auto& name : friendNames) {
		ImVec2 sz = font->CalcTextSizeA(fontSize, FLT_MAX, 0.f, name.c_str());
		float needed = padding + iconSize + 8.f + sz.x + padding;
		if (needed > maxWidth) maxWidth = needed;
	}

	float totalHeight = headerHeight + (friendNames.size() * itemHeight) + padding;


	ImVec2 boxEnd = ImVec2(startPos.x + maxWidth, startPos.y + totalHeight);
	drawList->AddRectFilled(startPos, boxEnd, IM_COL32(12, 12, 16, 210), 8.f);


	drawList->AddRect(startPos, boxEnd, IM_COL32(255, 255, 255, 18), 8.f, 0, 1.f);


	ImVec2 headerEnd = ImVec2(startPos.x + maxWidth, startPos.y + headerHeight);
	drawList->AddRectFilled(startPos, headerEnd, IM_COL32(255, 255, 255, 6), 8.f, ImDrawFlags_RoundCornersTop);


	drawList->AddRectFilled(
		ImVec2(startPos.x + padding, startPos.y + headerHeight - 1.f),
		ImVec2(startPos.x + maxWidth - padding, startPos.y + headerHeight),
		IM_COL32(0, 200, 80, 80));


	float hcx = startPos.x + padding + 5.f;
	float hcy = startPos.y + headerHeight * 0.5f;
	drawList->AddCircleFilled(ImVec2(hcx, hcy - 3.f), 3.5f, IM_COL32(0, 220, 90, 255));
	drawList->AddRectFilled(ImVec2(hcx - 4.f, hcy + 1.f), ImVec2(hcx + 4.f, hcy + 5.f), IM_COL32(0, 220, 90, 255), 2.f);


	drawList->AddText(font, 13.f, ImVec2(startPos.x + padding + 16.f, startPos.y + (headerHeight - 13.f) * 0.5f),
		IM_COL32(0, 220, 90, 255), "Friends");


	std::string countStr = std::to_string(friendNames.size());
	ImVec2 countSz = font->CalcTextSizeA(11.f, FLT_MAX, 0.f, countStr.c_str());
	float badgeX = startPos.x + maxWidth - padding - countSz.x - 8.f;
	float badgeY = startPos.y + (headerHeight - 16.f) * 0.5f;
	drawList->AddRectFilled(ImVec2(badgeX, badgeY), ImVec2(badgeX + countSz.x + 8.f, badgeY + 16.f), IM_COL32(0, 200, 80, 40), 8.f);
	drawList->AddText(font, 11.f, ImVec2(badgeX + 4.f, badgeY + 2.f), IM_COL32(0, 220, 90, 220), countStr.c_str());


	float yOffset = startPos.y + headerHeight + 4.f;
	for (size_t i = 0; i < friendNames.size(); i++) {
		float itemY = yOffset + i * itemHeight;


		if (i % 2 == 0) {
			drawList->AddRectFilled(
				ImVec2(startPos.x + 2.f, itemY),
				ImVec2(startPos.x + maxWidth - 2.f, itemY + itemHeight),
				IM_COL32(255, 255, 255, 4), 4.f);
		}


		float dotY = itemY + itemHeight * 0.5f;
		drawList->AddCircleFilled(ImVec2(startPos.x + padding + 4.f, dotY), 3.f, IM_COL32(0, 220, 90, 255));

		drawList->AddCircleFilled(ImVec2(startPos.x + padding + 4.f, dotY), 5.f, IM_COL32(0, 220, 90, 30));


		float textY = itemY + (itemHeight - fontSize) * 0.5f;

		drawList->AddText(font, fontSize, ImVec2(startPos.x + padding + 14.f, textY + 1.f), IM_COL32(0, 0, 0, 120), friendNames[i].c_str());

		drawList->AddText(font, fontSize, ImVec2(startPos.x + padding + 14.f, textY), IM_COL32(235, 235, 240, 255), friendNames[i].c_str());
	}
}

void drawInfoBox() {
	if (!Cheats::Misc::showInfoBox) return;

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	const std::string brand = globals::brand::signature;

	auto now = std::chrono::system_clock::now();
	auto time_t_now = std::chrono::system_clock::to_time_t(now);
	std::tm local_time;
	localtime_s(&local_time, &time_t_now);
	char time_buffer[16];
	strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &local_time);
	std::string current_time = time_buffer;

	ImFont* font = (f::sbold16 != nullptr) ? f::sbold16 : ImGui::GetFont();
	const float fontSz = 15.0f;
	const float logoSize = 22.0f;

	ImVec2 brandSize = font->CalcTextSizeA(fontSz, FLT_MAX, 0.0f, brand.c_str());
	ImVec2 timeSize = font->CalcTextSizeA(fontSz, FLT_MAX, 0.0f, current_time.c_str());

	const float padX = 14.0f;
	const float padY = 9.0f;
	const float gap = 11.0f;
	const float logoGap = 12.0f;

	const float contentH = (std::max)(logoSize, (std::max)(brandSize.y, timeSize.y));
	const float boxHeight = contentH + padY * 2.0f;
	const float boxWidth = padX
		+ logoSize + logoGap
		+ brandSize.x + gap
		+ timeSize.x
		+ padX;

	ImVec2 screenSize = io.DisplaySize;
	ImVec2 boxPos = ImVec2(screenSize.x - boxWidth - 18.0f, 16.0f);
	ImVec2 boxMax = ImVec2(boxPos.x + boxWidth, boxPos.y + boxHeight);
	const float rounding = 8.0f;


	drawList->AddRectFilled(boxPos, boxMax, IM_COL32(0, 0, 0, 200), rounding);
	drawList->AddRectFilled(boxPos, boxMax, IM_COL32(255, 255, 255, 10), rounding);
	drawList->AddRect(boxPos, boxMax, IM_COL32(255, 255, 255, 40), rounding, 0, 1.0f);


	if (tex::transparent_logo != nullptr) {
		float accentX = boxPos.x + padX;
		float accentY = boxMax.y - 3.0f;
		drawList->AddRectFilled(
			ImVec2(accentX, accentY),
			ImVec2(accentX + logoSize, accentY + 2.0f),
			IM_COL32(255, 90, 170, 220), 1.0f);
	}

	float x = boxPos.x + padX;
	float cy = boxPos.y + boxHeight * 0.5f;

	auto drawBrightText = [&](ImFont* fnt, float sz, ImVec2 pos, const char* text, ImU32 col) {
		drawList->AddText(fnt, sz, pos + ImVec2(1.0f, 1.0f), IM_COL32(0, 0, 0, 180), text);
		drawList->AddText(fnt, sz, pos, col, text);
	};

	auto drawDot = [&](float sx) {
		drawList->AddCircleFilled(ImVec2(sx, cy), 2.0f, IM_COL32(255, 255, 255, 70), 8);
	};

	if (tex::transparent_logo != nullptr) {
		ImVec2 logoMin = ImVec2(x, cy - logoSize * 0.5f);
		ImVec2 logoMax = ImVec2(logoMin.x + logoSize, logoMin.y + logoSize);
		drawList->AddImage((ImTextureID)tex::transparent_logo, logoMin, logoMax,
			ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
		x += logoSize + logoGap;
	}

	const ImU32 white = IM_COL32(255, 255, 255, 255);

	drawBrightText(font, fontSz, ImVec2(x, cy - brandSize.y * 0.5f), brand.c_str(), white);
	x += brandSize.x + gap * 0.55f;
	drawDot(x);
	x += gap * 0.55f;

	drawBrightText(font, fontSz, ImVec2(x, cy - timeSize.y * 0.5f), current_time.c_str(), white);
}

void drawSemiGodModeIndicator() {

}

void drawActiveFeatures() {
	if (!Cheats::Misc::showActiveFeatures) return;

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	std::vector<std::string> features;
	if (Cheats::Misc::semiGodMode && semiGodModeActive) features.push_back("Semi God");
	if (Cheats::Aim::silent) features.push_back("Silent Aim");
	if (Cheats::Aim::aimBot) features.push_back("Aimbot");
	if (Cheats::Aim::triggerBot) features.push_back("Triggerbot");
	if (Cheats::Misc::noRecoil) features.push_back("No Recoil");
	if (Cheats::Misc::noSpread) features.push_back("No Spread");
	if (Cheats::Misc::infiniteAmmo) features.push_back("Inf. Ammo");
	if (Cheats::Misc::noReload) features.push_back("No Reload");
	if (Cheats::Misc::noRange) features.push_back("No Range");
	if (Cheats::Misc::damageBoost) features.push_back("Dmg Boost");
	if (Cheats::Misc::godMode) features.push_back("God Mode");
	if (Cheats::Misc::invisibleEnabled) features.push_back("Invisible");
	if (Cheats::Esp::skeletonEsp) features.push_back("Skeleton ESP");
	if (Cheats::Esp::nameEsp) features.push_back("Name ESP");
	if (Cheats::Misc::streamProof) features.push_back("Stream Proof");

	if (features.empty()) return;

	ImFont* font = (f::sbold16 != nullptr) ? f::sbold16 : ImGui::GetFont();
	const float fontSz = 14.0f;
	const float headerSz = 14.0f;
	const float padding = 12.0f;
	const float lineHeight = 18.0f;
	const float rounding = 8.0f;
	const float headerGap = 8.0f;

	const std::string header = std::string(globals::brand::signature) + " · Active";
	ImVec2 headerSize = font->CalcTextSizeA(headerSz, FLT_MAX, 0.0f, header.c_str());

	float maxTextW = headerSize.x;
	for (auto& f : features) {
		float w = font->CalcTextSizeA(fontSz, FLT_MAX, 0.0f, f.c_str()).x;
		if (w > maxTextW) maxTextW = w;
	}

	float boxW = maxTextW + padding * 2.0f + 14.0f;
	float boxH = padding + headerSize.y + headerGap + (float)features.size() * lineHeight + padding;

	ImVec2 screenSize = io.DisplaySize;

	ImVec2 pos = ImVec2(screenSize.x - boxW - 18.0f, 68.0f);
	ImVec2 boxMax = ImVec2(pos.x + boxW, pos.y + boxH);


	drawList->AddRectFilled(pos, boxMax, IM_COL32(0, 0, 0, 200), rounding);
	drawList->AddRectFilled(pos, boxMax, IM_COL32(255, 255, 255, 10), rounding);
	drawList->AddRect(pos, boxMax, IM_COL32(255, 255, 255, 40), rounding, 0, 1.0f);


	float accentY = pos.y + padding + headerSize.y + 3.0f;
	drawList->AddRectFilled(
		ImVec2(pos.x + padding, accentY),
		ImVec2(pos.x + padding + headerSize.x, accentY + 2.0f),
		IM_COL32(255, 90, 170, 220), 1.0f);

	auto drawBrightText = [&](float sz, ImVec2 p, const char* text, ImU32 col) {
		drawList->AddText(font, sz, p + ImVec2(1.0f, 1.0f), IM_COL32(0, 0, 0, 180), text);
		drawList->AddText(font, sz, p, col, text);
	};


	drawBrightText(headerSz, ImVec2(pos.x + padding, pos.y + padding), header.c_str(), IM_COL32(255, 255, 255, 255));

	float fy = accentY + 8.0f;
	for (auto& f : features) {
		drawList->AddCircleFilled(ImVec2(pos.x + padding + 3.0f, fy + lineHeight * 0.5f),
			2.0f, IM_COL32(255, 255, 255, 70), 8);
		drawBrightText(fontSz, ImVec2(pos.x + padding + 12.0f, fy + (lineHeight - fontSz) * 0.5f),
			f.c_str(), IM_COL32(255, 255, 255, 255));
		fy += lineHeight;
	}
}

void DrawAlways() {
	DrawFriendList();
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	if (globals::toggles::hitlog_enabled) {
		static float lastParticleUpdate = 0.0f;
		float currentTime = ImGui::GetTime();
		float deltaTime = ImGui::GetIO().DeltaTime;

		if (currentTime - lastParticleUpdate > 0.033f) {
			particleManager.update(deltaTime);
			lastParticleUpdate = currentTime;
		}

		try {
			Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);
			particleManager.render(drawList, viewMatrix);

			RenderMessages(drawList, ImVec2(50, 50));

			for (auto it = features::damageIndicators.begin(); it != features::damageIndicators.end();) {
				it->lifetime -= deltaTime;

				if (it->lifetime <= 0.0f) {
					it = features::damageIndicators.erase(it);
					continue;
				}

				Vector2 screenPos;
				Vector3 damagePos = it->worldPosition;
				damagePos.z += (it->maxLifetime - it->lifetime) * 0.5f;

				if (worldToScreen(viewMatrix, damagePos, screenPos)) {
					float alpha = it->lifetime / it->maxLifetime;
					float scale = 1.0f + (1.0f - alpha) * 0.5f;

					std::string damageText = it->isKill ? ("KILL " + std::to_string(it->damage)) : std::to_string(it->damage);
					ImVec2 textSize = ImGui::CalcTextSize(damageText.c_str());
					textSize.x *= scale;
					textSize.y *= scale;

					ImVec2 textPos = ImVec2(screenPos.x - textSize.x / 2, screenPos.y - textSize.y / 2);

					ImU32 color = IM_COL32(
						(int)(it->color.x * 255),
						(int)(it->color.y * 255),
						(int)(it->color.z * 255),
						(int)(alpha * 255)
					);

					ImFont* font = ImGui::GetFont();
					float fontSize = font->FontSize * scale;

					drawList->AddText(font, fontSize, textPos + ImVec2(2, 2), IM_COL32(0, 0, 0, (int)(alpha * 200)), damageText.c_str());
					drawList->AddText(font, fontSize, textPos, color, damageText.c_str());
				}

				++it;
			}
		}
		catch (...) {
		}
	}

	if (Cheats::Aim::crossHair) {
		const ImVec2 center = { Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f };
		const ImColor crossHairColor = ConvertToImColor(Cheats::Aim::crossHairColor);
		const float size = Cheats::Aim::crossHairSize;

		switch (Cheats::Aim::crossHairSelectedType) {
		case 0:
		{
			const float gapSize = 4.0f;
			const float dotSize = 2.0f;
			drawList->AddLine({ center.x - size - gapSize, center.y }, { center.x - gapSize, center.y }, crossHairColor, 1.5f);
			drawList->AddLine({ center.x + gapSize, center.y }, { center.x + size + gapSize, center.y }, crossHairColor, 1.5f);
			drawList->AddLine({ center.x, center.y - size - gapSize }, { center.x, center.y - gapSize }, crossHairColor, 1.5f);
			drawList->AddLine({ center.x, center.y + gapSize }, { center.x, center.y + size + gapSize }, crossHairColor, 1.5f);
			drawList->AddCircleFilled(center, dotSize, ImColor(255, 255, 255));
		}
		break;

		case 1:
		{
			drawList->AddLine({ center.x, center.y - size }, { center.x, center.y + size }, crossHairColor, 1.5f);
			drawList->AddLine({ center.x - size, center.y }, { center.x + size, center.y }, crossHairColor, 1.5f);
		}
		break;

		case 2:
		{
			drawList->AddLine({ center.x - size, center.y - size }, { center.x + size, center.y + size }, crossHairColor, 1.5f);
			drawList->AddLine({ center.x + size, center.y - size }, { center.x - size, center.y + size }, crossHairColor, 1.5f);
		}
		break;

		case 3:
		{
			const float radius = size / 2.f;
			drawList->AddCircle(center, radius, crossHairColor, 50, 1.5f);
			drawList->AddCircleFilled(center, radius / 2.f, crossHairColor);
		}
		break;

		case 4:
		{
			const float radius = size;
			const int spokeCount = 8;
			for (int i = 0; i < spokeCount; ++i) {
				const float angle = (360.f / spokeCount) * i * (3.14159265359f / 180.0f);
				ImVec2 p1 = { center.x + radius * cos(angle), center.y + radius * sin(angle) };
				ImVec2 p2 = { center.x - radius * cos(angle), center.y - radius * sin(angle) };
				drawList->AddLine(p1, p2, crossHairColor, 1.5f);
			}
		}
		break;

		case 5:
		{
			const float diamondSize = size;
			const float halfSize = diamondSize * 0.5f;
			ImVec2 p1 = { center.x - halfSize, center.y };
			ImVec2 p2 = { center.x, center.y - halfSize };
			ImVec2 p3 = { center.x + halfSize, center.y };
			ImVec2 p4 = { center.x, center.y + halfSize };
			drawList->AddLine(p1, p2, crossHairColor, 1.5f);
			drawList->AddLine(p2, p3, crossHairColor, 1.5f);
			drawList->AddLine(p3, p4, crossHairColor, 1.5f);
			drawList->AddLine(p4, p1, crossHairColor, 1.5f);
		}
		break;

		case 6:
		{
			const float outerRadius = size;
			const float innerRadius = outerRadius * 0.6f;
			drawList->AddCircle(center, outerRadius, crossHairColor, 50, 2.0f);
			drawList->AddCircle(center, innerRadius, crossHairColor, 50, 1.5f);
			drawList->AddCircleFilled(center, innerRadius * 0.5f, crossHairColor);
		}
		break;

		case 7:
		{
			const float sawRadius = size;
			const int sawTeeth = 12;
			const float innerRadius = sawRadius * 0.7f;
			for (int i = 0; i < sawTeeth; ++i) {
				const float angle1 = (360 / sawTeeth * i) * (3.14159265359f / 180.0f);
				const float angle2 = (360 / sawTeeth * (i + 1)) * (3.14159265359f / 180.0f);
				ImVec2 p1 = { center.x + sawRadius * cos(angle1), center.y + sawRadius * sin(angle1) };
				ImVec2 p2 = { center.x + innerRadius * cos(angle2), center.y + innerRadius * sin(angle2) };
				drawList->AddLine(p1, p2, crossHairColor, 1.5f);
			}
		}
		break;

		case 8:
		{
			const float webRadius = size;
			const int webLines = 6;
			const int webRings = 3;
			for (int i = 0; i < webLines; ++i) {
				const float angle = (360 / webLines * i) * (3.14159265359f / 180.0f);
				ImVec2 end = { center.x + webRadius * cos(angle), center.y + webRadius * sin(angle) };
				drawList->AddLine(center, end, crossHairColor, 1.5f);
			}
			for (int r = 1; r <= webRings; ++r) {
				const float ringRadius = (webRadius / webRings) * r;
				drawList->AddCircle(center, ringRadius, crossHairColor, 50, 1.5f);
			}
		}
		break;

		case 9:
		{
			const float outerRadius = size;
			const float innerRadius = outerRadius * 0.8f;
			drawList->AddCircle(center, outerRadius, crossHairColor, 100, 3.0f);
			drawList->AddCircle(center, innerRadius, crossHairColor, 100, 1.5f);
			drawList->AddCircleFilled(center, innerRadius * 0.5f, ImColor(255, 255, 255, 150));
		}
		break;

		default:
			break;
		}
	}


	static float smoothAimbotFov = 0.f;
	static float smoothSilentFov = 0.f;
	static float smoothTriggerFov = 0.f;
	static float smoothDynamicFov = 0.f;
	float dt = ImGui::GetIO().DeltaTime;
	float lerpSpeed = 12.f;

	if (Cheats::Aim::drawAimbotFov) {
		float targetFov = (float)Cheats::Aim::aimBotFov;
		smoothAimbotFov += (targetFov - smoothAimbotFov) * (std::min)(1.f, lerpSpeed * dt);
		drawList->AddCircle(ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f), smoothAimbotFov, ConvertToImColor(Cheats::Aim::drawAimbotFovColor), 100, 1.0f);
	} else {
		smoothAimbotFov = 0.f;
	}
	if (Cheats::Aim::drawSilentFov) {
		float centerX = Game.lpRect.right / 2.f;
		float centerY = Game.lpRect.bottom / 2.f;
		ImColor color = ConvertToImColor(Cheats::Aim::drawSilentFovColor);
		float thickness = 1.0f;

		float targetFov;
		if (Cheats::Aim::dynamic_fov_on) {
			Ped bestped = FindBestTarget(Cheats::Aim::silentFov, false, true, false);
			if (bestped.pointer) {
				Vector3 localorigin = ReadMemory<Vector3>(localPlayer.pointer + 0x90);
				Vector3 origin = ReadMemory<Vector3>(bestped.pointer + 0x90);

				float dx = localorigin.x - origin.x;
				float dy = localorigin.y - origin.y;
				float dz = localorigin.z - origin.z;

				float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

				float fov_radius = std::clamp(500.0f / distance, 2.0f, static_cast<float>(Cheats::Aim::silentFov));

				currentDynamicFov = fov_radius;
				targetFov = fov_radius;
			}
			else {
				currentDynamicFov = 0.0f;
				targetFov = (float)Cheats::Aim::silentFov;
			}
			smoothDynamicFov += (targetFov - smoothDynamicFov) * (std::min)(1.f, lerpSpeed * dt);
			drawList->AddCircle(ImVec2(centerX, centerY), smoothDynamicFov, color, 100, thickness);
		}
		else {
			targetFov = (float)Cheats::Aim::silentFov;
			smoothSilentFov += (targetFov - smoothSilentFov) * (std::min)(1.f, lerpSpeed * dt);
			drawList->AddCircle(ImVec2(centerX, centerY), smoothSilentFov, color, 100, thickness);
		}
	} else {
		smoothSilentFov = 0.f;
		smoothDynamicFov = 0.f;
	}
	if (Cheats::Aim::drawTriggerFov) {
		float targetFov = (float)Cheats::Aim::triggerFov;
		smoothTriggerFov += (targetFov - smoothTriggerFov) * (std::min)(1.f, lerpSpeed * dt);
		drawList->AddCircle(ImVec2(Game.lpRect.right / 2.f, Game.lpRect.bottom / 2.f), smoothTriggerFov, ConvertToImColor(Cheats::Aim::drawTriggerFovColor), 100, 1.0f);
	} else {
		smoothTriggerFov = 0.f;
	}

	if (Cheats::Misc::peekAssist && peekAssistActive && hasValidPosition) {
		Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);
		Vector3 savedPos = Vector3(savedPosX, savedPosY, savedPosZ);
		Vector2 screenPos;

		if (worldToScreen(viewMatrix, savedPos, screenPos)) {
			ImVec2 center = ImVec2(screenPos.x, screenPos.y);
			float radius = Cheats::Misc::peekEffectSize * 0.3f;
			float dt = ImGui::GetIO().DeltaTime;

			ImColor effectColor = ConvertToImColor(Cheats::Misc::peekEffectColor);
			ImVec4 ecv = effectColor.Value;

			static float animTime = 0.0f;
			animTime += dt;
			float breathe = sinf(animTime * 2.0f) * 0.5f + 0.5f;


			for (int i = 3; i >= 1; i--) {
				float glowR = radius + (float)i * 4.0f + breathe * 3.0f;
				float glowA = 0.08f - (float)i * 0.02f;
				drawList->AddCircleFilled(center, glowR,
					ImColor(ecv.x, ecv.y, ecv.z, glowA), 48);
			}


			drawList->AddCircleFilled(center, radius,
				ImColor(ecv.x * 0.15f, ecv.y * 0.15f, ecv.z * 0.15f, 0.85f), 48);
			drawList->AddCircle(center, radius,
				ImColor(ecv.x, ecv.y, ecv.z, 0.9f), 48, 2.2f);


			drawList->AddCircle(center, radius * 0.7f,
				ImColor(ecv.x, ecv.y, ecv.z, 0.25f + breathe * 0.1f), 48, 1.0f);


			static float rotAngle = 0.0f;
			rotAngle += dt * 1.8f;
			float arcR = radius + 5.0f;
			int arcSegments = 16;
			float arcSpan = 1.2f;
			for (int i = 0; i < arcSegments - 1; i++) {
				float t1 = rotAngle + ((float)i / (float)arcSegments) * arcSpan;
				float t2 = rotAngle + ((float)(i + 1) / (float)arcSegments) * arcSpan;
				float a = 0.7f * (1.0f - (float)i / (float)arcSegments);
				drawList->AddLine(
					ImVec2(center.x + cosf(t1) * arcR, center.y + sinf(t1) * arcR),
					ImVec2(center.x + cosf(t2) * arcR, center.y + sinf(t2) * arcR),
					ImColor(ecv.x, ecv.y, ecv.z, a), 2.0f);
			}

			for (int i = 0; i < arcSegments - 1; i++) {
				float t1 = rotAngle + 3.14159f + ((float)i / (float)arcSegments) * arcSpan;
				float t2 = rotAngle + 3.14159f + ((float)(i + 1) / (float)arcSegments) * arcSpan;
				float a = 0.7f * (1.0f - (float)i / (float)arcSegments);
				drawList->AddLine(
					ImVec2(center.x + cosf(t1) * arcR, center.y + sinf(t1) * arcR),
					ImVec2(center.x + cosf(t2) * arcR, center.y + sinf(t2) * arcR),
					ImColor(ecv.x, ecv.y, ecv.z, a), 2.0f);
			}


			float dotR = 2.5f + breathe * 0.8f;
			drawList->AddCircleFilled(center, dotR,
				ImColor(ecv.x, ecv.y, ecv.z, 0.95f), 16);
			drawList->AddCircleFilled(center, dotR * 0.5f,
				ImColor(1.0f, 1.0f, 1.0f, 0.8f), 12);


			float gapInner = radius * 0.22f;
			float gapOuter = radius * 0.55f;
			float lineAlpha = 0.75f + breathe * 0.15f;
			ImU32 lineCol = ImColor(ecv.x, ecv.y, ecv.z, lineAlpha);

			drawList->AddLine(ImVec2(center.x - gapOuter, center.y), ImVec2(center.x - gapInner, center.y), lineCol, 1.5f);
			drawList->AddLine(ImVec2(center.x + gapInner, center.y), ImVec2(center.x + gapOuter, center.y), lineCol, 1.5f);

			drawList->AddLine(ImVec2(center.x, center.y - gapOuter), ImVec2(center.x, center.y - gapInner), lineCol, 1.5f);
			drawList->AddLine(ImVec2(center.x, center.y + gapInner), ImVec2(center.x, center.y + gapOuter), lineCol, 1.5f);


			static float pulsePhase = 0.0f;
			pulsePhase += dt * 0.8f;
			if (pulsePhase > 1.0f) pulsePhase -= 1.0f;
			float pulseR = radius + pulsePhase * 18.0f;
			float pulseA = (1.0f - pulsePhase) * 0.4f;
			drawList->AddCircle(center, pulseR,
				ImColor(ecv.x, ecv.y, ecv.z, pulseA), 48, 1.2f);
		}
	}

	drawSemiGodModeIndicator();

}

namespace GetPlayerNames {
	uintptr_t citizenbase_address = 0;
	std::unordered_map<int, std::string> PlayerIdToName_Table;
	std::mutex NameMapMutex;

	inline bool IsValidASCII(char c) {
		return c >= 32 && c <= 126;
	}

	inline bool IsASCIIAlphaNum(char c) {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
	}

	inline bool IsEmojiOrSpecialChar(uint32_t codePoint) {
		if (codePoint >= 0x1F300 && codePoint <= 0x1F9FF) return true;
		if (codePoint >= 0x1F600 && codePoint <= 0x1F64F) return true;
		if (codePoint >= 0x1F680 && codePoint <= 0x1F6FF) return true;
		if (codePoint >= 0x2600 && codePoint <= 0x26FF) return true;
		if (codePoint >= 0x2700 && codePoint <= 0x27BF) return true;
		if (codePoint >= 0xFE00 && codePoint <= 0xFE0F) return true;
		if (codePoint >= 0x200D && codePoint <= 0x200D) return true;
		if (codePoint >= 0x203C && codePoint <= 0x3299) return true;
		return false;
	}

	std::string CleanPlayerName(const std::string& name) {
		if (name.empty()) return "";

		std::string cleaned;
		cleaned.reserve(name.length());
		bool hasValidChars = false;

		for (size_t i = 0; i < name.length(); ++i) {
			unsigned char c = static_cast<unsigned char>(name[i]);

			if (c == '^' && i + 1 < name.length()) {
				unsigned char next = static_cast<unsigned char>(name[i + 1]);
				if (next >= '0' && next <= '9') {
					i++;
					continue;
				}
			}

			if (c == '~') {
				size_t endPos = name.find('~', i + 1);
				if (endPos != std::string::npos) {
					if (endPos > i + 1) {
						i = endPos;
						continue;
					}
				}
			}

			if (c == 0 || (c < 32 && c != ' ' && c != '\t' && c != '\n')) {
				continue;
			}

			if (c >= 0x80) {
				int utf8Len = 0;
				if ((c & 0xE0) == 0xC0) utf8Len = 2;
				else if ((c & 0xF0) == 0xE0) utf8Len = 3;
				else if ((c & 0xF8) == 0xF0) utf8Len = 4;
				else if ((c & 0xC0) == 0x80) {
					continue;
				}

				if (utf8Len > 0 && i + utf8Len <= name.length()) {
					bool validUTF8 = true;
					for (int j = 1; j < utf8Len; ++j) {
						unsigned char cont = static_cast<unsigned char>(name[i + j]);
						if ((cont & 0xC0) != 0x80) {
							validUTF8 = false;
							break;
						}
					}

					if (validUTF8) {
						uint32_t codePoint = 0;
						if (utf8Len == 2) {
							codePoint = ((c & 0x1F) << 6) | (static_cast<unsigned char>(name[i + 1]) & 0x3F);
						}
						else if (utf8Len == 3) {
							codePoint = ((c & 0x0F) << 12) | ((static_cast<unsigned char>(name[i + 1]) & 0x3F) << 6) | (static_cast<unsigned char>(name[i + 2]) & 0x3F);
						}
						else if (utf8Len == 4) {
							codePoint = ((c & 0x07) << 18) | ((static_cast<unsigned char>(name[i + 1]) & 0x3F) << 12) | ((static_cast<unsigned char>(name[i + 2]) & 0x3F) << 6) | (static_cast<unsigned char>(name[i + 3]) & 0x3F);
						}

						if (!IsEmojiOrSpecialChar(codePoint)) {
							for (int j = 0; j < utf8Len; ++j) {
								cleaned += name[i + j];
							}
							i += utf8Len - 1;
							hasValidChars = true;
							continue;
						}
						else if (codePoint == 0x0131) {
							for (int j = 0; j < utf8Len; ++j) {
								cleaned += name[i + j];
							}
							i += utf8Len - 1;
							hasValidChars = true;
							continue;
						}
					}
				}
				continue;
			}

			if (c == 0x3F) {
				if (i > 0 && i < name.length() - 1) {
					unsigned char prev = static_cast<unsigned char>(name[i - 1]);
					unsigned char next = static_cast<unsigned char>(name[i + 1]);
					if (IsASCIIAlphaNum(prev) && IsASCIIAlphaNum(next)) {
						continue;
					}
				}
				cleaned += c;
				hasValidChars = true;
			}
			else if (IsValidASCII(c)) {
				cleaned += c;
				if (c != ' ' && c != '\t' && c != '?') {
					hasValidChars = true;
				}
			}
		}

		if (!hasValidChars || cleaned.empty()) {
			return "";
		}

		while (!cleaned.empty() && (cleaned.front() == ' ' || cleaned.front() == '\t' || cleaned.front() == '?')) {
			cleaned.erase(cleaned.begin());
		}
		while (!cleaned.empty() && (cleaned.back() == ' ' || cleaned.back() == '\t' || cleaned.back() == '?')) {
			cleaned.pop_back();
		}

		std::string finalCleaned;
		finalCleaned.reserve(cleaned.length());
		for (size_t i = 0; i < cleaned.length(); ++i) {
			if (cleaned[i] == '?' && (i == 0 || cleaned[i - 1] != '?') &&
				(i + 1 >= cleaned.length() || cleaned[i + 1] != '?')) {
				continue;
			}
			else {
				finalCleaned += cleaned[i];
			}
		}

		if (finalCleaned.length() > 32) {
			size_t truncatePos = 29;
			if (truncatePos < finalCleaned.length()) {
				unsigned char byte = static_cast<unsigned char>(finalCleaned[truncatePos]);
				while (truncatePos > 0 && (byte & 0xC0) == 0x80) {
					truncatePos--;
					byte = static_cast<unsigned char>(finalCleaned[truncatePos]);
				}
			}
			finalCleaned = finalCleaned.substr(0, truncatePos) + "...";
		}

		for (size_t i = 0; i < finalCleaned.length(); ++i) {
			unsigned char ch = static_cast<unsigned char>(finalCleaned[i]);
			if (ch != '?' && ch != ' ' && ch != '\t') {
				if (ch < 0x80 || (ch >= 0xC0 && ch <= 0xF7)) {
					return finalCleaned;
				}
			}
		}

		return "";
	}

	void UpdatePlayerNames() {
		while (exitLoop == false) {
			try {
				if (citizenbase_address == 0) {
					citizenbase_address = GetBaseAddress("citizen-playernames-five.dll");
					if (citizenbase_address == 0) {
						Sleep(5000);
						continue;
					}
				}

				uint64_t playerNamesPtr = ReadMemory<uint64_t>(citizenbase_address + 0x31D98);
				if (!playerNamesPtr || playerNamesPtr == citizenbase_address) {
					Sleep(1000);
					continue;
				}

				uint64_t playerNameCount = ReadMemory<uint64_t>(citizenbase_address + 0x31DA0);
				if (playerNameCount == 0 || playerNameCount > 10024) {
					Sleep(1000);
					continue;
				}

				uintptr_t list = ReadMemory<uintptr_t>(playerNamesPtr + 0x8);
				if (!list || list == playerNamesPtr) {
					Sleep(1000);
					continue;
				}

				std::unordered_map<int, std::string> newNameTable;
				uintptr_t visitedNodes[1024] = { 0 };
				int visitedCount = 0;

				for (uint32_t i = 0; i < playerNameCount && i < 1024; ++i) {
					if (!list) break;

					bool alreadyVisited = false;
					for (int j = 0; j < visitedCount; ++j) {
						if (visitedNodes[j] == list) {
							alreadyVisited = true;
							break;
						}
					}
					if (alreadyVisited) break;
					visitedNodes[visitedCount++] = list;

					int id = ReadMemory<int>(list + 0x10);
					if (id > 0 && id < 100000) {
						std::string rawName;

						uintptr_t namePtr = ReadMemory<uintptr_t>(list + 0x18);
						if (namePtr && namePtr > 0x1000 && namePtr < 0x7FFFFFFFFFFF) {
							rawName = ReadStringAuto(namePtr);
						}

						if (rawName.empty() || rawName.length() < 2 ||
							(rawName.length() == 2 && static_cast<unsigned char>(rawName[0]) == 0xD9)) {
							rawName = ReadStringAuto(list + 0x18);
						}

						if (rawName.empty() || rawName.length() < 2 ||
							(rawName.length() == 2 && static_cast<unsigned char>(rawName[0]) == 0xD9)) {
							rawName = ReadStringW(list + 0x18);
						}

						if ((rawName.empty() || rawName.length() < 2 ||
							(rawName.length() == 2 && static_cast<unsigned char>(rawName[0]) == 0xD9)) &&
							namePtr && namePtr > 0x1000 && namePtr < 0x7FFFFFFFFFFF) {
							rawName = ReadStringW(namePtr);
						}

						if (!rawName.empty() && rawName.length() > 1 &&
							!(rawName.length() == 2 && static_cast<unsigned char>(rawName[0]) == 0xD9)) {
							std::string cleanedName = CleanPlayerName(rawName);
							if (!cleanedName.empty() && cleanedName != "?") {
								newNameTable[id] = cleanedName;
							}
						}
					}

					uintptr_t next = ReadMemory<uintptr_t>(list + 0x8);
					if (!next || next == list) break;
					list = next;
				}

				{
					std::lock_guard<std::mutex> lock(NameMapMutex);
					for (const auto& pair : newNameTable) {
						PlayerIdToName_Table[pair.first] = pair.second;
					}
				}
			}
			catch (...) {
			}
			Sleep(2000);
		}
	}

	inline std::string GetPedName(Ped& ped) {
		int playerId = ped.cachedId;
		if (playerId <= 0) {
			return "Unknown";
		}

		std::lock_guard<std::mutex> lock(NameMapMutex);
		auto it = PlayerIdToName_Table.find(playerId);
		if (it != PlayerIdToName_Table.end() && !it->second.empty() && it->second != "?") {
			return it->second;
		}

		char buffer[32];
		sprintf_s(buffer, sizeof(buffer), "player %d", playerId);
		return std::string(buffer);
	}
}

void TeleportObject(uintptr_t Object, uintptr_t Navigation, uintptr_t ModelInfo, Vector3 Position, Vector3 VisualPosition, bool Stop) {
	float BackupMagic = 0.f;
	if (Stop) {
		BackupMagic = ReadMemory<float>(ModelInfo + 0x2C);
		WriteMemory(ModelInfo + 0x2C, 0.f);
	}

	WriteMemory(Object + 0x90, VisualPosition);
	WriteMemory(Navigation + 0x50, Position);
	if (Stop) {
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
		WriteMemory(ModelInfo + 0x2C, BackupMagic);
	}
}

void AddPlayerList() {
	while (true) {
		if (!localPlayer.update()) {
			Sleep(1000);
			continue;
		}

		std::vector<int> oldPlayerIDs = playerIDs;
		std::vector<int> newPlayerIDs;

		{
			std::lock_guard<std::mutex> lock(pedListMutex);
			newPlayerIDs.reserve(pedList.size());
			for (auto& ped : pedList) {
				if (!ped.update()) continue;
				if (Cheats::Esp::ignorePed && !ped.isPlayer()) continue;

				float distance = getDistance(ped.position, localPlayer.position);
				if (distance > 300.0f) continue;

				int pedID = ped.cachedId;
				std::string playerName = GetPlayerNames::GetPedName(ped);
				playerNames[pedID] = playerName;
				newPlayerIDs.push_back(pedID);

				if (friendStatus.find(pedID) == friendStatus.end()) {
					friendStatus[pedID] = false;
				}
			}
		}
		int lastSelectedID = (selectedItemPlayer >= 0 && selectedItemPlayer < (int)playerIDs.size()) ? playerIDs[selectedItemPlayer] : -1;
		playerIDs = std::move(newPlayerIDs);
		if (lastSelectedID != -1) {
			auto it = std::find(playerIDs.begin(), playerIDs.end(), lastSelectedID);
			selectedItemPlayer = (it != playerIDs.end()) ? std::distance(playerIDs.begin(), it) : 0;
		}
		else {
			selectedItemPlayer = -1;
		}
		if (Teleport && selectedItemPlayer >= 0 && selectedItemPlayer < (int)playerIDs.size()) {
			int targetID = playerIDs[selectedItemPlayer];
			Vector3 pos = GetPositionByID(targetID);
			if (!(pos.x == 0.f && pos.y == 0.f && pos.z == 0.f)) {
				uint64_t Object = (uint64_t)localPlayer.pointer;
				if (Object) {
					uint64_t Navigation = ReadMemory<uint64_t>(Object + 0x30);
					uint64_t ModelInfo = ReadMemory<uint64_t>(Object + 0x20);

					if (Navigation && ModelInfo) {
						Vector3 TeleportPos = Vector3(pos.x, pos.y, pos.z);
						TeleportObject(Object, Navigation, ModelInfo, TeleportPos, TeleportPos, true);
					}
				}
			}
			Teleport = false;
		}
		Sleep(250);
	}
}

std::string GetVehicleModelName(uintptr_t vehicle) {
	try {
		uintptr_t modelInfo = ReadMemory<uintptr_t>(vehicle + 0x20);
		if (modelInfo == 0) return "Unknown";

		std::string modelName = ReadString(modelInfo + 0x298);
		if (modelName.empty() || modelName.size() > 24) {
			modelName = ReadString(modelInfo + 0x2A0);
		}
		if (modelName.empty() || modelName.size() > 24) {
			modelName = ReadString(modelInfo + 0x2A8);
		}

		if (modelName.empty() || modelName.size() > 24) {
			return "Unknown";
		}

		return modelName;
	}
	catch (...) {
		return "Unknown";
	}
}

void AddVehicleList() {
	while (true) {
		if (!localPlayer.update()) {
			Sleep(1000);
			continue;
		}

		std::vector<uintptr_t> newVehiclePointers;
		std::vector<std::string> newVehicleNames;
		std::vector<Vector3> newVehiclePositions;
		std::vector<bool> newVehicleLockStatus;
		std::vector<float> newVehicleDistances;

		std::unordered_set<uintptr_t> addedVehicles;
		std::unordered_set<std::string> addedVehicleSignatures;

		uintptr_t replayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
		if (replayInterface) {
			uintptr_t vehicleInterface = ReadMemory<DWORD64>(replayInterface + 0x10);
			uintptr_t vehicleList = ReadMemory<DWORD64>(vehicleInterface + 0x180);
			int vehicleListCount = ReadMemory<int>(vehicleInterface + 0x188);

			for (int i = 0; i < vehicleListCount && i < 32; ++i) {
				uintptr_t vehicle = ReadMemory<uintptr_t>(vehicleList + (i * 0x10));
				if (!vehicle) continue;

				if (addedVehicles.find(vehicle) != addedVehicles.end()) {
					continue;
				}
				addedVehicles.insert(vehicle);

				Vector3 vehiclePos = ReadMemory<Vector3>(vehicle + 0x90);
				Vector3 localPos = ReadMemory<Vector3>(localPlayer.pointer + 0x90);

				float distance = getDistance(vehiclePos, localPos);
				if (distance > 1250.0f) continue;

				std::string modelName = GetVehicleModelName(vehicle);
				std::string vehicleSignature = modelName + "_" +
					std::to_string((int)(vehiclePos.x * 10)) + "_" +
					std::to_string((int)(vehiclePos.y * 10)) + "_" +
					std::to_string((int)(vehiclePos.z * 10));

				if (addedVehicleSignatures.find(vehicleSignature) != addedVehicleSignatures.end()) {
					continue;
				}
				addedVehicleSignatures.insert(vehicleSignature);

				uint32_t lockStatus = ReadMemory<uint32_t>(vehicle + Offsets.VehicleDoorsLockState);
				bool isLocked = (lockStatus == 2);

				newVehiclePointers.push_back(vehicle);
				newVehicleNames.push_back(modelName);
				newVehiclePositions.push_back(vehiclePos);
				newVehicleLockStatus.push_back(isLocked);
				newVehicleDistances.push_back(distance);
			}
		}

		std::vector<std::pair<float, size_t>> distanceIndexPairs;
		for (size_t i = 0; i < newVehicleDistances.size(); i++) {
			distanceIndexPairs.push_back({ newVehicleDistances[i], i });
		}
		std::sort(distanceIndexPairs.begin(), distanceIndexPairs.end());

		uintptr_t selectedPointer = selectedVehiclePointer;
		int newSelectedIndex = -1;

		std::vector<uintptr_t> sortedVehiclePointers;
		std::vector<std::string> sortedVehicleNames;
		std::vector<Vector3> sortedVehiclePositions;
		std::vector<bool> sortedVehicleLockStatus;
		std::vector<float> sortedVehicleDistances;

		for (const auto& pair : distanceIndexPairs) {
			size_t originalIndex = pair.second;
			sortedVehiclePointers.push_back(newVehiclePointers[originalIndex]);
			sortedVehicleNames.push_back(newVehicleNames[originalIndex]);
			sortedVehiclePositions.push_back(newVehiclePositions[originalIndex]);
			sortedVehicleLockStatus.push_back(newVehicleLockStatus[originalIndex]);
			sortedVehicleDistances.push_back(newVehicleDistances[originalIndex]);

			if (newVehiclePointers[originalIndex] == selectedPointer) {
				newSelectedIndex = sortedVehiclePointers.size() - 1;
			}
		}

		vehiclePointers = std::move(sortedVehiclePointers);
		vehicleNames = std::move(sortedVehicleNames);
		vehiclePositions = std::move(sortedVehiclePositions);
		vehicleLockStatus = std::move(sortedVehicleLockStatus);
		vehicleDistances = std::move(sortedVehicleDistances);

		if (newSelectedIndex != -1) {
			selectedVehicleItem = newSelectedIndex;
		}
		else if (selectedPointer != 0) {

			selectedVehicleItem = -1;
			selectedVehiclePointer = 0;
		}

		if (TeleportToVehicle && selectedVehiclePointer != 0) {
			if (selectedVehicleItem < vehiclePositions.size()) {
				TeleportToPosition(vehiclePositions[selectedVehicleItem]);
				printa<normal>("Teleporting to vehicle...");
			}
			TeleportToVehicle = false;
		}

		if (UnlockVehicle && selectedVehiclePointer != 0) {
			Vehicle vehicle_instance(selectedVehiclePointer);
			vehicle_instance.DoorState(true);
			UnlockVehicle = false;
		}

		if (LockVehicle && selectedVehiclePointer != 0) {
			Vehicle vehicle_instance(selectedVehiclePointer);
			vehicle_instance.DoorState(false);
			LockVehicle = false;
		}

		Sleep(500);
	}
}

void DrawVehicleList() {
	if (!Cheats::Vehicle::vehicleList && !Cheats::Esp::carEsp) return;

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);

	if (!localPlayer.update()) return;
	Vector3 localPos = ReadMemory<Vector3>(localPlayer.pointer + 0x90);

	uintptr_t replayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);
	if (!replayInterface) return;

	uintptr_t vehicleInterface = ReadMemory<DWORD64>(replayInterface + 0x10);
	uintptr_t vehicleListPtr = ReadMemory<DWORD64>(vehicleInterface + 0x180);
	int vehicleListCount = ReadMemory<int>(vehicleInterface + 0x188);

	for (int i = 0; i < vehicleListCount && i < 300; ++i) {
		uintptr_t vehicle = ReadMemory<uintptr_t>(vehicleListPtr + (i * 0x10));
		if (!vehicle || vehicle == 0) continue;

		Vector3 vehiclePos = ReadMemory<Vector3>(vehicle + 0x90);


		if (vec3Empty(vehiclePos)) continue;

		float distance = getDistance(vehiclePos, localPos);
		if (distance > 1250.0f) continue;

		Vector2 screenPos;
		if (!worldToScreen(viewMatrix, vehiclePos, screenPos)) continue;


		if (Cheats::Esp::carEsp) {
			int playerCount = 0;


			{
				std::lock_guard<std::mutex> lock(pedListMutex);
				for (auto& ped : pedList) {
					if (!ped.pointer || ped.pointer == 0) continue;


					uintptr_t offsets[] = { Offsets.Vehicle, 0xD10, 0xD18, 0xD20, 0xD28 };
					bool foundInVehicle = false;

					for (uintptr_t offset : offsets) {
						uintptr_t pedVehicle = ReadMemory<uintptr_t>(ped.pointer + offset);


						if (pedVehicle == vehicle && pedVehicle != 0) {


							Vector3 pedPos = ReadMemory<Vector3>(ped.pointer + 0x90);
							float distanceToVehicle = getDistance(pedPos, vehiclePos);


							if (distanceToVehicle < 5.0f) {
								foundInVehicle = true;
								break;
							}
						}
					}

					if (foundInVehicle) {
						playerCount++;
					}
				}
			}


			if (playerCount > 0) {

				Vector3 textPos = vehiclePos;
				textPos.z += 2.0f;

				Vector2 textScreenPos;
				if (worldToScreen(viewMatrix, textPos, textScreenPos)) {

					std::string countText = std::to_string(playerCount);


					ImColor textColor = ImColor(255, 255, 255, 255);
					ImColor shadowColor = ImColor(0, 0, 0, 200);


					ImFont* font = ImGui::GetFont();
					float fontSize = 15.0f;

					ImVec2 textSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, countText.c_str());
					ImVec2 textPosVec = ImVec2(textScreenPos.x - textSize.x / 2, textScreenPos.y - textSize.y / 2);


					ImVec2 bgPadding = ImVec2(5.0f, 3.0f);
					drawList->AddRectFilled(
						ImVec2(textPosVec.x - bgPadding.x, textPosVec.y - bgPadding.y),
						ImVec2(textPosVec.x + textSize.x + bgPadding.x, textPosVec.y + textSize.y + bgPadding.y),
						ImColor(0, 0, 0, 140), 2.0f
					);


					drawList->AddText(font, fontSize, ImVec2(textPosVec.x + 1, textPosVec.y + 1), shadowColor, countText.c_str());

					drawList->AddText(font, fontSize, textPosVec, textColor, countText.c_str());
				}
			}
		}


		if (Cheats::Vehicle::vehicleList) {
			if (Cheats::Vehicle::vehicleListMarker) {
				ImColor markerColor = ImColor(255, 255, 0, 255);
				drawList->AddCircle(ImVec2(screenPos.x, screenPos.y), 8, ImColor(0, 0, 0, 100), 12, 2.0f);
				drawList->AddCircle(ImVec2(screenPos.x, screenPos.y), 6, markerColor, 12, 2.0f);
				drawList->AddCircleFilled(ImVec2(screenPos.x, screenPos.y), 3, markerColor);
			}

			if (Cheats::Vehicle::vehicleListName) {
				std::string vehicleName = GetVehicleModelName(vehicle);
				DrawShadowedTexts(drawList, vehicleName, ImVec2(screenPos.x, screenPos.y - 25), 14.f,
					ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 0.0f, 1.0f)), true);
			}

			if (Cheats::Vehicle::vehicleListDistance) {
				std::string distanceText = std::to_string((int)(distance * 0.2f)) + "m";
				DrawShadowedTexts(drawList, distanceText, ImVec2(screenPos.x, screenPos.y + 15), 12.f,
					ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), true);
			}

			if (Cheats::Vehicle::vehicleListSnapline) {
				ImVec2 screenCenter = ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN));
				drawList->AddLine(screenCenter, ImVec2(screenPos.x, screenPos.y),
					ImGui::ColorConvertFloat4ToU32(ImColor(255, 255, 0)), 1.5f);
			}
		}
	}
}

void DrawEsp() {
	if (!Cheats::Esp::skeletonEsp && !Cheats::Esp::boxEsp && !Cheats::Esp::lineEsp &&
		!Cheats::Esp::nameEsp && !Cheats::Esp::idEsp && !Cheats::Esp::weaponEsp &&
		!Cheats::Esp::distanceEsp && !Cheats::Esp::healthBar && !Cheats::Esp::armorBar &&
		!Cheats::Esp::directionEsp && !Cheats::Esp::genderEsp && !Cheats::Esp::radarEsp &&
		!Cheats::Aim::showLine && !Cheats::Aim::showTargetBone) {
		return;
	}

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();


	uintptr_t freshViewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
	if (!freshViewPort) {
		return;
	}


	Matrix viewMatrix = ReadMemory<Matrix>(freshViewPort + 0x24C);


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float val = viewMatrix.m[i][j];
			if (std::isnan(val) || std::isinf(val) || (val > 1000000.0f || val < -1000000.0f)) {
				return;
			}
		}
	}

	if (!localPlayer.update()) {
		return;
	}

	DrawVehicleList();


	static std::vector<uintptr_t> pedPointers;
	pedPointers.clear();
	{
		std::lock_guard<std::mutex> lock(pedListMutex);
		pedPointers.reserve(pedList.size());
		for (const auto& p : pedList) {
			if (p.pointer != 0) pedPointers.push_back(p.pointer);
		}
	}

	const Vector3& localPos = localPlayer.position;
	const float maxDistance = static_cast<float>(Cheats::Esp::distance);
	const float maxDistSq = maxDistance * maxDistance;

	Ped ped{};
	for (auto ptr : pedPointers) {
		ped.pointer = ptr;
		if (!ped.updateEsp()) continue;

		float dx = ped.position.x - localPos.x;
		float dy = ped.position.y - localPos.y;
		float dz = ped.position.z - localPos.z;
		float distSq = dx*dx + dy*dy + dz*dz;
		if (distSq > maxDistSq) continue;
		float pDistance = sqrtf(distSq);

		if (globals::toggles::esp_ignore_peds && !ped.isPlayer()) continue;
		if (Cheats::Esp::ignoreDeath && ped.isDead()) continue;

		Vector2 pBase{};
		if (!worldToScreen(viewMatrix, ped.position, pBase)) continue;

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;
		if (pBase.x < -500.0f || pBase.x > screenSize.x + 500.0f ||
			pBase.y < -500.0f || pBase.y > screenSize.y + 500.0f) continue;

		if (vec3Empty(ped.boneList[Head]) || vec3Empty(ped.boneList[Neck]) ||
			vec3Empty(ped.boneList[LeftFoot]) || vec3Empty(ped.boneList[RightFoot])) {
			continue;
		}

		Vector2 pHead{}, pNeck{}, pLeftFoot{}, pRightFoot{}, pHip{}, pLeftHand{}, pRightHand{}, pLeftKnee{}, pRightKnee{};

		if (!worldToScreen(viewMatrix, ped.boneList[Head], pHead) ||
			!worldToScreen(viewMatrix, ped.boneList[Neck], pNeck) ||
			!worldToScreen(viewMatrix, ped.boneList[LeftFoot], pLeftFoot) ||
			!worldToScreen(viewMatrix, ped.boneList[RightFoot], pRightFoot)) {
			continue;
		}

		worldToScreen(viewMatrix, ped.boneList[Hip], pHip);
		worldToScreen(viewMatrix, ped.boneList[LeftHand], pLeftHand);
		worldToScreen(viewMatrix, ped.boneList[RightHand], pRightHand);
		worldToScreen(viewMatrix, ped.boneList[LeftKnee], pLeftKnee);
		worldToScreen(viewMatrix, ped.boneList[RightKnee], pRightKnee);


		const float HeadToNeck = pNeck.y - pHead.y;
		const float pTop = pHead.y - (HeadToNeck * 2.5f);
		const float pBottom = (pLeftFoot.y > pRightFoot.y ? pLeftFoot.y : pRightFoot.y) * 1.001f;
		const float pHeight = pBottom - pTop;
		const float pWidth = pHeight / 3.5f;
		const float bScale = pWidth / 1.5f;


		const bool lod_full = pDistance < 600.0f;
		const bool lod_medium = pDistance >= 600.0f && pDistance < 800.0f;

		if (Cheats::Esp::skeletonEsp && lod_full) {
			ImColor color;
			if (IsFriend(ped.cachedId)) {
				color = ImColor(0, 255, 0, 255);
			} else {
				color = ConvertToImColor(Cheats::Esp::skeletonEspColor);
			}

			if (Cheats::Esp::headEsp) {
				float headRadius = (pNeck.y - pHead.y) * 0.8f;
				if (headRadius > 2.0f && headRadius < 100.0f) {
					drawList->AddCircle(ImVec2(pHead.x, pHead.y), headRadius, IM_COL32(0,0,0,160), 20, Cheats::Esp::skeletonEspSize + 2);
					drawList->AddCircle(ImVec2(pHead.x, pHead.y), headRadius, color, 20, Cheats::Esp::skeletonEspSize);
				}
			}



			Vector3 nW = ped.boneList[Neck];
			Vector3 hW = ped.boneList[Hip];
			Vector3 lhW = ped.boneList[LeftHand];
			Vector3 rhW = ped.boneList[RightHand];



			float shoulderSpreadX = (rhW.x - lhW.x) * 0.40f;
			float shoulderSpreadY = (rhW.y - lhW.y) * 0.40f;

			Vector3 lShoulder = {
				nW.x - shoulderSpreadX * 0.5f,
				nW.y - shoulderSpreadY * 0.5f,
				nW.z
			};
			Vector3 rShoulder = {
				nW.x + shoulderSpreadX * 0.5f,
				nW.y + shoulderSpreadY * 0.5f,
				nW.z
			};

			Vector3 lElbow = {
				lShoulder.x + (lhW.x - lShoulder.x) * 0.55f,
				lShoulder.y + (lhW.y - lShoulder.y) * 0.55f,
				lShoulder.z + (lhW.z - lShoulder.z) * 0.55f
			};
			Vector3 rElbow = {
				rShoulder.x + (rhW.x - rShoulder.x) * 0.55f,
				rShoulder.y + (rhW.y - rShoulder.y) * 0.55f,
				rShoulder.z + (rhW.z - rShoulder.z) * 0.55f
			};

			Vector2 sLS{}, sRS{}, sLE{}, sRE{};
			bool armOk = worldToScreen(viewMatrix, lShoulder, sLS) &&
						 worldToScreen(viewMatrix, rShoulder, sRS) &&
						 worldToScreen(viewMatrix, lElbow, sLE) &&
						 worldToScreen(viewMatrix, rElbow, sRE);

			float t = (float)Cheats::Esp::skeletonEspSize;
			ImU32 oc = IM_COL32(0, 0, 0, 160);
			ImU32 mc = (ImU32)color;


			drawList->AddLine({pHead.x, pHead.y}, {pNeck.x, pNeck.y}, oc, t+2);
			drawList->AddLine({pHead.x, pHead.y}, {pNeck.x, pNeck.y}, mc, t);
			drawList->AddLine({pNeck.x, pNeck.y}, {pHip.x, pHip.y}, oc, t+2);
			drawList->AddLine({pNeck.x, pNeck.y}, {pHip.x, pHip.y}, mc, t);

			if (armOk) {

				drawList->AddLine({sLS.x, sLS.y}, {sRS.x, sRS.y}, oc, t+2);
				drawList->AddLine({sLS.x, sLS.y}, {sRS.x, sRS.y}, mc, t);

				drawList->AddLine({sLS.x, sLS.y}, {sLE.x, sLE.y}, oc, t+2);
				drawList->AddLine({sLS.x, sLS.y}, {sLE.x, sLE.y}, mc, t);
				drawList->AddLine({sLE.x, sLE.y}, {pLeftHand.x, pLeftHand.y}, oc, t+2);
				drawList->AddLine({sLE.x, sLE.y}, {pLeftHand.x, pLeftHand.y}, mc, t);

				drawList->AddLine({sRS.x, sRS.y}, {sRE.x, sRE.y}, oc, t+2);
				drawList->AddLine({sRS.x, sRS.y}, {sRE.x, sRE.y}, mc, t);
				drawList->AddLine({sRE.x, sRE.y}, {pRightHand.x, pRightHand.y}, oc, t+2);
				drawList->AddLine({sRE.x, sRE.y}, {pRightHand.x, pRightHand.y}, mc, t);
			}


			drawList->AddLine({pHip.x, pHip.y}, {pLeftKnee.x, pLeftKnee.y}, oc, t+2);
			drawList->AddLine({pHip.x, pHip.y}, {pLeftKnee.x, pLeftKnee.y}, mc, t);
			drawList->AddLine({pLeftKnee.x, pLeftKnee.y}, {pLeftFoot.x, pLeftFoot.y}, oc, t+2);
			drawList->AddLine({pLeftKnee.x, pLeftKnee.y}, {pLeftFoot.x, pLeftFoot.y}, mc, t);
			drawList->AddLine({pHip.x, pHip.y}, {pRightKnee.x, pRightKnee.y}, oc, t+2);
			drawList->AddLine({pHip.x, pHip.y}, {pRightKnee.x, pRightKnee.y}, mc, t);
			drawList->AddLine({pRightKnee.x, pRightKnee.y}, {pRightFoot.x, pRightFoot.y}, oc, t+2);
			drawList->AddLine({pRightKnee.x, pRightKnee.y}, {pRightFoot.x, pRightFoot.y}, mc, t);
		}


		if (Cheats::Aim::showTargetBone && Cheats::Aim::silent && (GetAsyncKeyState(Cheats::Aim::silentKey) & 0x8000) && lod_full) {
			Vector3 closestPos;
			uintptr_t targetPtr;
			{
				std::lock_guard<std::mutex> lock(globalClosestBone.mutex);
				closestPos = globalClosestBone.position;
				targetPtr = globalClosestBone.targetPointer;
			}

			if (targetPtr != 0 && targetPtr == ped.pointer && !vec3Empty(closestPos)) {
				float effectiveSilentFov = Cheats::Aim::dynamic_fov_on && currentDynamicFov > 0.0f ? currentDynamicFov : static_cast<float>(Cheats::Aim::silentFov);
				Vector2 closestBoneScreen;
				if (worldToScreen(viewMatrix, closestPos, closestBoneScreen)) {
					float distToCrosshair = GetCrosshairDistance(closestBoneScreen);
					if (distToCrosshair <= effectiveSilentFov) {
						float circleRadius = 5.0f;
						float outlineRadius = 6.5f;

						ImColor outerColor = ImColor(0, 0, 0, 200);
						ImColor innerColor = ImColor(138, 43, 226, 255);

						drawList->AddCircleFilled(ImVec2(closestBoneScreen.x, closestBoneScreen.y),
							outlineRadius, outerColor, 20);
						drawList->AddCircleFilled(ImVec2(closestBoneScreen.x, closestBoneScreen.y),
							circleRadius, innerColor, 20);
					}
				}
			}
		}

		float reducedWidth = pWidth * Cheats::Esp::BoxSizeX;
		if (Cheats::Esp::boxEsp) {
			ImColor color = ConvertToImColor(Cheats::Esp::boxEspColor);

			ImVec4 mainColor = color.Value;

			auto DrawBoxWithGradient = [&](bool isCornerBox) {

				if (Cheats::Esp::useCustomGradient && Cheats::Esp::gradientEnabled) {
					ImVec4 topColor, bottomColor;

					topColor = ImVec4(
						Cheats::Esp::boxGradientTopColor[0],
						Cheats::Esp::boxGradientTopColor[1],
						Cheats::Esp::boxGradientTopColor[2],
						Cheats::Esp::boxGradientTopColor[3] * Cheats::Esp::gradientIntensity
					);
					bottomColor = ImVec4(
						Cheats::Esp::boxGradientBottomColor[0],
						Cheats::Esp::boxGradientBottomColor[1],
						Cheats::Esp::boxGradientBottomColor[2],
						Cheats::Esp::boxGradientBottomColor[3] * Cheats::Esp::gradientIntensity
					);

					drawList->AddRectFilledMultiColor(
						ImVec2(pBase.x - reducedWidth + 1, pTop + 1),
						ImVec2(pBase.x + reducedWidth - 1, pBottom - 1),
						ImGui::ColorConvertFloat4ToU32(topColor),
						ImGui::ColorConvertFloat4ToU32(topColor),
						ImGui::ColorConvertFloat4ToU32(bottomColor),
						ImGui::ColorConvertFloat4ToU32(bottomColor)
					);
				}

				if (!isCornerBox) {

					DrawLineOutline(ImVec2(pBase.x - reducedWidth, pTop), ImVec2(pBase.x + reducedWidth, pTop), color, 1.f);
					DrawLineOutline(ImVec2(pBase.x - reducedWidth, pTop), ImVec2(pBase.x - reducedWidth, pBottom), color, 1.f);
					DrawLineOutline(ImVec2(pBase.x + reducedWidth, pTop), ImVec2(pBase.x + reducedWidth, pBottom), color, 1.f);
					DrawLineOutline(ImVec2(pBase.x - reducedWidth, pBottom), ImVec2(pBase.x + reducedWidth, pBottom), color, 1.f);
				}
				else {

					DrawLineOutline(ImVec2((pBase.x - reducedWidth), pTop), ImVec2((pBase.x - reducedWidth) + bScale, pTop), color, 1.f);
					DrawLineOutline(ImVec2((pBase.x + reducedWidth), pTop), ImVec2((pBase.x + reducedWidth) - bScale, pTop), color, 1.f);
					DrawLineOutline(ImVec2(pBase.x - reducedWidth, pTop), ImVec2(pBase.x - reducedWidth, pTop + bScale), color, 1.f);
					DrawLineOutline(ImVec2(pBase.x - reducedWidth, pBottom), ImVec2(pBase.x - reducedWidth, pBottom - bScale), color, 1.f);
					DrawLineOutline(ImVec2(pBase.x + reducedWidth, pTop), ImVec2(pBase.x + reducedWidth, pTop + bScale), color, 1.f);
					DrawLineOutline(ImVec2(pBase.x + reducedWidth, pBottom), ImVec2(pBase.x + reducedWidth, pBottom - bScale), color, 1.f);
					DrawLineOutline(ImVec2((pBase.x - reducedWidth), pBottom), ImVec2((pBase.x - reducedWidth) + bScale, pBottom), color, 1.f);
					DrawLineOutline(ImVec2((pBase.x + reducedWidth), pBottom), ImVec2((pBase.x + reducedWidth) - bScale, pBottom), color, 1.f);
				}
				};

			if (Cheats::Esp::boxEspSelectedType == 0) {
				DrawBoxWithGradient(false);
			}
			else if (Cheats::Esp::boxEspSelectedType == 1) {
				DrawBoxWithGradient(true);
			}
		}

		if (Cheats::Esp::lineEsp && lod_full) {
			ImColor color = ConvertToImColor(Cheats::Esp::lineEspColor);

			ImVec2 startPos, endPos;
			ImGuiIO& io = ImGui::GetIO();
			if (Cheats::Esp::lineEspSelectedType == 0) {
				startPos.x = pBase.x + reducedWidth;
				startPos.y = pTop;
				endPos.x = Game.lpRect.right / 2.f;
				endPos.y = 0;
			}
			if (Cheats::Esp::lineEspSelectedType == 1) {
				startPos.x = pBase.x + reducedWidth;
				startPos.y = pTop;
				endPos.x = io.DisplaySize.x / 2.0f;
				endPos.y = io.DisplaySize.y / 2.0f;
			}
			if (Cheats::Esp::lineEspSelectedType == 2) {
				startPos.x = pBase.x;
				startPos.y = pBottom;
				endPos.x = io.DisplaySize.x / 2.0f;
				endPos.y = io.DisplaySize.y;
			}
			DrawLineOutline(startPos, endPos, color, 1.f);
		}

		float posOffsets[8] = {0};

		float boxCenterX = pBase.x;
		float boxLeft = pBase.x - reducedWidth;
		float boxRight = pBase.x + reducedWidth;

		auto getEspPosition = [&](int pos, const std::string& text, float& offset) -> ImVec2 {

			ImFont* pFont = ImGui::GetIO().Fonts->Fonts[0];
			ImVec2 textSize = pFont->CalcTextSizeA(15.0f * 0.8f, FLT_MAX, 0.0f, text.c_str());
			ImVec2 result;

			switch (pos) {
			case 0:

				result = ImVec2(boxLeft - 5.0f - textSize.x, pTop + offset);
				offset += textSize.y + 2.0f;
				break;
			case 1:

				result = ImVec2(boxRight + 5.0f, pTop + offset);
				offset += textSize.y + 2.0f;
				break;
			case 2:

				result = ImVec2(boxCenterX, pTop - 5.0f - offset - textSize.y);
				offset += textSize.y + 2.0f;
				break;
			case 3:

				result = ImVec2(boxCenterX, pBottom + 5.0f + offset);
				offset += textSize.y + 2.0f;
				break;
			case 4:

				result = ImVec2(boxRight + 5.0f, pTop - 5.0f - offset - textSize.y);
				offset += textSize.y + 2.0f;
				break;
			case 5:

				result = ImVec2(boxRight + 5.0f, pBottom + 5.0f + offset);
				offset += textSize.y + 2.0f;
				break;
			case 6:

				result = ImVec2(boxLeft - 5.0f - textSize.x, pTop - 5.0f - offset - textSize.y);
				offset += textSize.y + 2.0f;
				break;
			case 7:

				result = ImVec2(boxLeft - 5.0f - textSize.x, pBottom + 5.0f + offset);
				offset += textSize.y + 2.0f;
				break;
			default:

				result = ImVec2(boxCenterX, pTop - 5.0f);
				break;
			}
			return result;
			};

		if ((Cheats::Esp::healthBar || Cheats::Esp::armorBar) && lod_full) {

			PedBarFix fix;
			fix.id = ped.cachedId;
			fix.health = ped.health;
			fix.armor = ped.armor;

			auto it = pedBarFix.find(ped.cachedId);
			if (it != pedBarFix.end()) {
				if (ped.health > 0) it->second.health = ped.health;
				else ped.health = it->second.health;
				if (ped.armor > 0) it->second.armor = ped.armor;
				else ped.armor = it->second.armor;
			}
			else if (pedBarFix.size() < 128) {
				pedBarFix[ped.cachedId] = fix;
			}
		}

		int healthPos = Cheats::Esp::healthBarPos;

		if (healthPos < 0 || healthPos > 3) healthPos = 1;

		if (Cheats::Esp::healthBar && lod_full) {
			const float hFactor = pHeight / 100.0f;
			const float hFull = hFactor * 100.0f;
			const float hPerc = hFactor * (ped.health - 100.0f);
			const float halfH = ped.health / 2.0f;
			const ImVec4 colB(0.2f, 1.0f, 0.2f, 1.0f);
			const ImVec4 colT(0.0f, 0.6f, 0.0f, 1.0f);
			const ImU32 bg = IM_COL32(0, 0, 0, 255);

			if (halfH > 0) {
				ImVec2 pos;
				const float barW = 2.5f;

				if (healthPos == 0) {
					pos = ImVec2(pBase.x - reducedWidth - 7.0f, pBottom);
					drawList->AddRectFilled(pos - ImVec2(0, hFull), ImVec2(pos.x + barW, pos.y), bg);
					ImVec2 barTop = ImVec2(pos.x, pos.y - hPerc);
					ImVec2 barBottom = ImVec2(pos.x + barW, pos.y);
					drawList->AddRectFilledMultiColor(barTop, barBottom, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB));
				}
				else if (healthPos == 1) {
					pos = ImVec2(pBase.x + reducedWidth + 4.0f, pBottom);
					drawList->AddRectFilled(pos - ImVec2(0, hFull), ImVec2(pos.x + barW, pos.y), bg);
					ImVec2 barTop = ImVec2(pos.x, pos.y - hPerc);
					ImVec2 barBottom = ImVec2(pos.x + barW, pos.y);
					drawList->AddRectFilledMultiColor(barTop, barBottom, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB));
				}
				else if (healthPos == 2) {
					const float barWidth = reducedWidth * 2.0f;
					float healthPerc = (ped.health - 100.0f) / 100.0f;
					healthPerc = (healthPerc > 1.0f) ? 1.0f : ((healthPerc < 0.0f) ? 0.0f : healthPerc);
					pos = ImVec2(boxCenterX - barWidth / 2.0f, pTop - 4.0f - posOffsets[2]);

					drawList->AddRectFilled(pos, ImVec2(pos.x + barWidth, pos.y + 3.0f), bg);
					ImVec2 barEnd = ImVec2(pos.x + barWidth * healthPerc, pos.y + 3.0f);
					drawList->AddRectFilledMultiColor(pos, barEnd, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colT));
					posOffsets[2] += 5.0f;
				}
				else if (healthPos == 3) {
					const float barWidth = reducedWidth * 2.0f;
					float healthPerc = (ped.health - 100.0f) / 100.0f;
					healthPerc = (healthPerc > 1.0f) ? 1.0f : ((healthPerc < 0.0f) ? 0.0f : healthPerc);
					pos = ImVec2(boxCenterX - barWidth / 2.0f, pBottom + 4.0f + posOffsets[3]);

					drawList->AddRectFilled(pos, ImVec2(pos.x + barWidth, pos.y + 3.0f), bg);
					ImVec2 barEnd = ImVec2(pos.x + barWidth * healthPerc, pos.y + 3.0f);
					drawList->AddRectFilledMultiColor(pos, barEnd, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colT));
					posOffsets[3] += 5.0f;
				}
			}
		}

		if (Cheats::Esp::armorBar && lod_full) {
			const ImVec4 colB(0.7f, 0.4f, 1.0f, 1.0f);
			const ImVec4 colT(0.5f, 0.2f, 0.8f, 1.0f);
			const ImU32 bg = IM_COL32(0, 0, 0, 255);
			ImVec2 pos;
			const float barW = 2.5f;
			const float barH = pHeight;
			const float aPerc = (ped.armor / 100.0f) * barH;

			int armorPos = Cheats::Esp::armorBarPos;
			if (armorPos < 0 || armorPos > 3) armorPos = 0;

			if (armorPos == 0) {
				const float offsetX = (healthPos == 0) ? 4.5f : 0.0f;
				pos = ImVec2(pBase.x - reducedWidth - 7.0f - offsetX, pBottom);
				drawList->AddRectFilled(pos - ImVec2(0, barH), ImVec2(pos.x + barW, pos.y), bg);
				ImVec2 barTop = ImVec2(pos.x, pos.y - aPerc);
				ImVec2 barBottom = ImVec2(pos.x + barW, pos.y);
				drawList->AddRectFilledMultiColor(barTop, barBottom, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB));
			}
			else if (armorPos == 1) {
				const float offsetX = (healthPos == 1) ? 4.5f : 0.0f;
				pos = ImVec2(pBase.x + reducedWidth + 4.0f + offsetX, pBottom);
				drawList->AddRectFilled(pos - ImVec2(0, barH), ImVec2(pos.x + barW, pos.y), bg);
				ImVec2 barTop = ImVec2(pos.x, pos.y - aPerc);
				ImVec2 barBottom = ImVec2(pos.x + barW, pos.y);
				drawList->AddRectFilledMultiColor(barTop, barBottom, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB));
			}
			else if (armorPos == 2) {
				const float barWidth = reducedWidth * 2.0f;
				float armorPerc = (ped.armor / 100.0f);
				armorPerc = (armorPerc > 1.0f) ? 1.0f : ((armorPerc < 0.0f) ? 0.0f : armorPerc);
				pos = ImVec2(boxCenterX - barWidth / 2.0f, pTop - 4.0f - posOffsets[2]);

				drawList->AddRectFilled(pos, ImVec2(pos.x + barWidth, pos.y + 3.0f), bg);
				ImVec2 barEnd = ImVec2(pos.x + barWidth * armorPerc, pos.y + 3.0f);
				drawList->AddRectFilledMultiColor(pos, barEnd, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colT));
				posOffsets[2] += 5.0f;
			}
			else if (armorPos == 3) {
				const float barWidth = reducedWidth * 2.0f;
				float armorPerc = (ped.armor / 100.0f);
				armorPerc = (armorPerc > 1.0f) ? 1.0f : ((armorPerc < 0.0f) ? 0.0f : armorPerc);
				pos = ImVec2(boxCenterX - barWidth / 2.0f, pBottom + 4.0f + posOffsets[3]);

				drawList->AddRectFilled(pos, ImVec2(pos.x + barWidth, pos.y + 3.0f), bg);
				ImVec2 barEnd = ImVec2(pos.x + barWidth * armorPerc, pos.y + 3.0f);
				drawList->AddRectFilledMultiColor(pos, barEnd, ImGui::ColorConvertFloat4ToU32(colT), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colB), ImGui::ColorConvertFloat4ToU32(colT));
				posOffsets[3] += 5.0f;
			}
		}

		if (Cheats::Esp::nameEsp && (lod_full || lod_medium)) {
			if (ped.isPlayer()) {
				std::string pedName = GetPlayerNames::GetPedName(ped);
				int pos = Cheats::Esp::nameEspPos;

				if (pos < 0 || pos > 7) pos = 2;
				ImVec2 namePos = getEspPosition(pos, pedName, posOffsets[pos]);
				DrawShadowedTexts(drawList, pedName.c_str(), namePos, 14.0f, IM_COL32(255, 255, 255, 255), true);
			}
		}

		if (Cheats::Esp::idEsp && (lod_full || lod_medium)) {
			ImColor color = ConvertToImColor(Cheats::Esp::idEspColor);
			std::string idText = std::to_string(ped.cachedId);
			int pos = Cheats::Esp::idEspPos;

			if (pos < 0 || pos > 7) pos = 6;
			ImVec2 idPos = getEspPosition(pos, idText, posOffsets[pos]);
			DrawShadowedTexts(drawList, idText, idPos + ImVec2(1, 1), 15.0f, ImGui::GetColorU32(ImVec4(0, 0, 0, 150)), true);
			DrawShadowedTexts(drawList, idText, idPos, 15.0f, color, true);
		}

		if (Cheats::Esp::weaponEsp && lod_full) {
			ImColor color = ConvertToImColor(Cheats::Esp::weaponEspColor);
			std::string weaponText = getWeaponName(ped.getWeapon());
			int pos = Cheats::Esp::weaponEspPos;

			if (pos < 0 || pos > 7) pos = 3;
			ImVec2 weaponPos = getEspPosition(pos, weaponText, posOffsets[pos]);
			DrawShadowedTexts(drawList, weaponText, weaponPos + ImVec2(1, 1), 15.0f, ImColor(0, 0, 0, 150), true);
			DrawShadowedTexts(drawList, weaponText, weaponPos, 15.0f, color, true);
		}

		if (Cheats::Esp::distanceEsp) {
			ImColor color = ConvertToImColor(Cheats::Esp::distanceEspColor);
			std::string dataText = "[" + std::to_string((int)pDistance) + "m]";
			int pos = Cheats::Esp::distanceEspPos;

			if (pos < 0 || pos > 7) pos = 7;
			ImVec2 distancePos = getEspPosition(pos, dataText, posOffsets[pos]);
			DrawShadowedTexts(ImGui::GetBackgroundDrawList(), dataText, distancePos + ImVec2(1, 1), 15.0f, ImGui::GetColorU32(ImVec4(0, 0, 0, 150)), true);
			DrawShadowedTexts(ImGui::GetBackgroundDrawList(), dataText, distancePos, 15.0f, color, true);
		}


		if (Cheats::Esp::genderEsp && ped.isPlayer()) {
			std::string genderText = "";
			ImColor genderColor;

			if (ped.isFemale()) {
				genderText = "[F]";
				genderColor = ImColor(255, 105, 180, 255);
			}
			else if (ped.isMale()) {
				genderText = "[M]";
				genderColor = ImColor(0, 150, 255, 255);
			}

			if (!genderText.empty()) {

				ImVec2 genderPos = ImVec2(boxRight + 5.0f, pTop);


				DrawShadowedTexts(drawList, genderText, genderPos + ImVec2(1, 1), 15.0f, ImGui::GetColorU32(ImVec4(0, 0, 0, 150)), false);
				DrawShadowedTexts(drawList, genderText, genderPos, 15.0f, genderColor, false);
			}
		}

		if (Cheats::Esp::directionEsp && lod_full) {
			ImColor color = ConvertToImColor(Cheats::Esp::directionEspColor);
			ImVec2 screenCenter = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
			ImVec2 directionToPed = ImVec2(pBase.x - screenCenter.x, pBase.y - screenCenter.y);
			float length = sqrt(directionToPed.x * directionToPed.x + directionToPed.y * directionToPed.y);
			directionToPed.x /= length;
			directionToPed.y /= length;

			static float animationTime = 0.0f;
			animationTime += ImGui::GetIO().DeltaTime * 0.10f;
			if (animationTime > 1.0f)
				animationTime = 0.0f;

			float arrowDistanceFromCenter = 100.0f + sin(animationTime * 3.14f) * 10.0f;
			ImVec2 arrowPos = ImVec2(screenCenter.x + directionToPed.x * arrowDistanceFromCenter, screenCenter.y + directionToPed.y * arrowDistanceFromCenter);
			ImVec2 arrowEndPos = ImVec2(screenCenter.x + directionToPed.x * (arrowDistanceFromCenter + 20.0f), screenCenter.y + directionToPed.y * (arrowDistanceFromCenter + 20.0f));
			ImColor animatedColor = color;
			animatedColor.Value.w = 0.8f + sin(animationTime * 6.28f) * 0.2f;
			DrawArrowTriangleOutlined(drawList, arrowPos, arrowEndPos, animatedColor, 2.0f);
		}
	}


	if (Cheats::Aim::showLine && Cheats::Aim::silent) {
		Vector3 closestPos;
		uintptr_t targetPtr;
		{
			std::lock_guard<std::mutex> lock(globalClosestBone.mutex);
			closestPos = globalClosestBone.position;
			targetPtr = globalClosestBone.targetPointer;
		}

		if (targetPtr != 0 && !vec3Empty(closestPos)) {
			float effectiveSilentFov = Cheats::Aim::dynamic_fov_on && currentDynamicFov > 0.0f ? currentDynamicFov : static_cast<float>(Cheats::Aim::silentFov);
			Vector2 boneScreen;
			if (worldToScreen(viewMatrix, closestPos, boneScreen)) {
				float distToCrosshair = GetCrosshairDistance(boneScreen);
				if (distToCrosshair <= effectiveSilentFov) {
					float centerX = Game.lpRect.right / 2.f;
					float centerY = Game.lpRect.bottom / 2.f;
					ImColor lineColor = ConvertToImColor(Cheats::Aim::showLineColor);
					drawList->AddLine(ImVec2(centerX, centerY), ImVec2(boneScreen.x, boneScreen.y), lineColor, 1.5f);
				}
			}
		}
	}
}

struct VehicleWheelData {
	uintptr_t vehiclePtr;
	Vector3 wheelPositions[4];
	bool valid;
};

void vehicleEsp() {

	bool showOnlySelected = (selectedVehiclePointer != 0 && isMenuVisible);

	if (!Cheats::Vehicle::vehicleEsp && !showOnlySelected)
		return;

	uintptr_t localplayer = ReadMemory<uintptr_t>(gameWorld + 0x8);
	uintptr_t replayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);

	if (replayInterface) {
		uintptr_t vehicleInterface = ReadMemory<DWORD64>(replayInterface + 0x10);
		uintptr_t vehicleList = ReadMemory<DWORD64>(vehicleInterface + 0x180);
		int vehicleListCount = ReadMemory<int>(vehicleInterface + 0x188);
		Vector3 getCordLocal = ReadMemory<Vector3>(localplayer + 0x90);
		Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);

		for (int i = 0; i < vehicleListCount; ++i) {
			uintptr_t vehicle = ReadMemory<uintptr_t>(vehicleList + (i * 0x10));
			if (!vehicle) continue;

			bool isSelectedVehicle = (selectedVehiclePointer != 0 && vehicle == selectedVehiclePointer);

			if (showOnlySelected && !isSelectedVehicle) continue;

			float vehicleHealth = ReadMemory<float>(vehicle + Offsets.Health);
			float vehicleMaxHealth = 1000;

			Vector3 getCorrdinateVehicle = ReadMemory<Vector3>(vehicle + 0x90);
			Vector2 vehicleLocation;

			if (!worldToScreen(viewMatrix, getCorrdinateVehicle, vehicleLocation))
				continue;

			Vector3 distanceCalculation = getCordLocal - getCorrdinateVehicle;
			double Distance = sqrtf(distanceCalculation.x * distanceCalculation.x + distanceCalculation.y * distanceCalculation.y + distanceCalculation.z * distanceCalculation.z);

			if (Distance <= 1250.0f) {
				if ((int)Distance == 0 && !Cheats::Vehicle::drawLocalVehicle && !isSelectedVehicle)
					continue;

				if ((int)Distance >= 1 && !Cheats::Vehicle::drawEnemyVehicle && !isSelectedVehicle)
					continue;

				if (vehicleLocation.x == 0 || vehicleLocation.y == 0)
					continue;

				if (isSelectedVehicle) {

					ImColor selectedColor = ImColor(255, 165, 0, 255);
					float boxSize = 60.0f;
					ImVec2 topLeft(vehicleLocation.x - boxSize / 2, vehicleLocation.y - boxSize / 2);
					ImVec2 bottomRight(vehicleLocation.x + boxSize / 2, vehicleLocation.y + boxSize / 2);

					static float pulseTime = 0.0f;
					pulseTime += ImGui::GetIO().DeltaTime * 3.0f;
					float pulse = (sin(pulseTime) + 1.0f) * 0.5f;
					float thickness = 2.0f + pulse * 2.0f;

					ImGui::GetBackgroundDrawList()->AddRect(topLeft, bottomRight, selectedColor, 5.0f, 0, thickness);

					std::string selectedText = "SELECTED VEHICLE";
					DrawShadowedTexts(ImGui::GetBackgroundDrawList(), selectedText,
						ImVec2(vehicleLocation.x, vehicleLocation.y - 50), 16.f,
						ImGui::GetColorU32(ImVec4(1.0f, 0.65f, 0.0f, 1.0f)), true);
				}

				if (Cheats::Vehicle::vehicleHealt || isSelectedVehicle) {
					float healthPercentage = vehicleHealth / vehicleMaxHealth;
					float barWidth = 50.0f;
					float barHeight = 5.0f;
					float filledWidth = barWidth * healthPercentage;

					ImColor healthColor = ImColor(255 - (int)(255 * healthPercentage), (int)(255 * healthPercentage), 0, 255);
					ImColor backgroundColor = ImColor(0, 0, 0, 255);
					ImVec2 barPosition(vehicleLocation.x - barWidth / 2, vehicleLocation.y - 20);
					ImVec2 barFilled(barPosition.x + filledWidth, barPosition.y + barHeight);

					ImGui::GetBackgroundDrawList()->AddRectFilled(barPosition, ImVec2(barPosition.x + barWidth, barPosition.y + barHeight), backgroundColor);
					ImGui::GetBackgroundDrawList()->AddRectFilled(barPosition, barFilled, healthColor);
				}

				if (Cheats::Vehicle::vehicleEspShowDistance || isSelectedVehicle) {

					std::string vehicleDistanceText = std::to_string((int)(Distance * 0.2f)) + " m";
					ImColor distanceColor = isSelectedVehicle ? ImColor(255, 165, 0, 255) : ImColor(255, 255, 255, 255);
					DrawShadowedTexts(ImGui::GetBackgroundDrawList(), vehicleDistanceText,
						ImVec2(vehicleLocation.x, vehicleLocation.y + 20), 14.f,
						ImGui::GetColorU32(ImVec4(distanceColor.Value.x, distanceColor.Value.y, distanceColor.Value.z, 1.0f)), true);
				}

				if (Cheats::Vehicle::vehicleEspSnapline || isSelectedVehicle) {
					ImColor snaplineColor = isSelectedVehicle ? ImColor(255, 165, 0, 255) : ImColor(255, 255, 255, 255);
					float snaplineThickness = isSelectedVehicle ? 2.0f : 1.0f;
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, 0),
						ImVec2(vehicleLocation.x, vehicleLocation.y),
						ImGui::ColorConvertFloat4ToU32(snaplineColor), snaplineThickness);
				}

				if (Cheats::Vehicle::vehicleMarker || isSelectedVehicle) {
					ImColor markerColor = isSelectedVehicle ? ImColor(255, 165, 0, 255) : ImColor(255, 0, 0, 255);
					float markerSize = isSelectedVehicle ? 6.0f : 4.0f;
					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(vehicleLocation.x, vehicleLocation.y + 45), markerSize, ImColor(0, 0, 0, 100), 100, 3.0f);
					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(vehicleLocation.x, vehicleLocation.y + 45), markerSize - 2, markerColor, 100, 3.0f);
				}

				uint32_t lockStatus = ReadMemory<uint32_t>(vehicle + Offsets.VehicleDoorsLockState);
				bool isLocked = (lockStatus == 2);

				const static std::string VehicleLockedText = "Unlock";
				const static std::string VehicleUnlockedText = "Lock";
				const static int fovRadius = 80;
				const static int L_Key = 0x4C;
				const static int KeyDelay = 500;

				float centerX = Game.lpRect.right / 2.f;
				float centerY = Game.lpRect.bottom / 2.f;
				int FovSize = std::hypot(vehicleLocation.x - centerX, vehicleLocation.y - centerY);

				if (FovSize < fovRadius)
				{
					Vehicle vehicle_instance(vehicle);

					if (isLocked)
					{
						DrawShadowedTexts(ImGui::GetBackgroundDrawList(), VehicleLockedText, ImVec2(vehicleLocation.x, vehicleLocation.y), 14.f, ImColor(255, 255, 255), true);

						if (Utils::KeyPressedWithDelay(L_Key, KeyDelay))
							vehicle_instance.DoorState(true);
					}
					else
					{
						DrawShadowedTexts(ImGui::GetBackgroundDrawList(), VehicleUnlockedText, ImVec2(vehicleLocation.x, vehicleLocation.y), 14.f, ImColor(255, 255, 255), true);

						if (Utils::KeyPressedWithDelay(L_Key, KeyDelay))
							vehicle_instance.DoorState(false);
					}
				}
			}
		}
	}
}

void UpdatePeds() {
	const int maxPlayerCount = 500;

	while (exitLoop == false) {
		try {
			gameWorld = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.GameWorld);
			viewPort = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ViewPort);
			uintptr_t replayInterface = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.ReplayInterface);

			if (!gameWorld || !replayInterface) {
				Sleep(1000);
				continue;
			}

			localPlayer.pointer = ReadMemory<uintptr_t>(gameWorld + Offsets.LocalPlayer);
			if (!localPlayer.pointer) {
				Sleep(100);
				continue;
			}

			uintptr_t entityListPtr = ReadMemory<uintptr_t>(replayInterface + 0x18);
			if (!entityListPtr) {
				Sleep(100);
				continue;
			}

			uintptr_t entityList = ReadMemory<uintptr_t>(entityListPtr + 0x100);
			if (!entityList) {
				Sleep(100);
				continue;
			}

			std::vector<Ped> updatedPedList;
			updatedPedList.reserve(128);

			for (int i = 0; i < maxPlayerCount; i++) {
				try {
					uintptr_t player = ReadMemory<uintptr_t>(entityList + (i * 0x10));
					if (!player) continue;
					if (player == localPlayer.pointer) continue;

					Ped ped;
					if (!ped.getPlayer(player)) continue;
					if (!ped.update()) continue;

					updatedPedList.push_back(ped);
				}
				catch (...) {
					continue;
				}
			}


			{
				std::lock_guard<std::mutex> lock(pedListMutex);
				pedList.swap(updatedPedList);
			}

			Sleep(16);
		}
		catch (...) {
			Sleep(16);
		}
	}
}

uint32_t CalCulateOffsets(uint64_t currentAddress, uint64_t targetAddress, int offset = 0x000000000000000A + 0x0000000000000205 + 0x0000000000000805 - 0x0000000000000A0F) {
	intptr_t relativeOffset = static_cast<intptr_t>(targetAddress - (currentAddress + offset));
	return static_cast<uint32_t>(relativeOffset);
}

namespace CamBypass {

	#define CAM_HEADING_PATTERN "40 53 48 83 EC ? 48 8B 99 ? ? ? ? 48 8B 0D ? ? ? ? 0F 29 74 24 ? 44 0F 29 44 24"

	static bool running = false;
	static float lastRotZ = 0.0f;
	static LPVOID shellcode = nullptr;
	static LPVOID headingPtr = nullptr;
	static uint8_t original[14] = { 0 };
	static uintptr_t camHeadingOffset = 0;

	uintptr_t FindCamHeadingOffset() {
		if (camHeadingOffset != 0) return camHeadingOffset;

		std::vector<uint8_t> pattern = Pattern2Vector(CAM_HEADING_PATTERN);
		uintptr_t addr = FindSignature(pattern, Offsets.GameBase, 0x7FFFFFFF);

		if (addr != 0) {
			camHeadingOffset = addr - Offsets.GameBase;
		}

		return camHeadingOffset;
	}

	bool Hook() {
		if (shellcode) return true;

		uintptr_t offset = FindCamHeadingOffset();
		if (offset == 0) {
			return false;
		}

		uintptr_t func = Offsets.GameBase + offset;

		headingPtr = VirtualAllocEx(Game.hProcess, NULL, 8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!headingPtr) {
			return false;
		}

		float initVals[2] = { 0.0f, 0.001f };
		SIZE_T n;
		WriteProcessMemory(Game.hProcess, headingPtr, initVals, 8, &n);

		uint8_t code[] = {
			0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xF3, 0x0F, 0x10, 0x00,
			0xF3, 0x0F, 0x10, 0x48, 0x04,
			0xF3, 0x0F, 0x58, 0xC1,
			0xF3, 0x0F, 0x11, 0x00,
			0xC3
		};
		memcpy(&code[2], &headingPtr, 8);

		ReadProcessMemory(Game.hProcess, (LPVOID)func, original, 14, &n);

		shellcode = VirtualAllocEx(Game.hProcess, NULL, sizeof(code), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!shellcode) {
			VirtualFreeEx(Game.hProcess, headingPtr, 0, MEM_RELEASE);
			return false;
		}

		WriteProcessMemory(Game.hProcess, shellcode, code, sizeof(code), &n);

		uint8_t jmp[] = { 0x48, 0xB8, 0,0,0,0,0,0,0,0, 0xFF, 0xE0, 0x90, 0x90 };
		memcpy(&jmp[2], &shellcode, 8);
		WriteBytes(func, jmp, 14);

		return true;
	}

	void Unhook() {
		if (!shellcode) return;
		uintptr_t offset = FindCamHeadingOffset();
		if (offset != 0) {
			WriteBytes(Offsets.GameBase + offset, original, 14);
		}
		VirtualFreeEx(Game.hProcess, shellcode, 0, MEM_RELEASE);
		VirtualFreeEx(Game.hProcess, headingPtr, 0, MEM_RELEASE);
		shellcode = nullptr;
		headingPtr = nullptr;
	}

	void SetHeading(float val) {
		if (!headingPtr) return;
		SIZE_T n;
		WriteProcessMemory(Game.hProcess, headingPtr, &val, 4, &n);
	}

	void SyncLoop() {
		uintptr_t camAddr = Offsets.GameBase + Offsets.Camera;

		while (running && !exitLoop) {
			uintptr_t cam = ReadMemory<uintptr_t>(camAddr);
			if (cam) {
				Vector3 rot = ReadMemory<Vector3>(cam + 0x3D0);
				float delta = rot.z - lastRotZ;
				if (fabs(delta) > 0.5f) {
					float currentHeading = 0.0f;
					SIZE_T n;
					ReadProcessMemory(Game.hProcess, headingPtr, &currentHeading, 4, &n);
					currentHeading += delta * 0.0174533f;
					while (currentHeading > 3.14159f) currentHeading -= 6.28318f;
					while (currentHeading < -3.14159f) currentHeading += 6.28318f;
					SetHeading(currentHeading);
				}
				lastRotZ = rot.z;
			}
			Sleep(1);
		}
	}

	void Start() {
		if (running) return;
		if (!Hook()) {
			return;
		}
		running = true;
		std::thread(SyncLoop).detach();
	}

	void Stop() {
		running = false;
		Sleep(50);
		Unhook();
	}
}

void RestoreSilent() {

	if (Offsets.Silent != 0) {
		std::vector<uint8_t> ReWriteTable = { 0xF3, 0x41, 0x0F, 0x10, 0x19, 0xF3, 0x41, 0x0F, 0x10, 0x41, 0x04, 0xF3, 0x41, 0x0F, 0x10, 0x51, 0x08 };
		WriteBytes(Offsets.GameBase + Offsets.Silent, &ReWriteTable[0], ReWriteTable.size());
		std::vector<uint8_t> AngleReWriteTable(33, 0x00);
		WriteBytes(Offsets.GameBase + 0x34E, &AngleReWriteTable[0], AngleReWriteTable.size());
	}
}

void SilentCleanup() {
	RestoreSilent();
	impact_coord_bypass::shutdown();
}

Vector3 EndBulletPos;
void ApplySilent() {

	if (Offsets.Silent == 0) return;


	uint64_t HandleBulletAddress = Offsets.GameBase + Offsets.Silent;
	uint64_t AllocPtr = Offsets.GameBase + 0x34E;

	if (!HandleBulletAddress || !AllocPtr) return;

	auto CalculateRelativeOffset = [](uint64_t CurrentAddress, uint64_t TargetAddress, int Offset = 5) {
		intptr_t RelativeOffset = static_cast<intptr_t>(TargetAddress - (CurrentAddress + Offset));
		return static_cast<uint32_t>(RelativeOffset);
	};

	union { float f; uint32_t i; } EndPosX, EndPosY, EndPosZ;
	EndPosX.f = EndBulletPos.x;
	EndPosY.f = EndBulletPos.y;
	EndPosZ.f = EndBulletPos.z;

	std::vector<uint8_t> ReWriteTable = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	uint32_t JmpOffset = CalculateRelativeOffset(HandleBulletAddress, AllocPtr);
	std::memcpy(&ReWriteTable[1], &JmpOffset, sizeof(JmpOffset));
	WriteBytes(HandleBulletAddress, &ReWriteTable[0], ReWriteTable.size());

	uint32_t jmpOffset = CalculateRelativeOffset(AllocPtr, HandleBulletAddress, 28);
	std::vector<uint8_t> SilentTable = {
		0x41, 0xC7, 0x01, static_cast<uint8_t>(EndPosX.i), static_cast<uint8_t>(EndPosX.i >> 8),
		static_cast<uint8_t>(EndPosX.i >> 16), static_cast<uint8_t>(EndPosX.i >> 24),
		0x41, 0xC7, 0x41, 0x04, static_cast<uint8_t>(EndPosY.i), static_cast<uint8_t>(EndPosY.i >> 8),
		static_cast<uint8_t>(EndPosY.i >> 16), static_cast<uint8_t>(EndPosY.i >> 24),
		0x41, 0xC7, 0x41, 0x08, static_cast<uint8_t>(EndPosZ.i), static_cast<uint8_t>(EndPosZ.i >> 8),
		static_cast<uint8_t>(EndPosZ.i >> 16), static_cast<uint8_t>(EndPosZ.i >> 24),
		0xF3, 0x41, 0x0F, 0x10, 0x19, 0xE9, 0x00, 0x00, 0x00, 0x00
	};
	std::memcpy(&SilentTable[29], &jmpOffset, sizeof(jmpOffset));
	WriteBytes(AllocPtr, &SilentTable[0], SilentTable.size());
}

Ped FindBestTarget(int aimFov, bool isAimbot, bool isSilent, bool isTrigger) {
	Ped bestTarget;
	float minFov = 9999.f;

	std::lock_guard<std::mutex> lock(pedListMutex);
	if (!localPlayer.update()) {
		return bestTarget;
	}
	Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);
	for (auto& ped : pedList) {
		if (!ped.update()) {
			continue;
		}

		float pDistance = getDistance(ped.position, localPlayer.position);
		if (isAimbot && pDistance >= Cheats::Aim::distance)
			continue;
		if (isSilent && pDistance >= Cheats::Aim::silentDistance)
			continue;
		if (isTrigger && pDistance >= Cheats::Aim::distance)
			continue;

		if ((isAimbot && Cheats::Aim::aimbotOnlyVisible && !ped.isVisible()) ||
			(isSilent && Cheats::Aim::silentOnlyVisible && !ped.isVisible()) ||
			(isTrigger && Cheats::Aim::triggerOnlyVisible && !ped.isVisible())) {
			continue;
		}

		if (globals::toggles::aim_ignore_peds && !ped.isPlayer()) {
			continue;
		}

		if ((isAimbot && Cheats::Aim::ignoreDeath && ped.isDead()) ||
			(isSilent && Cheats::Aim::silentIgnoreDeath && ped.isDead()) ||
			(isTrigger && Cheats::Aim::ignoreDeath && ped.isDead())) {
			continue;
		}

		if (IsFriend(ped.cachedId)) {
			continue;
		}

		float closestBoneFov = 9999.f;
		bool isTargetInFov = false;

		for (int i = 0; i < BONE_COUNT; ++i) {
			if (vec3Empty(ped.boneList[i])) continue;

			Vector2 screenPosition;
			if (worldToScreen(viewMatrix, ped.boneList[i], screenPosition)) {
				float fov = GetCrosshairDistance(screenPosition);
				if (fov < aimFov || (isSilent && fov <= (float)aimFov)) {
					isTargetInFov = true;

					if (fov < closestBoneFov) {
						closestBoneFov = fov;
					}
				}
			}
		}

		eAnimBoneTag extendedBones[] = {
			BONETAG_R_FOREARM, BONETAG_L_FOREARM
		};

		for (int i = 0; i < sizeof(extendedBones) / sizeof(extendedBones[0]); i++) {
			Vector3 bonePos = ped.getExtendedBonePosition(extendedBones[i]);
			if (!vec3Empty(bonePos)) {
				Vector2 screenPosition;
				if (worldToScreen(viewMatrix, bonePos, screenPosition)) {
					float fov = GetCrosshairDistance(screenPosition);
					if (fov < aimFov || (isSilent && fov <= (float)aimFov)) {
						isTargetInFov = true;

						if (fov < closestBoneFov) {
							closestBoneFov = fov;
						}
					}
				}
			}
		}

		if (isTargetInFov && closestBoneFov < minFov) {
			minFov = closestBoneFov;
			bestTarget = ped;
		}
	}
	return bestTarget;
}


inline Vector3 ResolveSilentAimBone(Ped& target) {
	auto valid = [](const Vector3& v) { return !vec3Empty(v); };

	Vector3 result{};
	switch (Cheats::Aim::SlientSlectedType) {
	case 0: result = target.boneList[Head]; break;
	case 1: result = target.boneList[Hip]; break;
	case 2: result = target.boneList[Neck]; break;
	case 3: result = target.boneList[LeftHand]; break;
	case 4: result = target.boneList[RightHand]; break;
	case 5:
	{
		Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);
		float best = 99999.f;
		int bones[] = { Head, Neck, Hip, LeftHand, RightHand, LeftFoot, RightFoot, LeftCalf, RightCalf };
		for (int idx : bones) {
			if (!valid(target.boneList[idx])) continue;
			Vector2 sp;
			if (!worldToScreen(viewMatrix, target.boneList[idx], sp)) continue;
			float d = GetCrosshairDistance(sp);
			if (d < best) { best = d; result = target.boneList[idx]; }
		}
		break;
	}
	case 6:
		if (hasGlobalTarget && valid(globalTargetBone))
			result = globalTargetBone;
		else
			result = target.boneList[Head];
		break;
	case 7: result = target.getExtendedBonePosition(BONETAG_L_CLAVICLE); break;
	case 8: result = target.getExtendedBonePosition(BONETAG_R_CLAVICLE); break;
	case 9: result = target.getExtendedBonePosition(BONETAG_SPINE); break;
	default: result = target.boneList[Head]; break;
	}


	if (!valid(result)) result = target.boneList[Head];
	if (!valid(result)) result = target.boneList[Neck];
	if (!valid(result)) result = target.boneList[Hip];
	if (!valid(result)) result = target.position;
	return result;
}

Vector3 GetTargetBoneByType(Ped& target, int boneType) {
	int boneIndices[] = { Head, Hip, Neck, Head, LeftHand, RightHand, LeftFoot, RightFoot,
		LeftElbow, RightElbow, LeftKnee, RightKnee, LeftWrist, RightWrist, LeftCalf, RightCalf };

	if (boneType >= 0 && boneType < 16 && boneType != 3) {
		return target.boneList[boneIndices[boneType]];
	}

	if (boneType == 3) {
		Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);
		float lowestDistance = 9999.f;
		Vector3 closestBone;
		bool found = false;

		int allowedBones[] = {
			Head, LeftFoot, RightFoot, LeftHand, RightHand, Neck, Hip,
			LeftArm, RightArm, LeftElbow, RightElbow, LeftKnee, RightKnee,
			LeftWrist, RightWrist, LeftCalf, RightCalf
		};

		for (int i = 0; i < sizeof(allowedBones) / sizeof(allowedBones[0]); i++) {
			int boneIndex = allowedBones[i];
			if (vec3Empty(target.boneList[boneIndex])) continue;
			Vector2 screenPos;
			if (worldToScreen(viewMatrix, target.boneList[boneIndex], screenPos)) {
				float dist = GetCrosshairDistance(screenPos);
				if (dist <= Cheats::Aim::aimBotFov && dist < lowestDistance) {
					lowestDistance = dist;
					closestBone = target.boneList[boneIndex];
					found = true;
				}
			}
		}
		return found ? closestBone : Vector3();
	}

	return target.boneList[Head];
}

void SetAim() {
	static float lastAimTime = 0.0f;
	static uintptr_t lockedTarget = 0;
	static float lockStartTime = 0.0f;
	static float currentMaxLockTime = 0.0f;
	static int currentBoneType = 0;
	static int shotsOnCurrentTarget = 0;
	static float lastBoneSwitchTime = 0.0f;
	static Vector3 currentJitter = Vector3();
	static float lastJitterTime = 0.0f;
	static float reactionDelayTimer = 0.0f;
	static bool waitingForReaction = false;
	static bool hooksInitialized = false;
	static bool camBypassStarted = false;

	while (exitLoop == false) {
		if (!camBypassStarted) {
			CamBypass::Start();
			camBypassStarted = true;
		}

		if (Cheats::Aim::aimBot) {
			float currentTime = static_cast<float>(GetTickCount64()) / 1000.0f;

			if (Cheats::Aim::legitAim) {
				globals::legit_aim::CleanupCooldowns(currentTime);
			}

			if (GetAsyncKeyState(Cheats::Aim::aimBotKey) & 0x8000) {
				Ped target = FindBestTarget(Cheats::Aim::aimBotFov, true, false, false);

				if (!target.pointer || vec3Empty(target.boneList[Head])) {
					lockedTarget = 0;
					waitingForReaction = false;
					Sleep(1);
					continue;
				}

				if (Cheats::Aim::legitAim) {
					if (globals::legit_aim::IsTargetOnCooldown(target.pointer, currentTime)) {
						lockedTarget = 0;
						Sleep(1);
						continue;
					}

					if (lockedTarget != target.pointer) {
						if (currentTime - globals::legit_aim::last_target_switch_time < Cheats::Aim::legitSwitchDelay) {
							Sleep(1);
							continue;
						}

						waitingForReaction = true;
						reactionDelayTimer = currentTime;
						lockedTarget = target.pointer;
						lockStartTime = currentTime;
						currentMaxLockTime = globals::legit_aim::GetRandomLockTime();
						shotsOnCurrentTarget = 0;
						currentBoneType = Cheats::Aim::aimbotSelectedType;
						globals::legit_aim::last_target_switch_time = currentTime;
					}

					if (waitingForReaction) {
						if (currentTime - reactionDelayTimer < Cheats::Aim::legitReactionDelay) {
							Sleep(1);
							continue;
						}
						waitingForReaction = false;
					}

					float lockDuration = currentTime - lockStartTime;
					if (lockDuration > currentMaxLockTime) {
						globals::legit_aim::AddTargetCooldown(lockedTarget, currentTime);
						lockedTarget = 0;
						Sleep(1);
						continue;
					}

					if (lockDuration > currentMaxLockTime * 0.7f && globals::legit_aim::ShouldBreakLock()) {
						globals::legit_aim::AddTargetCooldown(lockedTarget, currentTime);
						lockedTarget = 0;
						Sleep(1);
						continue;
					}

					if (Cheats::Aim::legitRandomBone && shotsOnCurrentTarget >= Cheats::Aim::legitAim) {
						if (currentTime - lastBoneSwitchTime > 0.5f) {
							int boneOptions[] = { 0, 1, 2, 10, 11 };
							currentBoneType = boneOptions[rand() % 5];
							lastBoneSwitchTime = currentTime;
							shotsOnCurrentTarget = 0;
						}
					}

					if (currentTime - lastJitterTime > 0.08f) {
						lastJitterTime = currentTime;
						float jitterScale = Cheats::Aim::legitJitter * 0.01f;
						currentJitter.x = ((rand() % 200) - 100) / 100.0f * jitterScale;
						currentJitter.y = ((rand() % 200) - 100) / 100.0f * jitterScale;
						currentJitter.z = ((rand() % 100) - 50) / 100.0f * jitterScale * 0.3f;
					}
				}

				uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
				if (!camera) {
					Sleep(1);
					continue;
				}

				Vector3 viewAngle = ReadMemory<Vector3>(camera + 0x3D0);
				Vector3 cameraPosition = ReadMemory<Vector3>(camera + 0x60);

				int boneToUse = Cheats::Aim::legitAim ? currentBoneType : Cheats::Aim::aimbotSelectedType;
				Vector3 targetBone = GetTargetBoneByType(target, boneToUse);

				if (vec3Empty(targetBone)) {
					Sleep(1);
					continue;
				}

				if (Cheats::Aim::legitAim) {
					targetBone.x += currentJitter.x;
					targetBone.y += currentJitter.y;
					targetBone.z += currentJitter.z;
				}

				Vector3 angle = calcAngle(cameraPosition, targetBone);
				normalizeAngles(angle);

				Vector3 delta = angle - viewAngle;
				normalizeAngles(delta);

				float smoothFactor;
				if (Cheats::Aim::legitAim) {
					float deltaLen = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
					float maxDelta = static_cast<float>(Cheats::Aim::aimBotFov) * 0.1f;
					float normalizedDelta = deltaLen / maxDelta;
					if (normalizedDelta > 1.0f) normalizedDelta = 1.0f;

					float curve = globals::legit_aim::EaseInOutCubic(normalizedDelta);
					float baseSmooth = static_cast<float>(Cheats::Aim::aimBotSmooth);
					float humanVariation = 1.0f + ((rand() % 30) - 15) / 100.0f * (1.0f - Cheats::Aim::legitHumanize);

					smoothFactor = baseSmooth * (1.0f + curve * 0.5f) * humanVariation;
					if (smoothFactor < 1.0f) smoothFactor = 1.0f;
				}
				else {
					smoothFactor = static_cast<float>(Cheats::Aim::aimBotSmooth);
				}

				Vector3 writeAngle;
				if (smoothFactor > 0.0f) {
					writeAngle = viewAngle + delta / smoothFactor;
				}
				else {
					writeAngle = angle;
				}

				normalizeAngles(writeAngle);

				if (!vec3Empty(writeAngle)) {
					if (CamBypass::running) {
						WriteMemory<Vector3>(camera + 0x3D0, writeAngle);
					}
					shotsOnCurrentTarget++;
				}
			}
			else {
				if (lockedTarget != 0) {
					lockedTarget = 0;
					waitingForReaction = false;
				}
			}
		}
		static bool wasSilentOn = false;
		if (Cheats::Aim::silent) {
			wasSilentOn = true;
			static bool initialized = false;
			static bool SilentApplied = false;

			if (!initialized) {
				srand(static_cast<unsigned>(time(0)));
				initialized = true;
			}

			HANDLE x9c;
			Ped target = FindBestTarget(Cheats::Aim::silentFov, false, true, false);
			Vector3 SlientType;
			uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
			Vector3 viewAngle = ReadMemory<Vector3>(camera + 0x3D0);
			Vector3 cameraPosition = ReadMemory<Vector3>(camera + 0x60);



			if (target.pointer) {
				if (Cheats::Aim::ignoreFriends && IsFriend(target.getID())) {
					CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RestoreSilent, NULL, NULL, NULL);
					continue;
				}


				target.update();
				SlientType = ResolveSilentAimBone(target);
			}


			if (!vec3Empty(SlientType) && target.pointer) {
				std::lock_guard<std::mutex> lock(globalClosestBone.mutex);
				globalClosestBone.position = SlientType;
				globalClosestBone.targetPointer = target.pointer;
			}

			if (vec3Empty(SlientType) || !target.pointer) {
				impact_coord_bypass::restore();
				if (SilentApplied) {
					RestoreSilent();
					SilentApplied = false;
				}
			}
			else {

				EndBulletPos = SlientType;

				impact_coord_bypass::apply(cameraPosition, viewAngle, EndBulletPos);

				if (GetAsyncKeyState(Cheats::Aim::silentKey) & 0x8000) {

					if (Cheats::Aim::Magic && (GetAsyncKeyState(Cheats::Aim::MagicKey) & 0x8000)) {
						uintptr_t weaponManager = ReadMemory<uintptr_t>(localPlayer.pointer + Offsets.WeaponManager);
						if (weaponManager) {
							uintptr_t weaponObg = ReadMemory<uintptr_t>(weaponManager + 0x78);
							uintptr_t cWeapon = ReadMemory<uintptr_t>(weaponObg + 0x320);
							Vector3 heading = ReadMemory<Vector3>(target.pointer + 0x70);
							float distanceAhead = 5.0f;
							Vector3 forwardVector = { 0.0f, 0.0f, distanceAhead };
							Vector3 spawnPosition = (SlientType + heading + forwardVector);
							if (cWeapon) {
								WriteMemory<Vector3>(cWeapon + 0x20, spawnPosition);
							}
						}
					}

					ApplySilent();
					SilentApplied = true;
				}
				else {
					if (SilentApplied) {
						RestoreSilent();
						SilentApplied = false;
					}
				}
			}
		}
		else {

			if (wasSilentOn) {
				impact_coord_bypass::shutdown();
				wasSilentOn = false;
			}
		}

		Sleep(1);
	}
}
bool isTargetInCrosshair(const Vector2& screenPosition) {
	const float crosshairX = (float)Game.lpRect.right / 2;
	const float crosshairY = (float)Game.lpRect.bottom / 2;
	return (abs(screenPosition.x - crosshairX) <= Cheats::Aim::crosshairTolerance && abs(screenPosition.y - crosshairY) <= Cheats::Aim::crosshairTolerance);
}

void shoot() {
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(Cheats::Aim::triggerDelay);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void TriggerBot() {
	while (exitLoop == false) {
		try {
			if (Cheats::Aim::triggerBot && (GetAsyncKeyState(Cheats::Aim::triggerKey) & 0x8000)) {
				if (!viewPort) {
					Sleep(50);
					continue;
				}

				Matrix viewMatrix = ReadMemory<Matrix>(viewPort + 0x24C);
				Ped target = FindBestTarget(Cheats::Aim::triggerFov, false, false, true);

				if (Cheats::Aim::triggerOnlyVisible && !target.isVisible()) {
					Sleep(1);
					continue;
				}

				if (target.pointer) {
					Vector3 TriggerBoneType;

					switch (Cheats::Aim::triggerBotSelectedType) {
					case 0:
						TriggerBoneType = target.boneList[Head];
						break;
					case 1:
						TriggerBoneType = target.boneList[Hip];
						break;
					case 2:
						TriggerBoneType = target.boneList[Neck];
						break;
					default:
						TriggerBoneType = target.boneList[Head];
						break;
					}

					if (!vec3Empty(TriggerBoneType)) {
						Vector2 screenPosition;
						if (worldToScreen(viewMatrix, TriggerBoneType, screenPosition)) {
							if (isTargetInCrosshair(screenPosition)) {
								shoot();
							}
						}
					}
				}
			}
		}
		catch (...) {

			Sleep(50);
			continue;
		}

		Sleep(8);
	}
}

void TeleportToPosition(Vector3 position) {

	if (!localPlayer.pointer) {
		printa<error>("Cannot teleport: localPlayer.pointer is null");
		return;
	}

	uintptr_t modelInfo = ReadMemory<uintptr_t>(localPlayer.pointer + 0x20);
	uintptr_t navigation = ReadMemory<uintptr_t>(localPlayer.pointer + 0x30);

	if (!modelInfo || !navigation) {
		printa<error>("Cannot teleport: Invalid modelInfo or navigation");
		return;
	}

	TeleportObject(localPlayer.pointer, navigation, modelInfo, position, position, true);

	TPPosition = position;

	entity_list::force_refresh = true;
	entity_list::ped_list.clear();

	printa<hit>("Teleported to position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z));
}

void TeleportToWaypoint() {
	for (int i = 0; i < 2000; i++) {
		uint64_t Blip = ReadMemory<uint64_t>(Offsets.GameBase + Offsets.Waypoint + (i * 8));
		if (!Blip)
			continue;

		int BlipIcon = ReadMemory<int>(Blip + 0x40);


		if (BlipIcon != 8)
			continue;

		Vector2 WaypointPos = ReadMemory<Vector2>(Blip + 0x10);
		if (WaypointPos.x != 0 && WaypointPos.y != 0) {
			uint64_t Object = NULL;
			uint64_t Navigation = NULL;
			uint64_t ModelInfo = NULL;

			if (!Object || !Navigation || !ModelInfo) {
				Object = (uint64_t)localPlayer.pointer;
				Navigation = ReadMemory<uint64_t>(localPlayer.pointer + 0x30);
				ModelInfo = ReadMemory<uint64_t>(localPlayer.pointer + 0x20);
			}

			Vector3 TeleportPos = Vector3(WaypointPos.x, WaypointPos.y, -210.f);
			TeleportObject(Object, Navigation, ModelInfo, TeleportPos, TeleportPos, true);

			entity_list::force_refresh = true;
			entity_list::ped_list.clear();
			return;
		}
	}


	if (Offsets.BlipList) {
		for (int i = 0; i < 2000; i++) {
			uint64_t Blip = ReadMemory<uint64_t>(Offsets.GameBase + Offsets.BlipList + (i * 8));
			if (!Blip) continue;
			if (ReadMemory<int>(Blip + 0x40) != 8) continue;
			Vector2 WaypointPos = ReadMemory<Vector2>(Blip + 0x10);
			if (WaypointPos.x == 0 && WaypointPos.y == 0) continue;
			uintptr_t nav = ReadMemory<uintptr_t>(localPlayer.pointer + 0x30);
			uintptr_t mi = ReadMemory<uintptr_t>(localPlayer.pointer + 0x20);
			TeleportObject(localPlayer.pointer, nav, mi,
				Vector3(WaypointPos.x, WaypointPos.y, -210.f),
				Vector3(WaypointPos.x, WaypointPos.y, -210.f), true);
			return;
		}
	}
}

class CVehicle {
public:
	static void FixLastVehicle() {
		CVehicle* vehicle = reinterpret_cast<CVehicle*>(ReadMemory<uint64_t>(localPlayer.pointer + 0xD10));
		if (vehicle) WriteMemory(reinterpret_cast<uint64_t>(vehicle) + 0x972, (BYTE)0x17);
	}

	static void BreakLastVehicle() {
		CVehicle* vehicle = reinterpret_cast<CVehicle*>(ReadMemory<uint64_t>(localPlayer.pointer + 0xD10));
		if (vehicle) WriteMemory(reinterpret_cast<uint64_t>(vehicle) + 0x972, (BYTE)0x0);
	}

	static void SetDoorLock(int Lock)
	{
		if (!localPlayer.pointer)
			return;

		WriteMemory(Offsets.Vehicle + Offsets.VehicleDoorsLockState, Lock);
	}
};

class CVehicleLegacy {
public:
	static void FixLastVehicle() {
		CVehicleLegacy* vehicle = reinterpret_cast<CVehicleLegacy*>(ReadMemory<uint64_t>(localPlayer.pointer + 0xD10));
		if (vehicle) WriteMemory(reinterpret_cast<uint64_t>(vehicle) + 0x972, (BYTE)0x17);
	}

	static void BreakLastVehicle() {
		CVehicleLegacy* vehicle = reinterpret_cast<CVehicleLegacy*>(ReadMemory<uint64_t>(localPlayer.pointer + 0xD10));
		if (vehicle) WriteMemory(reinterpret_cast<uint64_t>(vehicle) + 0x972, (BYTE)0x0);
	}

};

void MiscFeatures() {
	static bool lastState = false;
	while (exitLoop == false) {
		globals::toggles::hitlog_enabled = Cheats::Misc::hitlogEnabled;
		globals::toggles::hit_sound = Cheats::Misc::hitSound;
		globals::toggles::kill_sound = Cheats::Misc::killSound;
		globals::toggles::hit_particles = Cheats::Misc::hitParticles;
		globals::hit_sound_effect = Cheats::Misc::hitSoundEffect;
		globals::kill_sound_effect = Cheats::Misc::killSoundEffect;
		globals::particle_hit_color = ImVec4(Cheats::Misc::hitParticleColor[0], Cheats::Misc::hitParticleColor[1],
			Cheats::Misc::hitParticleColor[2], Cheats::Misc::hitParticleColor[3]);

		uintptr_t weaponManager = ReadMemory<uintptr_t>(localPlayer.pointer + Offsets.WeaponManager);
		uintptr_t weaponinfo = ReadMemory<uintptr_t>(weaponManager + 0x20);

		TPModelInfo = ReadMemory<uintptr_t>(localPlayer.pointer + 0x20);
		TPPosition = ReadMemory<Vector3>(localPlayer.pointer + 0x90);
		TPNavigation = ReadMemory<uintptr_t>(localPlayer.pointer + 0x30);

		uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
		uintptr_t camera2 = ReadMemory<uintptr_t>(camera + 0x3C0);
		Vector3 camerapos = ReadMemory<Vector3>(camera2 + 0x40);

		if (Cheats::Misc::infiniteAmmo) {
			if (!infiniteAmmoApplied && weaponinfo) {
				uintptr_t ammoInfo = ReadMemory<uintptr_t>(weaponinfo + 0x60);
				if (ammoInfo) {
					uintptr_t ammoCount = ReadMemory<uintptr_t>(ammoInfo + 0x8);
					if (ammoCount) {
						uintptr_t ammoCount2 = ReadMemory<uintptr_t>(ammoCount + 0x0);
						if (ammoCount2) {
							oldAmmoValue = ReadMemory<float>(ammoCount2 + 0x18);
							infiniteAmmoApplied = true;
						}
					}
				}
			}
			if (weaponinfo) {
				uintptr_t ammoInfo = ReadMemory<uintptr_t>(weaponinfo + 0x60);
				if (ammoInfo) {
					uintptr_t ammoCount = ReadMemory<uintptr_t>(ammoInfo + 0x8);
					if (ammoCount) {
						uintptr_t ammoCount2 = ReadMemory<uintptr_t>(ammoCount + 0x0);
						if (ammoCount2) {
							WriteMemory<float>(ammoCount2 + 0x18, 30);
						}
					}
				}
			}
		}
		else if (infiniteAmmoApplied) {
			if (weaponinfo) {
				uintptr_t ammoInfo = ReadMemory<uintptr_t>(weaponinfo + 0x60);
				if (ammoInfo) {
					uintptr_t ammoCount = ReadMemory<uintptr_t>(ammoInfo + 0x8);
					if (ammoCount) {
						uintptr_t ammoCount2 = ReadMemory<uintptr_t>(ammoCount + 0x0);
						if (ammoCount2) {
							WriteMemory<float>(ammoCount2 + 0x18, oldAmmoValue);
						}
					}
				}
			}
			infiniteAmmoApplied = false;
		}
		if (Cheats::Misc::TeleportWaypoint) {
			TeleportToWaypoint();
			Cheats::Misc::TeleportWaypoint = false;
		}

		if (Cheats::Misc::noRecoil) {
			if (!noRecoilApplied && weaponinfo) {
				oldRecoilValue = ReadMemory<float>(weaponinfo + 0x2F4);
				noRecoilApplied = true;
			}
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x2F4, 0.f);
			}
		}
		else if (noRecoilApplied) {
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x2F4, oldRecoilValue);
			}
			noRecoilApplied = false;
		}

		if (Cheats::Misc::noSpread) {
			if (!noSpreadApplied && weaponinfo) {
				oldSpreadValue = ReadMemory<float>(weaponinfo + 0x84);
				noSpreadApplied = true;
			}
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x84, 0.0f);
			}
		}
		else if (noSpreadApplied) {
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x84, oldSpreadValue);
			}
			noSpreadApplied = false;
		}

		if (Cheats::Misc::noRange) {
			if (!noRangeApplied && weaponinfo) {
				oldRangeValue = ReadMemory<float>(weaponinfo + 0x28C);
				noRangeApplied = true;
			}
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x28C, 1000.f);
			}
		}
		else if (noRangeApplied) {
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x28C, oldRangeValue);
			}
			noRangeApplied = false;
		}

		if (Cheats::Misc::noReload) {
			if (!noReloadApplied && weaponinfo) {
				oldReloadValue = ReadMemory<float>(weaponinfo + 0x134);
				noReloadApplied = true;
			}
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x134, 1000);
			}
		}
		else if (noReloadApplied) {
			if (weaponinfo) {
				WriteMemory<float>(weaponinfo + 0x134, oldReloadValue);
			}
			noReloadApplied = false;
		}

		static bool damageBoostKeyPressed = false;
		if (Cheats::Misc::damageBoostKey != 0) {
			bool isKeyDown = GetAsyncKeyState(Cheats::Misc::damageBoostKey) & 0x8000;
			if (isKeyDown && !damageBoostKeyPressed) {
				Cheats::Misc::damageBoost = !Cheats::Misc::damageBoost;
			}
			damageBoostKeyPressed = isKeyDown;
		}

		if (weaponinfo && weaponinfo != lastWeaponInfo) {
			if (damageBoostApplied) {
				WriteMemory<uint32_t>(weaponinfo + 0x120, oldDamageValue);
				WriteMemory<uint32_t>(weaponinfo + 0x124, oldDamageValue2);
				WriteMemory<float>(weaponinfo + 0x12C, oldDamageValue3);
			}
			damageBoostApplied = false;
			lastWeaponInfo = weaponinfo;
		}

		if (Cheats::Misc::damageBoost) {
			if (weaponinfo) {
				if (!damageBoostApplied) {
					oldDamageValue = ReadMemory<uint32_t>(weaponinfo + 0x120);
					oldDamageValue2 = ReadMemory<uint32_t>(weaponinfo + 0x124);
					oldDamageValue3 = ReadMemory<float>(weaponinfo + 0x12C);
					damageBoostApplied = true;
				}

				float damageMultiplier = (float)Cheats::Misc::damageValue;
				uint32_t targetDamage = (uint32_t)damageMultiplier;

				WriteMemory<uint32_t>(weaponinfo + 0x120, targetDamage);
				WriteMemory<uint32_t>(weaponinfo + 0x124, targetDamage);
				WriteMemory<float>(weaponinfo + 0x12C, damageMultiplier);
			}
		}
		else {
			if (damageBoostApplied && weaponinfo) {
				WriteMemory<uint32_t>(weaponinfo + 0x120, oldDamageValue);
				WriteMemory<uint32_t>(weaponinfo + 0x124, oldDamageValue2);
				WriteMemory<float>(weaponinfo + 0x12C, oldDamageValue3);
				damageBoostApplied = false;
			}
		}

		if (Cheats::Misc::reloadAmmo) {
			if (GetAsyncKeyState(Cheats::Misc::reloadAmmoKey) & 0x8000) {
				uintptr_t AmmoInfo = ReadMemory<uintptr_t>(weaponinfo + 0x60);
				uintptr_t AmmoCount = ReadMemory<uintptr_t>(AmmoInfo + 0x8);
				uintptr_t AmmoCount2 = ReadMemory<uintptr_t>(AmmoCount + 0x0);
				WriteMemory<float>(AmmoCount2 + 0x18, Cheats::Misc::reloadValue);
			}
		}

		if (Cheats::Misc::godMode && (GetAsyncKeyState(Cheats::Misc::godModeKey) & 0x8000)) {
			if (!lastState) {
				Beep(1000, 100);
				lastState = true;
			}
			WriteMemory<int>(localPlayer.pointer + 0x189, 1);
		}
		else {
			if (lastState) {
				Beep(500, 100);
				lastState = false;

			}
			WriteMemory<int>(localPlayer.pointer + 0x189, 0);
		}

		if (Cheats::Misc::healthBoost) {
			if (Cheats::Misc::healthBoostKey != 0 && (GetAsyncKeyState(Cheats::Misc::healthBoostKey) & 0x8000)) {

				float targetHealth = 100.0f + static_cast<float>(Cheats::Misc::healthBoostValue);
				WriteMemory<float>(localPlayer.pointer + 0x280, targetHealth);
			}
		}

		if (Cheats::Misc::armorBoost) {
			if (Cheats::Misc::armorBoostKey != 0 && (GetAsyncKeyState(Cheats::Misc::armorBoostKey) & 0x8000)) {
				float targetArmor = static_cast<float>(Cheats::Misc::armorBoostValue);
				float currentArmor = ReadMemory<float>(localPlayer.pointer + Offsets.Armor);
				if (currentArmor < targetArmor) {
					WriteMemory<float>(localPlayer.pointer + Offsets.Armor, targetArmor);
				}
			}
		}

		static bool semiGodModeKeyPressed = false;
		if (Cheats::Misc::semiGodModeKey && (GetAsyncKeyState(Cheats::Misc::semiGodModeKey) & 0x8000)) {
			if (!semiGodModeKeyPressed) {
				semiGodModeKeyPressed = true;
				Cheats::Misc::semiGodMode = !Cheats::Misc::semiGodMode;
			}
		}
		else {
			semiGodModeKeyPressed = false;
		}

		if (Cheats::Misc::semiGodMode) {
			semiGodModeActive = true;
			float currentHealth = ReadMemory<float>(localPlayer.pointer + Offsets.Health);
			if (currentHealth < 197.0f) {
				WriteMemory<float>(localPlayer.pointer + Offsets.Health, 199.0f);
			}
			float currentArmor = ReadMemory<float>(localPlayer.pointer + Offsets.Armor);
			if (currentArmor < 97.0f) {
				WriteMemory<float>(localPlayer.pointer + Offsets.Armor, 99.0f);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		else {
			semiGodModeActive = false;
		}

		static bool invisibleKeyPressed = false;
		if (Cheats::Misc::invisibleKey && (GetAsyncKeyState(Cheats::Misc::invisibleKey) & 0x8000)) {
			if (!invisibleKeyPressed) {
				invisibleKeyPressed = true;
				Cheats::Misc::invisibleEnabled = !Cheats::Misc::invisibleEnabled;
			}
		}
		else {
			invisibleKeyPressed = false;
		}

		static bool bWasInvisible = false;
		if (Cheats::Misc::invisibleEnabled && !bWasInvisible && localPlayer.pointer && !exitLoop) {
			if (Offsets.GameBase && Offsets.m_Invisible) {
				WriteMemory<uintptr_t>(Offsets.GameBase + Offsets.m_Invisible, 0x18598B4822100FF3);
				bWasInvisible = true;
			}
		}
		else if (!Cheats::Misc::invisibleEnabled && bWasInvisible && localPlayer.pointer && !exitLoop) {
			if (Offsets.GameBase && Offsets.m_Invisible) {
				WriteMemory<uintptr_t>(Offsets.GameBase + Offsets.m_Invisible, 0x18598B480A100FF3);
				bWasInvisible = false;
			}
		}

		if (Cheats::Misc::fovChanger) {

			uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
			WriteMemory<float>(ReadMemory<uintptr_t>(camera + 0x10) + 0x30, Cheats::Misc::fovValue);
		}
		else {
			uintptr_t camera = ReadMemory<uintptr_t>(Offsets.GameBase + Offsets.Camera);
			WriteMemory<float>(ReadMemory<uintptr_t>(camera + 0x10) + 0x30, 50);
		}

		{
			static bool wasKeyPressed = false;
			bool isKeyPressed = (GetAsyncKeyState(Cheats::Misc::peekAssistKey) & 0x8000) != 0;

			if (Cheats::Misc::peekAssist && Cheats::Misc::peekAssistKey != 0) {
				if (isKeyPressed && !wasKeyPressed) {
					if (!peekAssistActive) {
						if (localPlayer.update()) {
							Vector3 currentPos = ReadMemory<Vector3>(localPlayer.pointer + 0x90);
							savedPosX = currentPos.x;
							savedPosY = currentPos.y;
							savedPosZ = currentPos.z;
							peekAssistActive = true;
							hasValidPosition = true;
						}
					}
					else if (hasValidPosition) {
						if (localPlayer.update()) {
							uint64_t Object = (uint64_t)localPlayer.pointer;
							uint64_t Navigation = ReadMemory<uint64_t>(localPlayer.pointer + 0x30);
							uint64_t ModelInfo = ReadMemory<uint64_t>(localPlayer.pointer + 0x20);

							if (Object && Navigation && ModelInfo) {
								Vector3 savedPos = Vector3(savedPosX, savedPosY, savedPosZ);
								TeleportObject(Object, Navigation, ModelInfo, savedPos, savedPos, true);
							}
						}
						peekAssistActive = false;
						hasValidPosition = false;
					}
				}
			}

			wasKeyPressed = isKeyPressed;
		}


		{
			static std::thread autoStrafeThread;
			static bool autoStrafeThreadRunning = false;

			if (Cheats::Misc::autoStrafeEnabled && !autoStrafeThreadRunning) {
				autoStrafeThreadRunning = true;
				autoStrafeThread = std::thread([]() {
					const std::array<std::array<int, 5>, 2> autoStrafeKeyCombos = { {
						{VK_SHIFT, 'W', 'A', 'S', 'D'},
						{VK_SHIFT, 'D', 'S', 'A', 'W'}
					} };

					auto GetScanCode = [](WORD vk) -> WORD {
						switch (vk) {
						case 'W': return 0x11;
						case 'A': return 0x1E;
						case 'S': return 0x1F;
						case 'D': return 0x20;
						case VK_SHIFT: return 0x2A;
						default: return 0;
						}
					};

					auto MakeLParam = [](WORD scanCode, bool keyUp) -> LPARAM {
						LPARAM lp = 1;
						lp |= ((LPARAM)scanCode << 16);
						if (keyUp) {
							lp |= (1 << 30);
							lp |= (1 << 31);
						}
						return lp;
					};

					auto PressKey = [&](WORD vk, WORD scan = 0) {
						HWND target = Game.hWnd;
						if (target) PostMessageA(target, WM_KEYDOWN, vk, MakeLParam(scan, false));
					};

					auto ReleaseKey = [&](WORD vk, WORD scan = 0) {
						HWND target = Game.hWnd;
						if (target) PostMessageA(target, WM_KEYUP, vk, MakeLParam(scan, true));
					};

					while (Cheats::Misc::autoStrafeEnabled) {
						if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (Cheats::Misc::autoStrafeBind > 0) && (GetAsyncKeyState(Cheats::Misc::autoStrafeBind) & 0x8000)) {
							int c = Cheats::Misc::autoStrafeCombo;
							if (c < 0 || c > 1) c = 0;
							const auto& keys = autoStrafeKeyCombos[c];

							while ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState(Cheats::Misc::autoStrafeBind) & 0x8000) && Cheats::Misc::autoStrafeEnabled) {
								for (int k = 1; k < 5; ++k) {
									for (int j = 1; j < 5; ++j) {
										if (j != k) ReleaseKey(keys[j], GetScanCode(keys[j]));
									}
									PressKey(keys[k], GetScanCode(keys[k]));
									std::this_thread::sleep_for(std::chrono::milliseconds(Cheats::Misc::autoStrafeInterval));
									ReleaseKey(keys[k], GetScanCode(keys[k]));
								}
							}
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
					autoStrafeThreadRunning = false;
				});
				autoStrafeThread.detach();
			}
		}

		if (Cheats::Vehicle::vehicleFix)
		{
			if (GetAsyncKeyState(Cheats::Vehicle::vehicleFixKey) & 0x8000) {
				CVehicle::FixLastVehicle();
			}
		}

		if (Cheats::Vehicle::vehicleBreak)
		{
			if (GetAsyncKeyState(Cheats::Vehicle::vehicleBreakKey) & 0x8000) {
				CVehicle::BreakLastVehicle();
			}
		}

		if (Cheats::Vehicle::rocketBoost)
		{
			if (GetAsyncKeyState(Cheats::Vehicle::rocketBoostKey) & 0x8000) {
				uintptr_t vehicle = ReadMemory<uintptr_t>(localPlayer.pointer + 0xD10);
				if (vehicle) {
					uintptr_t vehicleModelInfo = ReadMemory<uintptr_t>(vehicle + 0x20);
					if (vehicleModelInfo) {
						WriteMemory<uint16_t>(vehicleModelInfo + 0x58B, 1 << 6);
					}
				}
			}
		}

		{
			static bool gravityWasEnabled = false;
			static bool gravityKeyPressed = false;

			if (Cheats::Vehicle::gravityKey != 0) {
				bool isKeyDown = GetAsyncKeyState(Cheats::Vehicle::gravityKey) & 0x8000;
				if (isKeyDown && !gravityKeyPressed) {
					Cheats::Vehicle::gravity = !Cheats::Vehicle::gravity;
				}
				gravityKeyPressed = isKeyDown;
			}

			uintptr_t gravityOffset = (Offsets.CurrentBuild == 3095 || Offsets.CurrentBuild == 3258) ? 0xC8C : 0xC9C;

			if (Cheats::Vehicle::gravity)
			{
				uintptr_t vehicle = ReadMemory<uintptr_t>(localPlayer.pointer + Offsets.Vehicle);
				if (vehicle) {
					WriteMemory<float>(vehicle + gravityOffset, Cheats::Vehicle::gravityValue);
				}
				gravityWasEnabled = true;
			}
			else if (gravityWasEnabled)
			{
				uintptr_t vehicle = ReadMemory<uintptr_t>(localPlayer.pointer + Offsets.Vehicle);
				if (vehicle) {
					WriteMemory<float>(vehicle + gravityOffset, 9.8f);
				}
				gravityWasEnabled = false;
			}
		}

		{
			if (Cheats::Vehicle::parachute)
			{
				uintptr_t vehicle = ReadMemory<uintptr_t>(localPlayer.pointer + Offsets.Vehicle);
				if (vehicle)
				{
					uint64_t modelInfo = ReadMemory<uint64_t>(vehicle + 0x20);
					if (modelInfo)
					{
						WriteMemory<uint16_t>(modelInfo + 0x058B, 1 << 8);
					}
				}
			}
		}

		{
			static bool vehicleSpeedKeyPressed = false;

			if (Cheats::Vehicle::vehicleSpeedKey != 0) {
				bool currentKeyState = (GetAsyncKeyState(Cheats::Vehicle::vehicleSpeedKey) & 0x8000) != 0;
				if (currentKeyState && !vehicleSpeedKeyPressed) {
					Cheats::Vehicle::vehicleSpeed = !Cheats::Vehicle::vehicleSpeed;
				}
				vehicleSpeedKeyPressed = currentKeyState;
			}

			if (Cheats::Vehicle::vehicleSpeed) {
				uintptr_t vehiclePtr = ReadMemory<uintptr_t>(localPlayer.pointer + Offsets.Vehicle);
				if (vehiclePtr) {
					Vehicle vehicleInstance(vehiclePtr);
					vehicleInstance.set_acceleration(Cheats::Vehicle::vehicleSpeedAmount);
				}
			}
		}


		if (Cheats::Aim::magicBullet && (GetAsyncKeyState(Cheats::Aim::magicBulletKey != 0 ? Cheats::Aim::magicBulletKey : VK_RBUTTON) & 0x8000)) {
			Ped* closestPed = nullptr;
			float closestDistance = FLT_MAX;
			Vector3 localPos = localPlayer.position;

			std::vector<Ped> pedListCopy;
			{
				std::lock_guard<std::mutex> lock(pedListMutex);
				pedListCopy = pedList;
			}

			for (auto& ped : pedListCopy) {
				if (!ped.update() || !ped.isPlayer() || ped.pointer == localPlayer.pointer) continue;
				if (ped.health <= 0 || (ped.position.x == 0 && ped.position.y == 0 && ped.position.z == 0)) continue;

				float dx = localPos.x - ped.position.x;
				float dy = localPos.y - ped.position.y;
				float dz = localPos.z - ped.position.z;
				float distance = sqrtf(dx * dx + dy * dy + dz * dz);
				if (distance < closestDistance && distance < 200.0f) {
					closestDistance = distance;
					closestPed = &ped;
				}
			}

			if (closestPed) {
				uintptr_t weaponMgr = ReadMemory<uintptr_t>(localPlayer.pointer + Offsets.WeaponManager);
				if (weaponMgr != 0) {
					uintptr_t weaponObg = ReadMemory<uintptr_t>(weaponMgr + 0x78);
					if (weaponObg != 0) {
						uintptr_t CWeapon = ReadMemory<uintptr_t>(weaponObg + 0x320);
						if (CWeapon != 0) {
							Vector3 headBone = closestPed->boneList[Head];
							float distanceAhead = 5.f;
							Vector3 forwardVector = { 0.0f, 0.0f, distanceAhead };
							Vector3 spawnPosition = Vector3(headBone.x + forwardVector.x, headBone.y + forwardVector.y, headBone.z + forwardVector.z);
							WriteMemory<Vector3>(CWeapon + 0x20, spawnPosition);
						}
					}
				}
			}
		}

		Sleep(16);
	}
}
