
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


#endif
