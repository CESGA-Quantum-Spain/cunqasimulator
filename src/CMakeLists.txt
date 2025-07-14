add_library(cunqasimulator executor.cpp mpi_executor.cpp implementations.cpp mpi_implementations.cpp result_cunqasim.cpp)
target_link_libraries(cunqasimulator PUBLIC nlohmann_json::nlohmann_json mpi Threads::Threads)
target_include_directories(cunqasimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/utils")


if(USE_FLOAT_PRECISION)
    message(STATUS "Configuring with FLOAT precision for specific library.")
    target_compile_definitions(cunqasimulator PUBLIC USE_FLOAT_PRECISION_MACRO)
else()
    message(STATUS "Configuring with DOUBLE precision (default) for specific library.")
    target_compile_definitions(cunqasimulator PUBLIC USE_DOUBLE_PRECISION_MACRO)
endif()