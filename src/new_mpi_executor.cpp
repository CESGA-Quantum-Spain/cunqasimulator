#include "new_mpi_executor.hpp"

#include <iostream>
#include <complex>
#include <chrono>
#include <string>
#include <chrono>
#include <mpi.h>

#include "new_mpi_implementations.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"


NewMPIExecutor::NewMPIExecutor(int n_qubits) : n_qubits{n_qubits}
{
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    if (mpi_rank == 0) {
        statevector.resize(std::min((1ULL << n_qubits), (1ULL << 35))); // 35 qubits -> 512GB -> 1 entire node because I cannot use full 1TB
        statevector[0] = 1.0;
    } else if (n_qubits > 35) {
        for (int i = 0; i < n_qubits - 35; i++) {
            if (mpi_rank == i) {
                statevector.resize(1ULL << 35);
            }  
        }
    }
}

NewMPIExecutor::~NewMPIExecutor()
{
    MPI_Finalize();
}


int NewMPIExecutor::apply_measure(const std::vector<int>& qubits) 
{
    meas_out measurement = new_mpi_cunqa_apply_measure(statevector, qubits, mpi_rank);

    return measurement.measure;
}


void NewMPIExecutor::apply_gate(const std::string& gate_name, const std::vector<int>& qubits)
{
    switch (instructions_map[gate_name])
    {
        case id:
            break; 
        case x:
            new_mpi_cunqa_apply_x(statevector, qubits, mpi_rank);
            break;
        case y:
            new_mpi_cunqa_apply_y(statevector, qubits, mpi_rank);
            break;
        case z:
            new_mpi_cunqa_apply_z(statevector, qubits, mpi_rank);
            break;
        case h:
            new_mpi_cunqa_apply_h(statevector, qubits, mpi_rank);
            break;
        case sx:
            new_mpi_cunqa_apply_sx(statevector, qubits, mpi_rank);
            break;
        case cx:
            new_mpi_cunqa_apply_cx(statevector, qubits, mpi_rank);
            break;
        case cy:
            new_mpi_cunqa_apply_cy(statevector, qubits, mpi_rank);
            break;
        case cz:
            new_mpi_cunqa_apply_cz(statevector, qubits, mpi_rank);
            break;
        case ecr:
            new_mpi_cunqa_apply_ecr(statevector, qubits, mpi_rank);
            break;
        case c_if_x:
            new_mpi_cunqa_apply_cifx(statevector, qubits, mpi_rank);
            break;
        case c_if_y:
            new_mpi_cunqa_apply_cify(statevector, qubits, mpi_rank);
            break;
        case c_if_z:
            new_mpi_cunqa_apply_cifz(statevector, qubits, mpi_rank);
            break;
        case c_if_h:
            new_mpi_cunqa_apply_cifh(statevector, qubits, mpi_rank);
            break;
        case c_if_sx:
            new_mpi_cunqa_apply_cifsx(statevector, qubits, mpi_rank);
            break;
        case c_if_cx:
            new_mpi_cunqa_apply_cifcx(statevector, qubits, mpi_rank);
            break;
        case c_if_cy:
            new_mpi_cunqa_apply_cifcy(statevector, qubits, mpi_rank);
            break;
        case c_if_cz:
            new_mpi_cunqa_apply_cifcz(statevector, qubits, mpi_rank);
            break;
        case c_if_ecr:
            new_mpi_cunqa_apply_cifecr(statevector, qubits, mpi_rank);
            break;
        default:
            std::cout << "Error. Invalid gate name" << "\n";
            break;
    }
}


void NewMPIExecutor::apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, std::vector<double>& param)
{
    switch (instructions_map[gate_name])
    {
        case rx:
            new_mpi_cunqa_apply_rx(statevector, qubits, param, mpi_rank);
            break;
        case ry:
            new_mpi_cunqa_apply_ry(statevector, qubits, param, mpi_rank);
            break;
        case rz:
            new_mpi_cunqa_apply_rz(statevector, qubits, param, mpi_rank);
            break;
        case crx:
            new_mpi_cunqa_apply_crx(statevector, qubits, param, mpi_rank);
            break;
        case cry:
            new_mpi_cunqa_apply_cry(statevector, qubits, param, mpi_rank);
            break;
        case crz:
            new_mpi_cunqa_apply_crz(statevector, qubits, param, mpi_rank);
            break;
        case c_if_rx:
            new_mpi_cunqa_apply_cifrx(statevector, qubits, param, mpi_rank);
            break;
        case c_if_ry:
            new_mpi_cunqa_apply_cifry(statevector, qubits, param, mpi_rank);
            break;
        case c_if_rz:
            new_mpi_cunqa_apply_cifrz(statevector, qubits, param, mpi_rank);
            break;
        default:
            std::cout << "Error. Invalid gate name" << "\n";
            break;
    }
}


void NewMPIExecutor::apply_unitary(const std::string& gate_name, const Matrix& matrix, const std::vector<int>& qubits)
{
    switch (instructions_map[gate_name])
    {
        case unitary:
        {
            if (matrix.size() == 2){
                new_mpi_cunqa_apply_1_gate(matrix, statevector, qubits, mpi_rank);
            } else if (matrix.size() == 4) {
                new_mpi_cunqa_apply_2_gate(matrix, statevector, qubits, mpi_rank);
            } else {
                std::cout << "Error. Invalid matrix dimension" << "\n";
            }
        }
        case c_if_unitary:
        {
            if (matrix.size() == 2){
                new_mpi_cunqa_apply_cif1gate(matrix, statevector, qubits, mpi_rank);
            } else if (matrix.size() == 4) {
                new_mpi_cunqa_apply_cif2gate(matrix, statevector, qubits, mpi_rank);
            } else {
                std::cout << "Error. Invalid matrix dimension" << "\n";
            }
        }
    }
}


uint64_t NewMPIExecutor::get_nonzero_position()
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


void NewMPIExecutor::restart_statevector()
{
    statevector.assign(statevector.size(), {0.0, 0.0});
    if (mpi_rank == 0) {
        statevector[0] = 1.0;
    }
}


ResultCunqa NewMPIExecutor::run(QuantumCircuit& quantumcircuit, const int shots)
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