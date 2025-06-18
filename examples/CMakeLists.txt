add_executable(example example.cpp)
target_link_libraries(example PUBLIC cunqasimulator nlohmann_json::nlohmann_json) 
target_include_directories(example PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../src")