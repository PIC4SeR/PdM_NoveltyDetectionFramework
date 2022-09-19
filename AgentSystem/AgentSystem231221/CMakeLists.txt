# CMakeList.txt : CMake project for AgentSystem, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(HEADER_FILES "include/TcEvent.h" "include/TcMeasure.h" "include/TcMongoDriver.h" "include/TcAgentSystem.h" "include/TcAgentManager.h" "include/Agent/TcAgentStatus.h" "include/Agent/IAgent.h" "include/Agent/TcAgent.h"  "include/INotifyDriverAgentInterface.h" "include/TcNotification.h" "include/IMongoDriverAgentInterface.h" "include/Agent/TcThresholdsAgent.h" "include/Agent/TcThresholdsComputeAgent.h" "include/Agent/MeasurementsDeviationDetectorAgent/TcMeasurementsDeviationDetectorAgent.h" "include/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.h" "include/Agent/MeasurementsDeviationDetectorAgent/TcRegressor.h" "include/Agent/MeasurementsDeviationDetectorAgent/TcMathSupporter.h")
set(SOURCE_FILES "source/TcAgentSystem.cpp" "source/TcAgentManager.cpp" "source/TcMongoDriver.cpp" "source/TcEvent.cpp" "source/TcMeasure.cpp" "source/TcNotification.cpp"  "source/INotifyDriverAgentInterface.cpp" "source/IMongoDriverAgentInterface.cpp"  "source/Agent/TcAgentStatus.cpp" "source/Agent/IAgent.cpp" "source/Agent/TcAgent.cpp" "source/Agent/TcThresholdsAgent.cpp" "source/Agent/TcThresholdsComputeAgent.cpp" "source/Agent/MeasurementsDeviationDetectorAgent/TcStatisticTestMeasures.cpp" "source/Agent/MeasurementsDeviationDetectorAgent/TcMeasurementsDeviationDetectorAgent.cpp" "source/Agent/MeasurementsDeviationDetectorAgent/TcMathSupporter.cpp")


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

add_library(libmongocxx INTERFACE IMPORTED)
add_library(libbsoncxx INTERFACE IMPORTED)

set_property(TARGET libmongocxx PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBMONGOCXX_INCLUDE_DIRS})
set_property(TARGET libbsoncxx PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBBSONCXX_INCLUDE_DIRS})

add_library(boost INTERFACE IMPORTED)

set_property(TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

include_directories(${LIBMONGOCXX_INCLUDE_DIRS} ${LIBBSONCXX_INCLUDE_DIRS})

link_directories(${LIBMONGOCXX_INCLUDE_DIRS})
link_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBMONGOCXX_LIBRARIES} ${lIBBSONCXX_LIBRARIES})