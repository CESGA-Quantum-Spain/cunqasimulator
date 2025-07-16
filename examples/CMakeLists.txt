add_executable(example example.cpp)
target_link_libraries(example PUBLIC cunqasimulator nlohmann_json::nlohmann_json) 
target_include_directories(example PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(mpi_example mpi_example.cpp)
target_link_libraries(mpi_example PUBLIC cunqasimulator nlohmann_json::nlohmann_json) 
target_include_directories(mpi_example PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(gate_test gate_test.cpp)
target_link_libraries(gate_test PUBLIC cunqasimulator nlohmann_json::nlohmann_json) 
target_include_directories(gate_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(gate_bench gate_benchmarking.cpp)
target_link_libraries(gate_bench PUBLIC cunqasimulator nlohmann_json::nlohmann_json) 
target_include_directories(gate_bench PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(run_test run_test.cpp)
target_link_libraries(run_test PUBLIC cunqasimulator nlohmann_json::nlohmann_json) 
target_include_directories(run_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../src")