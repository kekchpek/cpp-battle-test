cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE FEATURE_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Features/*.cpp"
)
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Ecs/*.cpp"
)
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/App/*.cpp"
)

add_library(sw_battle_lib
	${APP_SOURCES}
	${CORE_SOURCES}
	${FEATURE_SOURCES}
	src/IO/System/CommandParser.cpp
)

target_include_directories(sw_battle_lib PUBLIC src/)

add_executable(sw_battle_test src/main.cpp)
target_link_libraries(sw_battle_test PRIVATE sw_battle_lib)

enable_testing()

add_executable(sw_battle_test_validator tests/commands_example_test.cpp)
target_link_libraries(sw_battle_test_validator PRIVATE sw_battle_lib)
target_include_directories(sw_battle_test_validator PRIVATE tests)
target_compile_definitions(sw_battle_test_validator PRIVATE TEST_DATA_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME simulation_events COMMAND sw_battle_test_validator)
