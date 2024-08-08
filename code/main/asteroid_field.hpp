#ifndef ASTEROID_FIELD_HPP_7D5A0B40_4466_4CAC_B7CC_85E8DC927E08
#define ASTEROID_FIELD_HPP_7D5A0B40_4466_4CAC_B7CC_85E8DC927E08

#include <vector>

#include <cstdlib>

#include "../draw2d/forward.hpp"

#include "../vmlib/vec2.hpp"
#include "../vmlib/mat22.hpp"

#include "defaults.hpp"

/** Asteroid field
 *
 * Similar to the particle fields, the  asteroid field only exists around the
 * "player" (i.e., it is always centered on the screen's position). This gives
 * the appearance of an infinite asteroid field (without us having to track an
 * ever increasing amount of asteroids). Like the particle field, the asteroid
 * field extends a bit outside of the field of view. This means that asteroids
 * to not disappear the moment they are outside of the visible region (i.e., if
 * a asteroid exits the screen, the player turns around immediately, the same
 * asteroid still exists).
 *
 * With the current implementation, the asteroid field is a purely visual
 * effect.
 */
class AsteroidField
{
	public:
		AsteroidField(
			RNG&,
			std::uint32_t aImageWidth, std::uint32_t aImageHeight,
			float aDensity = 1e-5f,
			float aInitialSpeedStddev = 100.f,
			float aMaximumSpeed = 500.f,
			float aInitialRotStddev = 1.5f,
			float aPadding = 300.f
		);

		~AsteroidField();

		// Not copyable nor movable
		AsteroidField( AsteroidField const& ) = delete;
		AsteroidField& operator= (AsteroidField const&) = delete;

	public:
		void update( float aElapsedTimeSec, Vec2f const& aMovement );

		void draw( Surface& ) const;

		void resize( std::uint32_t aWidth, std::uint32_t aHeight );

	private:
		struct Asteroid_
		{
			Vec2f pos;
			Vec2f vel;
			
			Mat22f rot;
			float radpersec;
		};

	private:
		Vec2f mBoundsMin, mBoundsMax;
		Vec2f mExactExtent, mActualExtent;
		
		std::vector<Asteroid_> mAsteroids;
		std::vector<TriangleFan> mShapes;

		float mInitialSpeed, mMaximumSpeed;
		float mInitialRot;
		float mPadding, mDensity;

		RNG& mRNG;
};

#endif // ASTEROID_FIELD_HPP_7D5A0B40_4466_4CAC_B7CC_85E8DC927E08
