add_library(cunqasimulator executor.cpp mpi_executor.cpp instructions.cpp mpi_instructions.cpp implementations.cpp result_cunqasim.cpp)
target_link_libraries(cunqasimulator PUBLIC nlohmann_json::nlohmann_json mpi)
target_include_directories(cunqasimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/utils")