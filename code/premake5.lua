workspace "COMP3811-cw1"
	language "C++"
	cppdialect "C++17"

	platforms { "x64" }
	configurations { "debug", "release" }

	flags "NoPCH"
	flags "MultiProcessorCompile"

	startproject "main"

	debugdir "%{wks.location}"
	objdir "_build_/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"
	targetsuffix "-%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"
	
	-- Default toolset options
	filter "toolset:gcc or toolset:clang"
		linkoptions { "-pthread" }
		buildoptions { "-march=native", "-Wall", "-pthread" }

		-- Varriable-length arrays (VLAs) are an extension that GCC and clang
		-- have long supported. However, they are not part of the C++ standard.
		-- (MSVC will not compile code with VLAs.)
		buildoptions { "-Werror=vla" }

	filter "toolset:msc-*"
		warnings "extra" -- this enables /W4; default is /W3
		--buildoptions { "/W4" }
		buildoptions { "/utf-8" }
		buildoptions { "/permissive-" }
		defines { "_CRT_SECURE_NO_WARNINGS=1" }
		defines { "_SCL_SECURE_NO_WARNINGS=1" }
	
	filter "*"

	-- default libraries
	filter "system:linux"
		links "dl"
	
	filter "system:windows"
		links "OpenGL32"

	filter "*"

	-- default outputs
	filter "kind:StaticLib"
		targetdir "lib/"

	filter "kind:ConsoleApp"
		targetdir "bin/"
		targetextension ".exe"
	
	filter "*"

	--configurations
	filter "debug"
		symbols "On"
		defines { "_DEBUG=1" }

	filter "release"
		optimize "On"
		defines { "NDEBUG=1" }

	filter "*"




-- Third party dependencies
include "third_party" 

-- Projects
project "main"
	local sources = { 
		"main/**.cpp",
		"main/**.hpp",
		"main/**.hxx",
		"main/**.inl"
	}

	kind "ConsoleApp"
	location "main"

	files( sources )

	links "vmlib"
	links "support"
	links "draw2d"

	links "x-stb"
	links "x-glad"
	links "x-glfw"

project "draw2d"
	local sources = { 
		"draw2d/**.cpp",
		"draw2d/**.hpp",
		"draw2d/**.hxx",
		"draw2d/**.inl"
	}

	kind "StaticLib"
	location "draw2d"

	files( sources )

project "support"
	local sources = { 
		"support/checkpoint.cpp",
		--"support/context.cpp", -- separate implementation on Apple
		"support/error.cpp",
		"support/runconfig.cpp",
		"support/checkpoint.hpp",
		"support/context.hpp",
		"support/error.hpp",
		"support/runconfig.hpp",
	}

	kind "StaticLib"
	location "support"

	files( sources )

	-- Most systems use the normal context.cpp implementation
	filter "system:not macosx"
		files( "support/context.cpp" );

	-- Apple is stuck with older OpenGL
	filter "system:macosx" 
		files( "support/context-oldgl.cpp" );
	
	filter "*"

project "vmlib"
	local sources = { 
		"vmlib/**.cpp",
		"vmlib/**.hpp",
		"vmlib/**.hxx",
		"vmlib/**.inl"
	}

	kind "StaticLib"
	location "vmlib"

	files( sources )

project "lines-sandbox"
	local sources = { 
		"lines-sandbox/**.cpp",
		"lines-sandbox/**.hpp",
		"lines-sandbox/**.hxx",
		"lines-sandbox/**.inl"
	}

	kind "ConsoleApp"
	location "lines-sandbox"

	files( sources )

	links "vmlib"
	links "support"
	links "draw2d"

	links "x-glad"
	links "x-glfw"

project "lines-test"
	local sources = { 
		"lines-test/**.cpp",
		"lines-test/**.hpp",
		"lines-test/**.hxx",
		"lines-test/**.inl"
	}

	kind "ConsoleApp"
	location "lines-test"

	files( sources )

	links "vmlib"
	links "draw2d"

	links "x-catch2"


project "triangles-sandbox"
	local sources = { 
		"triangles-sandbox/**.cpp",
		"triangles-sandbox/**.hpp",
		"triangles-sandbox/**.hxx",
		"triangles-sandbox/**.inl"
	}

	kind "ConsoleApp"
	location "triangles-sandbox"

	files( sources )

	links "vmlib"
	links "support"
	links "draw2d"

	links "x-glad"
	links "x-glfw"

project "triangles-test"
	local sources = { 
		"triangles-test/**.cpp",
		"triangles-test/**.hpp",
		"triangles-test/**.hxx",
		"triangles-test/**.inl"
	}

	kind "ConsoleApp"
	location "triangles-test"

	files( sources )

	links "vmlib"
	links "draw2d"

	links "x-catch2"

project "blit-benchmark"
	local sources = { 
		"blit-benchmark/**.cpp",
		"blit-benchmark/**.hpp",
		"blit-benchmark/**.hxx",
		"blit-benchmark/**.inl"
	}

	kind "ConsoleApp"
	location "blit-benchmark"

	files( sources )

	links "vmlib"
	links "draw2d"
	links "support"

	links "x-stb"
	links "x-benchmark"

project "lines-benchmark"
	local sources = { 
		"lines-benchmark/**.cpp",
		"lines-benchmark/**.hpp",
		"lines-benchmark/**.hxx",
		"lines-benchmark/**.inl"
	}

	kind "ConsoleApp"
	location "lines-benchmark"

	files( sources )

	links "vmlib"
	links "draw2d"

	links "x-benchmark"

--EOF
