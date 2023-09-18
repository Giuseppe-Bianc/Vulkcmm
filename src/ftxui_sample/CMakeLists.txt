add_executable(vulkcmmsrc main.cpp
                     headers.h
                     Timer.h
                     casts.h)

target_link_libraries(
  vulkcmmsrc
  PRIVATE Vulkcmm::Vulkcmm_options
          Vulkcmm::Vulkcmm_warnings)

target_link_system_libraries(
  vulkcmmsrc
  PRIVATE
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
