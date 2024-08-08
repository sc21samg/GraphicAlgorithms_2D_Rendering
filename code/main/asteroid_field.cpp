#include "asteroid_field.hpp"

#include <random>
#include <algorithm>

#include <cassert>

#include "../draw2d/shape.hpp"

#include "asteroid.hpp"

namespace
{
	// C++20 adds a number of standardized mathematical constants:
	// https://en.cppreference.com/w/cpp/numeric/constants
	// This defines a custom (worse) one:
	constexpr float kPI = 3.1415926535897932385f; // pi
}

AsteroidField::AsteroidField( RNG& aRNG, std::uint32_t aWidth, std::uint32_t aHeight, float aDensity, float aInitialSpeedStddev, float aMaximumSpeed, float aInitialRotStddev, float aPadding )
	: mInitialSpeed( aInitialSpeedStddev )
	, mMaximumSpeed( aMaximumSpeed )
	, mInitialRot( aInitialRotStddev )
	, mPadding( aPadding )
	, mDensity( aDensity )
	, mRNG( aRNG )
{
	// Compute area of simulation
	mExactExtent = Vec2f{ float(aWidth), float(aHeight) };

	mBoundsMin = Vec2f{ -mPadding, -mPadding };
	mBoundsMax = mExactExtent + Vec2f{ mPadding, mPadding };

	mActualExtent = mBoundsMax - mBoundsMin;

	// Generate initial asteroids
	float const numAsteroidsf = mActualExtent.x*mActualExtent.y * mDensity;
	std::size_t const numAsteroids = std::size_t(numAsteroidsf+0.5f);
	
	mAsteroids.resize( numAsteroids );
	mShapes.reserve( numAsteroids ); // reserve! not resize!

	using Uniform_ = std::uniform_real_distribution<float>;
	using Normal_ = std::normal_distribution<float>;

	Uniform_ xpos{ mBoundsMin.x, mBoundsMax.x };
	Uniform_ ypos{ mBoundsMin.y, mBoundsMax.y };

	Uniform_ angle( 0.f, 2*kPI );

	Normal_ vvel{ 0.f, mInitialSpeed };
	Normal_ rots{ 0.f, mInitialRot };

	for( std::size_t i = 0; i < numAsteroids; ++i )
	{
		auto& astr = mAsteroids[i];
		astr.pos = Vec2f{ xpos( mRNG ), ypos( mRNG ) };
		astr.vel = Vec2f{ vvel( mRNG ), vvel( mRNG ) };

		astr.rot = make_rotation_2d( angle( mRNG ) );
		astr.radpersec = rots( mRNG );

		// Don't break the speed limits. The space police will get you!
		astr.vel.x = std::clamp( astr.vel.x, -mMaximumSpeed, +mMaximumSpeed );
		astr.vel.y = std::clamp( astr.vel.y, -mMaximumSpeed, +mMaximumSpeed );

		// Create shape
		mShapes.emplace_back( make_asteroid( mRNG ) );
	}
}

AsteroidField::~AsteroidField() = default;


void AsteroidField::update( float aElapsed, Vec2f const& aTransl )
{
	auto const numAsteroids = mAsteroids.size();
	assert( numAsteroids == mShapes.size() );

	using Uniform_ = std::uniform_real_distribution<float>;
	using Normal_ = std::normal_distribution<float>;

	Uniform_ xpos{ mBoundsMin.x, mBoundsMax.x };
	Uniform_ ypos{ mBoundsMin.y, mBoundsMax.y };

	Uniform_ angle( 0.f, 2*kPI );

	Normal_ vvel{ 0.f, mInitialSpeed };
	Normal_ rots{ 0.f, mInitialRot };

	for( std::size_t i = 0; i < numAsteroids; ++i )
	{
		auto& astr = mAsteroids[i];
		astr.pos += astr.vel * aElapsed - aTransl;

		// If the asteroid is outside of the simulation area, replace it
		// with a fresh one.
		//
		// The method here isn't entirely optimal. The density of asteroids on
		// screen will reduce slightly over time (until some minimum) if the
		// player is standing still. New asteroids are generated with random
		// movement vectors. The random vectors are picked uniformly, meaning
		// that the asteroid has a fair chance to move off-screen without ever
		// becoming visible.
		if( astr.pos.x < mBoundsMin.x || astr.pos.x > mBoundsMax.x || astr.pos.y < mBoundsMin.y || astr.pos.y > mBoundsMax.x )
		{
			if( astr.pos.x < mBoundsMin.x )
			{
				astr.pos.x = mBoundsMax.x - mPadding/2.f;
				astr.pos.y = ypos( mRNG );
			}
			else if( astr.pos.x > mBoundsMax.x )
			{
				astr.pos.x = mBoundsMin.x + mPadding/2.f;
				astr.pos.y = ypos( mRNG );
			}
			else if( astr.pos.y < mBoundsMin.y )
			{
				astr.pos.x = xpos( mRNG );
				astr.pos.y = mBoundsMax.y - mPadding/2.f;
			}
			else if( astr.pos.y > mBoundsMax.y )
			{
				astr.pos.x = xpos( mRNG );
				astr.pos.y = mBoundsMin.y + mPadding/2.f;
			}

			astr.vel = Vec2f{ vvel( mRNG ), vvel( mRNG ) };

			astr.rot = make_rotation_2d( angle( mRNG ) );
			astr.radpersec = rots( mRNG );

			// Don't break the speed limits. The space police will get you!
			astr.vel.x = std::clamp( astr.vel.x, -mMaximumSpeed, +mMaximumSpeed );
			astr.vel.y = std::clamp( astr.vel.y, -mMaximumSpeed, +mMaximumSpeed );

			// Create shape
			mShapes[i] = make_asteroid( mRNG );
		}
		else
		{
			astr.rot = make_rotation_2d( astr.radpersec * aElapsed ) * astr.rot;
		}
	}
}

void AsteroidField::draw( Surface& aSurface ) const
{
	auto const numAsteroids = mAsteroids.size();
	assert( numAsteroids == mShapes.size() );

	for( std::size_t i = 0; i < numAsteroids; ++i )
	{
		auto const& astr = mAsteroids[i];
		auto const& shape = mShapes[i];

		// Performance: culling asteroids here would remove some work; right
		// now each triangle will be culled individually.

		shape.draw(
			aSurface,
			astr.rot,
			astr.pos
		);
	}
}

void AsteroidField::resize( std::uint32_t aWidth, std::uint32_t aHeight )
{
	// WARNING: This is a bit of a hack...

	auto const oldMax = mBoundsMax;

	// New area and asteroid count
	mExactExtent = Vec2f{ float(aWidth), float(aHeight) };

	mBoundsMin = Vec2f{ -mPadding, -mPadding };
	mBoundsMax = mExactExtent + Vec2f{ mPadding, mPadding };

	mActualExtent = mBoundsMax - mBoundsMin;

	float const numAsteroidsf = mActualExtent.x*mActualExtent.y * mDensity;
	std::size_t const numAsteroids = std::size_t(numAsteroidsf+0.5f);

	// Remove asteroids now outside
	std::size_t activeAsteroids = 0;

	auto jt = mShapes.begin();
	for( auto it = mAsteroids.begin(); it != mAsteroids.end(); )
	{
		auto const& aster = *it;

		if( aster.pos.x > mBoundsMax.x || aster.pos.y > mBoundsMax.y )
		{
			it = mAsteroids.erase( it );
			jt = mShapes.erase( jt );
		}
		else
		{
			++it;
			++jt;
			++activeAsteroids;
		}
	}

	mAsteroids.resize( numAsteroids );
	activeAsteroids = std::min( activeAsteroids, numAsteroids );

	mShapes.erase( mShapes.begin()+activeAsteroids, mShapes.end() );
	mShapes.reserve( numAsteroids );

	assert( mShapes.size() == activeAsteroids );

	// Generate new asteroids.
	using Normal_ = std::normal_distribution<float>;
	using Uniform_ = std::uniform_real_distribution<float>;

	if( activeAsteroids < numAsteroids )
	{
		auto dd = mBoundsMax - oldMax;
		if( dd.x < 0.f ) dd.x = 0.f;
		if( dd.y < 0.f ) dd.y = 0.f;

		float xarea = dd.x * mBoundsMax.y;
		float yarea = dd.y * (mBoundsMax.x - dd.x);

		Uniform_ area( 0.f, xarea+yarea );
		Uniform_ xax( oldMax.x, oldMax.x+dd.x );
		Uniform_ xay( 0.f, mBoundsMax.y );
		Uniform_ yax( 0.f, mBoundsMax.x - dd.x );
		Uniform_ yay( oldMax.y, oldMax.y+dd.y );

		Uniform_ angle( 0.f, 2*kPI );

		Normal_ vvel{ 0.f, mInitialSpeed };
		Normal_ rots{ 0.f, mInitialRot };

		for( std::size_t i = activeAsteroids; i < numAsteroids; ++i )
		{
			Vec2f pos;

			auto const where = area(mRNG);
			if( where <= xarea )
			{
				pos.x = xax( mRNG );
				pos.y = xay( mRNG );
			}
			else
			{
				pos.x = yax( mRNG );
				pos.y = yay( mRNG );
			}

			auto& astr = mAsteroids[i];

			astr.pos = pos;
			astr.vel = Vec2f{ vvel( mRNG ), vvel( mRNG ) };

			astr.rot = make_rotation_2d( angle( mRNG ) );
			astr.radpersec = rots( mRNG );

			// Don't break the speed limits. The space police will get you!
			astr.vel.x = std::clamp( astr.vel.x, -mMaximumSpeed, +mMaximumSpeed );
			astr.vel.y = std::clamp( astr.vel.y, -mMaximumSpeed, +mMaximumSpeed );

			// Create shape
			assert( i == mShapes.size() );
			mShapes.emplace_back( make_asteroid( mRNG ) );
		}
	}

	assert( mAsteroids.size() == mShapes.size() );
}

