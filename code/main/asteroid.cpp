#include "asteroid.hpp"

#include <random>
#include <vector>
#include <algorithm>

#include <cmath>
#include <cassert>

#include "../draw2d/shape.hpp"

#include "../vmlib/vec2.hpp"
#include "../vmlib/mat22.hpp"

namespace
{
	// C++20 adds a number of standardized mathematical constants:
	// https://en.cppreference.com/w/cpp/numeric/constants
	// This defines a custom (worse) one:
	constexpr float kPI = 3.1415926535897932385f; // pi
}

TriangleFan make_asteroid( std::minstd_rand& aRNG, std::size_t aNumPoints, float aRadiusMean, float aRadiusStddev, float aSquishStddev, float aDisplaceStddev, ColorF const& aBaseColor, float aColorBaseStddev, float aColorVar )
{
	// Sample general parameters
	float const radius = std::normal_distribution<float>{aRadiusMean, aRadiusStddev}(aRNG);
	float const squish = std::normal_distribution<float>{1.f, aSquishStddev}(aRNG);

	float crand = std::normal_distribution<float>{ 0.f, aColorBaseStddev }(aRNG);

	ColorF baseColor = aBaseColor;
	baseColor.r = std::clamp( baseColor.r + crand, 0.1f, 1.f );
	baseColor.g = std::clamp( baseColor.g + crand, 0.1f, 1.f );
	baseColor.b = std::clamp( baseColor.b + crand, 0.1f, 1.f );

	// Generate initial circle
	float const astep = 2.f*kPI / aNumPoints;

	std::vector<Vec2f> verts;
	verts.reserve( aNumPoints+1 );
	verts.resize( aNumPoints );

	for( std::size_t i = 0; i < aNumPoints; ++i )
	{
		verts[i] = radius * Vec2f{
			std::cos( i*astep ),
			std::sin( i*astep )
		};
	}

	// Displace vertices
	std::normal_distribution<float> displace( 0.f, aDisplaceStddev );
	for( auto& vert : verts )
	{
		float const displacement = displace( aRNG );
		float const length = std::sqrt( dot( vert, vert ) );
		Vec2f const delta = (displacement / length) * vert;

		vert += delta;
	}
	
	// Squish
	// We only need to squish along one axis to make the shape less round. The
	// asteroids are rotated randomly later.
	for( auto& vert : verts )
		vert.x *= squish;

	// Generate colors
	std::uniform_real_distribution<float> cdist( -aColorVar, aColorVar );

	std::vector<ColorF> colors;
	colors.reserve( aNumPoints + 1 );
	for( std::size_t i = 0; i < aNumPoints; ++i )
	{
		float cvar = cdist(aRNG);

		ColorF col = baseColor;
		col.r = std::clamp( col.r + cvar, 0.f, 1.f );
		col.g = std::clamp( col.g + cvar, 0.f, 1.f );
		col.b = std::clamp( col.b + cvar, 0.f, 1.f );

		colors.emplace_back( col );
	}

	// Complete shape
	verts.emplace( verts.begin(), Vec2f{ 0.f, 0.f } );
	colors.emplace( colors.begin(), baseColor );

	// Return shape
	// We could be a bit more clever here and avoid the double allocations...
	return TriangleFan( verts.size(), verts.data(), colors.data() );
}
