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
    
    uint64_t max_easy_position = std::floor(((double)max_vector_position - (double)initial_position)/(1ULL << (qubit_position + 1)));
    return (1ULL << (qubit_position + 1)) * max_easy_position;
}

} // End anonymous namespace

meas_out mpi_cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) 
{
    StateVector sv;
    meas_out res;
    res.statevector = sv;
    res.measure = 0;
    return res;
}
// One-Qubit Gates
void mpi_cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) 
{
    std::cout << "Start of mpi_cunqa_apply_x on process " << mpi_rank << "\n";
    std::cout << "statevector_ranges[1] on " << mpi_rank << " :" << statevector_ranges.at(mpi_rank)[1] << "\n";
    std::complex<double> aux;
    double comming_real_part;
    double comming_imag_part;
    double my_real_part;
    double my_imag_part;
    std::complex<double> comming_complex;

    uint64_t number_of_communications_with_previous = 0;
    uint64_t previous_first_communication_position;
    uint64_t first_communication_position_with_previous = 0;
    if (mpi_rank != 0) {
        MPI_Recv(&number_of_communications_with_previous, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&previous_first_communication_position, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //std::cout << "number_of_communications_with_previous on " << mpi_rank << ": " << number_of_communications_with_previous << "\n";
        //std::cout << "previous_first_communication_position on " << mpi_rank << ": " << previous_first_communication_position << "\n";

        if (previous_first_communication_position + (1ULL << qubits[0]) > statevector_ranges.at(mpi_rank - 1)[1]) {
            uint64_t number_first_easies = statevector_ranges.at(mpi_rank - 1)[0] + previous_first_communication_position + (1ULL << qubits[0]) - statevector_ranges.at(mpi_rank - 1)[1];
            for (uint64_t i = statevector_ranges.at(mpi_rank - 1)[0]; i < number_first_easies + 1; i++) {
                aux = statevector[i];
                statevector[i] = statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = aux;
            }
            first_communication_position_with_previous = number_first_easies + 1;
        }

        for (uint64_t i = first_communication_position_with_previous; i < first_communication_position_with_previous + number_of_communications_with_previous; i++) {
            MPI_Recv(&comming_real_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&comming_imag_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&my_real_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD);
            MPI_Send(&my_imag_part, 1, MPI_DOUBLE, mpi_rank - 1, 0, MPI_COMM_WORLD);    
            comming_complex = std::complex<double>(comming_real_part, comming_imag_part);
            statevector[i] = comming_complex;
        }
    }

    
    uint64_t max_easy_position = get_max_easy_position(first_communication_position_with_previous + number_of_communications_with_previous, statevector_ranges.at(mpi_rank)[1], qubits[0]);
    std::cout << "max_easy_position on " << mpi_rank << " :" << max_easy_position << "\n";
    uint64_t P = max_easy_position + (1ULL << qubits[0]); // TODO: naming
    std::cout << "P on " << mpi_rank << " :" << P << "\n";
    uint64_t number_of_not_easy_no_comm;
    uint64_t max_no_comm_position;
    uint64_t first_communication_position;
    uint64_t last_communication_position;
    uint64_t number_of_communications_with_following;
    if (P <= statevector_ranges.at(mpi_rank)[1]) {
        number_of_not_easy_no_comm = statevector_ranges.at(mpi_rank)[1] - P;
        max_no_comm_position = max_easy_position + number_of_not_easy_no_comm; // +1?
        first_communication_position = std::min(max_no_comm_position + 1, statevector_ranges.at(mpi_rank)[1]);
        last_communication_position = P - first_communication_position;
        number_of_communications_with_following = last_communication_position - first_communication_position;
        /* std::cout << "P > statevector_ranges.at(mpi_rank)[1]" << "\n";
        std::cout << "P on else on " << mpi_rank << ": " << P << "\n";
        std::cout << "max_no_comm_position on " << mpi_rank << ": " << max_no_comm_position << "\n";
        std::cout << "first_communication_position on " << mpi_rank << ": " << first_communication_position << "\n";
        std::cout << "last_communication_position on " << mpi_rank << ": " << last_communication_position << "\n";
        std::cout << "number_of_communications_with_following on " << mpi_rank << ": " << number_of_communications_with_following << "\n"; */
    } else {
        P = statevector_ranges.at(mpi_rank)[1];
        number_of_not_easy_no_comm = 0;
        max_no_comm_position = max_easy_position; // +1?
        first_communication_position = std::min(max_no_comm_position + 1, statevector_ranges.at(mpi_rank)[1]);
        last_communication_position = statevector_ranges.at(mpi_rank)[1];
        number_of_communications_with_following = last_communication_position - first_communication_position;
        
        std::cout << "P > statevector_ranges.at(mpi_rank)[1]" << "\n";
        std::cout << "P on else on " << mpi_rank << ": " << P << "\n";
        std::cout << "max_no_comm_position on " << mpi_rank << ": " << max_no_comm_position << "\n";
        std::cout << "first_communication_position on " << mpi_rank << ": " << first_communication_position << "\n";
        std::cout << "last_communication_position on " << mpi_rank << ": " << last_communication_position << "\n";
        std::cout << "number_of_communications_with_following on " << mpi_rank << ": " << number_of_communications_with_following << "\n";

    }
    
    if (mpi_rank != mpi_size - 1) {
        MPI_Send(&number_of_communications_with_following, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&first_communication_position, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
    }

    std::cout << "max_easy_position on " << mpi_rank << ": " << max_easy_position << "\n";
    std::cout << "(1 << (qubits[0] + 1)) on " << mpi_rank << ": " << (1 << (qubits[0] + 1)) << "\n";
    for (uint64_t j = first_communication_position_with_previous + number_of_communications_with_previous; j < max_easy_position + 1; j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j; i < j + (1 << qubits[0]); i++) {
            aux = statevector[i];
            if (i == 2) { std::cout << "IS REACHING 2!" << "\n";}
            if (i == 0 || i == 1) {
                std::cout << "statevector[" << i << "]: " << statevector[i] << " on rank " << mpi_rank << "\n";
                std::cout << "flipbit(i, qubits[0]) " << flipbit(i, qubits[0]) << " on rank " << mpi_rank << "\n";
            }
            statevector[i] = statevector[flipbit(i, qubits[0])];
            if (i == 0 || i == 1) {
                std::cout << "statevector[" << i << "]: " << statevector[i] << " on rank " << mpi_rank << "\n";
            }
            statevector[flipbit(i, qubits[0])] = aux;
            if (i == 0 || i == 1) {
                std::cout << "statevector[" << flipbit(i, qubits[0]) << "]: " << statevector[flipbit(i, qubits[0])] << " on rank " << mpi_rank << "\n";
            }
        }
    }

    std::cout << "statevector[0]: " << statevector[0] << "\n"; 
    std::cout << "statevector[1]: " << statevector[1] << "\n";
    std::cout << "statevector[2]: " << statevector[2] << "\n";

    for (uint64_t i = max_easy_position + 1; i < max_easy_position + 1 + number_of_not_easy_no_comm; i++) {
        aux = statevector[i];
        statevector[i] = statevector[flipbit(i, qubits[0])];
        statevector[flipbit(i, qubits[0])] = aux;
    }

    for (uint64_t i = first_communication_position; i < first_communication_position + number_of_communications_with_following; i++) {
        my_real_part = statevector[i].real();
        my_imag_part = statevector[i].imag();
        MPI_Send(&my_real_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(&my_imag_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&comming_real_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&comming_imag_part, 1, MPI_DOUBLE, mpi_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        comming_complex = std::complex<double>(comming_real_part, comming_imag_part);
        statevector[i] = comming_complex;
    }
    
}


void mpi_cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
// Two-Qubit Gates
void mpi_cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
//Classical conditional one-qubit gates
void mpi_cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
//Classical conditional two-qubits gates
void mpi_cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
// Personalized gates
void mpi_cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}
void mpi_cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank, const int& mpi_size, const std::unordered_map<int, std::vector<uint64_t>>& statevector_ranges) {}