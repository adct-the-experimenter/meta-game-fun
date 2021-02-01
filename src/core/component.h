
#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <bitset>
#include <array>
#include <vector>
#include "raylib.h" //for vector2

// A simple type alias
using ComponentType = std::uint8_t;

// Used to define the size of arrays later on
const ComponentType MAX_COMPONENTS = 34;

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
enum class LooksStatus : std::uint8_t { SUPER_UGLY=0,UGLY=1, NORMAL=0, GOOD_LOOKING, BEAUTIFUL}; //distorted face for ugly, sparkles for beautiful
enum class ActivityStatus : std::uint8_t { ROAMING_WORLD=0, PLAYING_GAME, PLAYING_SPORT };


struct Player
{
	std::string name;
	std::string romantic_partner_name;
	PlayerTimeStatus time_status;
	std::uint16_t money;
	std::uint8_t health;
	std::string job_occupation;
	LooksStatus look_status;
	ActivityStatus activity_status;
};


//enum class for helping systems identify what render component is to manipulate it.
enum class RenderPartDescription : std::uint8_t {LEG, ARM, EYE, HAIR, HEAD,
												UPPER_BODY_CLOTHING, LOWER_BODY_CLOTHING, SHOES,
											WHOLE_BODY, OTHER};

//not an actualy component, just contains org. info for component
struct RenderInfo
{
	Vector2 position;
	Texture2D* texture_ptr;
	Rectangle frame_rect;
	Color tint;
	RenderPartDescription part_description;
};

struct SingleRenderComponent
{
	RenderInfo render_part;
};

struct MultipleRenderComponent
{
	//
	std::uint8_t num_render_parts;
	std::vector <RenderInfo> render_parts_vec;
};

enum class InputReactorType : std::uint8_t { NONE=0, PLAYER, CAR};
struct InputReact
{
	InputReactorType actor_type;
};

enum class EnemyType : std::uint8_t {NONE=0, SKELETON};
struct Enemy
{
	EnemyType type;
};

#endif
