#include "mpi_executor.hpp"

#include <iostream>
#include <complex>
#include <chrono>
#include <string>
#include <mpi.h>

#include "mpi_implementations.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"


MPIExecutor::MPIExecutor(int n_qubits) : n_qubits{n_qubits}
{
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);


    total_statevector_len = (ONE << n_qubits); // 2^n_qubits - 1
    std::cout << "total_statevector_len: " << total_statevector_len << "\n";
    std::cout.flush();

    uint64_t MEM_BYTES = get_process_memory_in_bytes();
    if (MEM_BYTES == 0) {
        throw std::runtime_error("Error getting the process memory.");
    }

    double double_MAX_VECTOR_LEN = MEM_BYTES / 16.0;
    max_len_on_this_process = std::floor(double_MAX_VECTOR_LEN);
    uint64_t aux_first_element;
    uint64_t aux_last_element;

    if (mpi_rank == 0) {
        first_element_on_this_process = 0;
        last_element_on_this_process = max_len_on_this_process;
        aux_first_element = first_element_on_this_process;
        aux_last_element = last_element_on_this_process;
    }

    for (int i = 0; i < mpi_size; i++) {
        MPI_Bcast(&aux_first_element, 1, MPI_UINT64_T, i, MPI_COMM_WORLD);
        MPI_Bcast(&aux_last_element, 1, MPI_UINT64_T, i, MPI_COMM_WORLD);
        statevector_ranges[i] = {aux_first_element, aux_last_element};
        first_element_on_this_process = aux_last_element + 1;
        last_element_on_this_process = first_element_on_this_process + max_len_on_this_process;
        aux_first_element = first_element_on_this_process;
        aux_last_element = last_element_on_this_process;
    }   

    std::cout << "Maximum len suported: " << statevector_ranges[mpi_size - 1][1] << "\n";
    std::cout.flush();

    if (total_statevector_len > statevector_ranges[mpi_size - 1][1]) {
        throw std::runtime_error("Statevector is too long.");
    }
    else if (total_statevector_len > statevector_ranges[mpi_rank][1]) {
        statevector.resize(max_len_on_this_process);
        local_statevector_len = statevector.size();
    } else if (statevector_ranges[mpi_rank][0] <= total_statevector_len && total_statevector_len <= statevector_ranges[mpi_rank][1]) { 
        statevector.resize(total_statevector_len - statevector_ranges[mpi_rank][0] + 1);   // Then resize
        local_statevector_len = statevector.size();
        is_last_process = true;
    } else {
        mpi_process_needed = false;
    }
    if (mpi_rank == 0) {
        statevector[0] = 1.0;
    }
    
    if(mpi_process_needed) {
        std::cout << "The process " << mpi_rank << " will manage vector elements from " << statevector_ranges[mpi_rank][0] << " to " << statevector_ranges[mpi_rank][0] + statevector.size() << "\n";
    } else {
        std::cout << "The process " << mpi_rank << " is not needed to simulate this statevector." << "\n";
    }

    if (is_last_process) {
        std::cout << "Last process: " << mpi_rank << "\n";
    }
    
}

MPIExecutor::~MPIExecutor()
{
    MPI_Finalize();
}


int MPIExecutor::apply_measure(const std::vector<int>& qubits) 
{
    meas_out measurement = mpi_cunqa_apply_measure(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);

    return measurement.measure;
}


void MPIExecutor::apply_gate(const std::string& gate_name, const std::vector<int>& qubits)
{
    switch (instructions_map[gate_name])
    {
        case id:
            break; 
        case x:
            if (mpi_process_needed) {
                std::cout << "Before mpi_cunqa_apply_x on process " << mpi_rank << "\n";
                mpi_cunqa_apply_x(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
                std::cout << "After mpi_cunqa_apply_x on process " << mpi_rank << "\n";
            }
            break;
        case y:
            mpi_cunqa_apply_y(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case z:
            mpi_cunqa_apply_z(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case h:
            mpi_cunqa_apply_h(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case sx:
            mpi_cunqa_apply_sx(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case cx:
            mpi_cunqa_apply_cx(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case cy:
            mpi_cunqa_apply_cy(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case cz:
            mpi_cunqa_apply_cz(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case ecr:
            mpi_cunqa_apply_ecr(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_x:
            mpi_cunqa_apply_cifx(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_y:
            mpi_cunqa_apply_cify(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_z:
            mpi_cunqa_apply_cifz(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_h:
            mpi_cunqa_apply_cifh(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_sx:
            mpi_cunqa_apply_cifsx(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_cx:
            mpi_cunqa_apply_cifcx(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_cy:
            mpi_cunqa_apply_cifcy(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_cz:
            mpi_cunqa_apply_cifcz(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_ecr:
            mpi_cunqa_apply_cifecr(statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            break;
        default:
            std::cout << "Error. Invalid gate name" << "\n";
            break;
    }
}


void MPIExecutor::apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, std::vector<double>& param)
{
    switch (instructions_map[gate_name])
    {
        case rx:
            mpi_cunqa_apply_rx(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case ry:
            mpi_cunqa_apply_ry(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case rz:
            mpi_cunqa_apply_rz(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case crx:
            mpi_cunqa_apply_crx(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case cry:
            mpi_cunqa_apply_cry(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case crz:
            mpi_cunqa_apply_crz(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_rx:
            mpi_cunqa_apply_cifrx(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_ry:
            mpi_cunqa_apply_cifry(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        case c_if_rz:
            mpi_cunqa_apply_cifrz(statevector, qubits, local_statevector_len, param, mpi_rank, is_last_process, statevector_ranges);
            break;
        default:
            std::cout << "Error. Invalid gate name" << "\n";
            break;
    }
}


void MPIExecutor::apply_unitary(const std::string& gate_name, const Matrix& matrix, const std::vector<int>& qubits)
{
    switch (instructions_map[gate_name])
    {
        case unitary:
        {
            if (matrix.size() == 2){
                mpi_cunqa_apply_1_gate(matrix, statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            } else if (matrix.size() == 4) {
                mpi_cunqa_apply_2_gate(matrix, statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            } else {
                std::cout << "Error. Invalid matrix dimension" << "\n";
            }
        }
        case c_if_unitary:
        {
            if (matrix.size() == 2){
                mpi_cunqa_apply_cif1gate(matrix, statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            } else if (matrix.size() == 4) {
                mpi_cunqa_apply_cif2gate(matrix, statevector, qubits, local_statevector_len, mpi_rank, is_last_process, statevector_ranges);
            } else {
                std::cout << "Error. Invalid matrix dimension" << "\n";
            }
        }
    }
}


int MPIExecutor::get_nonzero_position()
{
    try {
        auto it = std::find_if(statevector.begin(), statevector.end(), [](const State& c) {
            return c != std::complex<double>(0.0, 0.0);
        });

        if (it == statevector.end()) {
            return -1;
        } else {
            return std::distance(statevector.begin(), it);
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return -1;
    }
}


void MPIExecutor::restart_statevector()
{
    statevector.assign(statevector.size(), {0.0, 0.0});
    if (mpi_rank == 0) {
        statevector[0] = 1.0;
    }
}


ResultCunqa MPIExecutor::run(QuantumCircuit& quantumcircuit, const int shots)
{
    ResultCunqa result;
    std::string instruction_name;
    std::vector<int> qubits;
    int measurement;
    Params param;
    Matrix matrix;

    result.n_qubits = n_qubits;

    //SPDLOG_LOGGER_DEBUG(logger, "In Cunqa run: variables defined and n_qubits set.");

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < shots; i++) {

        for (auto& instruction : quantumcircuit) {
            instruction_name = instruction.at("name");
            qubits = instruction.at("qubits").get<std::vector<int>>();
            switch (case_map[instruction_name])
            {
                case measure:
                    measurement = apply_measure(qubits);
                    break;
                case gate:
                    apply_gate(instruction_name, qubits);
                    break;
                case parametric_gate:
                    param = instruction.at("params").get<Params>();
                    apply_parametric_gate(instruction_name, qubits, param);
                    break;
                case unitary:
                    matrix = instruction.at("params").get<Matrix>();
                    apply_unitary(instruction_name, matrix, qubits);
                    break;
            }
        } // End one shot
        
        int position_result = get_nonzero_position();
        //SPDLOG_LOGGER_DEBUG(logger, "position_result: {}", position_result);
        result.counts[position_result]++;
        
        restart_statevector();

    } // End all shots

    auto stop_time = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> duration = stop_time - start_time;

    double total_time = duration.count();

    result.total_time = total_time;

    return result;
}