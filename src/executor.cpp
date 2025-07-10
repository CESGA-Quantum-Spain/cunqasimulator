#include "executor.hpp"

#include <iostream>
#include <complex>
#include <chrono>
#include <string>

#include "implementations.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

Executor::Executor(const int n_qubits, bool with_threads) : n_qubits{n_qubits}, with_threads{with_threads}
{
    if (with_threads) {
        if (n_qubits <= 18) {
            std::cout << "For n_qubits <= 18 the simulator is faster without threads" << "\n";
            with_threads = false; 
            n_threads_exponent = 0;
        } else if (n_qubits > 18 && n_qubits <= 22) {
            std::cout << "Number of threads set to 16" << "\n";
            n_threads_exponent = 4;
        } else {
            std::cout << "Number of threads set to 32" << "\n";
            n_threads_exponent = 5;
        }
    }
    
    statevector.resize(1ULL << n_qubits); // Danger. May cause silent errors.
    statevector[0] = ONE;
}

Executor::Executor(StateVector initial_state, bool with_threads) : n_qubits(initial_state.size()), statevector(initial_state), with_threads{with_threads}
{}


int Executor::apply_measure(const std::vector<int>& qubits)
{
    int measurement = cunqa_apply_measure(statevector, qubits, n_qubits, with_threads, n_threads_exponent);

    return measurement;
}

void Executor::apply_gate(const std::string& gate_name, const std::vector<int>& qubits)
{
    switch (instructions_map[gate_name])
    {
        case id:
            break;
        case x:
            cunqa_apply_x(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case y:
            cunqa_apply_y(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case z:
            cunqa_apply_z(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case h:
            cunqa_apply_h(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case sx:
            cunqa_apply_sx(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case swap:
            cunqa_apply_swap(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case cx:
            cunqa_apply_cx(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case cy:
            cunqa_apply_cy(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case cz:
            cunqa_apply_cz(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case ecr:
            cunqa_apply_ecr(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_x:
            cunqa_apply_cifx(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_y:
            cunqa_apply_cify(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_z:
            cunqa_apply_cifz(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_h:
            cunqa_apply_cifh(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_sx:
            cunqa_apply_cifsx(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_cx:
            cunqa_apply_cifcx(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_cy:
            cunqa_apply_cifcy(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_cz:
            cunqa_apply_cifcz(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_ecr:
            cunqa_apply_cifecr(statevector, qubits, n_qubits, with_threads, n_threads_exponent);
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
            cunqa_apply_rx(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case ry:
            cunqa_apply_ry(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case rz:
            cunqa_apply_rz(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case crx:
            cunqa_apply_crx(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case cry:
            cunqa_apply_cry(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case crz:
            cunqa_apply_crz(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_rx:
            cunqa_apply_cifrx(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_ry:
            cunqa_apply_cifry(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
            break;
        case c_if_rz:
            cunqa_apply_cifrz(statevector, qubits, param, n_qubits, with_threads, n_threads_exponent);
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
                cunqa_apply_1_gate(matrix, statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            } else if (matrix.size() == 4) {
                cunqa_apply_2_gate(matrix, statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            } else {
                std::cout << "Error. Invalid matrix dimension" << "\n";
            }
        }
        case c_if_unitary:
        {
            if (matrix.size() == 2){
                cunqa_apply_cif1gate(matrix, statevector, qubits, n_qubits, with_threads, n_threads_exponent);
            } else if (matrix.size() == 4) {
                cunqa_apply_cif2gate(matrix, statevector, qubits, n_qubits, with_threads, n_threads_exponent);
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

uint64_t Executor::get_nonzero_position()
{
    try {
        auto it = std::find_if(statevector.begin(), statevector.end(), [](const State& c) {
            return c != std::complex<Precision>(ZERO, ZERO);
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
