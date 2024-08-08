#ifndef STATE_HPP_BE728505_2D00_4E60_9F37_6C58B3569251
#define STATE_HPP_BE728505_2D00_4E60_9F37_6C58B3569251

#include <glad.h> // Make sure <GL/gl.h> isn't being included.
#include <GLFW/glfw3.h>

#include "../vmlib/vec2.hpp"

enum class EInputMode
{
	standard,
	piloting
};

struct State
{
	EInputMode inputMode = EInputMode::standard;

	// "player" data
	struct Player_
	{
		float angle = 0.f;
		Vec2f velocity = { 0.f, 0.f };
		Vec2f position = { 0.f, 0.f };

		float accelerationMagnitude = 0.f;
	} player;

	// Frame data
	struct Frame_
	{
		float dt = 0.f;
		Vec2f movement = { 0.f, 0.f };
	} thisFrame;

	// Misc.
	GLFWcursor* crosshair = nullptr;
};


void state_update( State&, float aDeltaSeconds );

#endif // STATE_HPP_BE728505_2D00_4E60_9F37_6C58B3569251
