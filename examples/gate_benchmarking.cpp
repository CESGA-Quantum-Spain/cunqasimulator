#include <iostream>
#include <fstream>
#include <vector>
#include <complex>

#include "executor.hpp"

int main() {

    int initial_n_qubits = 7;
    int max_qubits = 35;

    // No threads part
    bool with_threads = false;

    /* std::ofstream h_no_threads_file("H_No_Threads_Bench.txt");
    std::ofstream cx_no_threads_file("CX_No_Threads_Bench.txt");
    std::ofstream measure_no_threads_file("Measure_No_Threads_Bench.txt");

    for (int i = initial_n_qubits; i < max_qubits + 1; i++) {
        Executor executor(i, with_threads); 

        auto start_gate = std::chrono::high_resolution_clock::now();
        executor.apply_gate("h", {i - 2});
        auto end_gate = std::chrono::high_resolution_clock::now();
        auto duration_gate = std::chrono::duration_cast<std::chrono::nanoseconds>(end_gate - start_gate);
        h_no_threads_file << duration_gate.count() << ", ";

        auto start_cx = std::chrono::high_resolution_clock::now();
        executor.apply_gate("cx", {i - 2, i - 1});
        auto end_cx = std::chrono::high_resolution_clock::now();
        auto duration_cx = std::chrono::duration_cast<std::chrono::nanoseconds>(end_cx - start_cx);
        cx_no_threads_file << duration_cx.count() << ", ";

        auto start_measure = std::chrono::high_resolution_clock::now();
        executor.apply_measure({i - 2});
        auto end_measure = std::chrono::high_resolution_clock::now();
        auto duration_measure = std::chrono::duration_cast<std::chrono::nanoseconds>(end_measure - start_measure);
        measure_no_threads_file << duration_measure.count() << ", ";

    }
    h_no_threads_file.close();
    cx_no_threads_file.close();
    measure_no_threads_file.close(); */

    // With threads Part
    with_threads = true;

    std::ofstream h_threads_file("H_16_Threads_Bench.txt");
    std::ofstream cx_threads_file("CX_16_Threads_Bench.txt");
    std::ofstream measure_threads_file("Measure_16_Threads_Bench.txt");

    for (int i = initial_n_qubits; i < max_qubits + 1; i++) {
        Executor executor(i, with_threads); 

        auto start_gate = std::chrono::high_resolution_clock::now();
        executor.apply_gate("h", {i - 2});
        auto end_gate = std::chrono::high_resolution_clock::now();
        auto duration_gate = std::chrono::duration_cast<std::chrono::nanoseconds>(end_gate - start_gate);
        h_threads_file << duration_gate.count() << ", ";

        auto start_cx = std::chrono::high_resolution_clock::now();
        executor.apply_gate("cx", {i - 2, i - 1});
        auto end_cx = std::chrono::high_resolution_clock::now();
        auto duration_cx = std::chrono::duration_cast<std::chrono::nanoseconds>(end_cx - start_cx);
        cx_threads_file << duration_cx.count() << ", ";

        auto start_measure = std::chrono::high_resolution_clock::now();
        executor.apply_measure({i - 2});
        auto end_measure = std::chrono::high_resolution_clock::now();
        auto duration_measure = std::chrono::duration_cast<std::chrono::nanoseconds>(end_measure - start_measure);
        measure_threads_file << duration_measure.count() << ", ";
        
    }
    h_threads_file.close();
    cx_threads_file.close();
    measure_threads_file.close();


    return 0;
}