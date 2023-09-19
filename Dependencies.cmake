include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(Vulkcmm_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 10.1.1
        OPTIONS
        "FMT_PEDANTIC ON"
        "FMT_WERROR ON"
    )
  endif()

  if(NOT TARGET spdlog::spdlog)
    cpmaddpackage(
        NAME
        spdlog
        VERSION
        1.12.0
        GITHUB_REPOSITORY
        "gabime/spdlog"
        OPTIONS
        "SPDLOG_TIDY ON"
        "SPDLOG_FMT_EXTERNAL ON"
        "SPDLOG_ENABLE_PCH ON"
        "SPDLOG_BUILD_PIC ON"
        "SPDLOG_WCHAR_SUPPORT ON"
        "SPDLOG_WCHAR_FILENAMES ON"
        "SPDLOG_BUILD_WARNINGS ON"
        "SPDLOG_SANITIZE_ADDRESS ON")
  endif()

  if(NOT TARGET glfw::glfw)
    cpmaddpackage(
        NAME GLFW
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.3.8
        OPTIONS
        "GLFW_BUILD_DOCS OFF"
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
    )
  endif()

  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage("gh:CLIUtils/CLI11@2.3.2")
  endif()

  #if(NOT TARGET tools::tools)
  #  cpmaddpackage("gh:lefticus/tools#update_build_system")
  #endif()

endfunction()
