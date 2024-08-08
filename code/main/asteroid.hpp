#ifndef ASTEROID_HPP_477C5E99_10A3_4AEB_8FE5_99A52EDF26EC
#define ASTEROID_HPP_477C5E99_10A3_4AEB_8FE5_99A52EDF26EC

#include <cstdlib>

#include "../draw2d/forward.hpp"
#include "../draw2d/color.hpp"

#include "defaults.hpp"

/* Generate a procedural asteroid
 *
 * The method used here is fairly ad-hoc, entirely focusing on generating
 * decent looking shapes. There isn't really any physical background to the
 * generation method.
 *
 * Each asteroid is represented by a triangle fan. This limits the possible
 * shapes to deformed circles where lines going from the mid point to the
 * periphery cannot pass outside of the shape.
 */

#if 1
// Default asteroid with 18+1 points.
TriangleFan make_asteroid(
	RNG&,
	std::size_t aNumPoints = 18,
	float aRadiusMean = 30.f, 
	float aRadiusStddev = 5.f,
	float aSquishStddev = 0.20f,
	float aDisplaceStddev = 2.5f,
	ColorF const& aBaseColor = { 0.3f, 0.3f, 0.3f },
	float aColorBaseStddev = 0.2f,
	float aColorVariation = 0.05f
);
#else
// This creates a lower resolution asteroid with only 7+1 points.
TriangleFan make_asteroid(
	RNG&,
	std::size_t aNumPoints = 7,
	float aRadiusMean = 30.f, 
	float aRadiusStddev = 5.f,
	float aSquishStddev = 0.15f,
	float aDisplaceStddef = 9.f,
	ColorF const& aBaseColor = { 0.3f, 0.3f, 0.3f },
	float aColorBaseStddev = 0.2f,
	float aColorVariation = 0.05f
);
#endif

// Note that the same function is used to generate either type of asteroid;
// only the default parameter values change.

#endif // ASTEROID_HPP_477C5E99_10A3_4AEB_8FE5_99A52EDF26EC
