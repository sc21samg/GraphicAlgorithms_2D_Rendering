#include "shape.hpp"

#include <utility>

#include <cassert>
#include <cstring>

#include "draw.hpp"
#include "color.hpp"
#include "surface.hpp"

LineStrip::LineStrip( std::size_t aCount, Vec2f const* aVerts )
	: mCount( aCount )
	, mVertices( nullptr )
{
	assert( aVerts );

	mVertices = new Vec2f[mCount];
	std::memcpy( mVertices, aVerts, sizeof(Vec2f)*mCount );
}

LineStrip::~LineStrip()
{
	delete [] mVertices;
}

LineStrip::LineStrip( LineStrip&& aOther ) noexcept
	: mCount( std::exchange( aOther.mCount, 0 ) )
	, mVertices( std::exchange( aOther.mVertices, nullptr ) )
{}
LineStrip& LineStrip::operator= (LineStrip&& aOther)  noexcept
{
	std::swap( mCount, aOther.mCount );
	std::swap( mVertices, aOther.mVertices );
	return *this;
}

void LineStrip::draw( Surface& aSurface, ColorF const& aColor, Mat22f const& aRotation, Vec2f const& aTranslation ) const
{
	ColorU8_sRGB const color = linear_to_srgb( aColor );

	Vec2f previous = aRotation * mVertices[0] + aTranslation;

	for( std::size_t i = 1; i < mCount; ++i )
	{
		Vec2f const current = aRotation * mVertices[i] + aTranslation;
		draw_line_solid( aSurface, previous, current, color );
		previous = current;
	}
}


TriangleFan::TriangleFan( std::size_t aCount, PosAndCol const* aVerts )
	: mCount( aCount )
	, mVertices( nullptr )
	, mColors( nullptr )
{
	// Note: technically unsafe if "new" fails to allocate memory

	mVertices = new Vec2f[mCount];
	mColors = new ColorF[mCount];

	for( std::size_t i = 0; i < mCount; ++i )
	{
		mVertices[i] = aVerts[i].pos;
		mColors[i] = aVerts[i].col;
	}
}
TriangleFan::TriangleFan( std::size_t aCount, Vec2f const* aVerts, ColorF const* aColors )
	: mCount( aCount )
	, mVertices( nullptr )
	, mColors( nullptr )
{
	assert( aVerts && aColors );

	// Note: technically unsafe if "new" fails to allocate memory

	mVertices = new Vec2f[mCount];
	std::memcpy( mVertices, aVerts, sizeof(Vec2f)*mCount );

	mColors = new ColorF[mCount];
	std::memcpy( mColors, aColors, sizeof(ColorF)*mCount );
}

TriangleFan::~TriangleFan()
{
	delete [] mColors;
	delete [] mVertices;
}


TriangleFan::TriangleFan( TriangleFan&& aOther ) noexcept
	: mCount( std::exchange( aOther.mCount, 0 ) )
	, mVertices( std::exchange( aOther.mVertices, nullptr ) )
	, mColors( std::exchange( aOther.mColors, nullptr ) )
{}
TriangleFan& TriangleFan::operator= (TriangleFan&& aOther)  noexcept
{
	std::swap( mCount, aOther.mCount );
	std::swap( mVertices, aOther.mVertices );
	std::swap( mColors, aOther.mColors );
	return *this;
}


void TriangleFan::draw( Surface& aSurface, Mat22f const& aRotation, Vec2f const& aTranslation ) const
{
	Vec2f const center = aRotation * mVertices[0] + aTranslation;
	ColorF const cencol = mColors[0];

	Vec2f previous = aRotation * mVertices[1] + aTranslation;
	ColorF pcol = mColors[1];
	for( std::size_t i = 2; i < mCount; ++i )
	{
		Vec2f const current = aRotation * mVertices[i] + aTranslation;
		ColorF const curcol = mColors[i];
		draw_triangle_interp( aSurface, center, previous, current, cencol, pcol, curcol );
		previous = current;
		pcol = curcol;
	}

	Vec2f const first = aRotation * mVertices[1] + aTranslation;
	ColorF const fcol = mColors[1];
	draw_triangle_interp( aSurface, center, previous, first, cencol, pcol, fcol );
}
