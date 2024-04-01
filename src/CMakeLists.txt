add_library(logging STATIC logging.c)
target_include_directories(logging PUBLIC ${PROJECT_SOURCE_DIR}/includes)

FILE(GLOB_RECURSE MODELS models/*.c)

add_library(${PROJECT_NAME} STATIC ufs.c ${MODELS})
target_link_libraries(${PROJECT_NAME} logging m)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/includes)