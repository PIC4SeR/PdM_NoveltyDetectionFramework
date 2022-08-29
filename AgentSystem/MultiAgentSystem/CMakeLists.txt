# CMakeList.txt : CMake project for AgentSystem, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(MultiAgentSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/")

set(HEADER_FILES "include/TcMathSupporter.h" "include/TcRegressor.h" "include/TcEvent.h" "include/TcMeasure.h" "include/TcMongoDriver.h" "include/TcAgentSystem.h" "include/TcAgentManager.h" "include/Agent/TcAgentStatus.h" "include/Agent/IAgent.h"  "include/Agent/TcAgent.h" "include/INotifyDriverAgentInterface.h" "include/TcNotification.h" "include/IMongoDriverAgentInterface.h" "include/Agent/ThresholdsAgent/TcThresholdsAgent.h" "include/Agent/TcOneShotAgent.h")
set(SOURCE_FILES "source/TcMathSupporter.cpp" "source/TcEvent.cpp" "source/TcMeasure.cpp" "source/TcMongoDriver.cpp" "source/TcAgentSystem.cpp" "source/TcAgentManager.cpp" "source/Agent/TcAgentStatus.cpp" "source/Agent/IAgent.cpp"  "source/Agent/TcAgent.cpp" "source/INotifyDriverAgentInterface.cpp" "source/TcNotification.cpp" "source/IMongoDriverAgentInterface.cpp" "source/Agent/ThresholdsAgent/TcThresholdsAgent.cpp" "source/Agent/TcOneShotAgent.cpp")



set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

find_package(libmongoc-1.0 CONFIG REQUIRED)
find_package(bson-1.0 CONFIG REQUIRED)

find_package(libmongocxx CONFIG REQUIRED)
find_package(libbsoncxx CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(boost INTERFACE IMPORTED)
add_library(libmongocxx INTERFACE IMPORTED)
add_library(libbsoncxx INTERFACE IMPORTED)

set_property(TARGET libmongocxx PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBMONGOCXX_INCLUDE_DIRS})
set_property(TARGET libbsoncxx PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBBSONCXX_INCLUDE_DIRS})
set_property(TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

link_directories(${LIBMONGOCXX_INCLUDE_DIRS})
link_directories(${LIBBSONCXX_INCLUDE_DIR})

include_directories(${LIBMONGOCXX_INCLUDE_DIRS} ${LIBBSONCXX_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES} pthread)
