include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(Vulkcmm_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(Vulkcmm_setup_options)
  option(Vulkcmm_ENABLE_HARDENING "Enable hardening" ON)
  option(Vulkcmm_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    Vulkcmm_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    Vulkcmm_ENABLE_HARDENING
    OFF)

  Vulkcmm_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR Vulkcmm_PACKAGING_MAINTAINER_MODE)
    option(Vulkcmm_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(Vulkcmm_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(Vulkcmm_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Vulkcmm_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Vulkcmm_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(Vulkcmm_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(Vulkcmm_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Vulkcmm_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(Vulkcmm_ENABLE_IPO "Enable IPO/LTO" ON)
    option(Vulkcmm_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(Vulkcmm_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(Vulkcmm_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(Vulkcmm_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Vulkcmm_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Vulkcmm_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Vulkcmm_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(Vulkcmm_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(Vulkcmm_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Vulkcmm_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      Vulkcmm_ENABLE_IPO
      Vulkcmm_WARNINGS_AS_ERRORS
      Vulkcmm_ENABLE_USER_LINKER
      Vulkcmm_ENABLE_SANITIZER_ADDRESS
      Vulkcmm_ENABLE_SANITIZER_LEAK
      Vulkcmm_ENABLE_SANITIZER_UNDEFINED
      Vulkcmm_ENABLE_SANITIZER_THREAD
      Vulkcmm_ENABLE_SANITIZER_MEMORY
      Vulkcmm_ENABLE_UNITY_BUILD
      Vulkcmm_ENABLE_CLANG_TIDY
      Vulkcmm_ENABLE_CPPCHECK
      Vulkcmm_ENABLE_COVERAGE
      Vulkcmm_ENABLE_PCH
      Vulkcmm_ENABLE_CACHE)
  endif()

  #Vulkcmm_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  #if(LIBFUZZER_SUPPORTED AND (Vulkcmm_ENABLE_SANITIZER_ADDRESS OR Vulkcmm_ENABLE_SANITIZER_THREAD OR #Vulkcmm_ENABLE_SANITIZER_UNDEFINED))
  #  set(DEFAULT_FUZZER ON)
  #else()
  #  set(DEFAULT_FUZZER OFF)
  #endif()

  #option(Vulkcmm_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(Vulkcmm_global_options)
  if(Vulkcmm_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    Vulkcmm_enable_ipo()
  endif()

  Vulkcmm_supports_sanitizers()

  if(Vulkcmm_ENABLE_HARDENING AND Vulkcmm_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Vulkcmm_ENABLE_SANITIZER_UNDEFINED
       OR Vulkcmm_ENABLE_SANITIZER_ADDRESS
       OR Vulkcmm_ENABLE_SANITIZER_THREAD
       OR Vulkcmm_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${Vulkcmm_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${Vulkcmm_ENABLE_SANITIZER_UNDEFINED}")
    Vulkcmm_enable_hardening(Vulkcmm_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(Vulkcmm_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(Vulkcmm_warnings INTERFACE)
  add_library(Vulkcmm_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  Vulkcmm_set_project_warnings(
    Vulkcmm_warnings
    ${Vulkcmm_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(Vulkcmm_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(Vulkcmm_options)
  endif()

  include(cmake/Sanitizers.cmake)
  Vulkcmm_enable_sanitizers(
    Vulkcmm_options
    ${Vulkcmm_ENABLE_SANITIZER_ADDRESS}
    ${Vulkcmm_ENABLE_SANITIZER_LEAK}
    ${Vulkcmm_ENABLE_SANITIZER_UNDEFINED}
    ${Vulkcmm_ENABLE_SANITIZER_THREAD}
    ${Vulkcmm_ENABLE_SANITIZER_MEMORY})

  set_target_properties(Vulkcmm_options PROPERTIES UNITY_BUILD ${Vulkcmm_ENABLE_UNITY_BUILD})

  if(Vulkcmm_ENABLE_PCH)
    target_precompile_headers(
      Vulkcmm_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(Vulkcmm_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    Vulkcmm_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(Vulkcmm_ENABLE_CLANG_TIDY)
    Vulkcmm_enable_clang_tidy(Vulkcmm_options ${Vulkcmm_WARNINGS_AS_ERRORS})
  endif()

  if(Vulkcmm_ENABLE_CPPCHECK)
    Vulkcmm_enable_cppcheck(${Vulkcmm_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(Vulkcmm_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    Vulkcmm_enable_coverage(Vulkcmm_options)
  endif()

  if(Vulkcmm_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(Vulkcmm_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(Vulkcmm_ENABLE_HARDENING AND NOT Vulkcmm_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Vulkcmm_ENABLE_SANITIZER_UNDEFINED
       OR Vulkcmm_ENABLE_SANITIZER_ADDRESS
       OR Vulkcmm_ENABLE_SANITIZER_THREAD
       OR Vulkcmm_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    Vulkcmm_enable_hardening(Vulkcmm_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
