cmake_minimum_required(VERSION 3.21)
project(MyGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(InstallRequiredSystemLibraries)

# --- SFML ---
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.2   # Use stable branch/tag
)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

# --- Box2D ---
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG        v3.1.0
)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(box2d)

# --- Your executable ---
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(MyGame ${SOURCES})

# Link dependencies
target_link_libraries(MyGame PRIVATE sfml-graphics sfml-window sfml-system box2d)

# Copy SFML DLLs on Windows
if(WIN32)
    add_custom_command(TARGET MyGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MyGame> $<TARGET_FILE_DIR:MyGame>
        COMMAND_EXPAND_LISTS
    )
endif()