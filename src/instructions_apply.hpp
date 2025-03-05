#pragma once

#include <vector>
#include <complex>
#include <array>
#include <cmath>
#include <random>
#include <mpi.h>

#include "../utils/utils.hpp"
#include "../utils/constants.hpp"
#include "../utils/types.hpp"


int get_mpi_rank()
{
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    return mpi_rank;
}

//Measure
meas_out apply_measure(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    meas_out output;
    bool zero;
    double prob_0 = 0;
    double prob_1 = 0;
    std::vector<int> index_0((int)statevector.size()/2);
    std::vector<int> index_1((int)statevector.size()/2);
    


    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero) {
            index_0.push_back(i);
            prob_0 = prob_0 + std::norm(statevector[i]);
        } else {
            index_1.push_back(i);
            prob_1 = prob_1 + std::norm(statevector[i]);
        }
    }

    std::random_device rd;  // Seed source
    std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::discrete_distribution<int> dist({prob_0, prob_1});
    int sample = dist(gen);
    output.measure = sample;
    auto it0 = index_0.begin();
    auto it1 = index_1.begin();

    //TODO: The following is parallelizable
    switch (sample)
    {
        case 0:
            for (; it0 != index_0.end() && it1 != index_1.end(); it0++, it1++) {
                statevector[*it0] = (1.0/std::sqrt((1 - prob_1))) * statevector[*it0];
                statevector[*it1] = 0.0;
            }
            break;
        case 1:
        for (; it0 != index_0.end() && it1 != index_1.end(); it0++, it1++) {
            statevector[*it0] = 0.0;
            statevector[*it1] = (1.0/std::sqrt((1 - prob_0))) * statevector[*it1];
        }
            break;
    }

    output.statevector = statevector;

    return output;
}

// One-Qubit Gates
StateVector apply_h(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;
    double ampl = 1.0 / std::sqrt(2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = ampl * aux_statevector[i] + ampl * aux_statevector[flipbit(i, n_qubits - 1 - qubits[0])];
        } else {
            statevector[i] = -ampl * aux_statevector[i] + ampl * aux_statevector[flipbit(i, n_qubits - 1 - qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_x(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;

    for (int i = 0; i < statevector.size(); i++) {
        statevector[i] = aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
    }

    return statevector;
}


StateVector apply_y(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = imag * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else {
            statevector[i] = -imag * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_z(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == false) { 
            statevector[i] = -statevector[i]; 
        }
    }

    return statevector;
}


StateVector apply_rx(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param/2.0);
    double cos = std::cos(param/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] - imag * sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  - imag * sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_ry(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param/2.0);
    double cos = std::cos(param/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] + sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  +  sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_rz(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    bool zero;
    double sin = std::sin(param/2.0);
    double cos = std::cos(param/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero) { 
            statevector[i] = (cos - imag * sin) * statevector[i];
        } else {
            statevector[i] = (cos + imag * sin) * statevector[i];
        }
    }

    return statevector;
}


// Two-Qubit Gates

StateVector apply_cx(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (!zero) {
            statevector[i] = aux_statevector[flipbit(i, n_qubits - 1 - qubits[1])];
        }
    }


    return statevector;
}

StateVector apply_cy(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, n_qubits - 1 - qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, n_qubits - 1 - qubits[1]);
            if (zero_1) { 
                statevector[i] = imag * aux_statevector[flipbit(i, n_qubits - 1 - qubits[1])];
            } else {
                statevector[i] = -imag * aux_statevector[flipbit(i, n_qubits - 1 - qubits[1])];
            }
        }
    }

    return statevector;
}

StateVector apply_cz(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, n_qubits - 1 - qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, n_qubits - 1 - qubits[1]);
            if (!zero_1) { 
                statevector[i] = -statevector[i]; 
            }
        }
    }

    return statevector;
}

StateVector apply_ecr(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    double ampl = 1.0 / std::sqrt(2.0);
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, n_qubits - 1 - qubits[0]);
        zero_1 = is_zero(i, n_qubits - 1 - qubits[1]);
        if (zero_0  && zero_1) {
            statevector[i] = ampl * aux_statevector[flipbit(i, n_qubits - 1 - qubits[0])] + imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])];
        } else if (zero_0 && !zero_1) {
            statevector[i] = imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])] + ampl * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else if (!zero_0 && zero_1) {
            statevector[i] = ampl * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])] - imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])];
        } else if (!zero_0 && !zero_1) {
            statevector[i] = -imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])] + ampl * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}

//Classical conditional one-qubit gates
StateVector apply_cifh(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_h(statevector, {qubits[0]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifx(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_x(statevector, {qubits[0]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cify(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_y(statevector, {qubits[0]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifz(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_z(statevector, {qubits[0]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifrx(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_rx(statevector, param, {qubits[0]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifry(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_ry(statevector, param, {qubits[0]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifrz(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_rz(statevector, param, {qubits[0]}, n_qubits);
    }

    return statevector;
}

//Classical conditional two-qubits gates
StateVector apply_cifcx(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_cx(statevector, {qubits[0], qubits[1]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifcy(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_cy(statevector, {qubits[0], qubits[1]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifcz(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_cz(statevector, {qubits[0], qubits[1]}, n_qubits);
    }

    return statevector;
}

StateVector apply_cifecr(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]}, n_qubits);
    if (meas.measure == 1) {
        statevector = apply_ecr(statevector, {qubits[0], qubits[1]}, n_qubits);
    }

    return statevector;
}

//DISTRIBUTED GATES (qpus[0]->send, qpus[1]->recv)
//Distributed classical conditional one-qubit gates
StateVector apply_dcifh(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_h(statevector, {qubits[1], -1, -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifx(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_x(statevector, {qubits[1], -1, -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcify(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_y(statevector, {qubits[1], -1, -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifz(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_z(statevector, {qubits[1], -1, -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifrx(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_rx(statevector, param, {qubits[1], -1, -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifry(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_ry(statevector, param, {qubits[1], -1, -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifrz(StateVector& statevector, double& param, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_rz(statevector, param, {qubits[1], -1, -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

//Distributed classical conditional two-qubit gates
StateVector apply_dcifcx(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_cx(statevector, {qubits[1], qubits[2], -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifcy(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_cy(statevector, {qubits[1], qubits[2], -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifcz(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_cz(statevector, {qubits[1], qubits[2], -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}

StateVector apply_dcifecr(StateVector& statevector, std::array<int, 3> qubits, int& n_qubits, std::array<int, 2>& qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1}, n_qubits);
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_ecr(statevector, {qubits[1], qubits[2], -1}, n_qubits);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << qpus[0] << " nor " << qpus[1] << "\n"; 
    }

    return statevector;
}