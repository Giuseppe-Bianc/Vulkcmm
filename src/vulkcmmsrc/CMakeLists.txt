file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(vulkcmmsrc ${SOURCES})

find_package(Vulkan REQUIRED)

target_link_libraries(
  vulkcmmsrc
  PRIVATE Vulkcmm::Vulkcmm_options
          Vulkcmm::Vulkcmm_warnings)

target_link_system_libraries(
  vulkcmmsrc
  PRIVATE
          Vulkan::Vulkan
          glfw
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog
          lefticus::tools)

target_compile_definitions(vulkcmmsrc
    PRIVATE
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG>
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
    $<$<CONFIG:MinSizeRel>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
    $<$<CONFIG:RelWithDebInfo>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
)
target_include_directories(vulkcmmsrc PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
