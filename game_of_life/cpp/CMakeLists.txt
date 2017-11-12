cmake_minimum_required (VERSION 3.9)
project (GameOfLife)

include (CheckIPOSupported)

set (CMAKE_POSITION_INDEPENDENT_CODE on)
check_ipo_supported (RESULT IPO_SUPPORTED)
if (IPO_SUPPORTED)
	set (CMAKE_INTERPROCEDURAL_OPTIMIZATION on)
endif()
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build." FORCE)
endif()

option (BUILD_TESTING "Enable tests")

set (GOL_CFLAGS -Wall -fstack-protector-strong)
set (GOL_CFLAGS_DEBUG ${GOL_CFLAGS} -O0 -ggdb3)
set (GOL_CFLAGS_RELEASE ${GOL_CFLAGS} -O2 -D__FORTIFY_SOURCE=2)
set (GOL_CFLAGS_RELWDBG ${GOL_CFLAGS_RELEASE} -ggdb3)

find_package (Curses REQUIRED)
find_path (FOO_INCLUDE_DIRS foo.h PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../libfoo/include)
find_library (FOO_LIBRARIES foo PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../libfoo/lib)

add_library (gol_int
	World.cpp
	Cell.cpp
)

target_compile_options (gol_int
	PRIVATE $<$<CONFIG:Debug>:${GOL_CFLAGS_DEBUG}>
	PRIVATE $<$<CONFIG:Release>:${GOL_CFLAGS_RELEASE}>
	PRIVATE $<$<CONFIG:RelWithDebInfo>:${GOL_CFLAGS_RELWDBG}>
)

target_compile_features(gol_int PUBLIC cxx_std_11)

target_include_directories (gol_int
	PRIVATE ${CURSES_INCLUDE_DIRS}
)

add_executable (gol
	gol.cpp
)

target_include_directories (gol
	PRIVATE ${FOO_INCLUDE_DIRS}
	PRIVATE ${CURSES_INCLUDE_DIRS}
)

target_compile_options (gol
	PRIVATE $<$<CONFIG:Debug>:${GOL_CFLAGS_DEBUG}>
	PRIVATE $<$<CONFIG:Release>:${GOL_CFLAGS_RELEASE}>
	PRIVATE $<$<CONFIG:RelWithDebInfo>:${GOL_CFLAGS_RELWDBG}>
)

target_link_libraries(gol
	gol_int
	${FOO_LIBRARIES}
	${CURSES_LIBRARIES}
)

install (TARGETS gol DESTINATION bin EXPORT gol)

if (BUILD_TESTING)
	enable_testing ()

	# test Cell
	add_executable (test_exec_cell
		tests/cell.cpp
	)
	target_include_directories (test_exec_cell
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	)
	target_link_libraries (test_exec_cell
		gol_int
	)

	add_test (NAME test_cell COMMAND test_exec_cell)
endif ()
