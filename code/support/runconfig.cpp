#include "runconfig.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "error.hpp"

namespace
{
	void synopsis_( char const* );
}

RuntimeConfig parse_command_line( int aArgc, char const* const* aArgv )
{
	RuntimeConfig config;

	for( int i = 1; i < aArgc; ++i )
	{
		char name[128], value[128];
		int ret = std::sscanf( aArgv[i], "--%127[a-zA-Z0-9_]=%127s", name, value );

		if( ret == 1 )
		{
			if( 0 == std::strcmp( "help", name ) )
			{
				synopsis_( aArgv[0] );
				std::exit( 0 );
			}
			else
			{
				throw Error( "Error while parsing command line\n" 
					"Unrecognized flag '--%s'\n"
					"Use --help to print available command line options", name );
			}

		}
		else if( ret == 2 )
		{
			char dummy;
			if( 0 == std::strcmp( "fbshift", name ) )
			{
				unsigned shift = 0;
				if( 1 != std::sscanf( value, "%u%c", &shift, &dummy ) )
				{
					throw Error( "Error while parsing command line\n" 
						"Value '%s' not valid for --fbshift; expected unsigned integer\n"
						"Use --help to print available command line options", name );
				}

				config.framebufferScaleShift = shift;
			}
			else if( 0 == std::strcmp( "geometry", name ) )
			{
				unsigned width = 0, height = 0;
				if( 2 != std::sscanf( value, "%ux%u%c", &width, &height, &dummy ) )
				{
					throw Error( "Error while parsing command line\n" 
						"Value '%s' not valid for --geometry; expected <width>x<height>\n"
						"Use --help to print available command line options", name );
				}

				config.initialWindowWidth = width;
				config.initialWindowHeight = height;
			}
			else
			{
				throw Error( "Error while parsing command line\n" 
					"Unrecognized option '--%s' \n"
					"Use --help to print available command line options", name );
			}
		}
		else
		{
			throw Error( "Error while parsing command line\n" 
				"Expected flag, got unparsable mayhem '%s'\n"
				"Use --help to print available command line options", aArgv[i] );
		}
	}

	return config;
}


namespace
{
	constexpr char const synopsis[] = R"(Synopsis: %s [--<flag> [, ...]] [--<option>=<value> [, ...]]

Where <flag> may be one off the following
  help         : print this help and exit successfully

and where <option> and <value> may be the following
  geometry    <width>x<height>    set initial window size to (width, height)
  fbshift     <shift>             scale framebuffer by 2^-<shift> (unsigned int)

Example:
  %s --geometry=1920x1080 --fbshift=1
Creates a window that is 1920x1080 in size. The framebuffer is half size:
(1920>>1)x(1080>>1) = 1920/2^1 x 1080^2^1 = 960x540 pixels
The framebuffer will consequently be magnified by a factor two.
)";
	
	void synopsis_( char const* aProgramName )
	{
		std::printf( synopsis, aProgramName, aProgramName );

	}
}
