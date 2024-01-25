workspace "ClickMapper"
    architecture "x86_64" -- required 32 bit support.
    configurations { "Debug", "Release"}
    startproject "ClickMapper"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "vendors/Walnut/WalnutExternal.lua"
include "ClickMapper"

