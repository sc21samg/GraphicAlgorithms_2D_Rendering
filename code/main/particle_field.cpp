#include "particle_field.hpp"

#include "../draw2d/surface.hpp"

#include <cassert> 

ParticleField::ParticleField( RNG& aRNG, std::uint32_t aImageWidth, std::uint32_t aImageHeight, ColorF const& aParticleColor, float aParticleDensity, float aParticleSpeedMult, float aPadding )
	: mColor( linear_to_srgb( aParticleColor ) )
	, mParticleSpeedMult( aParticleSpeedMult )
	, mParticleDensity( aParticleDensity )
	, mPadding( aPadding )
	, mRNG( aRNG )
{
	// Store extents
	mVisibleExtent.x = float(aImageWidth);
	mVisibleExtent.y = float(aImageHeight);

	mBoxMin = Vec2f{ -mPadding, -mPadding };
	mBoxMax = mVisibleExtent + Vec2f{ mPadding, mPadding };

	// Allocate particles
	Vec2f const extent = mBoxMax - mBoxMin;
	
	float const totalArea = extent.x * extent.y;
	float const particleCountf = totalArea * mParticleDensity;
	std::size_t const particleCount = std::size_t(particleCountf+0.5f);

	mParticles.resize( particleCount );

	// Initialize particles
	std::uniform_real_distribution<float> xdist( mBoxMin.x, mBoxMax.x );
	std::uniform_real_distribution<float> ydist( mBoxMin.y, mBoxMax.y );

	for( auto& p : mParticles )
	{
		p.x = xdist(mRNG);
		p.y = ydist(mRNG);
	}
}


void ParticleField::update( Vec2f aDelta ) noexcept
{
	// Note: the delta here is reversed -- the particles move in the 
	// opposite direction as the "player".
	auto const delta = -mParticleSpeedMult * aDelta;

	std::uniform_real_distribution<float> xdist( mBoxMin.x, mBoxMax.x );
	std::uniform_real_distribution<float> ydist( mBoxMin.y, mBoxMax.y );

	float const padX = std::max( std::abs(aDelta.x), mPadding );
	float const padY = std::max( std::abs(aDelta.y), mPadding );

	std::uniform_real_distribution<float> xpad( 0.f, padX );
	std::uniform_real_distribution<float> ypad( 0.f, padY );


	for( auto& particle : mParticles )
	{
		Vec2f p = particle + delta;

		if( p.x < mBoxMin.x )
		{
			p.x = mBoxMax.x - xpad(mRNG);
			p.y = ydist(mRNG);
		}
		else if( p.x > mBoxMax.x )
		{
			p.x = mBoxMin.x + xpad(mRNG);
			p.y = ydist(mRNG);
		}

		if( p.y < mBoxMin.y )
		{
			p.x = xdist(mRNG);
			p.y = mBoxMax.y - ypad(mRNG);
		}
		else if( p.y > mBoxMax.y )
		{
			p.x = xdist(mRNG);
			p.y = mBoxMin.y + ypad(mRNG);
		}

		particle = p;
	}
}

void ParticleField::draw( Surface& aSurface ) const
{
	for( auto const& particle : mParticles )
	{
		auto const p = particle + Vec2f{ .5f, .5f };

		if( p.x < 0.f || p.y < 0.f )
			continue;
		
		std::uint32_t const xpos = std::uint32_t( p.x + .5f );
		std::uint32_t const ypos = std::uint32_t( p.y + .5f );

		if( xpos < aSurface.get_width() && ypos < aSurface.get_height() )
			aSurface.set_pixel_srgb( xpos, ypos, mColor );
	}
}

void ParticleField::resize( std::uint32_t aImageWidth, std::uint32_t aImageHeight )
{
	auto const oldMax = mBoxMax;

	// New extents and particle count
	mVisibleExtent.x = float(aImageWidth);
	mVisibleExtent.y = float(aImageHeight);

	mBoxMax = mVisibleExtent + Vec2f{ mPadding, mPadding };

	Vec2f const extent = mBoxMax - mBoxMin;
	
	float const totalArea = extent.x * extent.y;
	float const particleCountf = totalArea * mParticleDensity;
	std::size_t const particleCount = std::size_t(particleCountf+0.5f);

	// Remove particles now outside
	std::size_t activeParticles = 0;
	for( auto it = mParticles.begin(); it != mParticles.end(); )
	{
		auto const& part = *it;

		if( part.x > mBoxMax.x || part.y > mBoxMax.y )
			it = mParticles.erase( it );
		else
			++it, ++activeParticles;
	}

	mParticles.resize( particleCount ); // This may kill a few visible particles..

	// Add new particles (if necessary)
	if( activeParticles < particleCount )
	{
		auto dd = mBoxMax - oldMax;
		if( dd.x < 0.f ) dd.x = 0.f;
		if( dd.y < 0.f ) dd.y = 0.f;

		float xarea = dd.x * mBoxMax.y;
		float yarea = dd.y * (mBoxMax.x - dd.x);

		std::uniform_real_distribution<float> area( 0.f, xarea+yarea );
		std::uniform_real_distribution<float> xax( oldMax.x, oldMax.x+dd.x );
		std::uniform_real_distribution<float> xay( 0.f, mBoxMax.y );
		std::uniform_real_distribution<float> yax( 0.f, mBoxMax.x - dd.x );
		std::uniform_real_distribution<float> yay( oldMax.y, oldMax.y+dd.y );

		for( std::size_t i = activeParticles; i < particleCount; ++i )
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

			mParticles[i] = pos;
		}
	}
}
