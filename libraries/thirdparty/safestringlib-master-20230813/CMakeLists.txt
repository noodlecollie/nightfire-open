option(BUILD_OPT_DEFAULT "" OFF)
option(BUILD_UNITTESTS "" OFF)

option(BUILD_STRCASECMP "" ON)
option(BUILD_STRCAT "" ON)
option(BUILD_STRNCAT "" ON)
option(BUILD_STRCPY "" ON)
option(BUILD_STRNCPY "" ON)
option(BUILD_STRTOK "" ON)

add_subdirectory(safestringlib-master-20230813)
