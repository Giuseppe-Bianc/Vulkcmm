@echo off

rem Check if the VULKAN_SDK environment variable is defined
if "%VULKAN_SDK%" == "" (
    echo ERROR: VULKAN_SDK environment variable is not defined.
    pause
    exit /b 1
)

rem Set Vulkan SDK paths
set VULKAN_PATH=%VULKAN_SDK%\Bin
del /s /q shaders\vert.spv
del /s /q shaders\frag.spv

%VULKAN_PATH%\glslc.exe shaders\vert.vert -o shaders\vert.spv
%VULKAN_PATH%\glslc.exe shaders\frag.frag -o shaders\frag.spv