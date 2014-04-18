add_library (login login.cpp)

include_directories(${CURL_INCLUDE_DIRS}/curl ${JSON_INCLUDE_DIRS}
		${PROJ_INCLUDE_DIRS})
