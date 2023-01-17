# CMakeList.txt : CMake project for AgentSystem, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(MultiAgentSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(HEADER_FILES "include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h" "include/TcMathSupporter.h" "include/TcLinearRegressor.h" "include/TcEvent.h" "include/TcMeasure.h" "include/TcMongoDriver.h" "include/TcAgentSystem.h" "include/TcAgentManager.h" "include/Agent/TcAgentStatus.h" "include/Agent/IAgent.h"  "include/Agent/TcAgent.h" "include/INotifyDriverAgentInterface.h" "include/TcNotification.h" "include/IMongoDriverAgentInterface.h" "include/Agent/ThresholdsAgent/TcThresholdsAgent.h" "include/Agent/TcOneShotAgent.h")
#set(SOURCE_FILES "source/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.cpp" "source/TcMathSupporter.cpp" "source/TcLinearRegressor.cpp" "source/TcEvent.cpp" "source/TcMeasure.cpp" "source/TcMongoDriver.cpp" "source/TcAgentSystem.cpp" "source/TcAgentManager.cpp" "source/Agent/TcAgentStatus.cpp" "source/Agent/IAgent.cpp"  "source/Agent/TcAgent.cpp" "source/INotifyDriverAgentInterface.cpp" "source/TcNotification.cpp" "source/IMongoDriverAgentInterface.cpp" "source/Agent/ThresholdsAgent/TcThresholdsAgent.cpp" "source/Agent/TcOneShotAgent.cpp")
set(HEADER_FILES "include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h" "include/TcLinearRegressor.h" "include/TcMongoDriver.h" "include/TcAgentSystem.h" "include/TcAgentManager.h" "include/Agent/TcAgentStatus.h" "include/Agent/IAgent.h"  "include/Agent/TcAgent.h" "include/IMongoDriverAgentInterface.h" "include/Agent/TcOneShotAgent.h")
set(SOURCE_FILES "source/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.cpp" "source/TcMongoDriver.cpp" "source/TcAgentSystem.cpp" "source/TcAgentManager.cpp" "source/Agent/TcAgentStatus.cpp" "source/Agent/IAgent.cpp"  "source/Agent/TcAgent.cpp" "source/IMongoDriverAgentInterface.cpp" "source/Agent/TcOneShotAgent.cpp")
set(CMAKE_DIR "./cmake")


set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_DIR}/include")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_DIR}/lib")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_DIR}/share")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)


find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions( "-DHAS_BOOST" )
    link_directories(${BOOST_LIBRARY_DIRS})
endif()

#find_path(libmongoc-1.0 "libmongoc-1.0-config" "C:/Users/umber/Documents/VCPKG/vcpkg/packages/mongo-c-driver_x64-windows/share/libmongoc-1.0")
find_package(libmongoc-1.0 CONFIG REQUIRED)

if(LIBMONGOC_FOUND)
    include_directories(${LIBMONGO_INCLUDE_DIR})
    link_directories(${LIBMONGO_LIBRARY_DIRS})
endif()

find_package(libbson-1.0 CONFIG REQUIRED)
if(LIBBSON_FOUND)
    include_directories(${LIBBSON_INCLUDE_DIR})
    link_directories(${LIBBSON_LIBRARY_DIRS})
endif()

find_package(mongocxx CONFIG REQUIRED)
if(LIBMONGOCXX_FOUND)
    include_directories(${LIBMONGOCXX_INCLUDE_DIR})
    link_directories(${LIBMONGOCXX_LIBRARY_DIRS})
endif()

find_package(bsoncxx CONFIG REQUIRED)

if(LIBBSONCXX_FOUND)
    include_directories(${LIBBSONCXX_INCLUDE_DIR})
    link_directories(${LIBBSONCXX_LIBRARY_DIRS})
endif()


find_package(OpenSSL REQUIRED)  
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    link_directories(${OPENSSL_LIBRARY_DIRS})
endif()

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC mongo::bsoncxx_shared mongo::mongocxx_shared)
