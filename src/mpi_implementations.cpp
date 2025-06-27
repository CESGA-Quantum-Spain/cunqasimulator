#include "mpi_implementations.hpp"

#include <iostream>
#include <complex>
#include <algorithm>
#include <cmath>
#include <random>
#include <mpi.h>

#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

namespace {
int find_mpi_rank(const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges, const uint64_t element)
{
    for (const auto& pair : statevector_ranges) {
        int key = pair.first;
        const std::vector<uint64_t>& range = pair.second;

        if (range.size() != 2) {
            throw std::runtime_error("Vector associated with key " + std::to_string(key) + " does not have size 2.");
        }

        uint64_t low = range[0];
        uint64_t high = range[1];

        if (element >= low && element <= high) {
            return key; 
        }
    }

    throw std::runtime_error("Value X (" + std::to_string(element) + ") not found within any range in the map.");
}


uint64_t get_max_easy_position(uint64_t initial_position, uint64_t max_vector_position, const uint64_t& qubit_position)
{
    
    uint64_t max_easy_position = std::floor(((double)max_vector_position + 1 - (double)initial_position - (1ULL << qubit_position)));
    return max_easy_position;
}


} // End anonymous namespace

meas_out mpi_cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) 
{
    StateVector sv;
    meas_out res;
    res.statevector = sv;
    res.measure = 0;
    return res;
}
// One-Qubit Gates
void mpi_cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) 
{
    std::complex<double> aux;
    std::complex<double> comming_complex;
    uint64_t max_easy_position;
    uint64_t possibly_last_easy_element;
    uint64_t number_of_not_easy_no_comm;
    uint64_t max_no_comm_position;
    uint64_t first_communication_position;
    uint64_t last_communication_position;
    uint64_t number_of_communications_with_following;
    uint64_t number_of_communications_with_previous = 0;
    uint64_t previous_first_communication_position = 0;
    uint64_t first_communication_position_with_previous = 0;

    if (mpi_rank != 0) {
        MPI_Recv(&number_of_communications_with_previous, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&previous_first_communication_position, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        if (previous_first_communication_position + number_of_communications_with_previous == statevector_ranges.at(mpi_rank - 1)[1]) {
            uint64_t number_first_easies =  (1ULL << qubits[0]) - statevector_ranges.at(mpi_rank - 1)[1] + previous_first_communication_position;
            for (uint64_t i = 0; i < number_first_easies; i++) {
                aux = statevector[i];
                statevector[i] = statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = aux;
            }
            first_communication_position_with_previous = number_first_easies - 1;
        }

        for (uint64_t i = first_communication_position_with_previous; i < first_communication_position_with_previous + number_of_communications_with_previous; i++) {
            MPI_Recv(&comming_complex, 1, MPI_CXX_DOUBLE_COMPLEX, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            statevector[i] = comming_complex;
            MPI_Send(&statevector[i], 1, MPI_CXX_DOUBLE_COMPLEX, mpi_rank - 1, 0, MPI_COMM_WORLD);
            statevector[i] = comming_complex;
        }
    }


    max_easy_position = get_max_easy_position(first_communication_position_with_previous + number_of_communications_with_previous, local_statevector_len, qubits[0]);
    possibly_last_easy_element = max_easy_position + (1ULL << qubits[0]);
    
    if (possibly_last_easy_element <= local_statevector_len) {
        number_of_not_easy_no_comm = local_statevector_len - possibly_last_easy_element;
        max_no_comm_position = max_easy_position + number_of_not_easy_no_comm; 
        first_communication_position = std::min(max_no_comm_position + 1, local_statevector_len); 
        last_communication_position = first_communication_position + (1ULL << qubits[0]);
        number_of_communications_with_following = last_communication_position - first_communication_position;
    } else {
        number_of_not_easy_no_comm = possibly_last_easy_element - local_statevector_len;
        max_no_comm_position = max_easy_position - number_of_not_easy_no_comm; 
        first_communication_position = std::min(max_no_comm_position + 1, local_statevector_len);
        last_communication_position = local_statevector_len; 
        number_of_communications_with_following = last_communication_position - first_communication_position;
    }

    
    if (!is_last_process) {
        MPI_Send(&number_of_communications_with_following, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&first_communication_position, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
        for (uint64_t i = first_communication_position; i < first_communication_position + number_of_communications_with_following; i++) {
            MPI_Send(&statevector[i], 1, MPI_CXX_DOUBLE_COMPLEX, mpi_rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&comming_complex, 1, MPI_CXX_DOUBLE_COMPLEX, mpi_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            statevector[i] = comming_complex;
        }
    }

    for (uint64_t j = first_communication_position_with_previous + number_of_communications_with_previous + (1ULL << qubits[0]); j < max_easy_position + (1ULL << qubits[0]); j = j + (1ULL << (qubits[0] + 1))) {
        for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
            aux = statevector[i];
            statevector[i] = statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = aux;
        }
    }

    for (uint64_t i = max_easy_position - (1ULL << qubits[0]) + 1; i < max_easy_position - (1ULL << qubits[0]) + 1 + number_of_not_easy_no_comm; i++) {
        aux = statevector[i];
        statevector[i] = statevector[flipbit(i, qubits[0])];
        statevector[flipbit(i, qubits[0])] = aux;
    }
}


void mpi_cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) 
{
    std::complex<double> aux;
    double comming_real_part;
    double comming_imag_part;
    double my_real_part;
    double my_imag_part;
    std::complex<double> comming_complex;
    uint64_t max_easy_position;
    uint64_t possibly_last_easy_element;
    uint64_t number_of_not_easy_no_comm;
    uint64_t max_no_comm_position;
    uint64_t first_communication_position;
    uint64_t last_communication_position;
    uint64_t number_of_communications_with_following;
    uint64_t number_of_communications_with_previous = 0;
    uint64_t previous_first_communication_position = 0;
    uint64_t first_communication_position_with_previous = 0;

    if (mpi_rank != 0) {
        MPI_Recv(&number_of_communications_with_previous, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&previous_first_communication_position, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (previous_first_communication_position + (1ULL << qubits[0]) > statevector_ranges.at(mpi_rank - 1)[1]) {
            uint64_t number_first_easies = previous_first_communication_position + (1ULL << qubits[0]) - statevector_ranges.at(mpi_rank - 1)[1];
            for (uint64_t i = 0; i < number_first_easies + 1; i++) {
                aux = statevector[i];
                statevector[i] = imag * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = -imag * aux;
            }
            first_communication_position_with_previous = number_first_easies + 1;
        }

        for (uint64_t i = first_communication_position_with_previous; i < first_communication_position_with_previous + number_of_communications_with_previous; i++) {
            MPI_Recv(&comming_real_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&comming_imag_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&my_real_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD);
            MPI_Send(&my_imag_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD);    
            comming_complex = std::complex<double>(comming_real_part, comming_imag_part);
            statevector[i] = -imag * comming_complex;
        }
    }

    
    max_easy_position = get_max_easy_position(first_communication_position_with_previous + number_of_communications_with_previous, local_statevector_len, qubits[0]);
    possibly_last_easy_element = max_easy_position + (1ULL << qubits[0]); // TODO: naming
    
    if (possibly_last_easy_element <= local_statevector_len) {
        number_of_not_easy_no_comm = local_statevector_len - possibly_last_easy_element;
        max_no_comm_position = max_easy_position + number_of_not_easy_no_comm; 
        first_communication_position = std::min(max_no_comm_position + 1, local_statevector_len); 
        last_communication_position = possibly_last_easy_element - first_communication_position; // TODO: Warning. Positions vs lengths
        number_of_communications_with_following = last_communication_position - first_communication_position;
    } else {
        possibly_last_easy_element = local_statevector_len;
        number_of_not_easy_no_comm = 0;
        max_no_comm_position = max_easy_position; // +1?
        first_communication_position = std::min(max_no_comm_position + 1, local_statevector_len);
        last_communication_position = local_statevector_len; // TODO: Warning. Positions vs lengths 
        number_of_communications_with_following = last_communication_position - first_communication_position;
    }
    
    if (!is_last_process) {
        MPI_Send(&number_of_communications_with_following, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&first_communication_position, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
    }

    for (uint64_t j = first_communication_position_with_previous + number_of_communications_with_previous + (1ULL << (qubits[0] + 1)); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
        for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
            aux = statevector[i];
            statevector[i] = statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = aux;
        }
    }

    for (uint64_t i = max_easy_position + 1; i < max_easy_position + 1 + number_of_not_easy_no_comm; i++) {
        aux = statevector[i];
        statevector[i] = imag * statevector[flipbit(i, qubits[0])];
        statevector[flipbit(i, qubits[0])] = -imag * aux;
    }

    for (uint64_t i = first_communication_position; i < first_communication_position + number_of_communications_with_following; i++) {
        my_real_part = statevector[i].real();
        my_imag_part = statevector[i].imag();
        MPI_Send(&my_real_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&my_imag_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&comming_real_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&comming_imag_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        comming_complex = std::complex<double>(comming_real_part, comming_imag_part);

        statevector[i] = imag * comming_complex;
    }
}


void mpi_cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) 
{
    // TODO   
}


void mpi_cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) 
{
    std::complex<double> aux;
    double comming_real_part;
    double comming_imag_part;
    double my_real_part;
    double my_imag_part;
    std::complex<double> comming_complex;
    uint64_t max_easy_position;
    uint64_t possibly_last_easy_element;
    uint64_t number_of_not_easy_no_comm;
    uint64_t max_no_comm_position;
    uint64_t first_communication_position;
    uint64_t last_communication_position;
    uint64_t number_of_communications_with_following;
    uint64_t number_of_communications_with_previous = 0;
    uint64_t previous_first_communication_position = 0;
    uint64_t first_communication_position_with_previous = 0;

    if (mpi_rank != 0) {
        MPI_Recv(&number_of_communications_with_previous, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&previous_first_communication_position, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (previous_first_communication_position + (1ULL << qubits[0]) > statevector_ranges.at(mpi_rank - 1)[1]) {
            uint64_t number_first_easies = previous_first_communication_position + (1ULL << qubits[0]) - statevector_ranges.at(mpi_rank - 1)[1];
            for (uint64_t i = 0; i < number_first_easies + 1; i++) {
                aux = statevector[i];
                statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
            }
            first_communication_position_with_previous = number_first_easies + 1;
        }

        for (uint64_t i = first_communication_position_with_previous; i < first_communication_position_with_previous + number_of_communications_with_previous; i++) {
            MPI_Recv(&comming_real_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&comming_imag_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&my_real_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD);
            MPI_Send(&my_imag_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD);    
            comming_complex = std::complex<double>(comming_real_part, comming_imag_part);
            statevector[i] = -inverse_sqrt_2 * comming_complex + inverse_sqrt_2 * statevector[i];
        }
    }

    
    max_easy_position = get_max_easy_position(first_communication_position_with_previous + number_of_communications_with_previous, local_statevector_len, qubits[0]);
    possibly_last_easy_element = max_easy_position + (1ULL << qubits[0]); // TODO: naming
    
    if (possibly_last_easy_element <= local_statevector_len) {
        number_of_not_easy_no_comm = local_statevector_len - possibly_last_easy_element;
        max_no_comm_position = max_easy_position + number_of_not_easy_no_comm; 
        first_communication_position = std::min(max_no_comm_position + 1, local_statevector_len); 
        last_communication_position = possibly_last_easy_element - first_communication_position; // TODO: Warning. Positions vs lengths
        number_of_communications_with_following = last_communication_position - first_communication_position;
    } else {
        possibly_last_easy_element = local_statevector_len;
        number_of_not_easy_no_comm = 0;
        max_no_comm_position = max_easy_position; // +1?
        first_communication_position = std::min(max_no_comm_position + 1, local_statevector_len);
        last_communication_position = local_statevector_len; // TODO: Warning. Positions vs lengths 
        number_of_communications_with_following = last_communication_position - first_communication_position;
    }
    
    if (!is_last_process) {
        MPI_Send(&number_of_communications_with_following, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&first_communication_position, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
    }

    for (uint64_t j = first_communication_position_with_previous + number_of_communications_with_previous + (1ULL << (qubits[0] + 1)); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
        for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
            aux = statevector[i];
            statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
        }
    }

    for (uint64_t i = max_easy_position + 1; i < max_easy_position + 1 + number_of_not_easy_no_comm; i++) {
        aux = statevector[i];
        statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
        statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
    }

    for (uint64_t i = first_communication_position; i < first_communication_position + number_of_communications_with_following; i++) {
        my_real_part = statevector[i].real();
        my_imag_part = statevector[i].imag();
        MPI_Send(&my_real_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&my_imag_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&comming_real_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&comming_imag_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        comming_complex = std::complex<double>(comming_real_part, comming_imag_part);

        statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * comming_complex;
    }
}
void mpi_cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
// Two-Qubit Gates
void mpi_cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
//Classical conditional one-qubit gates
void mpi_cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
//Classical conditional two-qubits gates
void mpi_cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
// Personalized gates
void mpi_cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const int& mpi_rank, const bool& is_last_process, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}