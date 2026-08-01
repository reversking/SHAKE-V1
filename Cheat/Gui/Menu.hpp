#include "../Cheat/Globals.hpp"
#include "../Cheat/Brand.hpp"
REVERS_STAMP(Menu_hpp)
#include "../Cheat/EntityList.hpp"
#include "../Cheat/Print.hpp"
#include <string>
#include <vector>
#include "../Cheat/Ped.hpp"

void TeleportToPosition(Vector3 position);
void esp_tab();
void trigger_tab();
void silent_tab();
void world_tab();
void misc_tab();
void vehicle_tab();
void settings_tab();

extern Vector3 TPPosition;
extern bool TPVehicle;

extern Ped localPlayer;
extern std::vector<Ped> pedList;

namespace game {
	extern Vector3 local_position;
}


namespace Notification {
	enum class NotificationType {
		Success,
		Info,
		Error
	};

	struct NotificationData {
		std::string message;
		float timer;
		NotificationType type;
		float alpha;
		float slideOffset;
	};

	inline std::vector<NotificationData> notifications;
	inline const float NOTIFICATION_DURATION = 3.5f;
	inline const float FADE_SPEED = 3.0f;
	inline const float SLIDE_SPEED = 15.0f;

	inline void Show(const std::string& message, NotificationType type = NotificationType::Success) {
		NotificationData notif;
		notif.message = message;
		notif.timer = NOTIFICATION_DURATION;
		notif.type = type;
		notif.alpha = 1.0f;
		notif.slideOffset = 100.f;
		notifications.push_back(notif);
	}


	inline void Show(const std::string& message, bool isError) {
		Show(message, isError ? NotificationType::Error : NotificationType::Success);
	}

	inline void Render() {
		if (notifications.empty()) return;

		ImDrawList* draw = ImGui::GetForegroundDrawList();
		ImVec2 screenSize = ImGui::GetIO().DisplaySize;

		float yOffset = 24.f;
		float paddingX = 16.f;
		float paddingY = 12.f;
		float cornerRadius = 8.f;
		float minWidth = 280.f;

		for (int i = static_cast<int>(notifications.size()) - 1; i >= 0; i--) {
			auto& notif = notifications[i];


			notif.timer -= ImGui::GetIO().DeltaTime;


			if (notif.slideOffset > 0.f) {
				notif.slideOffset -= ImGui::GetIO().DeltaTime * SLIDE_SPEED * notif.slideOffset;
				if (notif.slideOffset < 0.5f) notif.slideOffset = 0.f;
			}


			if (notif.timer < 0.5f) {
				notif.alpha -= ImGui::GetIO().DeltaTime * FADE_SPEED;
			}


			if (notif.alpha <= 0.0f || notif.timer <= 0.0f) {
				notifications.erase(notifications.begin() + i);
				continue;
			}


			ImVec2 textSize = ImGui::CalcTextSize(notif.message.c_str());
			float boxWidth = max(textSize.x + paddingX * 2 + 36.f, minWidth);
			float boxHeight = textSize.y + paddingY * 2 + 4.f;


			float xPos = screenSize.x - boxWidth - 24.f + notif.slideOffset;
			float yPos = screenSize.y - yOffset - boxHeight;

			ImVec2 boxMin(xPos, yPos);
			ImVec2 boxMax(xPos + boxWidth, yPos + boxHeight);


			ImU32 bgColor, borderColor, accentColor, iconBgColor;
			int alpha = 255;
			int borderAlpha = 255;
			int accentAlpha = 255;

			switch (notif.type) {
				case NotificationType::Success:
					bgColor = IM_COL32(0, 0, 0, 255);
					borderColor = IM_COL32(34, 197, 94, 255);
					accentColor = IM_COL32(34, 197, 94, 255);
					iconBgColor = IM_COL32(34, 197, 94, 100);
					break;
				case NotificationType::Info:
					bgColor = IM_COL32(0, 0, 0, 255);
					borderColor = IM_COL32(255, 20, 147, 255);
					accentColor = IM_COL32(255, 20, 147, 255);
					iconBgColor = IM_COL32(255, 20, 147, 100);
					break;
				case NotificationType::Error:
					bgColor = IM_COL32(0, 0, 0, 255);
					borderColor = IM_COL32(239, 68, 68, 255);
					accentColor = IM_COL32(239, 68, 68, 255);
					iconBgColor = IM_COL32(239, 68, 68, 100);
					break;
			}

			ImU32 textColor = IM_COL32(250, 250, 250, 255);
			ImU32 subtextColor = IM_COL32(161, 161, 170, 255);


			ImU32 shadowColor = IM_COL32(0, 0, 0, 180);
			draw->AddRectFilled(
				ImVec2(boxMin.x + 2, boxMin.y + 4),
				ImVec2(boxMax.x + 2, boxMax.y + 4),
				shadowColor, cornerRadius
			);


			draw->AddRectFilled(boxMin, boxMax, bgColor, cornerRadius);


			draw->AddRect(boxMin, boxMax, borderColor, cornerRadius, 0, 1.f);


			draw->AddRectFilled(
				ImVec2(boxMin.x, boxMin.y + 8),
				ImVec2(boxMin.x + 3, boxMax.y - 8),
				accentColor, 2.f
			);


			float iconSize = 20.f;
			ImVec2 iconCenter(boxMin.x + paddingX + 12.f, boxMin.y + boxHeight / 2.f);
			draw->AddCircleFilled(iconCenter, iconSize / 2.f + 4.f, iconBgColor);


			switch (notif.type) {
				case NotificationType::Success: {

					ImVec2 p1(iconCenter.x - 5, iconCenter.y);
					ImVec2 p2(iconCenter.x - 1, iconCenter.y + 4);
					ImVec2 p3(iconCenter.x + 6, iconCenter.y - 4);
					draw->AddLine(p1, p2, accentColor, 2.5f);
					draw->AddLine(p2, p3, accentColor, 2.5f);
					break;
				}
				case NotificationType::Info: {

					draw->AddLine(
						ImVec2(iconCenter.x, iconCenter.y - 5),
						ImVec2(iconCenter.x, iconCenter.y + 3),
						accentColor, 2.5f
					);
					draw->AddLine(
						ImVec2(iconCenter.x - 4, iconCenter.y - 1),
						ImVec2(iconCenter.x, iconCenter.y + 3),
						accentColor, 2.5f
					);
					draw->AddLine(
						ImVec2(iconCenter.x + 4, iconCenter.y - 1),
						ImVec2(iconCenter.x, iconCenter.y + 3),
						accentColor, 2.5f
					);
					draw->AddLine(
						ImVec2(iconCenter.x - 5, iconCenter.y + 5),
						ImVec2(iconCenter.x + 5, iconCenter.y + 5),
						accentColor, 2.f
					);
					break;
				}
				case NotificationType::Error: {

					draw->AddLine(
						ImVec2(iconCenter.x - 4, iconCenter.y - 4),
						ImVec2(iconCenter.x + 4, iconCenter.y + 4),
						accentColor, 2.5f
					);
					draw->AddLine(
						ImVec2(iconCenter.x + 4, iconCenter.y - 4),
						ImVec2(iconCenter.x - 4, iconCenter.y + 4),
						accentColor, 2.5f
					);
					break;
				}
			}


			ImVec2 textPos(boxMin.x + paddingX + 36.f, boxMin.y + paddingY + 2.f);
			draw->AddText(textPos, textColor, notif.message.c_str());


			float progress = notif.timer / NOTIFICATION_DURATION;
			float progressWidth = (boxWidth - 16.f) * progress;
			draw->AddRectFilled(
				ImVec2(boxMin.x + 8, boxMax.y - 3),
				ImVec2(boxMin.x + 8 + progressWidth, boxMax.y - 1),
				accentColor, 1.f
			);

			yOffset += boxHeight + 12.f;
		}
	}
}

static std::vector<int> playerIDs;
static std::map<int, bool> friendStatus;
static std::vector<int> newPlayerIDs;
static std::vector<int> oldPlayerIDs = playerIDs;
static int selectedPlayerID = -1;
static int selectedItemPlayer = -1;
static std::map<int, std::string> playerNames;
static char searchBuffer[128] = "";
static bool Teleport = false;

static std::vector<uintptr_t> vehiclePointers;
static std::vector<std::string> vehicleNames;
static std::vector<Vector3> vehiclePositions;
static std::vector<bool> vehicleLockStatus;
static std::vector<float> vehicleDistances;
static int selectedVehicleItem = -1;
static uintptr_t selectedVehiclePointer = 0;
static bool TeleportToVehicle = false;
static bool UnlockVehicle = false;
static bool LockVehicle = false;

void initializeMenu() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	{
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	}
}

namespace Cheats {
	int menuKey = VK_INSERT;
	namespace Esp {
		inline bool skeletonEsp = false;
		inline float skeletonEspColor[3] = { 1.0f, 1.0f, 1.0f };
		inline int skeletonEspSize = 1;
		inline bool headEsp = true;

		inline bool boxEsp = false;
		inline float boxEspColor[3] = { 1.0f, 1.0f, 1.0f };

		inline bool useCustomGradient = false;
		inline float boxGradientTopColor[4] = { 1.0f, 1.0f, 1.0f, 0.25f };
		inline float boxGradientBottomColor[4] = { 0.1f, 0.1f, 0.1f, 0.35f };
		inline bool gradientEnabled = false;
		inline float gradientIntensity = 1.0f;

		inline int boxEspSelectedType = 0;
		inline const char* boxEspTypes[2]{ "2D", "Corner" };
		inline float BoxSizeX = 0.75f;

		inline bool lineEsp = false;
		inline float lineEspColor[3] = { 1.0f, 1.0f, 1.0f };
		inline int lineEspSelectedType = 0;
		inline const char* lineEspTypes[3]{ "Top", "Center", "Bottom" };

		inline bool nameEsp = false;
		inline float nameEspColor[3] = { 1.0f, 1.0f, 1.0f };

		inline bool idEsp = false;
		inline float idEspColor[3] = { 1.0f, 1.0f, 1.0f };

		inline bool weaponEsp = false;
		inline float weaponEspColor[3] = { 1.0f, 1.0f, 1.0f };

		inline bool weaponIndicator = false;

		inline bool distanceEsp = false;
		inline float distanceEspColor[3] = { 1.0f, 1.0f, 1.0f };

		inline bool directionEsp = false;
		inline float directionEspColor[3] = { 1.0f, 1.0f, 1.0f };

		inline bool healthBar = false;
		inline bool armorBar = false;
		inline bool radarEsp = false;

		inline bool onlyVisible = false;
		inline bool ignorePed = true;
		inline bool ignoreDeath = true;
		inline int distance = 2000;

		inline int healthBarPos = 1;
		inline int armorBarPos = 0;
		inline int nameEspPos = 2;
		inline int weaponEspPos = 3;
		inline int idEspPos = 2;
		inline int distanceEspPos = 3;


		inline bool genderEsp = false;
		inline float maleColor[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
		inline float femaleColor[4] = { 1.0f, 0.4f, 0.7f, 1.0f };
		inline int genderEspPos = 3;


		inline bool carEsp = false;
	}

	namespace Vehicle {
		bool vehicleEsp = false;
		bool drawLocalVehicle = false;
		bool drawEnemyVehicle = false;
		bool vehicleHealt = false;
		bool vehicleEspShowDistance = false;
		bool vehicleEspSnapline = false;
		bool vehicleMarker = false;
		int distance = 200;
		bool vehicleFix = false;
		int vehicleFixKey;
		bool vehicleBreak = false;
		int vehicleBreakKey;
		bool rocketBoost = false;
		int rocketBoostKey = 0;
		bool stealCar = false;
		bool gravity = false;
		float gravityValue = 9.8f;
		int gravityKey = 0;
		bool parachute = false;

		bool vehicleList = false;
		bool vehicleListMarker = false;
		bool vehicleListName = false;
		bool vehicleListDistance = false;
		bool vehicleListSnapline = false;

		bool vehicleSpeed = false;
		float vehicleSpeedAmount = 1.0f;
		int vehicleSpeedKey = 0;
	};

	namespace Aim {
		bool aimBot = false;
		int aimbotSelectedType = 0;
		const char* aimbotTypes[28]{ "Head", "Body", "Neck", "Closest Bone", "Left Hand", "Right Hand", "Left Foot", "Right Foot", "Left Ankle", "Right Ankle", "Stomach", "Chest", "Left Shoulder", "Right Shoulder", "Left Elbow", "Right Elbow", "Left Knee", "Right Knee", "Spine", "Pelvis", "Left Wrist", "Right Wrist", "Left Thigh", "Right Thigh", "Left Calf", "Right Calf", "Upper Chest", "Lower Chest" };
		int aimBotFov = 150;
		int aimBotSmooth = 5;
		int aimBotKey;

		bool legitAim = false;
		float legitMaxLockTime = 2.5f;
		float legitMinLockTime = 0.8f;
		float legitBreakChance = 15.0f;
		float legitSwitchDelay = 0.3f;
		float legitJitter = 2.5f;
		float legitHumanize = 0.7f;
		float legitReactionDelay = 0.05f;
		float legitCooldown = 1.0f;
		bool legitRandomBone = true;

		bool silent = false;
		int SlientSlectedType = 5;
		const char* SlientTypes[10]{ "Head", "Body", "Neck", "Left Hand", "Right Hand", "Closest Bone", "Global Target", "Left Clavicle", "Right Clavicle", "Spine" };
		bool Pslient = false;

		int espPreviewSelectedBone = 0;

		bool espPreviewSelectedBones[11] = { false, false, false, false, false, false, false, false, false, false, false };
		int silentFov = 20;
		int silentDistance = 500;
		int silentKey;
		bool missChance = false;
		int missChanceValue = 5;
		bool silentIgnoreDeath = false;
		bool dynamic_fov_on = false;
		int dynamicFovKey;

		bool crossHair = false;
		int crossHairSelectedType = 0;
		const char* crossHairTypes[10]{ "Type 1", "Type 2", "Type 3", "Type 4", "Type 5", "Type 6", "Type 7", "Type 8", "Type 9", "Type 10" };
		int crossHairSize = 10;
		float crossHairColor[3] = { 1.0f, 1.0f, 1.0f };

		bool drawAimbotFov = false;
		float drawAimbotFovColor[3] = { 1.0f, 1.0f, 1.0f };

		bool drawSilentFov = false;
		float drawSilentFovColor[3] = { 1.0f, 1.0f, 1.0f };

		bool Magic = false;
		int MagicKey;

		bool ignorePed = true;
		bool ignoreDeath = true;

		bool aimbotIgnorePed = true;
		bool silentIgnorePed = true;
		bool triggerIgnorePed = true;
		int distance = 500;

		bool triggerBot = false;
		int triggerFov = 100;
		int triggerDelay = 1;
		int triggerKey;

		int crosshairTolerance = 5;
		const char* triggerBotTypes[3]{ "Head", "Body", "Neck" };
		int triggerBotSelectedType = 0;

		bool drawTriggerFov = false;
		float drawTriggerFovColor[3] = { 1.0f, 1.0f, 1.0f };

		bool aimbotOnlyVisible = false;
		bool silentOnlyVisible = false;
		bool triggerOnlyVisible = false;
		bool ignoreFriends = false;

		bool magicBullet = false;
		int magicBulletKey = 0;

		bool showTargetBone = false;
		bool showLine = false;
		float showLineColor[3] = { 1.0f, 1.0f, 1.0f };
	}

	namespace Misc {
		bool showInfoBox = true;
		bool showActiveFeatures = true;
		bool streamProof = false;
		bool christmasEnabled = false;

		bool infiniteAmmo = false;
		bool noRecoil = false;
		bool noSpread = false;
		bool noRange = false;
		bool noReload = false;

		bool damageBoost = false;
		int damageValue = 3;
		int damageBoostKey = 0;

		bool reloadAmmo = false;
		int reloadValue = 1;

		int reloadAmmoKey;

		bool TeleportWaypoint = false;
		bool NoClip = false;
		int NoClipSpeed;
		int NoClipKey;

		bool godMode = false;
		int godModeKey;

		bool healthBoost = false;
		float healthBoostValue = 100.0f;
		int healthBoostKey = 0;

		bool armorBoost = false;
		float armorBoostValue = 100.0f;
		int armorBoostKey = 0;

		bool fovChanger = false;
		int fovValue = 50;

		bool peekAssist = false;
		int peekAssistKey = 0;
		float peekEffectColor[4] = { 0.85f, 0.15f, 0.85f, 1.0f };
		float peekEffectSize = 50.0f;


		bool autoStrafeEnabled = false;
		int autoStrafeBind = 0;
		int autoStrafeCombo = 0;
		int autoStrafeInterval = 50;



		bool hitlogEnabled = false;
		bool hitSound = false;
		bool killSound = false;
		bool hitParticles = false;
		int hitSoundEffect = 0;
		int killSoundEffect = 1;
		float hitParticleColor[4] = { 0.51f, 0.51f, 1.0f, 1.0f };

		bool semiGodMode = false;
		int semiGodModeKey = 0;

		bool invisibleEnabled = false;
		int invisibleKey;

	}
}

#include "menu/interface/menu_i.h"
#include "menu/interface/elements_manager.h"
#include "menu/helpers/widgets.h"

void empty_tab() {};
void aimbot_tab()
{
	MGR->esp_p = false;

	{
		static bool general1 = true;
		child("Aimbot Settings", &general1, false);
		{
				checkbox("Enable Aimbot", &Cheats::Aim::aimBot);

				const char* bone_items[] = { "Head", "Body", "Neck", "Closest Bone", "Left Hand", "Right Hand", "Left Foot", "Right Foot", "Left Ankle", "Right Ankle", "Stomach", "Chest", "Left Shoulder", "Right Shoulder", "Left Elbow", "Right Elbow", "Left Knee", "Right Knee", "Spine", "Pelvis", "Left Wrist", "Right Wrist", "Left Thigh", "Right Thigh", "Left Calf", "Right Calf", "Upper Chest", "Lower Chest" };
				static int current_bone = 0;
				if (begincombo("Target Bone", bone_items[current_bone], IM_ARRAYSIZE(bone_items), 1)) {
					ImGuiWindow* window = GetCurrentWindow();
					if (!window) {
						EndCombo();
					}
					else {
						const auto& p = ::GetWindowPos();
						auto curr = anim::get_anim_obj(CONST_HASH("Target Bone"));
						ImVec2 region = ::GetContentRegionMax();
						region.y *= curr->val;

						window->DrawList->PushClipRectFullScreen();
						window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						for (int n = 0; n < IM_ARRAYSIZE(bone_items); n++) {
							bool is_selected = (current_bone == n);
							if (selectable("Target Bone", bone_items[n], n == (IM_ARRAYSIZE(bone_items) - 1), &is_selected)) {
								if (is_selected) {
									current_bone = n;
									Cheats::Aim::aimbotSelectedType = n;
								}
							}
						}
						window->DrawList->PopClipRect();

						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
					}
					EndCombo();
				}

				slider("FOV", &Cheats::Aim::aimBotFov, 1, 360);
				slider("Smooth", &Cheats::Aim::aimBotSmooth, 1, 20);
				keybind("Aimbot Key", &Cheats::Aim::aimBotKey);
				checkbox("Aimbot Visible Only", &Cheats::Aim::aimbotOnlyVisible);
				checkbox("Ignore Death", &Cheats::Aim::ignoreDeath);
				checkbox("Ignore NPCs", &globals::toggles::aim_ignore_peds);
				checkbox("Ignore Friends", &Cheats::Aim::ignoreFriends);
				slider("Max Distance", &Cheats::Aim::distance, 50, 1000);
		}
		end_child("Aimbot Settings");

	}

	{
		ImGui::SetCursorPos(ImVec2(368.f, 8.f));
		static bool general4 = true;
		child("Aimbot Visualization", &general4, false);
		{
				checkbox("Draw Aimbot FOV", &Cheats::Aim::drawAimbotFov);
				color_edit("Aimbot FOV Color", Cheats::Aim::drawAimbotFovColor);
				checkbox("Dynamic FOV", &Cheats::Aim::dynamic_fov_on);
		}
		end_child("Aimbot Visualization");

		ImGui::SetCursorPos(ImVec2(368.f, ImGui::GetCursorPos().y));
		static bool general5 = true;
		child("Crosshair", &general5, false);
		{
				checkbox("Enable Crosshair", &Cheats::Aim::crossHair);

				const char* crosshair_types[] = { "Type 1", "Type 2", "Type 3", "Type 4", "Type 5",
											   "Type 6", "Type 7", "Type 8", "Type 9", "Type 10" };
				static int current_crosshair = 0;
				if (begincombo("Crosshair Type", crosshair_types[current_crosshair], IM_ARRAYSIZE(crosshair_types), 1)) {
					ImGuiWindow* window = GetCurrentWindow();
					if (!window) {
						EndCombo();
					}
					else {
						const auto& p = ::GetWindowPos();
						auto curr = anim::get_anim_obj(CONST_HASH("Crosshair Type"));
						ImVec2 region = ::GetContentRegionMax();
						region.y *= curr->val;

						window->DrawList->PushClipRectFullScreen();
						window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						for (int n = 0; n < IM_ARRAYSIZE(crosshair_types); n++) {
							bool is_selected = (current_crosshair == n);
							if (selectable("Crosshair Type", crosshair_types[n], n == (IM_ARRAYSIZE(crosshair_types) - 1), &is_selected)) {
								if (is_selected) {
									current_crosshair = n;
									Cheats::Aim::crossHairSelectedType = n;
								}
							}
						}
						window->DrawList->PopClipRect();

						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
					}
					EndCombo();
				}

				slider("Crosshair Size", &Cheats::Aim::crossHairSize, 1, 50);
				color_edit("Crosshair Color", Cheats::Aim::crossHairColor);
		}
		end_child("Crosshair");
	}
};

void esp_tab()
{

	MGR->esp_p = true;


	ImGui::BeginChild("##esp_left", ImVec2(360.f, ImGui::GetContentRegionAvail().y), false,
		ImGuiWindowFlags_NoBackground);
	{
		static bool general1 = true;
		child("Skeleton ESP", &general1, false);
		{
			checkbox("Enable Skeleton", &Cheats::Esp::skeletonEsp);
			color_edit("Skeleton Color", Cheats::Esp::skeletonEspColor);
			slider("Skeleton Size", &Cheats::Esp::skeletonEspSize, 1, 5, "%d", 0);
			checkbox("Head ESP", &Cheats::Esp::headEsp);
		}
		end_child("Skeleton ESP");

		static bool general2 = true;
		child("Box ESP", &general2, false);
		{
			checkbox("Enable Box ESP", &Cheats::Esp::boxEsp);

			const char* box_types[] = { "2D", "Corner" };
			static int current_box_type = 0;
			bool combo_enabled = true;
			if (begincombo("Box Type", box_types[current_box_type], IM_ARRAYSIZE(box_types), combo_enabled ? 1 : 0)) {
				if (combo_enabled) {
					ImGuiWindow* window = GetCurrentWindow();
					if (!window) {
						EndCombo();
					}
					else {
						const auto& p = ::GetWindowPos();
						auto curr = anim::get_anim_obj(CONST_HASH("Box Type"));
						ImVec2 region = ::GetContentRegionMax();
						region.y *= curr->val;

						window->DrawList->PushClipRectFullScreen();
						window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						for (int n = 0; n < IM_ARRAYSIZE(box_types); n++) {
							bool is_selected = (current_box_type == n);
							if (selectable("Box Type", box_types[n], n == (IM_ARRAYSIZE(box_types) - 1), &is_selected)) {
								if (is_selected) {
									current_box_type = n;
									Cheats::Esp::boxEspSelectedType = n;
								}
							}
						}
						window->DrawList->PopClipRect();

						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
					}
					EndCombo();
				}
			}

			color_edit("Box Color", Cheats::Esp::boxEspColor);
			static int boxSizeXInt = (int)(Cheats::Esp::BoxSizeX * 100);
			if (slider("Box Size X", &boxSizeXInt, 10, 200, "%d", 0)) {
				Cheats::Esp::BoxSizeX = boxSizeXInt / 100.0f;
			}

			checkbox("Use Gradient", &Cheats::Esp::useCustomGradient);
			if (Cheats::Esp::useCustomGradient && general2) {
				color_edit("Gradient Top", Cheats::Esp::boxGradientTopColor);
				color_edit("Gradient Bottom", Cheats::Esp::boxGradientBottomColor);
				checkbox("Gradient Enabled", &Cheats::Esp::gradientEnabled);
				static int gradientIntensityInt = (int)(Cheats::Esp::gradientIntensity * 100);
				if (slider("Gradient Intensity", &gradientIntensityInt, 0, 200, "%d", 0)) {
					Cheats::Esp::gradientIntensity = gradientIntensityInt / 100.0f;
				}
			}
		}
		end_child("Box ESP");

		static bool general3 = true;
		child("Line ESP", &general3, false);
		{
			checkbox("Enable Line ESP", &Cheats::Esp::lineEsp);

			const char* line_types[] = { "Top", "Center", "Bottom" };
			static int current_line_type = 0;
			bool combo_enabled = true;
			if (begincombo("Line Type", line_types[current_line_type], IM_ARRAYSIZE(line_types), combo_enabled ? 1 : 0)) {
				if (combo_enabled) {
					ImGuiWindow* window = GetCurrentWindow();
					if (!window) {
						EndCombo();
					}
					else {
						const auto& p = ::GetWindowPos();
						auto curr = anim::get_anim_obj(CONST_HASH("Line Type"));
						ImVec2 region = ::GetContentRegionMax();
						region.y *= curr->val;

						window->DrawList->PushClipRectFullScreen();
						window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						for (int n = 0; n < IM_ARRAYSIZE(line_types); n++) {
							bool is_selected = (current_line_type == n);
							if (selectable("Line Type", line_types[n], n == (IM_ARRAYSIZE(line_types) - 1), &is_selected)) {
								if (is_selected) {
									current_line_type = n;
									Cheats::Esp::lineEspSelectedType = n;
								}
							}
						}
						window->DrawList->PopClipRect();

						window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
						window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
						window->DrawList->PopClipRect();

						ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
					}
					EndCombo();
				}
			}

			color_edit("Line Color", Cheats::Esp::lineEspColor);
		}
		end_child("Line ESP");
	}
	ImGui::EndChild();


	ImGui::SameLine(0, 0);
	ImGui::BeginChild("##esp_right", ImVec2(0, ImGui::GetContentRegionAvail().y), false,
		ImGuiWindowFlags_NoBackground);
	{
		static bool general4 = true;
		child("Information ESP", &general4, false);
		{
			checkbox("Name ESP", &Cheats::Esp::nameEsp);
			color_edit("Name Color", Cheats::Esp::nameEspColor);

			checkbox("ID ESP", &Cheats::Esp::idEsp);
			color_edit("ID Color", Cheats::Esp::idEspColor);

			checkbox("Weapon ESP", &Cheats::Esp::weaponEsp);
			color_edit("Weapon Color", Cheats::Esp::weaponEspColor);
			checkbox("Weapon Indicator", &Cheats::Esp::weaponIndicator);

			checkbox("Distance ESP", &Cheats::Esp::distanceEsp);
			color_edit("Distance Color", Cheats::Esp::distanceEspColor);

			slider("ESP Range (m)", &Cheats::Esp::distance, 10, 2000, "%d");

			checkbox("Gender ESP", &Cheats::Esp::genderEsp);
		}
		end_child("Information ESP");

		static bool general5 = true;
		child("Bars & Indicators", &general5, false);
		{
			checkbox("Health Bar", &Cheats::Esp::healthBar);
			checkbox("Armor Bar", &Cheats::Esp::armorBar);
		}
		end_child("Bars & Indicators");

		static bool general6 = true;
		child("ESP Options", &general6, false);
		{
			checkbox("Ignore Death", &Cheats::Esp::ignoreDeath);
			checkbox("Ignore NPCs", &globals::toggles::esp_ignore_peds);

			slider("Max Distance", &Cheats::Esp::distance, 50, 2000, "%d", 0);

			checkbox("Car ESP", &Cheats::Esp::carEsp);
		}
		end_child("ESP Options");

	}
	ImGui::EndChild();
}

void misc_tab()
{
	MGR->esp_p = false;

	{
		static bool peek_assist_section = false;
		child("Peek Assist", &peek_assist_section, false);
		{
			checkbox("Enable Peek Assist", &Cheats::Misc::peekAssist);
			if (Cheats::Misc::peekAssist) {
				keybind("Peek Assist Key", &Cheats::Misc::peekAssistKey);
				color_edit("Peek Effect Color", Cheats::Misc::peekEffectColor);
				static int peekEffectSizeInt = (int)Cheats::Misc::peekEffectSize;
				if (slider("Peek Effect Size", &peekEffectSizeInt, 20, 100)) {
					Cheats::Misc::peekEffectSize = (float)peekEffectSizeInt;
				}
			}
		}
		end_child("Peek Assist");

		static bool weapon_mods_section = false;
		child("Weapon Modifications", &weapon_mods_section, false);
		{
			checkbox("Infinite Ammo", &Cheats::Misc::infiniteAmmo);
			checkbox("No Recoil", &Cheats::Misc::noRecoil);
			checkbox("No Spread", &Cheats::Misc::noSpread);
			checkbox("No Range", &Cheats::Misc::noRange);
			checkbox("No Reload", &Cheats::Misc::noReload);
			checkbox("Safe Damage Boost", &Cheats::Misc::damageBoost);
			if (Cheats::Misc::damageBoost) {
				slider("Damage Value", &Cheats::Misc::damageValue, 1, 5);
				keybind("Damage Boost Key", &Cheats::Misc::damageBoostKey);
			}
		}
		end_child("Weapon Modifications");

		static bool strafe_section = false;
		child("Strafe Macro", &strafe_section, false);
		{
			checkbox("Enable Strafe", &Cheats::Misc::autoStrafeEnabled);
			keybind("Strafe Key", &Cheats::Misc::autoStrafeBind);

			const char* combo_items[] = { "W-A-S-D", "D-S-A-W" };
			if (begincombo("Pattern", combo_items[Cheats::Misc::autoStrafeCombo], 2, 1)) {
				ImGuiWindow* window = GetCurrentWindow();
				if (!window) { EndCombo(); }
				else {
					const auto& p = ::GetWindowPos();
					auto curr = anim::get_anim_obj(CONST_HASH("Pattern"));
					ImVec2 region = ::GetContentRegionMax();
					region.y *= curr->val;
					window->DrawList->PushClipRectFullScreen();
					window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
					window->DrawList->PopClipRect();
					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
					window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
					for (int n = 0; n < 2; n++) {
						bool is_selected = (Cheats::Misc::autoStrafeCombo == n);
						if (selectable("Pattern", combo_items[n], n == 1, &is_selected)) {
							if (is_selected) Cheats::Misc::autoStrafeCombo = n;
						}
					}
					window->DrawList->PopClipRect();
					window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
					window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
					window->DrawList->PopClipRect();
					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
				}
				EndCombo();
			}

			slider("Interval (ms)", &Cheats::Misc::autoStrafeInterval, 10, 200);
		}
		end_child("Strafe Macro");
	}

	{
		ImGui::SetCursorPos(ImVec2(368.f, 8.f));
		static bool general4 = true;
		child("Visual", &general4, false);
		{
			checkbox("FOV Changer", &Cheats::Misc::fovChanger);
			slider("FOV Value", &Cheats::Misc::fovValue, 30, 120);
		}
		end_child("Visual");

		ImGui::SetCursorPos(ImVec2(368.f, ImGui::GetCursorPos().y));
		static bool general3 = true;
		child("Health & Armor", &general3, false);
		{
			checkbox("Health Boost", &Cheats::Misc::healthBoost);

			static int healthValueInt = (int)Cheats::Misc::healthBoostValue;
			if (slider("Health Value", &healthValueInt, 0, 100)) {
				Cheats::Misc::healthBoostValue = (float)healthValueInt;
			}
			keybind("Health Boost Key", &Cheats::Misc::healthBoostKey);

			checkbox("Armor Boost", &Cheats::Misc::armorBoost);

			static int armorValueInt = (int)Cheats::Misc::armorBoostValue;
			if (slider("Armor Value", &armorValueInt, 1, 100)) {
				Cheats::Misc::armorBoostValue = (float)armorValueInt;
			}
			keybind("Armor Boost Key", &Cheats::Misc::armorBoostKey);

			checkbox("Semi God Mode", &Cheats::Misc::semiGodMode);
			keybind("Semi God Mode Key", &Cheats::Misc::semiGodModeKey);

			checkbox("Invisible", &Cheats::Misc::invisibleEnabled);
			keybind("Invisible Key", &Cheats::Misc::invisibleKey);
		}
		end_child("Health & Armor");

	}
}

void vehicle_tab()
{
	MGR->esp_p = false;

	{
		static bool general2 = true;
		child("Vehicle List", &general2, true);
		{

			ImDrawList* draw = ImGui::GetWindowDrawList();
			ImVec2 list_pos = ImGui::GetCursorScreenPos();
			ImVec2 list_size = ImVec2(ImGui::GetContentRegionAvail().x, 400.f);


			draw->AddRectFilled(list_pos, list_pos + list_size, IM_COL32(8, 6, 14, 240), 8.f);
			draw->AddRect(list_pos, list_pos + list_size, IM_COL32(255, 50, 160, 25), 8.f, 0, 1.0f);


			ImGui::BeginChild("##vehiclelist_scroll", list_size, false, ImGuiWindowFlags_NoBackground);
			{
				size_t vehicleCount = vehiclePointers.size();
				size_t nameCount = vehicleNames.size();
				size_t lockCount = vehicleLockStatus.size();
				size_t distCount = vehicleDistances.size();

				for (size_t i = 0; i < vehicleCount; i++) {
					std::string vehicle_name = (i < nameCount) ? vehicleNames[i] : "Vehicle " + std::to_string(i + 1);
					std::string lock_status = (i < lockCount) ? (vehicleLockStatus[i] ? " [LOCKED]" : " [UNLOCKED]") : "";
					std::string distance_text = (i < distCount) ? " (" + std::to_string((int)(vehicleDistances[i] * 0.2f)) + "m)" : "";
					std::string display_name = vehicle_name + distance_text + lock_status;

					const bool is_selected = (selectedVehicleItem == (int)i);

					ImVec2 item_pos = ImGui::GetCursorScreenPos();
					ImVec2 item_size = ImVec2(ImGui::GetContentRegionAvail().x - 10.f, 28.f);
					bool hovered = ImGui::IsMouseHoveringRect(item_pos, item_pos + item_size);


					if (is_selected) {
						draw->AddRectFilled(item_pos, item_pos + item_size, ImColor(255, 105, 180, 40), 4.f);
						draw->AddRect(item_pos, item_pos + item_size, ImColor(255, 105, 180, 120), 4.f, 0, 1.0f);
					} else if (hovered) {
						draw->AddRectFilled(item_pos, item_pos + item_size, ImColor(30, 30, 35, 255), 4.f);
					}


					ImColor text_color = is_selected ? ImColor(255, 255, 255, 255) :
					                     hovered ? ImColor(220, 220, 220, 255) :
					                     ImColor(180, 180, 185, 255);
					draw->AddText(item_pos + ImVec2(10.f, 6.f), text_color, display_name.c_str());


					ImGui::SetCursorScreenPos(item_pos);
					if (ImGui::InvisibleButton(("##vehicle_" + std::to_string(i)).c_str(), item_size)) {
						if (is_selected) {
							selectedVehicleItem = -1;
							selectedVehiclePointer = 0;
						} else {
							selectedVehicleItem = i;
							selectedVehiclePointer = vehiclePointers[i];
						}
					}

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.f);
				}
			}
			ImGui::EndChild();

			ImGui::Dummy(ImVec2(0, 10.f));

			if (selectedVehiclePointer != 0 && selectedVehicleItem >= 0 && selectedVehicleItem < (int)vehiclePointers.size()) {
				const float button_width1 = 100.f;
				const float button_width2 = 100.f;
				const float spacing = 10.f;
				const float total_width = button_width1 + button_width2 + spacing;

				ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - total_width) / 2.f);

				{
					ImVec2 button_size(button_width1, 30.f);
					ImVec2 button_pos = ImGui::GetCursorScreenPos();
					bool hovered = ImGui::IsMouseHoveringRect(button_pos, button_pos + button_size);
					ImColor bg = hovered ? ImColor(255, 105, 180, 200) : ImColor(255, 105, 180, 120);
					ImColor border = hovered ? ImColor(255, 105, 180, 255) : ImColor(255, 105, 180, 150);

					draw->AddRectFilled(button_pos, button_pos + button_size, bg, 6.f);
					draw->AddRect(button_pos, button_pos + button_size, border, 6.f, 0, 1.5f);

					const char* text = "Teleport";
					ImVec2 text_size = ImGui::CalcTextSize(text);
					ImVec2 text_pos = button_pos + ImVec2((button_size.x - text_size.x) / 2.f, (button_size.y - text_size.y) / 2.f);
					draw->AddText(text_pos + ImVec2(1, 1), ImColor(0, 0, 0, 120), text);
					draw->AddText(text_pos, ImColor(255, 255, 255, 255), text);

					ImGui::SetCursorScreenPos(button_pos);
					if (ImGui::InvisibleButton("##teleport_vehicle", button_size)) {
						if (selectedVehicleItem >= 0 && selectedVehicleItem < (int)vehiclePositions.size()) {
							TeleportToPosition(vehiclePositions[selectedVehicleItem]);
							printa<normal>("Teleporting to vehicle...");
						}
					}
				}

				ImGui::SameLine(0, spacing);

				{
					bool isLocked = (selectedVehicleItem >= 0 && selectedVehicleItem < (int)vehicleLockStatus.size()) ? vehicleLockStatus[selectedVehicleItem] : false;
					const char* lock_text = isLocked ? "Unlock" : "Lock";
					ImVec2 button_size(button_width2, 30.f);
					ImVec2 button_pos = ImGui::GetCursorScreenPos();

					bool hovered = ImGui::IsMouseHoveringRect(button_pos, button_pos + button_size);
					ImColor bg = hovered ? ImColor(255, 105, 180, 200) : ImColor(255, 105, 180, 120);
					ImColor border = hovered ? ImColor(255, 105, 180, 255) : ImColor(255, 105, 180, 150);

					draw->AddRectFilled(button_pos, button_pos + button_size, bg, 6.f);
					draw->AddRect(button_pos, button_pos + button_size, border, 6.f, 0, 1.5f);

					ImVec2 text_size = ImGui::CalcTextSize(lock_text);
					ImVec2 text_pos = button_pos + ImVec2((button_size.x - text_size.x) / 2.f, (button_size.y - text_size.y) / 2.f);
					draw->AddText(text_pos + ImVec2(1, 1), ImColor(0, 0, 0, 120), lock_text);
					draw->AddText(text_pos, ImColor(255, 255, 255, 255), lock_text);

					ImGui::SetCursorScreenPos(button_pos);
					if (ImGui::InvisibleButton("##lock_vehicle", button_size)) {
						if (isLocked) {
							UnlockVehicle = true;
						} else {
							LockVehicle = true;
						}
					}
				}
			}
		}
		end_child("Vehicle List");
	}

	{
		ImGui::SetCursorPos(ImVec2(368.f, 8.f));
		static bool general4 = true;
		child("Vehicle Functions", &general4, false);
		{
			checkbox("Fix Vehicle", &Cheats::Vehicle::vehicleFix);
			keybind("Fix Vehicle Key", &Cheats::Vehicle::vehicleFixKey);

			checkbox("Vehicle Speed", &Cheats::Vehicle::vehicleSpeed);
			if (Cheats::Vehicle::vehicleSpeed) {
				static int speedInt = (int)(Cheats::Vehicle::vehicleSpeedAmount * 10);
				if (slider("Speed Amount", &speedInt, 1, 100)) {
					Cheats::Vehicle::vehicleSpeedAmount = speedInt / 10.0f;
				}
				keybind("Vehicle Speed Key", &Cheats::Vehicle::vehicleSpeedKey);
			}
		}
		end_child("Vehicle Functions");
	}
}

void trigger_tab()
{
	MGR->esp_p = false;

	{
		static bool general1 = true;
		child("Trigger Bot Settings", &general1, false);
		{
			checkbox("Enable Trigger Bot", &Cheats::Aim::triggerBot);

			const char* trigger_bones[] = { "Head", "Body", "Neck" };
			static int current_trigger_bone = 0;
			if (begincombo("Trigger Bone", trigger_bones[current_trigger_bone], IM_ARRAYSIZE(trigger_bones), 1)) {
				ImGuiWindow* window = GetCurrentWindow();
				if (!window) {
					EndCombo();
				}
				else {
					const auto& p = ::GetWindowPos();
					auto curr = anim::get_anim_obj(CONST_HASH("Trigger Bone"));
					ImVec2 region = ::GetContentRegionMax();
					region.y *= curr->val;

					window->DrawList->PushClipRectFullScreen();
					window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
					window->DrawList->PopClipRect();

					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
					window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
					for (int n = 0; n < IM_ARRAYSIZE(trigger_bones); n++) {
						bool is_selected = (current_trigger_bone == n);
						if (selectable("Trigger Bone", trigger_bones[n], n == (IM_ARRAYSIZE(trigger_bones) - 1), &is_selected)) {
							if (is_selected) {
								current_trigger_bone = n;
								Cheats::Aim::triggerBotSelectedType = n;
							}
						}
					}
					window->DrawList->PopClipRect();

					window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
					window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
					window->DrawList->PopClipRect();

					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
				}
				EndCombo();
			}

			slider("Trigger FOV", &Cheats::Aim::triggerFov, 1, 180);
			slider("Trigger Delay", &Cheats::Aim::triggerDelay, 1, 100);
			slider("Tolerance", &Cheats::Aim::crosshairTolerance, 1, 20);
			checkbox("Ignore NPCs", &globals::toggles::aim_ignore_peds);
			checkbox("Trigger Visible Only", &Cheats::Aim::triggerOnlyVisible);
			keybind("Trigger Key", &Cheats::Aim::triggerKey);
		}
		end_child("Trigger Bot Settings");
	}

	{
		ImGui::SetCursorPos(ImVec2(368.f, 8.f));
		static bool general4 = true;
		child("Trigger Visualization", &general4, false);
		{
			checkbox("Draw Trigger FOV", &Cheats::Aim::drawTriggerFov);
			color_edit("Trigger FOV Color", Cheats::Aim::drawTriggerFovColor);
		}
		end_child("Trigger Visualization");
	}
}

void silent_tab()
{
	MGR->esp_p = false;

	{
		static bool general1 = true;
		child("Silent Aim Settings", &general1, false);
		{
			checkbox("Enable Silent Aim", &Cheats::Aim::silent);

			const char* silent_bones[] = { "Head", "Body", "Neck", "Left Hand", "Right Hand", "Closest Bone", "Global Target", "Left Clavicle", "Right Clavicle", "Spine" };
			static int current_silent_bone = 5;
			if (begincombo("Target Bone", silent_bones[current_silent_bone], IM_ARRAYSIZE(silent_bones), 1)) {
				ImGuiWindow* window = GetCurrentWindow();
				if (!window) {
					EndCombo();
				}
				else {
					const auto& p = ::GetWindowPos();
					auto curr = anim::get_anim_obj(CONST_HASH("Target Bone"));
					ImVec2 region = ::GetContentRegionMax();
					region.y *= curr->val;

					window->DrawList->PushClipRectFullScreen();
					window->DrawList->AddRectFilled(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, -4.f), IM_COL32(6, 4, 12, 245), 6.f, ImDrawFlags_RoundCornersBottom);
					window->DrawList->PopClipRect();

					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
					window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
					for (int n = 0; n < IM_ARRAYSIZE(silent_bones); n++) {
						bool is_selected = (current_silent_bone == n);
						if (selectable("Target Bone", silent_bones[n], n == (IM_ARRAYSIZE(silent_bones) - 1), &is_selected)) {
							if (is_selected) {
								current_silent_bone = n;
								Cheats::Aim::SlientSlectedType = n;
							}
						}
					}
					window->DrawList->PopClipRect();

					window->DrawList->PushClipRect(p - ImVec2(0.f, 3.f), p + region + ImVec2(4.f, 0.f));
					window->DrawList->AddRect(p - ImVec2(0.f, 4.f), p + region + ImVec2(4.f, -4.f), IM_COL32(255, 50, 160, 40), 6.f, ImDrawFlags_RoundCornersBottom);
					window->DrawList->PopClipRect();

					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 1.f);
				}
				EndCombo();
			}

			slider("Silent FOV", &Cheats::Aim::silentFov, 1, 800);
			slider("Distance (m)", &Cheats::Aim::silentDistance, 10, 500);
			checkbox("Ignore NPCs", &globals::toggles::aim_ignore_peds);
			checkbox("Ignore Death", &Cheats::Aim::silentIgnoreDeath);
			keybind("Silent Key", &Cheats::Aim::silentKey);
			checkbox("Miss Chance", &Cheats::Aim::missChance);
			if (Cheats::Aim::missChance) {
				slider("Miss Chance Value (%)", &Cheats::Aim::missChanceValue, 1, 10, "%d%");
			}
			checkbox("Dynamic FOV", &Cheats::Aim::dynamic_fov_on);
			if (Cheats::Aim::dynamic_fov_on) {
				keybind("Dynamic FOV Key", &Cheats::Aim::dynamicFovKey);
			}
			checkbox("Show Line", &Cheats::Aim::showLine);
			if (Cheats::Aim::showLine) {
				color_edit("Line Color", Cheats::Aim::showLineColor);
			}
		}
		end_child("Silent Aim Settings");
	}

	{
		ImGui::SetCursorPos(ImVec2(368.f, 8.f));
		static bool general4 = true;
		child("Silent Visualization", &general4, false);
		{
				checkbox("Draw Silent FOV", &Cheats::Aim::drawSilentFov);
				color_edit("Silent FOV Color", Cheats::Aim::drawSilentFovColor);
				checkbox("Magic Bullet", &Cheats::Aim::Magic);
				if (Cheats::Aim::Magic) {
					keybind("Magic Bullet Key", &Cheats::Aim::MagicKey);
				}
		}
		end_child("Silent Visualization");
	}
}

void world_tab()
{
	MGR->esp_p = false;


	static const char* tp_locations[] = {
		"Legion Square", "Paleto Bay", "Main LS Customs", "IAA Roof", "FIB Roof",
		"Maze Bank", "Mount Chiliad", "Casino", "Prison", "Military Base", "Void",
		"Central LS Hospital", "Pillbox Hill Medical", "Mount Zonah Medical",
		"Sandy Shores Medical", "Paleto Bay Medical",
		"Rockford Hills Medical", "Vinewood Medical", "Eclipse Medical Tower",
		"Davis Medical Center", "El Burro Heights Hospital", "Hawick Avenue Clinic",
		"Bayview Lodge Hospital", "Thomson Scrapyard Clinic", "Grapeseed Medical Clinic",
		"Torture Room X", "Trevor's Meth Lab", "Pacific Standard Vault",
		"Lester's House", "Strip Club DJ Booth", "Blaine County Savings",
		"Police Station", "Humane Labs Entrance", "Burnt FIB Building",
		"10 Car Garage Back Room", "Humane Labs Tunnel", "Floyd's Apartment"
	};
	static const Vector3 tp_coords[] = {
		{ 190.52f, -873.23f, 31.5f },
		{ -138.52f, 6356.99f, 31.49f },
		{ -365.425f, -131.809f, 37.873f },
		{ 134.085f, -637.859f, 262.851f },
		{ -150.126f, -754.591f, 262.865f },
		{ -75.015f, -818.215f, 326.176f },
		{ 495.0f, 5589.0f, 795.0f },
		{ 911.957f, 38.3382f, 80.7207f },
		{ 1702.08f, 2650.51f, 45.5649f },
		{ -2751.12f, 3316.4f, 32.8117f },
		{ 15000.0f, 15000.0f, 0.0f },
		{ 339.85f, -1394.56f, 32.51f },
		{ 307.87f, -595.55f, 43.28f },
		{ -450.99f, -340.91f, 34.50f },
		{ 1839.6f, 3672.93f, 34.28f },
		{ -247.76f, 6325.57f, 32.43f },
		{ -874.51f, -307.55f, 39.58f },
		{ 639.11f, 1.12f, 82.79f },
		{ -691.28f, 293.03f, 83.16f },
		{ 360.65f, -582.14f, 28.82f },
		{ 1154.02f, -1520.58f, 34.84f },
		{ 305.24f, -1434.0f, 29.8f },
		{ -245.18f, 6229.42f, 31.49f },
		{ 2487.47f, 4960.48f, 44.37f },
		{ 1826.9f, 3685.8f, 34.27f },
		{ 147.170f, -2201.804f, 4.688f },
		{ 1391.773f, 3608.716f, 38.942f },
		{ 255.851f, 217.030f, 101.683f },
		{ 1273.898f, -1719.304f, 54.771f },
		{ 126.135f, -1278.583f, 29.270f },
		{ -109.299f, 6464.035f, 31.627f },
		{ 436.491f, -982.172f, 30.699f },
		{ 3619.749f, 2742.740f, 28.690f },
		{ 160.868f, -745.831f, 250.063f },
		{ 223.193f, -967.322f, 99.000f },
		{ 3525.495f, 3705.301f, 20.992f },
		{ -1150.703f, -1520.713f, 10.633f }
	};
	static int selected_tp_location = -1;


	ImGui::BeginGroup();
	{
		static bool general1 = true;
		child("World Features", &general1, false);
		{
			ImGui::Dummy(ImVec2(0, 10.f));

			float button_width = 160.f;
			float start_x = (ImGui::GetWindowContentRegionMax().x - button_width) / 2.f;
			ImGui::SetCursorPosX(start_x);

			if (neon_button("Teleport to Waypoint", ImVec2(button_width, 30.f))) {
				Cheats::Misc::TeleportWaypoint = true;
				printa<normal>("Teleporting to waypoint...");
			}

			ImGui::Dummy(ImVec2(0, 8.f));


			static float tp_hover_anim[64] = {};
			static float tp_sel_anim[64] = {};
			static char tp_search[64] = "";


			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.04f, 0.02f, 0.06f, 0.9f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.06f, 0.03f, 0.09f, 0.95f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.08f, 0.04f, 0.12f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.2f, 0.63f, 0.15f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 4.0f));
			ImGui::SetNextItemWidth(-1);
			ImGui::InputTextWithHint("##tp_search", "search location...", tp_search, IM_ARRAYSIZE(tp_search));
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(5);

			ImGui::Dummy(ImVec2(0, 4.f));


			float listH = 220.f;
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.02f, 0.01f, 0.04f, 0.85f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.078f, 0.576f, 0.08f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.02f, 0.01f, 0.04f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(1.0f, 0.078f, 0.576f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(1.0f, 0.078f, 0.576f, 0.4f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(1.0f, 0.078f, 0.576f, 0.6f));
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
			ImGui::BeginChild("##tp_list_scroll", ImVec2(-1, listH), true);
			{
				ImDrawList* dl = ImGui::GetWindowDrawList();
				float dt = ImGui::GetIO().DeltaTime;
				float itemH = 30.f;
				float pad = 3.f;
				std::string searchLow = tp_search;
				std::transform(searchLow.begin(), searchLow.end(), searchLow.begin(), ::tolower);

				for (int i = 0; i < IM_ARRAYSIZE(tp_locations); i++) {
					std::string nameLow = tp_locations[i];
					std::transform(nameLow.begin(), nameLow.end(), nameLow.begin(), ::tolower);
					if (!searchLow.empty() && nameLow.find(searchLow) == std::string::npos) continue;

					bool is_sel = (selected_tp_location == i);
					bool is_hov = false;

					ImVec2 cursor = ImGui::GetCursorScreenPos();
					ImVec2 rowMin = ImVec2(cursor.x, cursor.y);
					ImVec2 rowMax = ImVec2(cursor.x + ImGui::GetContentRegionAvail().x, cursor.y + itemH);

					ImGui::PushID(i);
					ImGui::InvisibleButton("##tp_item", ImVec2(-1, itemH));
					is_hov = ImGui::IsItemHovered();
					if (ImGui::IsItemClicked(0)) selected_tp_location = i;
					if (is_sel && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
						if (i >= 0 && i < IM_ARRAYSIZE(tp_coords)) {
							TeleportToPosition(tp_coords[i]);
							printa<normal>("Teleporting to location...");
						}
					}
					ImGui::PopID();


					float hTarget = is_hov ? 1.0f : 0.0f;
					float sTarget = is_sel ? 1.0f : 0.0f;
					tp_hover_anim[i] += (hTarget - tp_hover_anim[i]) * dt * 12.0f;
					tp_sel_anim[i] += (sTarget - tp_sel_anim[i]) * dt * 10.0f;
					float ha = tp_hover_anim[i];
					float sa = tp_sel_anim[i];


					if (sa > 0.01f) {
						ImU32 selL = IM_COL32(255, 20, 147, (int)(45 * sa));
						ImU32 selR = IM_COL32(199, 21, 133, (int)(18 * sa));
						dl->AddRectFilledMultiColor(rowMin, rowMax, selL, selR, selR, selL);
						dl->AddRect(rowMin, rowMax, IM_COL32(255, 20, 147, (int)(80 * sa)), 4.0f, 0, 1.0f);

						dl->AddRectFilled(ImVec2(rowMin.x, rowMin.y + 3), ImVec2(rowMin.x + 2.5f, rowMax.y - 3),
							IM_COL32(255, 20, 147, (int)(200 * sa)), 2.0f);
					}
					if (ha > 0.01f && !is_sel) {
						dl->AddRectFilled(rowMin, rowMax,
							IM_COL32(255, 20, 147, (int)(12 * ha)), 4.0f);
					}


					char idx[8]; snprintf(idx, sizeof(idx), "%02d", i + 1);
					ImVec2 idxPos = ImVec2(rowMin.x + 8.f, rowMin.y + (itemH - 14.f) * 0.5f);
					ImU32 idxCol = is_sel ? IM_COL32(255, 140, 210, (int)(200 + 55 * sa)) : IM_COL32(160, 120, 180, (int)(130 + 80 * ha));
					dl->AddText(ImGui::GetFont(), 13.f, idxPos, idxCol, idx);


					ImVec2 txtPos = ImVec2(rowMin.x + 32.f, rowMin.y + (itemH - 15.f) * 0.5f);
					ImU32 txtShadow = IM_COL32(0, 0, 0, is_sel ? 120 : 60);
					dl->AddText(ImGui::GetFont(), 15.f, ImVec2(txtPos.x + 1, txtPos.y + 1), txtShadow, tp_locations[i]);
					ImU32 txtCol = is_sel ? IM_COL32(255, 235, 245, 255) : IM_COL32(215, 210, 225, (int)(190 + 65 * ha));
					dl->AddText(ImGui::GetFont(), 15.f, txtPos, txtCol, tp_locations[i]);


					if (!is_sel) {
						dl->AddLine(ImVec2(rowMin.x + 8, rowMax.y), ImVec2(rowMax.x - 8, rowMax.y),
							IM_COL32(255, 20, 147, 12));
					}
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(6);

			ImGui::Dummy(ImVec2(0, 5.f));


			if (selected_tp_location >= 0 && selected_tp_location < IM_ARRAYSIZE(tp_coords)) {
				float tp_btn_w = 140.f;
				float tp_start_x = (ImGui::GetWindowContentRegionMax().x - tp_btn_w) / 2.f;
				ImGui::SetCursorPosX(tp_start_x);
				if (neon_button("Teleport", ImVec2(tp_btn_w, 25.f))) {
					TeleportToPosition(tp_coords[selected_tp_location]);
					printa<normal>("Teleporting to location...");
				}
			}
		}
		end_child("World Features");
	}
	ImGui::EndGroup();

	ImGui::SameLine(0, 10.f);


	ImGui::BeginGroup();
	{

		static char playerSearchQuery[128] = "";

		static bool player_list_section = false;
		child("Player List", &player_list_section, false);
		{

			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			float searchWidth = 145.f;
			float searchPosX = contentRegion.x - searchWidth - 10.f;
			ImGui::SetCursorPosX(searchPosX);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.04f, 0.02f, 0.06f, 0.9f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.06f, 0.03f, 0.09f, 0.95f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.08f, 0.04f, 0.12f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.2f, 0.63f, 0.15f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 4.0f));
			ImGui::SetNextItemWidth(searchWidth);
			ImGui::InputTextWithHint("##playersearch", "search", playerSearchQuery, IM_ARRAYSIZE(playerSearchQuery));
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(5);

			ImGui::Dummy(ImVec2(0, 4.f));


			static float pl_hover_anim[256] = {};
			static float pl_sel_anim[256] = {};

			float plListH = 260.f;
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.02f, 0.01f, 0.04f, 0.85f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.078f, 0.576f, 0.08f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.02f, 0.01f, 0.04f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(1.0f, 0.078f, 0.576f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(1.0f, 0.078f, 0.576f, 0.4f));
			ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(1.0f, 0.078f, 0.576f, 0.6f));
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
			ImGui::BeginChild("##pl_list_scroll", ImVec2(-1, plListH), true);
			{
				ImDrawList* dl = ImGui::GetWindowDrawList();
				float dt = ImGui::GetIO().DeltaTime;
				float itemH = 32.f;
				size_t playerCount = playerIDs.size();
				std::string searchStr = playerSearchQuery;
				std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);

				int drawIdx = 0;
				for (size_t i = 0; i < playerCount; i++) {
					if (i >= playerIDs.size()) break;
					int player_id = playerIDs[i];
					std::string player_name = playerNames.count(player_id) ? playerNames[player_id] : "Player " + std::to_string(player_id);
					std::string player_name_lower = player_name;
					std::transform(player_name_lower.begin(), player_name_lower.end(), player_name_lower.begin(), ::tolower);
					if (!searchStr.empty() && player_name_lower.find(searchStr) == std::string::npos) continue;

					bool is_sel = (selectedItemPlayer == (int)i);
					bool is_friend = friendStatus.count(player_id) ? friendStatus[player_id] : false;
					int aIdx = drawIdx % 256;

					ImVec2 cursor = ImGui::GetCursorScreenPos();
					ImVec2 rowMin = cursor;
					ImVec2 rowMax = ImVec2(cursor.x + ImGui::GetContentRegionAvail().x, cursor.y + itemH);

					ImGui::PushID((int)i);
					ImGui::InvisibleButton("##pl_item", ImVec2(-1, itemH));
					bool is_hov = ImGui::IsItemHovered();
					if (ImGui::IsItemClicked(0)) {
						selectedItemPlayer = (int)i;
						selectedPlayerID = player_id;
					}
					ImGui::PopID();


					float hTarget = is_hov ? 1.0f : 0.0f;
					float sTarget = is_sel ? 1.0f : 0.0f;
					pl_hover_anim[aIdx] += (hTarget - pl_hover_anim[aIdx]) * dt * 12.0f;
					pl_sel_anim[aIdx] += (sTarget - pl_sel_anim[aIdx]) * dt * 10.0f;
					float ha = pl_hover_anim[aIdx];
					float sa = pl_sel_anim[aIdx];


					if (sa > 0.01f) {
						ImU32 selL = IM_COL32(255, 20, 147, (int)(45 * sa));
						ImU32 selR = IM_COL32(199, 21, 133, (int)(18 * sa));
						dl->AddRectFilledMultiColor(rowMin, rowMax, selL, selR, selR, selL);
						dl->AddRect(rowMin, rowMax, IM_COL32(255, 20, 147, (int)(80 * sa)), 4.0f, 0, 1.0f);
						dl->AddRectFilled(ImVec2(rowMin.x, rowMin.y + 3), ImVec2(rowMin.x + 2.5f, rowMax.y - 3),
							IM_COL32(255, 20, 147, (int)(200 * sa)), 2.0f);
					}
					if (ha > 0.01f && !is_sel) {
						dl->AddRectFilled(rowMin, rowMax,
							IM_COL32(255, 20, 147, (int)(12 * ha)), 4.0f);
					}


					float dotY = rowMin.y + itemH * 0.5f;
					if (is_friend) {
						dl->AddCircleFilled(ImVec2(rowMin.x + 10.f, dotY), 3.5f, IM_COL32(80, 255, 120, 220));
						dl->AddCircle(ImVec2(rowMin.x + 10.f, dotY), 3.5f, IM_COL32(80, 255, 120, 60), 12, 1.0f);
					} else {
						dl->AddCircleFilled(ImVec2(rowMin.x + 10.f, dotY), 3.0f, IM_COL32(160, 140, 180, (int)(100 + 60 * ha)));
					}


					ImVec2 txtPos = ImVec2(rowMin.x + 22.f, rowMin.y + (itemH - 15.f) * 0.5f);
					ImU32 txtShadow = IM_COL32(0, 0, 0, is_sel ? 120 : 60);
					dl->AddText(ImGui::GetFont(), 15.f, ImVec2(txtPos.x + 1, txtPos.y + 1), txtShadow, player_name.c_str());
					ImU32 txtCol = is_sel ? IM_COL32(255, 235, 245, 255) : IM_COL32(215, 210, 225, (int)(190 + 65 * ha));
					dl->AddText(ImGui::GetFont(), 15.f, txtPos, txtCol, player_name.c_str());


					char idBuf[16]; snprintf(idBuf, sizeof(idBuf), "[%d]", player_id);
					ImVec2 idSize = ImGui::GetFont()->CalcTextSizeA(12.f, FLT_MAX, -1.f, idBuf);
					ImVec2 idPos = ImVec2(rowMax.x - idSize.x - 12.f, rowMin.y + (itemH - 12.f) * 0.5f);
					dl->AddText(ImGui::GetFont(), 12.f, idPos, IM_COL32(160, 140, 185, (int)(120 + 80 * sa + 40 * ha)), idBuf);


					if (!is_sel) {
						dl->AddLine(ImVec2(rowMin.x + 8, rowMax.y), ImVec2(rowMax.x - 8, rowMax.y),
							IM_COL32(255, 20, 147, 10));
					}
					drawIdx++;
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(6);

			ImGui::Dummy(ImVec2(0, 8.f));

			if (selectedPlayerID != -1 && selectedItemPlayer >= 0 && selectedItemPlayer < (int)playerIDs.size()) {
				const float button_width1 = 50.f;
				const float button_width2 = 70.f;
				const float button_width3 = 55.f;
				const float button_width4 = 55.f;
				const float spacing = 5.f;
				const float total_width = button_width1 + button_width2 + button_width3 + button_width4 + (spacing * 3);

				float start_x = (ImGui::GetWindowContentRegionMax().x - total_width) / 2.f;
				ImGui::SetCursorPosX(start_x);


				if (neon_button("TP", ImVec2(button_width1, 25.f))) {
					try {
						bool playerFound = false;
						for (size_t j = 0; j < pedList.size(); j++) {
							if (pedList[j].getID() == selectedPlayerID) {
								float distance = getDistance(game::local_position, pedList[j].position);
								if (distance <= 50.0f) {
									TeleportToPosition(pedList[j].position);
									printa<normal>("Teleporting to player...");
								}
								else {
									hitlog::printa<hitlog::MessageType::Warning>("50m fazla uzaksin tp atamazsin");
									Beep(1500, 100);
								}
								playerFound = true;
								break;
							}
						}
						if (!playerFound) {
							printa<normal>("Player not found!");
						}
					}
					catch (...) {
						printa<normal>("Error teleporting to player!");
					}
				}

				ImGui::SameLine(0, spacing);


				bool isFriend = friendStatus.count(selectedPlayerID) ? friendStatus[selectedPlayerID] : false;
				const char* friend_text = isFriend ? "Remove" : "Add Friend";
				if (neon_button(friend_text, ImVec2(button_width2, 25.f))) {
					friendStatus[selectedPlayerID] = !isFriend;
				}

				ImGui::SameLine(0, spacing);


				if (neon_button("Unlist", ImVec2(button_width3, 25.f))) {
					try {
						for (auto& pair : friendStatus) {
							pair.second = false;
						}
					}
					catch (...) {
					}
				}

				ImGui::SameLine(0, spacing);


				if (neon_button("Outfit", ImVec2(button_width4, 25.f))) {
					try {
						uintptr_t targetPed = 0;
						for (size_t j = 0; j < pedList.size(); j++) {
							if (pedList[j].getID() == selectedPlayerID) {
								targetPed = pedList[j].pointer;
								break;
							}
						}

						if (targetPed && localPlayer.pointer) {
							uintptr_t drawhandler = ReadMemory<uintptr_t>(targetPed + 0x48);

							if (drawhandler) {
								BYTE mask = ReadMemory<BYTE>(drawhandler + 0xEC);
								BYTE mask_tex = ReadMemory<BYTE>(drawhandler + 0xF0);
								BYTE hand = ReadMemory<BYTE>(drawhandler + 0xF4);
								BYTE hand_tex = ReadMemory<BYTE>(drawhandler + 0xF8);
								BYTE leg = ReadMemory<BYTE>(drawhandler + 0xFC);
								BYTE leg_tex = ReadMemory<BYTE>(drawhandler + 0x100);
								BYTE bag = ReadMemory<BYTE>(drawhandler + 0x104);
								BYTE bag_tex = ReadMemory<BYTE>(drawhandler + 0x108);
								BYTE shoes = ReadMemory<BYTE>(drawhandler + 0x10C);
								BYTE shoes_tex = ReadMemory<BYTE>(drawhandler + 0x110);
								BYTE accessory = ReadMemory<BYTE>(drawhandler + 0x114);
								BYTE accessory_tex = ReadMemory<BYTE>(drawhandler + 0x118);
								BYTE tshirt = ReadMemory<BYTE>(drawhandler + 0x11C);
								BYTE tshirt_tex = ReadMemory<BYTE>(drawhandler + 0x120);
								BYTE armor_d = ReadMemory<BYTE>(drawhandler + 0x124);
								BYTE armor_tex = ReadMemory<BYTE>(drawhandler + 0x128);
								BYTE decal = ReadMemory<BYTE>(drawhandler + 0x12C);
								BYTE decal_tex = ReadMemory<BYTE>(drawhandler + 0x130);
								BYTE torso = ReadMemory<BYTE>(drawhandler + 0x134);
								BYTE torso_tex = ReadMemory<BYTE>(drawhandler + 0x138);

								uintptr_t local_drawhandler = ReadMemory<uintptr_t>(localPlayer.pointer + 0x48);

								if (local_drawhandler) {
									WriteMemory<BYTE>(local_drawhandler + 0xEC, mask);
									WriteMemory<BYTE>(local_drawhandler + 0xF0, mask_tex);
									WriteMemory<BYTE>(local_drawhandler + 0xF4, hand);
									WriteMemory<BYTE>(local_drawhandler + 0xF8, hand_tex);
									WriteMemory<BYTE>(local_drawhandler + 0xFC, leg);
									WriteMemory<BYTE>(local_drawhandler + 0x100, leg_tex);
									WriteMemory<BYTE>(local_drawhandler + 0x104, bag);
									WriteMemory<BYTE>(local_drawhandler + 0x108, bag_tex);
									WriteMemory<BYTE>(local_drawhandler + 0x10C, shoes);
									WriteMemory<BYTE>(local_drawhandler + 0x110, shoes_tex);
									WriteMemory<BYTE>(local_drawhandler + 0x114, accessory);
									WriteMemory<BYTE>(local_drawhandler + 0x118, accessory_tex);
									WriteMemory<BYTE>(local_drawhandler + 0x11C, tshirt);
									WriteMemory<BYTE>(local_drawhandler + 0x120, tshirt_tex);
									WriteMemory<BYTE>(local_drawhandler + 0x124, armor_d);
									WriteMemory<BYTE>(local_drawhandler + 0x128, armor_tex);
									WriteMemory<BYTE>(local_drawhandler + 0x12C, decal);
									WriteMemory<BYTE>(local_drawhandler + 0x130, decal_tex);
									WriteMemory<BYTE>(local_drawhandler + 0x134, torso);
									WriteMemory<BYTE>(local_drawhandler + 0x138, torso_tex);

									WriteMemory<int>(local_drawhandler + 0x18, -1);
									WriteMemory<BYTE>(local_drawhandler + 0x1C, 1);

									printa<normal>("Outfit copied!");
								}
							}
						}
					}
					catch (...) {
						printa<normal>("Failed to copy outfit!");
					}
				}
			}
		}
		end_child("Player List");
	}
	ImGui::EndGroup();
}

void settings_tab()
{
	MGR->esp_p = false;

	{
		ImGui::SetCursorPos(ImVec2(16.f, 8.f));
		static bool general1 = true;
		child("Settings", &general1, false);
		{
			checkbox("Stream Proof", &Cheats::Misc::streamProof);
			checkbox("Watermark", &Cheats::Misc::showInfoBox);
			checkbox("Active Features", &Cheats::Misc::showActiveFeatures);
			keybind("Menu Key", &Cheats::menuKey);

			ImGui::Dummy(ImVec2(0, 20.f));

			ImDrawList* draw = ImGui::GetWindowDrawList();
			float window_width = ImGui::GetWindowContentRegionMax().x;




			static float exit_hover_t = 0.f;
			float dt = ImGui::GetIO().DeltaTime;

			float btn_w = window_width - 32.f;
			float btn_h = 42.f;
			float rnd = 6.f;
			ImVec2 bp = ImGui::GetCursorScreenPos() + ImVec2(16.f, 0.f);
			ImVec2 be = bp + ImVec2(btn_w, btn_h);

			bool hovered = ImGui::IsMouseHoveringRect(bp, be);
			exit_hover_t += ((hovered ? 1.f : 0.f) - exit_hover_t) * dt * 12.f;
			float t = exit_hover_t;


			int bg_r = (int)(22 + t * 28);
			int bg_g = (int)(8 + t * 4);
			int bg_b = (int)(12 + t * 8);
			draw->AddRectFilled(bp, be, ImColor(bg_r, bg_g, bg_b, 255), rnd);


			float line_w = btn_w * (0.3f + t * 0.7f);
			float line_x = bp.x + (btn_w - line_w) * 0.5f;
			int line_alpha = (int)(140 + t * 115);
			draw->AddRectFilled(
				ImVec2(line_x, be.y - 2.f),
				ImVec2(line_x + line_w, be.y),
				ImColor(220, 40, 50, line_alpha), 1.f
			);


			int border_alpha = (int)(30 + t * 50);
			draw->AddRect(bp, be, ImColor(255, 60, 60, border_alpha), rnd, 0, 1.f);


			float cy = bp.y + btn_h * 0.5f;
			float ix = bp.x + 46.f;
			float ir = 7.f;
			float ithk = 2.f;
			int icon_alpha = (int)(140 + t * 115);




			float gap = 0.5f;
			draw->PathArcTo(ImVec2(ix, cy), ir, IM_PI * 1.5f + gap, IM_PI * 3.5f - gap, 24);
			draw->PathStroke(ImColor(220, 50, 60, icon_alpha), false, ithk);


			draw->AddLine(
				ImVec2(ix, cy - ir - 1.f),
				ImVec2(ix, cy - 1.f),
				ImColor(220, 50, 60, icon_alpha), ithk
			);


			draw->AddLine(
				ImVec2(bp.x + 68.f, bp.y + 10.f),
				ImVec2(bp.x + 68.f, be.y - 10.f),
				ImColor(255, 255, 255, (int)(15 + t * 15)), 1.f
			);


			const char* label = "Safe Exit";
			ImVec2 ts = ImGui::CalcTextSize(label);
			float text_area_start = bp.x + 76.f;
			float text_area_w = btn_w - 76.f;
			ImVec2 tp = ImVec2(
				text_area_start + (text_area_w - ts.x) * 0.5f,
				bp.y + (btn_h - ts.y) * 0.5f
			);

			int txt_alpha = (int)(160 + t * 95);
			draw->AddText(tp, ImColor(255, 255, 255, txt_alpha), label);


			ImGui::SetCursorScreenPos(bp);
			if (ImGui::InvisibleButton("##safe_exit", ImVec2(btn_w, btn_h))) {
				std::thread([]() {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					TerminateProcess(GetCurrentProcess(), 0);
				}).detach();
			}
		}
		end_child("Settings");
	}
}void drawMenu() {

}
