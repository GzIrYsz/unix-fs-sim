add_executable(ufsss ufsss.c)
target_link_libraries(ufsss logging ${PROJECT_NAME})
target_include_directories(ufsss PUBLIC ${PROJECT_SOURCE_DIR}/includes)