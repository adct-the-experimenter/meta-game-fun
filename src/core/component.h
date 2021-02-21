
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

struct CollisionBox
{
	std::uint32_t width;
	std::uint32_t height;
};

enum class PlayerTimeStatus : std::uint8_t { NONE=0, FREE, WORKING, SLEEPING};
enum class LooksStatus : std::uint8_t { SUPER_UGLY=0,UGLY, NORMAL, GOOD_LOOKING, BEAUTIFUL}; //distorted face for ugly, sparkles for beautiful
enum class ActivityStatus : std::uint8_t { ROAMING_WORLD=0, PLAYING_GAME, PLAYING_SPORT };


struct Player
{
	std::string name;
	std::string romantic_partner_name;
	PlayerTimeStatus time_status;
	std::int16_t money;
	std::int8_t health;
	std::string job_occupation;
	LooksStatus look_status;
	ActivityStatus activity_status;
	std::uint8_t num_player;
};


//enum class for helping systems identify what render component is to manipulate it.
enum class RenderPartDescription : std::uint8_t {LEG=0, ARM, EYE, HAIR, HEAD,
												UPPER_BODY_CLOTHING, LOWER_BODY_CLOTHING, SHOES,
											WHOLE_BODY, OTHER};






struct RenderPosition
{
	//position of entire render structure
	Vector2 overall_position;
};

//only 8 directions
enum class FaceDirection : std::uint8_t {SOUTH=0,SOUTHWEST,WEST,NORTHWEST,NORTH,NORTHEAST,EAST,SOUTHEAST};
enum class AnimatedActorType : std::int8_t {NONE=0,PLAYER,ENEMY};


struct AnimationInfo
{
	AnimatedActorType anim_actor_type; 
	
	//position of frame and frame direction
	
	std::uint16_t horiz_frame_offset = 0;
	std::uint16_t frame_size = 30;
	std::uint8_t frame_count = 0;
	
	//for movement based animation
	Vector2 last_position;
};
struct Animation
{
	AnimationInfo info;
};

//not an actualy component, just contains org. info for component
struct RenderInfo
{
	Vector2 position;
	Texture2D* texture_ptr;
	Rectangle frame_rect;
	Color tint;
	RenderPartDescription part_description;
};

struct RenderComponent
{
	
	RenderInfo single_render_part;
	std::vector <RenderInfo> multi_render_parts_vec;
	bool multi_part;
};


enum class InputReactorType : std::uint8_t { NONE=0, PLAYER, CAR};
struct InputReact
{
	InputReactorType actor_type;
	bool reactToInput = false;
	std::uint8_t player_num = 0;
};

enum class PhysicsType : std::uint8_t {LIFE_RPG=0, PLATFORMER, FIGHTING_GAME};
struct PhysicsTypeComponent
{
	PhysicsType phy_type;
};

enum class EnemyType : std::uint8_t {NONE=0, SKELETON};
struct Enemy
{
	EnemyType type;
};

#endif
