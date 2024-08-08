#include "context.hpp"

#include <vector>

#include <cstdio>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "error.hpp"
#include "checkpoint.hpp"

#include "../draw2d/surface.hpp"

namespace
{
	// Note to Apple users:
	// See context-oldgl.cpp instead. Apple is <= GL 4.1, so some features used 
	// here are not available:
	//   - Debug output!
	//   - GLSL Version 4.20 (e.g., #version 420)
	//   - layout( binding = 0 ) uniform sampler2D ...
	//   - Immutable texture storage (glTexStorage2D)
#	if defined(__APPLE__)
#		error "Error: use context-oldgl.cpp on Apple devices."
#	endif
	
	constexpr char const kVertShaderGLSL[] = R"(
		#version 420
		
		// See 
		// https://www.saschawillems.de/blog/2016/08/13/vulkan-tutorial-on-rendering-a-fullscreen-quad-without-buffers/
		// This version is adapted for OpenGL's GLSL and coordinate system.

		layout( location = 0 ) out vec2 v2fTexCoord;

		void main()
		{
			v2fTexCoord = vec2(
				(gl_VertexID << 1) & 2,
				gl_VertexID & 2
			);

			gl_Position = vec4( 2.0 * v2fTexCoord - 1.0, 0.0, 1.0 );
		}
	)";
	constexpr char const kFragShaderGLSL[] = R"(
		#version 420

		layout( location = 0 ) in vec2 v2fTexCoord;

		layout( binding = 0 ) uniform sampler2D uTexSoftFB;

		layout( location = 0 ) out vec3 oColor;

		void main()
		{
			oColor = texture( uTexSoftFB, v2fTexCoord ).rgb;
		}
	)";

	// compile shader source
	GLuint compile_shader_( 
		GLenum aShaderType, 
		std::size_t aSourceLength, 
		char const* aSource,
		char const* aShaderIdent = nullptr
	);

	// lightweight std::experimental::scope_exit alternative
	// Not the most complete or convenient implementation...
	template< typename tFunc >
	struct ScopeExit_
	{
		template< typename tFuncIn >
		ScopeExit_( tFuncIn&& aFunc ) : mFunc( std::forward<tFuncIn>(aFunc) ) {}
		~ScopeExit_() { mFunc(); }

		tFunc mFunc;
	};

	template< typename tFunc > inline
	ScopeExit_<tFunc> scope_exit_( tFunc&& aFunc )
	{
		return ScopeExit_<tFunc>( std::forward<tFunc>(aFunc) );
	}

	// Debug callback
#	if !defined(NDEBUG)
	void GLAPIENTRY callback_gldebug_( GLenum, GLenum, GLuint, GLenum, GLsizei, GLchar const*, void const* );
#	endif // ~ !NDEBUG
}

Context::Context( std::size_t aWidth, std::size_t aHeight )
	: mTexImage( 0 )
	, mWidth( 0 ), mHeight( 0 )
	, mVAO( 0 )
	, mProgram( 0 )
{
	init_glad_();
	init_gl_();

#	if !defined(NDEBUG)
	init_gl_debug_();
#	endif // ~!NDEBUG

	resize( aWidth, aHeight );
}

Context::~Context()
{
	if( 0 != mTexImage )
		glDeleteTextures( 1, &mTexImage );

	if( 0 != mVAO )
		glDeleteVertexArrays( 1, &mVAO );

	if( 0 != mProgram )
		glDeleteProgram( mProgram );
}


void Context::draw( Surface const& aSurface )
{
	OGL_CHECKPOINT_DEBUG();

	// Calling glInvalidateFramebuffer() seems tempting at this point, to
	// indicate to the drivers that the framebuffer's contents are no longer
	// required and will be replaced fully. However, on my test systems with
	// NVIDIA cards, this ends up breaking things in rather strange ways.

	// Upload texture image
	// We could potentially use a PBO here to make the copy asynchronous.
	// (However, then using a PBO, we could also directly draw into PBO memory
	// if it's HOST_CACHED, so let's not go down that route).
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, mTexImage );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	glTexSubImage2D( GL_TEXTURE_2D,
		0,
		0, 0,
		GLsizei(mWidth), GLsizei(mHeight),
		GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV,
		aSurface.get_surface_ptr()
	);

	// Draw stuff
	glUseProgram( mProgram );
	glBindVertexArray( mVAO );
	glDrawArrays( GL_TRIANGLES, 0, 3 );

	// Cleanup (kinda optional)
	glBindVertexArray( 0 );
	glUseProgram( 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	
	OGL_CHECKPOINT_DEBUG();
}

void Context::resize( std::size_t aWidth, std::size_t aHeight )
{
	if( aWidth == mWidth && aHeight == mHeight )
		return;

	GLuint const tex = create_tex_image_( aWidth, aHeight );
	if( 0 != tex )
	{
		glDeleteTextures( 1, &mTexImage );

		mTexImage = tex;
		mWidth = aWidth;
		mHeight = aHeight;
	}
}


void Context::init_glad_()
{
	if( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
		throw Error( "GLAD returned error when loading GL API" );

	std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
	std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
	std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
	std::printf( "SHADING_LANGUAGE_VERSION %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	OGL_CHECKPOINT_ALWAYS();
}
void Context::init_gl_()
{
	OGL_CHECKPOINT_ALWAYS();

	glEnable( GL_FRAMEBUFFER_SRGB );
	
	OGL_CHECKPOINT_ALWAYS();

	mVAO = create_vao_();
	mProgram = create_program_();
}

#if !defined(NDEBUG)
void Context::init_gl_debug_()
{
	OGL_CHECKPOINT_ALWAYS();

	// glDebugMessageCallback() was standardized in 4.3. It gives us much
	glDebugMessageCallback( &callback_gldebug_, nullptr );
	glEnable( GL_DEBUG_OUTPUT );

	OGL_CHECKPOINT_ALWAYS();
}
#endif // ~ !NDEBUG

GLuint Context::create_vao_()
{
	OGL_CHECKPOINT_ALWAYS();

	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glBindVertexArray( 0 );
	
	OGL_CHECKPOINT_ALWAYS();
	
	return vao;
}
GLuint Context::create_program_()
{
	OGL_CHECKPOINT_ALWAYS();

	GLuint vert = compile_shader_( GL_VERTEX_SHADER, sizeof(kVertShaderGLSL), kVertShaderGLSL, "fullscreen blit" );
	auto const scopeV_ = scope_exit_( [&vert] {
		glDeleteShader( vert );
	} );

	OGL_CHECKPOINT_ALWAYS();

	GLuint frag = compile_shader_( GL_FRAGMENT_SHADER, sizeof(kFragShaderGLSL), kFragShaderGLSL, "fullscreen blit" );
	auto const scopeF_ = scope_exit_( [&frag] {
		glDeleteShader( frag );
	} );

	OGL_CHECKPOINT_ALWAYS();

	GLuint prog = glCreateProgram();

	{
		// Link individual shaders to create the final shader program
		glAttachShader( prog, vert );
		glAttachShader( prog, frag );

		glLinkProgram( prog );

		// Get info log
		GLint logLength = 0;
		glGetProgramiv( prog, GL_INFO_LOG_LENGTH, &logLength );

		std::vector<GLchar> log;
		if( logLength )
		{
			log.resize( logLength );
			glGetProgramInfoLog( prog, GLsizei(log.size()), nullptr, log.data() );
		}

		// Check link status
		GLint status = 0;
		glGetProgramiv( prog, GL_LINK_STATUS, &status );

		if( GL_TRUE != status )
			throw Error( "Shader program linking failed: \n%s\n", log.data() );

		if( !log.empty() )
			std::fprintf( stderr, "Note: shader program linking log:\n%s\n", log.data() );
	}
	
	OGL_CHECKPOINT_ALWAYS();

	return prog;
}

GLuint Context::create_tex_image_( std::size_t aWidth, std::size_t aHeight )
{
	OGL_CHECKPOINT_ALWAYS();
	
	GLuint tex = 0;
	glGenTextures( 1, &tex );
	glBindTexture( GL_TEXTURE_2D, tex );

	// Use immuatable texture storage, Immutable texture storage was added in
	// OpenGL 4.2. Immutable textures cannot change (storage) size, which gives
	// drivers another opportunity for optimizations. We cannot use
	// glTexImage2D to upload texture data, but rather must use something like
	// glTexSubImage2D().
	glTexStorage2D( GL_TEXTURE_2D, 
		1, 
		GL_SRGB8_ALPHA8,
		GLsizei(aWidth), GLsizei(aHeight)
	);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glBindTexture( GL_TEXTURE_2D, 0 );

	OGL_CHECKPOINT_ALWAYS();
	
	return tex;
}


namespace
{
	GLuint compile_shader_( GLenum aShaderType, std::size_t aSourceLength, char const* aSource, char const* aShaderIdent )
	{
		if( !aShaderIdent )
			aShaderIdent = "unnamed shader";

		GLuint shader = glCreateShader( aShaderType );

		// Compile shader
		GLchar const* sources[] = {
			aSource
		};
		GLsizei lengths[] = {
			GLsizei(aSourceLength)
		};

		glShaderSource( shader, sizeof(sources)/sizeof(sources[0]), sources, lengths );

		glCompileShader( shader );

		// Get compile info log
		// The compile log is mainly relevant if there is an error. However, on some
		// systems, it can include additional information even if compilation was
		// successful. This might include warnings and/or usage hints.
		GLint logLength = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );

		std::vector<GLchar> log;
		if( logLength )
		{
			log.resize( logLength );
			glGetShaderInfoLog( shader, GLsizei(log.size()), nullptr, log.data() );
		}

		char const* shaderTypeName = "unknown shader";
		switch( aShaderType )
		{
			case GL_VERTEX_SHADER: shaderTypeName = "vertex shader"; break;
			case GL_FRAGMENT_SHADER: shaderTypeName = "fragment shader"; break;
		}

		// Check compile status
		GLint status = 0;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &status );

		if( GL_TRUE != status )
			throw Error( "%s \"%s\" compilation failed:\n%s\n", shaderTypeName, aShaderIdent, log.data() );

		if( !log.empty() )
			std::fprintf( stderr, "Note: %s \"%s\" log:\n%s\n", shaderTypeName, aShaderIdent, log.data() );

		return shader;
	}
}

namespace
{
#	if !defined(NDEBUG)
	char const* type_str_( GLenum aType ) noexcept
	{
		switch( aType )
		{
			case GL_DEBUG_TYPE_ERROR: return "Error";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behaviour";
			case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
			case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
			case GL_DEBUG_TYPE_MARKER: return "Marker";
			case GL_DEBUG_TYPE_PUSH_GROUP: return "Push";
			case GL_DEBUG_TYPE_POP_GROUP: return "Pop";
			case GL_DEBUG_TYPE_OTHER: return "Other";
		}

		return "<unknown type>";
	}
	char const* severity_str_( GLenum aSeverity ) noexcept
	{
		switch( aSeverity )
		{
			case GL_DEBUG_SEVERITY_HIGH: return "ERROR";
			case GL_DEBUG_SEVERITY_MEDIUM: return "Warning";
			case GL_DEBUG_SEVERITY_LOW: return "Note";
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "Info";
		}

		return "<unknown severity>";
	}

	void GLAPIENTRY callback_gldebug_( GLenum, GLenum aType, GLuint, GLenum aSeverity, GLsizei, GLchar const* aMessage, void const* /*aUser*/ )
	{
		std::fprintf( stderr, "OpenGL Debug: %s [%s]: %s\n", severity_str_(aSeverity), type_str_(aType), aMessage );

		// For high severity errors, break into the debugger now.
		if( GL_DEBUG_SEVERITY_HIGH == aSeverity )
			assert( false );
	}
#	endif // ~ !NDEBUG
}
