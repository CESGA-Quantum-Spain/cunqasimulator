#pragma once

#include <cstdlib>

#include "types_cunqasim.hpp"

//Flips the bit in position p of the binary expansion of N
inline uint64_t flipbit(uint64_t N, uint64_t p) {
    return N ^ (1ULL << p);  
}

//Checks if the position p of the binary expansion of N is 0
inline bool is_zero(uint64_t N, uint64_t p) {
    bool bit = (N & (1ULL << p)) == 0;
    return bit;
}

//Reverse the first M bits of the integer N
inline uint64_t reverse_bits(uint64_t N, uint64_t M) {
    uint64_t result = 0;
    for (uint64_t i = 0; i < M; ++i) {
        if (N & (1ULL << i)) {
            result |= (1ULL << (M - 1 - i));
        }
    }
    return result | (N & ~((1 << M) - 1));
}

inline uint64_t get_process_memory_in_bytes()
{
    std::string cpus_per_task_str;
    std::string mem_per_cpu_str;
    std::string mem_per_node_str;
    int int_SLURM_CPUS_PER_TASK;
    uint64_t int_SLURM_MEM_PER_CPU;
    uint64_t int_SLURM_MEM_PER_NODE;
    uint64_t final_mem_per_cpu;
    uint64_t final_mem_per_node;


    const char* cpus_per_task_cstr = std::getenv("SLURM_CPUS_PER_TASK");
    if (cpus_per_task_cstr != nullptr) {
        cpus_per_task_str = cpus_per_task_cstr;
        int_SLURM_CPUS_PER_TASK = std::stoi(cpus_per_task_str);
    } else {
        return 0;
    }

    const char* mem_per_cpu_cstr = std::getenv("SLURM_MEM_PER_CPU");
    const char* mem_per_node_cstr = std::getenv("SLURM_MEM_PER_NODE");
    const char* n_cores_cstr = std::getenv("SLURM_CPUS_PER_TASK");
    if (mem_per_cpu_cstr != nullptr) {
        mem_per_cpu_str = mem_per_cpu_cstr;
        int_SLURM_MEM_PER_CPU = std::stoi(mem_per_cpu_str);
        return int_SLURM_MEM_PER_CPU * 1024 * 1024 * int_SLURM_CPUS_PER_TASK;
        //return int_SLURM_CPUS_PER_TASK * int_SLURM_MEM_PER_CPU * 1024 * 1024;
    } else if (mem_per_node_cstr != nullptr) {
        mem_per_node_str = mem_per_node_cstr;
        int_SLURM_MEM_PER_NODE = std::stoi(mem_per_node_str);
        return int_SLURM_MEM_PER_NODE * 1024 * 1024;
    } else {
        return 0;
    }
}