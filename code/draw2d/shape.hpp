#ifndef SHAPE_HPP_4AC47446_8CA0_4AFF_AD91_D6B54EFEF21A
#define SHAPE_HPP_4AC47446_8CA0_4AFF_AD91_D6B54EFEF21A

// IMPORTANT: DO NOT CHANGE THIS FILE WHEN YOU ARE SOLVING COURSEWORK 1!
//
// For CW1, the shape.hpp file must remain exactly as it is. In particular, you
// must not change the LineStrip or TriangleFan class interfaces.

#include <cstdlib>

#include "forward.hpp"
#include "color.hpp"

#include "../vmlib/vec2.hpp"
#include "../vmlib/mat22.hpp"

/** Line strip
 *
 * A line strip is a sequence of lines. It is defined by N points, which are
 * connected by N-1 lines. For example, three points (P0, P1, and P2) would
 * form the two lines P0 to P1 followed by P1 to P2.
 */
class LineStrip final
{
	public:
		LineStrip( std::size_t aCount, Vec2f const* );

		/* This allows the user to create a "hand-defined" line strip more 
		 * easily. Example:
		 *
		 * LineStrip ls{ 
		 * 	{ 1.f, 1.f },
		 * 	{ 2.f, 2.f },
		 * 	...
		 * 	{ 3.f, 3.f }
		 * };
		 */
		template< std::size_t tCount >
		LineStrip( Vec2f const (&aArray)[tCount] )
			: LineStrip( tCount, aArray )
		{}

		~LineStrip();

		// Not copyable but movable
		LineStrip( LineStrip const& ) = delete;
		LineStrip& operator= (LineStrip const&) = delete;

		LineStrip( LineStrip&& ) noexcept;
		LineStrip& operator= (LineStrip&&) noexcept;

	public:
		/* Draw the line strip onto the specified Surface in the specified
		 * Color (linear color). Vertices are transformed by the provided
		 * matrices and translated by the provided vector:
		 *
		 * finalVertex = vertexIn * matrix + vector
		 *
		 * LineStrip::draw() uses draw_line_solid() internally.
		 */
		void draw( Surface&, ColorF const&, Mat22f const&, Vec2f const& ) const;

		std::size_t vertex_count() const noexcept { return mCount; }

	private:
		std::size_t mCount;
		Vec2f* mVertices;
};

/** Triangle fan
 *
 * A triangle fan is a set of triangles, defined by a central vertex, and with
 * other vertex providing vertices around the central vertex. The fan is closed
 * by connecting the final vertex with the first one (note that this differs
 * from the OpenGL triangle fan definition; OpenGL does not include the last
 * triangle). Given N vertices, a triangle fan creates N-1 triangles.
 *
 * Example: the points P0, P1, P2, P3, P4 create the triangles
 *
 *   Triangle 1: P0 P1 P2
 *   Triangle 2: P0 P2 P3
 *   Triangle 3: P0 P3 P4
 *   Triangle 4: P0 P4 P1
 */
class TriangleFan final
{
	public:
		// This is mainly used for two of the constructors. The actual data is
		// stored in SoA and not AoS, though.
		struct PosAndCol
		{
			Vec2f pos;
			ColorF col;
		};

	public:
		TriangleFan( std::size_t aCount, PosAndCol const* );
		TriangleFan( std::size_t aCount, Vec2f const*, ColorF const* );

		// See LineStrip above.
		template< std::size_t tCount >
		TriangleFan( PosAndCol const (&aArray)[tCount] )
			: TriangleFan( tCount, aArray )
		{}

		~TriangleFan();

		// Not copyable but movable
		TriangleFan( TriangleFan const& ) = delete;
		TriangleFan& operator= (TriangleFan const&) = delete;

		TriangleFan( TriangleFan&& ) noexcept;
		TriangleFan& operator= ( TriangleFan&& ) noexcept;

	public:
		/* Draw the triangle fan onto the specified Surface. Vertices are
		 * transformed by the provided matrices and translated by the provided
		 * vector:
		 *
		 * finalVertex = vertexIn * matrix + vector
		 *
		 * TriangleFan::draw() uses draw_triangle_interp() internally.  It uses
		 * the (linear) per-vertex colors assigned at construction time.
		 */
		void draw( Surface&, Mat22f const&, Vec2f const& ) const;


	private:
		std::size_t mCount;
		Vec2f* mVertices;
		ColorF* mColors;
};

#endif // SHAPE_HPP_4AC47446_8CA0_4AFF_AD91_D6B54EFEF21A
