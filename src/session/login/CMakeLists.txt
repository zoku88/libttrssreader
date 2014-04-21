add_library (login login.cpp)

include_directories(${CURL_INCLUDE_DIRS}/curl ${JSON_INCLUDE_DIRS}
		${PROJ_INCLUDE_DIRS})

IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
    set_target_properties(login PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
