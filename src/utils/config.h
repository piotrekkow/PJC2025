#pragma once
#include <raylib.h>

constexpr int WINDOW_WIDTH{ 1920 };
constexpr int WINDOW_HEIGHT{ 1080 };

constexpr Color BACKGROUND_COLOR{ RAYWHITE };
constexpr Color EDGE_COLOR{ LIGHTGRAY };
constexpr Color VERTEX_COLOR{ BLACK };
constexpr Color TANGENT_COLOR{ BLUE };
constexpr Color NORMAL_COLOR{ RED };

constexpr Color ROAD_COLOR{ LIGHTGRAY };
constexpr float VERTEX_RADIUS{ 5.0f };
constexpr float LANE_WIDTH{ 20.0f };
constexpr float DEBUG_AXES_LENGTH{ 20.0f };
constexpr auto WINDOW_TITLE{ "PJC2025" };