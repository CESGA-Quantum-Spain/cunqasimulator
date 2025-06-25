#include "executor.hpp"

#include <iostream>
#include <complex>
#include <chrono>
#include <string>

#include "implementations.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

Executor::Executor(const int n_qubits) : n_qubits{n_qubits}, statevector(1 << n_qubits)
{
    statevector[0] = 1.0;
}

Executor::Executor(StateVector initial_state) : n_qubits(initial_state.size()), statevector(initial_state)
{}


int Executor::apply_measure(const std::vector<int>& qubits)
{
    meas_out measurement = cunqa_apply_measure(statevector, qubits);

    return measurement.measure;
}

void Executor::apply_gate(const std::string& gate_name, const std::vector<int>& qubits)
{
    switch (instructions_map[gate_name])
    {
        case id:
            break;
        case x:
            cunqa_apply_x(statevector, qubits);
            break;
        case y:
            cunqa_apply_y(statevector, qubits);
            break;
        case z:
            cunqa_apply_z(statevector, qubits);
            break;
        case h:
            cunqa_apply_h(statevector, qubits);
            break;
        case sx:
            cunqa_apply_sx(statevector, qubits);
            break;
        case cx:
            cunqa_apply_cx(statevector, qubits);
            break;
        case cy:
            cunqa_apply_cy(statevector, qubits);
            break;
        case cz:
            cunqa_apply_cz(statevector, qubits);
            break;
        case ecr:
            cunqa_apply_ecr(statevector, qubits);
            break;
        case c_if_x:
            cunqa_apply_cifx(statevector, qubits);
            break;
        case c_if_y:
            cunqa_apply_cify(statevector, qubits);
            break;
        case c_if_z:
            cunqa_apply_cifz(statevector, qubits);
            break;
        case c_if_h:
            cunqa_apply_cifh(statevector, qubits);
            break;
        case c_if_sx:
            cunqa_apply_cifsx(statevector, qubits);
            break;
        case c_if_cx:
            cunqa_apply_cifcx(statevector, qubits);
            break;
        case c_if_cy:
            cunqa_apply_cifcy(statevector, qubits);
            break;
        case c_if_cz:
            cunqa_apply_cifcz(statevector, qubits);
            break;
        case c_if_ecr:
            cunqa_apply_cifecr(statevector, qubits);
            break;
        default:
            std::cout << "Error. Invalid gate name" << "\n";
            break;
    }
}

void Executor::apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, Params& param)
{
    switch (instructions_map[gate_name])
    {
        case rx:
            cunqa_apply_rx(statevector, qubits, param);
            break;
        case ry:
            cunqa_apply_ry(statevector, qubits, param);
            break;
        case rz:
            cunqa_apply_rz(statevector, qubits, param);
            break;
        case crx:
            cunqa_apply_crx(statevector, qubits, param);
            break;
        case cry:
            cunqa_apply_cry(statevector, qubits, param);
            break;
        case crz:
            cunqa_apply_crz(statevector, qubits, param);
            break;
        case c_if_rx:
            cunqa_apply_cifrx(statevector, qubits, param);
            break;
        case c_if_ry:
            cunqa_apply_cifry(statevector, qubits, param);
            break;
        case c_if_rz:
            cunqa_apply_cifrz(statevector, qubits, param);
            break;
        default:
            std::cout << "Error. Invalid gate name" << "\n";
            break;
    }
}

void Executor::apply_unitary(const std::string& gate_name, const Matrix& matrix, const std::vector<int>& qubits)
{
    switch (case_map[gate_name])
    {
        case unitary:
        {
            if (matrix.size() == 2){
                cunqa_apply_1_gate(matrix, statevector, qubits);
            } else if (matrix.size() == 4) {
                cunqa_apply_2_gate(matrix, statevector, qubits);
            } else {
                std::cout << "Error. Invalid matrix dimension" << "\n";
            }
        }
        case c_if_unitary:
        {
            if (matrix.size() == 2){
                cunqa_apply_cif1gate(matrix, statevector, qubits);
            } else if (matrix.size() == 4) {
                cunqa_apply_cif2gate(matrix, statevector, qubits);
            } else {
                std::cout << "Error. Invalid matrix dimension" << "\n";
            }
        }
    }
}

void Executor::restart_statevector()
{
    statevector.assign(statevector.size(), {0.0, 0.0});
    statevector[0] = 1.0;
}

int Executor::get_nonzero_position()
{
    int position;
    try {
        auto it = std::find_if(statevector.begin(), statevector.end(), [](const State& c) {
            return c != std::complex<double>(0, 0); // Check for nonzero
        });
        position = std::distance(statevector.begin(), it);
    } catch (const std::exception& e) {
        //SPDLOG_LOGGER_ERROR(logger, "Error findind the non-zero position. Check if all qubits were measured.");
        return -1;
    }

    return position;
}


ResultCunqa Executor::run(QuantumCircuit& quantumcircuit, const int shots)
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
