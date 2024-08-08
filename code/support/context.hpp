#ifndef CONTEXT_HPP_10336F78_4E1A_4D2A_A794_D47D8406FF58
#define CONTEXT_HPP_10336F78_4E1A_4D2A_A794_D47D8406FF58

#include <glad.h>

#include <cstdint>
#include <cstdlib>

#include "../draw2d/forward.hpp"

class Context final
{
	public:
		Context( std::size_t aWidth, std::size_t aHeight );
		~Context();

		Context( Context const& ) = delete;
		Context& operator= (Context const&) = delete;

		Context( Context&& ) noexcept;
		Context& operator= (Context&&) noexcept;

	public:
		void draw( Surface const& );

		void resize( std::size_t aWidth, std::size_t aHeight );

	private:
		void init_glad_();
		void init_gl_();

#		if !defined(NDEBUG)
		void init_gl_debug_();
#		endif // ~ NDEBUG
		
		GLuint create_vao_();
		GLuint create_program_();

		GLuint create_tex_image_( std::size_t aWidth, std::size_t aHeight );

	private:
		// Surface texture
		GLuint mTexImage;
		std::size_t mWidth, mHeight;
		
		// Drawing
		// We need an empty VAO for attribute-less rendering. Drawing with the
		// default VAO (=0) is disallowed.
		GLuint mVAO;
		GLuint mProgram;
};

#endif // CONTEXT_HPP_10336F78_4E1A_4D2A_A794_D47D8406FF58
