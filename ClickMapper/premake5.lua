project "ClickMapper"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "off"
    files {"src/**.h" , "src/**.hpp", "src/**.cpp"}

    includedirs {
        "../vendors/walnut/vendor/imgui",
        "../vendors/walnut/vendor/glfw/include",
        "../vendors/walnut/Walnut/src",
        "../vendors/imgui-filebrowser",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.glm}"
    }

    links {"Walnut"}

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
       systemversion "latest"
       defines { "WL_PLATFORM_WINDOWS", "KEY" }

    filter "configurations:Debug"
       defines { "WL_DEBUG" }
       runtime "Debug"
       symbols "On"

    filter "configurations:Release"
       defines { "WL_RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"
       
