add_executable(${PROJECT_NAME} file.cpp main.cpp translator.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)