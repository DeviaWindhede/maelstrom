include "../premake/extensions.lua"
-- include for common stuff 
include "../premake/common.lua"

workspace "Maelstrom"
	location "../"
	startproject "Game"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

include (dirs.external)
include (dirs.engine)
include (dirs.game)
