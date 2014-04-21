cmake_minimum_required (VERSION 2.6)
project (CurlTest)

add_executable(CurlTest main.cpp)

FIND_PACKAGE(CURL)

set(JSON_INCLUDE_DIRS /usr/include/jsoncpp)
set(JSON_LIBRARIES json jsoncpp)
set(PROJ_INCLUDE_DIRS  ${PROJECT_SOURCE_DIR}/../include)

include_directories(${PROJECT_SOURCE_DIR}/session/login
		${PROJECT_SOURCE_DIR}/session/categories ${CURL_INCLUDE_DIRS}/curl
		${JSON_INCLUDE_DIRS} ${PROJ_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/session)

add_subdirectory(session)

target_link_libraries(CurlTest ttrsssession  ${CURL_LIBRARIES}
		${JSON_LIBRARIES})
