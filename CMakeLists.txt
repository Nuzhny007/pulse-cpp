cmake_minimum_required(VERSION 3.5)

project(pulse)

FIND_PACKAGE(OpenCV 4.0 REQUIRED)
                                   
unset(CMAKE_CXX_FLAGS CACHE)

find_package(OpenMP)
if (OPENMP_FOUND)
    list(APPEND CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors --std=c++14" CACHE STRING COMPILE_FLAGS FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native --fast-math -ffast-math -funroll-loops -Wall -DNDEBUG -DBOOST_DISABLE_ASSERTS" CACHE STRING COMPILE_FLAGS FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -march=native -mtune=native -Wall -DDEBUG" CACHE STRING COMPILE_FLAGS FORCE)
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4" CACHE STRING COMPILE_FLAGS FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:AVX /fp:fast /O2 /Ob2 /Oi /Ot" CACHE STRING COMPILE_FLAGS FORCE)
    add_definitions(-DWIN32 -D_USE_MATH_DEFINES -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

# ----------------------------------------------------------------------

set(SOURCE
    src/main.cpp
    src/EvmGdownIIR.cpp
    src/Pulse.cpp
    src/Window.cpp
    src/profiler/Profiler.cpp
    src/ext/opencv.cpp
)

set(HEADERS
    src/main.cpp
    src/EvmGdownIIR.hpp
    src/Pulse.hpp
    src/Window.hpp
    src/profiler/Profiler.h
    src/ext/opencv.hpp
)

set(LIBS
    ${OpenCV_LIBS}
)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${LIBS})

# ----------------------------------------------------------------------

