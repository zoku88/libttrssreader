add_subdirectory(login)

add_subdirectory(categories)

add_library(session session.cpp)

add_library(libsession SHARED session.cpp)

include_directories(${CURL_INCLUDE_DIRS}/curl ${JSON_INCLUDE_DIRS} ./login ./categories)

target_link_libraries(session login categories  ${CURL_LIBRARIES} ${JSON_LIBRARIES})
