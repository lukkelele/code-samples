find_package(PkgConfig QUIET)

if (PKG_CONFIG_FOUND)
  pkg_check_modules(PC_SDL2 QUIET sdl2)
endif()

find_path(SDL2_INCLUDE_DIR
    NAMES SDL.h
    HINTS ${PC_SDL2_INCLUDE_DIRS}
    PATH_SUFFIXES SDL2
)

find_library(SDL2_LIBRARY
    NAMES SDL2
    HINTS ${PC_SDL2_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2
    REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR
    VERSION_VAR PC_SDL2_VERSION
)

if (SDL2_FOUND)
  set(SDL2_LIBRARIES ${SDL2_LIBRARY})
  set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
endif()

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY)
