
#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <bitset>
#include "raylib.h" //for vector2

// A simple type alias
using ComponentType = std::uint8_t;

// Used to define the size of arrays later on
const ComponentType MAX_COMPONENTS = 32;

struct Gravity3D
{
	Vector3 force;
};

struct Gravity2D
{
	Vector2 force;
};

struct RigidBody3D
{
	Vector3 velocity;
	Vector3 acceleration;
};

struct RigidBody2D
{
	Vector2 velocity;
	Vector2 acceleration;
};

struct Transform3D
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

struct Transform2D
{
	Vector2 position;
};

enum class PlayerTimeStatus : std::uint8_t { NONE=0, FREE, WORKING, SLEEPING};
enum class LooksStatus : std::uint8_t { NORMAL=0, UGLY, BEAUTIFUL}; //distorted face for ugly, sparkles for beautiful
enum class ActivityStatus : std::uint8_t { ROAMING_WORLD=0, PLAYING_GAME, PLAYING_SPORT };

struct Player
{
	PlayerTimeStatus time_status;
	std::uint16_t money;
	std::uint8_t health;
	std::string job_occupation;
	LooksStatus look_status;
	ActivityStatus activity_status;
};

struct RenderInfo
{
	Texture2D* texture_ptr;
	Rectangle frame_rect;
	Color tint;
};



struct KeyboardTypingInputInfo
{
	int key;
};

#endif
