﻿cmake_minimum_required (VERSION 3.8)

project(MultiAgentSystem LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(HEADER_FILES "include/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.h" "include/TcLinearRegressor.h" "include/TcMongoDriver.h" "include/TcAgentSystem.h" "include/TcAgentManager.h" "include/Agent/TcAgentStatus.h" "include/Agent/IAgent.h"  "include/Agent/TcAgent.h" "include/IMongoDriverAgentInterface.h" "include/Agent/TcOneShotAgent.h")
set(SOURCE_FILES "source/Agent/ErrorDegradationTimeEstimatorAgent/TcErrorDegradationTimeEstimator.cpp" "source/TcMongoDriver.cpp" "source/TcAgentSystem.cpp" "source/TcAgentManager.cpp" "source/Agent/TcAgentStatus.cpp" "source/Agent/IAgent.cpp"  "source/Agent/TcAgent.cpp" "source/IMongoDriverAgentInterface.cpp" "source/Agent/TcOneShotAgent.cpp")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(Boost CONFIG REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost found")
    if (CMAKE_VERSION VERSION_LESS 3.15.0)
        target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)
    endif()
    add_definitions( "-DHAS_BOOST" )
    message(STATUS "Boost include directory ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()


find_package(libmongoc-1.0 CONFIG REQUIRED)
if(libmongoc-1.0_FOUND)
    message(STATUS "libmongoc-1.0 found")
    message(STATUS "libmongoc-1.0 library ${MONGOC_LIBRARY}")
    link_directories(${MONGOC_LIBRARY})
endif()

find_package(libbson-1.0 CONFIG REQUIRED)
if(libbson-1.0_FOUND)
    message(STATUS "libbson-1.0 found")
    message(STATUS "libbson-1.0 library ${BSON_LIBRARY}")
    link_directories(${BSONC_LIBRARY})
endif()


find_package(libmongocxx CONFIG REQUIRED)
if(libmongocxx_FOUND)
    message(STATUS "libmongocxx found")
    include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
    link_directories(${LIBMONGOCXX_LIBRARY_PATH})
    message(STATUS "libmongocxx includes ${LIBMONGOCXX_INCLUDE_DIRS}")
    message(STATUS "libmongocxx library ${LIBMONGOCXX_LIBRARY_PATH}")
endif()

find_package(libbsoncxx CONFIG REQUIRED)
if(libbsoncxx_FOUND)
    message(STATUS "libbsoncxx found")
    include_directories(${LIBBSONCXX_INCLUDE_DIRS})
    link_directories(${LIBBSONCXX_LIBRARY_PATH})
    message(STATUS "libbsoncxx includes ${LIBBSONCXX_INCLUDE_DIRS}")
    message(STATUS "libbsoncxx library ${LIBBSONCXX_LIBRARY_PATH}")
endif()

find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    message(STATUS "OpenSSL found")
    include_directories(${OPENSSL_INCLUDE_DIR})
    link_directories(${OPENSSL_CRYPTO_LIBRARY})
    link_directories(${OPENSSL_SSL_LIBRARY})
    message(STATUS "OpenSSL include directory ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL crypto directory ${OPENSSL_CRYPTO_LIBRARY}")
    message(STATUS "OpenSSL sll library ${OPENSSL_SSL_LIBRARY}")
endif()


if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC mongo::bsoncxx_shared mongo::mongocxx_shared)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC bsoncxx mongocxx pthread)
endif()