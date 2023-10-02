-- premake5.lua
workspace "vulkcmm"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {
   VulkanSDK = "%{VULKAN_SDK}/Include",
   glfw = "external/glfw/include",
   spdlog = "external/spdlog/include",
   tinyobjloader = "external/tinyobjloader"
}

LibraryDir = {
   VulkanSDK = "%{VULKAN_SDK}/Lib",
   glfw = "external/glfw/lib-vc2022",
}

Library = {
   Vulkan = "%{LibraryDir.VulkanSDK}/vulkan-1.lib",
   glfw = "%{LibraryDir.glfw}/glfw3.lib"
}

project "vulkancmake"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
   staticruntime "off"

   files { 
      "src/**.h", 
      "src/**.cpp", 
      "src/**.c",
      "src/**.frag",
      "src/**.vert",
      "src/**.png"
   }

   vpaths {
        ["Headers"] = { "**.h", "**.hpp" },
        ["Sources/*"] = {"**.c", "**.cpp"},
        ["Resources"] = {"**.frag", "**.vert","**.png"};
        ["Docs"] = "**.md"
    }

   includedirs
   {
      "%{IncludeDir.glfw}",
      "%{IncludeDir.spdlog}",
      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.tinyobjloader}",
   }

   links {
         "%{Library.Vulkan}",
         "%{Library.glfw}"
      }

   filter { "system:windows", "action:vs*" }
   filter "system:windows"
      systemversion "latest"
      buildoptions { "/sdl", "/permissive-","/w14242", "/w14254", "/w14263", "/w14265", 
                     "/w14287", "/we4289", "/w14296","/w14311", "/w14545", "/w14546", 
                     "/w14547", "/w14549", "/w14555", "/w14619","/w14640", "/w14826", 
                     "/w14905", "/w14906", "/w14928", "/wd4996" }
      prebuildcommands { "call compile.bat"}
      prebuildmessage "compiling spv files"

    filter "configurations:Debug"
      defines { "WL_DEBUG", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG" }
      runtime "Debug"
      inlining "Auto"
      symbols "On"
      warnings "Extra"
      buildoptions {
      }
      linkoptions { "/NODEFAULTLIB:msvcrt.lib" }
      flags { "FatalWarnings"}

  filter "configurations:Release"
      defines { "WL_RELEASE", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }
      runtime "Release"
      optimize "Full"
      inlining "Auto"
      symbols "On"
      warnings "Extra"
      buildoptions { "/GS", "/DYNAMICBASE"}
      flags { "FatalWarnings","LinkTimeOptimization", "MultiProcessorCompile" }

   filter "configurations:Dist"
      defines { "WL_DIST", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }
      runtime "Release"
      optimize "Full"
      inlining "Auto"
      symbols "Off"
      omitframepointer "On"
      warnings "Extra"
      buildoptions { "/GS", "/DYNAMICBASE"}
      flags { "FatalWarnings","LinkTimeOptimization", "MultiProcessorCompile", "NoIncrementalLink" }