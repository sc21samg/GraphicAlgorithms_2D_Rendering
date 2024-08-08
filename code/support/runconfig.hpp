#ifndef RUNCONFIG_HPP_6700ED29_C137_4C7A_8BE7_00D6C7CDD0D1
#define RUNCONFIG_HPP_6700ED29_C137_4C7A_8BE7_00D6C7CDD0D1

namespace cfg
{
	constexpr unsigned kInitialWindowWidth = 1280;
	constexpr unsigned kInitialWindowHeight = 720;
}

struct RuntimeConfig
{
	unsigned initialWindowWidth = cfg::kInitialWindowWidth;
	unsigned initialWindowHeight = cfg::kInitialWindowHeight;

	unsigned framebufferScaleShift = 0;
};

RuntimeConfig parse_command_line( int aArgc, char const* const* aArgv );

#endif // RUNCONFIG_HPP_6700ED29_C137_4C7A_8BE7_00D6C7CDD0D1
