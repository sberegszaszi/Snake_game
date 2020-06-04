-------------------------------
-- Workspace configuration
-------------------------------

workspace "Snake_game"
    configurations { "Debug", "Release" }
    system "linux"
    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- -----------------------------
-- Compiler configuration
-- -----------------------------

-- premake defaults to using gcc/g++

filter "configurations:Debug"
	symbols  "On"

filter "configurations:Release"
	optimize "On"

configuration { "Debug", "linux", "gmake2" }
	buildoptions { "-Wall", "-Wextra", "-Wdeprecated", "-Weffc++", "-pedantic" }

-------------------------------
-- Project configuration
-------------------------------

project "Snake_game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir ("build/" .. outputdir .. "/")
	objdir ("build/" .. outputdir .. "/obj/")

files { "source/**.hpp", "source/**.cpp" }

-------------------------------
-- Dependencies
-------------------------------

configuration { "Release", "linux" }
	links { "ncurses" }

configuration { "Debug", "linux" }
	links { "ncurses_g" }