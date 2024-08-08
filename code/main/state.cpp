#include "state.hpp"

#include <cstdio>

void state_update( State& aState, float aDeltaSeconds )
{
	// Update "player" data
	Vec2f const acceleration{
		aState.player.accelerationMagnitude * std::cos( aState.player.angle ),
		aState.player.accelerationMagnitude * std::sin( aState.player.angle )
	};

	aState.player.velocity += acceleration * aDeltaSeconds;

	Vec2f const movement = aState.player.velocity * aDeltaSeconds;

	aState.player.position += movement;

	// Data for this frame
	aState.thisFrame.dt = aDeltaSeconds;
	aState.thisFrame.movement = movement;
}
