add_executable(${PROJECT_NAME} compiler.cpp file.cpp preprocessor.cpp tokenizer.cpp main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)