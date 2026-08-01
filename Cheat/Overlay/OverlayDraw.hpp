#pragma once
#include <sstream>
#include "../Cheat/Brand.hpp"
REVERS_STAMP(OverlayDraw_hpp)
typedef struct
{
	DWORD r;
	DWORD g;
	DWORD b;
	DWORD a;
} RGBA;

ImColor ConvertToImColor(float rgb[3]) {
	int r = static_cast<int>(rgb[0] * 255.0f);
	int g = static_cast<int>(rgb[1] * 255.0f);
	int b = static_cast<int>(rgb[2] * 255.0f);
	return ImColor(r, g, b);
}

ImColor DarkenColor(const ImColor& color, float factor) {
	float r = color.Value.x;
	float g = color.Value.y;
	float b = color.Value.z;
	r *= factor;
	g *= factor;
	b *= factor;
	r = (r < 0) ? 0 : (r > 1) ? 1 : r;
	g = (g < 0) ? 0 : (g > 1) ? 1 : g;
	b = (b < 0) ? 0 : (b > 1) ? 1 : b;
	return ImColor(r, g, b, color.Value.w);
}

void DrawLine(ImDrawList* drawList, int x1, int y1, int x2, int y2, RGBA* color, int thickness) {
	drawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->r / 255.0f, color->g / 255.0f, color->b / 255.0f, color->a / 255.0f)), thickness);
}

void DrawLine(ImDrawList* drawList, const ImVec2& x, const ImVec2& y, ImU32 color, float width) {
	drawList->AddLine(x, y, color, width);
}

void DrawLineOutline(ImVec2 a, ImVec2 b, ImColor color, float width) {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	drawList->AddLine(a, b, IM_COL32(0, 0, 0, 180), width + 2.0f);
	drawList->AddLine(a, b, color, width);
}

void DrawRect(ImDrawList* drawList, int x, int y, int w, int h, RGBA* color, int thickness) {
	drawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->r / 255.0f, color->g / 255.0f, color->b / 255.0f, color->a / 255.0f)), 0, 0, thickness);
}

void DrawFilledRect(ImDrawList* drawList, int x, int y, int w, int h, RGBA* color) {
	drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->r / 255.0f, color->g / 255.0f, color->b / 255.0f, color->a / 255.0f)), 0, 0);
}

void DrawCircleFilled(ImDrawList* drawList, int x, int y, int radius, RGBA* color) {
	drawList->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->r / 255.0f, color->g / 255.0f, color->b / 255.0f, color->a / 255.0f)));
}

void DrawCircle(ImDrawList* drawList, int x, int y, int radius, RGBA* color, int segments) {
	drawList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->r / 255.0f, color->g / 255.0f, color->b / 255.0f, color->a / 255.0f)), segments);
}

void CircleHead(ImDrawList* drawList, ImVec2 pos, float fov_size, ImColor color) {
	drawList->AddCircle(pos, fov_size, color, 100, 0);
}

void DrawHealthBarHorizontal(ImDrawList* drawList, ImVec2 pos, ImVec2 pos2, ImVec2 dim, ImColor col) {
	drawList->AddLine(ImVec2(pos.x + dim.y, pos.y), ImVec2(pos2.x, pos.y), col, dim.x);
}

void DrawRectImGui(ImDrawList* drawList, int x, int y, int w, int h, ImVec4 color, float thickness) {
	drawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0, thickness);
}

void DrawOutlinedHealtbar(ImDrawList* drawList, ImVec2 position, ImVec2 size, ImColor fillColor, ImColor outlineColor, float outlineThickness) {
	drawList->AddRectFilled(position, ImVec2(position.x + size.x, position.y + size.y), fillColor);
	drawList->AddRect(ImVec2(position.x - outlineThickness, position.y - outlineThickness), ImVec2(position.x + size.x + outlineThickness, position.y + size.y + outlineThickness), outlineColor, 0.0f, 0, outlineThickness);
}

void DrawArrowTriangleOutlined(ImDrawList* drawList, ImVec2 start, ImVec2 end, ImColor color, float thickness = 2.0f) {
	ImVec2 direction = ImVec2(end.x - start.x, end.y - start.y);
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length == 0) return;

	direction.x /= length;
	direction.y /= length;

	float arrowSize = 20.0f;
	float baseHalfWidth = 10.0f;

	ImVec2 tip = end;

	ImVec2 baseLeft = ImVec2(
		end.x - direction.x * arrowSize - direction.y * baseHalfWidth,
		end.y - direction.y * arrowSize + direction.x * baseHalfWidth
	);

	ImVec2 baseRight = ImVec2(
		end.x - direction.x * arrowSize + direction.y * baseHalfWidth,
		end.y - direction.y * arrowSize - direction.x * baseHalfWidth
	);

	ImVec2 shadowOffset = ImVec2(3, 3);
	ImColor shadowColor = ImColor(0, 0, 0, 150);
	drawList->AddTriangleFilled(tip + shadowOffset, baseLeft + shadowOffset, baseRight + shadowOffset, shadowColor);

	drawList->AddTriangleFilled(tip, baseLeft, baseRight, color);

	ImColor outlineColor = ImColor(255, 255, 255, 200);
	drawList->AddLine(tip, baseLeft, outlineColor, thickness);
	drawList->AddLine(tip, baseRight, outlineColor, thickness);
	drawList->AddLine(baseLeft, baseRight, outlineColor, thickness);
}

void drawHealthBar(ImDrawList* drawList, ImVec2 pos, ImVec2 dim, ImColor col) {
	drawList->AddLine(pos, ImVec2(pos.x, pos.y - dim.y), col, dim.x);
}

void DrawOutlinedTextWithBox(ImDrawList* drawList, ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 textColor, ImU32 outlineColor, ImU32 boxColor, float boxPadding, bool center) {
	std::stringstream stream(text);
	std::string line;
	float y = 0.0f;
	int index = 0;
	while (std::getline(stream, line)) {
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		ImVec2 textPos;

		if (center) {
			textPos = ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index);
		}
		else {
			textPos = ImVec2(pos.x, pos.y + textSize.y * index);
		}

		ImVec2 boxMin = textPos - ImVec2(boxPadding, boxPadding);
		ImVec2 boxMax = textPos + ImVec2(textSize.x + boxPadding, textSize.y + boxPadding);
		drawList->AddRectFilled(boxMin, boxMax, boxColor);

		drawList->AddText(pFont, size, textPos + ImVec2(1, 1), outlineColor, line.c_str());
		drawList->AddText(pFont, size, textPos, textColor, line.c_str());
		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

void DrawText(ImDrawList* drawList, ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) {
	std::stringstream stream(text);
	std::string line;
	float y = 0.0f;
	int index = 0;
	while (std::getline(stream, line)) {
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center) {
			drawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		else {
			drawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

void DrawOutlinedText(ImDrawList* drawList, ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) {
	std::stringstream stream(text);
	std::string line;

	ImU32 outlineColor = IM_COL32(0, 0, 0, 220);

	float y = 0.0f;
	int index = 0;
	while (std::getline(stream, line)) {
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		ImVec2 textPos;

		if (center) {
			textPos = ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index);
		}
		else {
			textPos = ImVec2(pos.x, pos.y + textSize.y * index);
		}


		drawList->AddText(pFont, size, textPos + ImVec2(1, 1), outlineColor, line.c_str());
		drawList->AddText(pFont, size, textPos + ImVec2(-1, -1), outlineColor, line.c_str());
		drawList->AddText(pFont, size, textPos, color, line.c_str());
		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

void DrawShadowedTexts(ImDrawList* drawList, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) {
	ImFont* pFont = ImGui::GetIO().Fonts->Fonts[0];

	std::stringstream stream(text);
	std::string line;

	ImU32 outlineColor = IM_COL32(0, 0, 0, 200);

	float y = 0.0f;
	int index = 0;
	while (std::getline(stream, line)) {
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		ImVec2 textPos;

		if (center) {
			textPos = ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index);
		}
		else {
			textPos = ImVec2(pos.x, pos.y + textSize.y * index);
		}


		drawList->AddText(pFont, size, textPos + ImVec2(1, 1), outlineColor, line.c_str());
		drawList->AddText(pFont, size, textPos, color, line.c_str());
		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

void DrawGradientRect(ImDrawList* drawList, ImVec2 topLeft, ImVec2 bottomRight, ImColor startColor, ImColor endColor) {

	ImU32 topCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
		startColor.Value.x * 0.5f, startColor.Value.y * 0.25f, startColor.Value.z * 0.5f, startColor.Value.w * 0.25f));
	ImU32 botCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
		endColor.Value.x, endColor.Value.y, endColor.Value.z, endColor.Value.w));
	drawList->AddRectFilledMultiColor(topLeft, bottomRight, topCol, topCol, botCol, botCol);
}