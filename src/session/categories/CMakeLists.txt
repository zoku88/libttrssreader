add_library (categories category.cpp categoryList.cpp)
include_directories(${CURL_INCLUDE_DIRS}/curl ${JSON_INCLUDE_DIRS}
		${PROJ_INCLUDE_DIRS})

IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
    set_target_properties(categories PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
