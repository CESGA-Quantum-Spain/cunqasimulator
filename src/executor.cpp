#include "executor.hpp"

#include <iostream>
#include <complex>
#include <chrono>
#include <string>

#include "instructions.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

Executor::Executor(int n_qubits) : n_qubits{n_qubits}, statevector(1 << this->n_qubits)
{
    statevector[0] = 1.0;
}

Executor::Executor(StateVector initial_state) : n_qubits(initial_state.size()), statevector(initial_state)
{}


//TODO: Classical Registers
ResultCunqa Executor::run(QuantumCircuit& quantumcircuit, int shots)
{
    ResultCunqa result;
    std::string instruction_name;
    std::vector<int> qubits;
    Params param;
    Matrix matrix;
    int dimension;

    result.n_qubits = this->n_qubits;

    //SPDLOG_LOGGER_DEBUG(logger, "In Cunqa run: variables defined and n_qubits set.");

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < shots; i++) {

        for (auto& instruction : quantumcircuit) {
            instruction_name = instruction.at("name");
            qubits = instruction.at("qubits").get<std::vector<int>>();
            switch (instructions_map[instruction_name])
            {
                case measure:
                    Instruction::apply_measure(this->statevector, qubits);
                    break;
                case id:
                case x:
                case y:
                case z:
                case h:
                case sx:
                case cx:
                case cy:
                case cz:
                case ecr:
                case c_if_x:
                case c_if_y:
                case c_if_z:
                case c_if_h:
                case c_if_sx:
                case c_if_cx:
                case c_if_cy:
                case c_if_cz:
                case c_if_ecr:
                    Instruction::apply_instruction(this->statevector, instruction_name, qubits);
                    break;
                case rx:
                case ry:
                case rz:
                case crx:
                case cry:
                case crz:
                case c_if_rx:
                case c_if_ry:
                case c_if_rz:
                    param = instruction.at("params").get<Params>();
                    Instruction::apply_param_instruction(this->statevector, instruction_name, qubits, param);
                    break;
                case unitary:{
                    matrix = instruction.at("params").get<Matrix>();
                    Instruction::apply_unitary(matrix, this->statevector, qubits);
                    break;
                }
                default:
                    std::cout << "Error. Invalid gate name" << "\n";
                    break;
            }
        }
        
        int position_result = this->get_nonzero_position();
        //SPDLOG_LOGGER_DEBUG(logger, "position_result: {}", position_result);
        result.counts[position_result]++;
        
        restart_statevector();

    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> duration = stop_time - start_time;

    double total_time = duration.count();

    result.total_time = total_time;

    return result;
}


int Executor::apply_measure(std::vector<int>& qubits)
{
    meas_out meas =  Instruction::apply_measure(this->statevector, qubits);

    return meas.measure;
}

void Executor::apply_unitary(Matrix& matrix, std::vector<int>& qubits)
{
    Instruction::apply_unitary(matrix, this->statevector, qubits);
}

void Executor::apply_gate(const std::string& gate_name, std::vector<int>& qubits)
{
    Instruction::apply_instruction(this->statevector, gate_name, qubits);
}

void Executor::apply_parametric_gate(const std::string& gate_name, std::vector<int>& qubits, std::vector<double>& param)
{
    Instruction::apply_param_instruction(this->statevector, gate_name, qubits, param);
}

void Executor::restart_statevector()
{
    this->statevector.assign(this->statevector.size(), {0.0, 0.0});
    this->statevector[0] = 1.0;
}

int Executor::get_nonzero_position()
{
    int position;
    try {
        auto it = std::find_if(this->statevector.begin(), this->statevector.end(), [](const State& c) {
            return c != std::complex<double>(0, 0); // Check for nonzero
        });
        position = std::distance(this->statevector.begin(), it);
    } catch (const std::exception& e) {
        //SPDLOG_LOGGER_ERROR(logger, "Error findind the non-zero position. Check if all qubits were measured.");
        return -1;
    }

    return position;
}