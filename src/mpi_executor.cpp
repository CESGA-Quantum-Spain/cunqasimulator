#include "mpi_executor.hpp"

#include <iostream>
#include <complex>
#include <chrono>
#include <string>
#include <mpi.h>

#include "mpi_instructions.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"


MPIExecutor::MPIExecutor(int n_qubits) : n_qubits{n_qubits}
{
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    STATEVECTOR_LEN = (ONE << n_qubits) - 1; // 2^n_qubits - 1
    std::cout << "STATEVECTOR_LEN: " << STATEVECTOR_LEN << "\n";

    uint64_t MEM_BYTES = get_process_memory_in_bytes();
    if (MEM_BYTES == 0) {
        throw std::runtime_error("Error getting the process memory.");
    }

    double double_MAX_VECTOR_LEN = MEM_BYTES / 16.0;
    MAX_VECTOR_LEN = std::floor(double_MAX_VECTOR_LEN);
    LAST_STATEVECTOR_ELEMENT = MAX_VECTOR_LEN;

    if (mpi_rank == 0) {
        std::cout << "MAX_VECTOR_LEN: " << MAX_VECTOR_LEN << " on " << mpi_rank << "\n";
        FIRST_STATEVECTOR_ELEMENT = 0;
        if (MAX_VECTOR_LEN > STATEVECTOR_LEN) {
            statevector.resize(STATEVECTOR_LEN);
            statevector[0] = 1.0;
            MPI_Send(&ZERO, 1, MPI_UINT64_T, 1, 0, MPI_COMM_WORLD);
        } else {
            statevector.resize(MAX_VECTOR_LEN);
            statevector[0] = 1.0;
            MPI_Send(&LAST_STATEVECTOR_ELEMENT, 1, MPI_UINT64_T, 1, 0, MPI_COMM_WORLD);
            std::cout << "The MPI_RANK " << mpi_rank << " will simulate the first " << LAST_STATEVECTOR_ELEMENT << " statevector elements" << "\n";
            std::cout.flush();
        }
    } else if (mpi_rank < mpi_size - 1) {
        std::cout << "MAX_VECTOR_LEN: " << MAX_VECTOR_LEN << " on " << mpi_rank << "\n";
        std::cout.flush();
        MPI_Recv(&previous_LAST_ELEMENT, 1, MPI_UINT64_T, mpi_rank - 1 , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (previous_LAST_ELEMENT == 0) {
            std::cout << "StateVector already fits on MPI_RANK " << mpi_rank - 1 << "\n";
            std::cout.flush();
            MPI_Send(&ZERO, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
        } else {
            FIRST_STATEVECTOR_ELEMENT = previous_LAST_ELEMENT + 1;
            LAST_STATEVECTOR_ELEMENT = FIRST_STATEVECTOR_ELEMENT + MAX_VECTOR_LEN;
            if (LAST_STATEVECTOR_ELEMENT < STATEVECTOR_LEN) {
                MPI_Send(&LAST_STATEVECTOR_ELEMENT, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
                std::cout << "The MPI_RANK " << mpi_rank << " will simulate the statevector elements from " << FIRST_STATEVECTOR_ELEMENT << " to " << LAST_STATEVECTOR_ELEMENT << "\n";
                std::cout.flush();
                statevector.resize(MAX_VECTOR_LEN);
            } else {
                std::cout << "StateVector fits on MPI_RANK " << mpi_rank << "\n";
                std::cout.flush();
                MPI_Send(&ZERO, 1, MPI_UINT64_T, mpi_rank + 1, 0, MPI_COMM_WORLD);
                statevector.resize(STATEVECTOR_LEN - FIRST_STATEVECTOR_ELEMENT);
            }
            
        }
    } else {
        MPI_Recv(&previous_LAST_ELEMENT, 1, MPI_UINT64_T, mpi_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (previous_LAST_ELEMENT == 0) {
            std::cout << "StateVector already fits on MPI_RANK " << mpi_rank - 1 << "\n";
            std::cout.flush();
        } else {
            FIRST_STATEVECTOR_ELEMENT = previous_LAST_ELEMENT + 1;
            LAST_STATEVECTOR_ELEMENT = FIRST_STATEVECTOR_ELEMENT + MAX_VECTOR_LEN;
            if (LAST_STATEVECTOR_ELEMENT < STATEVECTOR_LEN) {
                throw std::runtime_error("Too many qubits for the allocated resources.");
            } else {
                std::cout << "The MPI_RANK " << mpi_rank << " will simulate the statevector elements from " << FIRST_STATEVECTOR_ELEMENT + 1 << " to " << LAST_STATEVECTOR_ELEMENT + 1 << "\n";
                std::cout.flush();
                statevector.resize(STATEVECTOR_LEN - FIRST_STATEVECTOR_ELEMENT);
            }
        }
    }

}


int MPIExecutor::apply_measure(std::vector<int>& qubits) 
{
        meas_out meas =  MPIInstruction::apply_measure(statevector, qubits);

    return meas.measure;
}


void MPIExecutor::apply_unitary(Matrix& matrix, std::vector<int>& qubits)
{
    MPIInstruction::apply_unitary(matrix, statevector, qubits);
}


void MPIExecutor::apply_gate(const std::string& gate_name, std::vector<int>& qubits)
{
    MPIInstruction::apply_instruction(statevector, gate_name, qubits);
}


void MPIExecutor::apply_parametric_gate(const std::string& gate_name, std::vector<int>& qubits, std::vector<double>& param)
{
    MPIInstruction::apply_param_instruction(statevector, gate_name, qubits, param);
}


int MPIExecutor::get_nonzero_position()
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


void MPIExecutor::restart_statevector()
{
    statevector.assign(statevector.size(), {0.0, 0.0});
    if (mpi_rank == 0) {
        statevector[0] = 1.0;
    }
}