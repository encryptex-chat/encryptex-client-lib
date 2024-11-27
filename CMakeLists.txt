cmake_minimum_required(VERSION 3.22.1)

project(encryptex-client-lib)

set(CMAKE_CXX_STANDARD 23)

set(ENCRYPTEX_CLIENT_LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB ENCRYPTEX_CLIENT_LIB_SOURCE ${ENCRYPTEX_CLIENT_LIB_SOURCE_DIR}/*.cpp)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_subdirectory("encryptex-common-lib")
    add_executable(${PROJECT_NAME} ${ENCRYPTEX_CLIENT_LIB_SOURCE})
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} spdlog::spdlog encryptex-common-lib)
endif()

