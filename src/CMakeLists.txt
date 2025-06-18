add_library(cunqasimulator executor.cpp instructions.cpp implementations.cpp result_cunqasim.cpp)
target_link_libraries(cunqasimulator PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(cunqasimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/utils")