add_library(logging STATIC logging.c)
target_include_directories(logging PUBLIC ${PROJECT_SOURCE_DIR}/includes)

add_library(${PROJECT_NAME} STATIC ufs.c)
target_link_libraries(${PROJECT_NAME} logging)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/includes)