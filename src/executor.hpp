#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <array>
#include <chrono>
#include <string>
#include <optional>

#include "instructions.hpp"
#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"

#include "logger/logger.hpp"


class Executor 
{
public:
    int n_qubits;
    StateVector statevector;
    Instruction instruction_exec;

    Executor(int n_qubits);
    Executor(StateVector initial_state);

    #if defined(QPU_ZMQ)
/*     std::optional<zmq::socket_t> client; 
    std::optional<zmq::socket_t> server;
    std::optional<std::string> endpoint; */

    Executor(int n_qubits, 
        std::optional<zmq::socket_t> client = std::nullopt, 
        std::optional<zmq::socket_t> server = std::nullopt, 
        std::optional<std::string> endpoint = std::nullopt);
    Executor(StateVector initial_state, 
        std::optional<zmq::socket_t> client = std::nullopt, 
        std::optional<zmq::socket_t> server = std::nullopt, 
        std::optional<std::string> endpoint = std::nullopt);
    #endif

    void restart_statevector();

    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);
    
};

Executor::Executor(int n_qubits) : n_qubits{n_qubits}, statevector(1 << this->n_qubits), instruction_exec()
{
    statevector[0] = 1.0;
}

Executor::Executor(StateVector initial_state) : n_qubits(initial_state.size()), statevector(initial_state), instruction_exec()
{}

#if defined(QPU_ZMQ)
Executor::Executor(int n_qubits, 
    std::optional<zmq::socket_t> client, 
    std::optional<zmq::socket_t> server, 
    std::optional<std::string> endpoint) : 
    n_qubits{n_qubits}, statevector(1 << this->n_qubits), instruction_exec(std::move(client), std::move(server), endpoint)
{
    statevector[0] = 1.0;
    SPDLOG_LOGGER_DEBUG(logger, "Executor instanciated with ZMQ.");
}


Executor::Executor(StateVector initial_state, 
    std::optional<zmq::socket_t> client, 
    std::optional<zmq::socket_t> server, 
    std::optional<std::string> endpoint) :
    n_qubits(initial_state.size()), statevector(initial_state), instruction_exec(std::move(client), std::move(server), endpoint)
{
    SPDLOG_LOGGER_DEBUG(logger, "Executor instanciated with ZMQ.");
}
#endif


void Executor::restart_statevector()
{
    this->statevector.assign(this->statevector.size(), {0.0, 0.0});
    this->statevector[0] = 1.0;
}


//TODO: Classical Registers
ResultCunqa Executor::run(QuantumCircuit& quantumcircuit, int shots)
{
    ResultCunqa result;
    std::string instruction_name;
    std::array<int, 3> qubits;
    Params param;
    #if defined(QPU_MPI) || defined(QPU_ZMQ)
    type_comm comm_qpus;
    #endif

    result.n_qubits = this->n_qubits;

    SPDLOG_LOGGER_DEBUG(logger, "In Cunqa run: variables defined and n_qubits set.");

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < shots; i++) {

        for (auto& instruction : quantumcircuit) {
            instruction_name = instruction.at("name");
            qubits = instruction.at("qubits");
            switch (instructions_map[instruction_name])
            {
                case measure:
                case id:
                case x:
                case y:
                case z:
                case h:
                case cx:
                case cy:
                case cz:
                case ecr:
                case c_if_h:
                case c_if_x:
                case c_if_y:
                case c_if_z:
                case c_if_cx:
                case c_if_cy:
                case c_if_cz:
                case c_if_ecr:
                    this->statevector = this->instruction_exec.apply_instruction(this->statevector, instruction_name, qubits);
                    break;
                case rx:
                case ry:
                case rz:
                case c_if_rx:
                case c_if_ry:
                case c_if_rz:
                    param = instruction.at("params").get<Params>();
                    this->statevector = this->instruction_exec.apply_param_instruction(this->statevector, instruction_name, qubits, param);
                    break;
                #if defined(QPU_MPI)
                case d_c_if_h:
                case d_c_if_x:
                case d_c_if_y:
                case d_c_if_z:
                case d_c_if_cx:
                case d_c_if_cy:
                case d_c_if_cz:
                case d_c_if_ecr:
                    comm_qpus = instruction.at("qpus").get<qpu_comm_type>();
                    this->statevector = this->instruction_exec.apply_dist_instruction(this->statevector, instruction_name, qubits, comm_qpus);
                    break;
                case d_c_if_rx:
                case d_c_if_ry:
                case d_c_if_rz:
                    param = instruction.at("params").get<Params>();
                    comm_qpus = instruction.at("qpus").get<qpu_comm_type>();
                    this->statevector = this->instruction_exec.apply_dist_param_instruction(this->statevector, instruction_name, qubits, comm_qpus, param);
                    break;
                #elif defined(QPU_ZMQ)
                case d_c_if_h:
                case d_c_if_x:
                case d_c_if_y:
                case d_c_if_z:
                case d_c_if_cx:
                case d_c_if_cy:
                case d_c_if_cz:
                case d_c_if_ecr:
                    SPDLOG_LOGGER_DEBUG(logger, "Case d-simple gates.");
                    
                    comm_qpus = instruction.at("qpus").get<qpu_comm_type>();
                    SPDLOG_LOGGER_DEBUG(logger, "comm_qpus.comm_endpoints.");

                    this->statevector = this->instruction_exec.apply_dist_instruction(this->statevector, instruction_name, qubits, comm_qpus);
                    break;
                case d_c_if_rx:
                case d_c_if_ry:
                case d_c_if_rz:
                    SPDLOG_LOGGER_DEBUG(logger, "Case d-parametric gates.");
                    param = instruction.at("params").get<Params>();
                    SPDLOG_LOGGER_DEBUG(logger, "Params ready.");
                    comm_qpus = instruction.at("qpus").get<qpu_comm_type>();
                    SPDLOG_LOGGER_DEBUG(logger, "comm_qpus.comm_endpoints.");

                    this->statevector = this->instruction_exec.apply_dist_param_instruction(this->statevector, instruction_name, qubits, comm_qpus, param);
                    break;
                #endif
                default:
                    std::cout << "Error. Invalid gate name" << "\n";
                    break;
            }
        }
        
        int position_result = get_nonzero_position(statevector);
        SPDLOG_LOGGER_DEBUG(logger, "position_result: {}", position_result);
        result.counts[position_result]++;
        
        restart_statevector();

    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

    double total_time = duration.count();

    result.total_time = total_time;

    return result;
}

