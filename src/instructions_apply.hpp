#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include <array>
#include <cmath>
#include <random>
#include <chrono>

#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"

#include "logger/logger.hpp"


//Measure
inline meas_out apply_measure(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out output;
    bool zero;
    double prob_0 = 0;
    double prob_1 = 0;
    std::vector<int> index_0;
    std::vector<int> index_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero) {
            index_0.push_back(i);
            prob_0 = prob_0 + std::norm(statevector[i]);
        } else {
            index_1.push_back(i);
            prob_1 = prob_1 + std::norm(statevector[i]);
        }
    }

    std::random_device rd;  // Seed source
    std::mt19937 gen(rd() ^ std::chrono::steady_clock::now().time_since_epoch().count()); // Mersenne Twister RNG
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
inline StateVector apply_h(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = inverse_sqrt_2 * aux_statevector[i] + inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = -inverse_sqrt_2 * aux_statevector[i] + inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])];
        }
    }

    return statevector;
}


inline StateVector apply_x(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;

    for (int i = 0; i < statevector.size(); i++) {
        statevector[i] = aux_statevector[flipbit(i, qubits[0])];
    }

    return statevector;
}


inline StateVector apply_y(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = imag * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = -imag * aux_statevector[flipbit(i, qubits[0])];
        }
    }

    return statevector;
}


inline StateVector apply_z(StateVector& statevector, std::array<int, 3> qubits)
{
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == false) { 
            statevector[i] = -statevector[i]; 
        }
    }

    return statevector;
}

// Two-Qubit Gates

inline StateVector apply_cx(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (!zero) {
            statevector[i] = aux_statevector[flipbit(i, qubits[1])];
        }
    }


    return statevector;
}

inline StateVector apply_cy(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, qubits[1]);
            if (zero_1) { 
                statevector[i] = imag * aux_statevector[flipbit(i, qubits[1])];
            } else {
                statevector[i] = -imag * aux_statevector[flipbit(i, qubits[1])];
            }
        }
    }

    return statevector;
}

inline StateVector apply_cz(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, qubits[1]);
            if (!zero_1) { 
                statevector[i] = -statevector[i]; 
            }
        }
    }

    return statevector;
}

inline StateVector apply_ecr(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        zero_1 = is_zero(i, qubits[1]);
        if (zero_0  && zero_1) {
            statevector[i] = inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])] + imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
        } else if (zero_0 && !zero_1) {
            statevector[i] = imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else if (!zero_0 && zero_1) {
            statevector[i] = inverse_sqrt_2 * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])] - imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
        } else if (!zero_0 && !zero_1) {
            statevector[i] = -imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}

//Classical conditional one-qubit gates
inline StateVector apply_cifh(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_h(statevector, {qubits[0]});
    }

    return statevector;
}

inline StateVector apply_cifx(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_x(statevector, {qubits[0]});
    }

    return statevector;
}

inline StateVector apply_cify(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_y(statevector, {qubits[0]});
    }

    return statevector;
}

inline StateVector apply_cifz(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_z(statevector, {qubits[0]});
    }

    return statevector;
}

//Classical conditional two-qubits gates
inline StateVector apply_cifcx(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_cx(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}

inline StateVector apply_cifcy(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_cy(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}

inline StateVector apply_cifcz(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_cz(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}

inline StateVector apply_cifecr(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_ecr(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}


inline StateVector apply_rx(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] - imag * sin * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  - imag * sin * aux_statevector[flipbit(i, qubits[0])];
        }
    }


    return statevector;
}


inline StateVector apply_ry(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] + sin * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  +  sin * aux_statevector[flipbit(i, qubits[0])];
        }
    }

    return statevector;
}


inline StateVector apply_rz(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    bool zero;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero) { 
            statevector[i] = (cos - imag * sin) * statevector[i];
        } else {
            statevector[i] = (cos + imag * sin) * statevector[i];
        }
    }

    return statevector;
}



inline StateVector apply_cifrx(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_rx(statevector, {qubits[0]}, param);
    }

    return statevector;
}

inline StateVector apply_cifry(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_ry(statevector, {qubits[0]}, param);
    }

    return statevector;
}

inline StateVector apply_cifrz(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_rz(statevector, {qubits[0]}, param);
    }

    return statevector;
}



//DISTRIBUTED GATES (comm_qpus[0]->send, comm_qpus[1]->recv)
//Distributed classical conditional one-qubit gates
#if defined(QPU_MPI) 
inline StateVector apply_dcifh(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        MPI_Send(&measurement, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_h(statevector, {qubits[1], -1, -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifx(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_x(statevector, {qubits[1], -1, -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcify(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_y(statevector, {qubits[1], -1, -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifz(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_z(statevector, {qubits[1], -1, -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifrx(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus, Params& param)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_rx(statevector, {qubits[1], -1, -1}, param);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifry(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus, Params& param)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_ry(statevector, {qubits[1], -1, -1}, param);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifrz(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus, Params& param)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_rz(statevector, {qubits[1], -1, -1}, param);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

//Distributed classical conditional two-qubit gates
inline StateVector apply_dcifcx(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_cx(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifcy(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_cy(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifcz(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_cz(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifecr(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    int mpi_rank = get_mpi_rank();
    
    if (mpi_rank == comm_qpus[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});

        MPI_Send(&meas.measure, 1, MPI_INT, comm_qpus[1], 1, MPI_COMM_WORLD);

    } else if (mpi_rank == comm_qpus[1]) {
        int measurement;
        MPI_Recv(&measurement, 1, MPI_INT, comm_qpus[0], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (measurement == 1) {
            statevector = apply_ecr(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus[0] << " nor " << comm_qpus[1] << "\n"; 
    }

    return statevector;

}

//DISTRIBUTED GATES (comm_qpus[0]->send, comm_qpus[1]->recv)
#elif defined(QPU_ZMQ)
inline StateVector apply_dcifh(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_h(statevector, {qubits[1], -1, -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifx(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    SPDLOG_LOGGER_DEBUG(logger, "apply_dcifx.");
    SPDLOG_LOGGER_DEBUG(logger, "my_endpoint: {}", comm_qpus.my_endpoint);
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;
        SPDLOG_LOGGER_DEBUG(logger, "Measurement.");

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);
        SPDLOG_LOGGER_DEBUG(logger, "Client connected.");

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));
        SPDLOG_LOGGER_DEBUG(logger, "Message prepared.");

        comm_qpus.client.send(message);
        SPDLOG_LOGGER_DEBUG(logger, "Message sent.");


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);
        SPDLOG_LOGGER_DEBUG(logger, "Message recieved.");

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));
        SPDLOG_LOGGER_DEBUG(logger, "Measurement prepared.");

        if (measurement == 1) {
            statevector = apply_x(statevector, {qubits[1], -1, -1});
            SPDLOG_LOGGER_DEBUG(logger, "apply_x.");
        }

    } else {
        //TODO: Logger
        SPDLOG_LOGGER_ERROR(logger, "Error! This QPU has no Id {} nor {}", comm_qpus.comm_endpoints[0], comm_qpus.comm_endpoints[1]);
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcify(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_y(statevector, {qubits[1], -1, -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifz(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_z(statevector, {qubits[1], -1, -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifrx(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus, Params& param)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_rx(statevector, {qubits[1], -1, -1}, param);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifry(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus, Params& param)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_ry(statevector, {qubits[1], -1, -1}, param);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifrz(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus, Params& param)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_rz(statevector, {qubits[1], -1, -1}, param);
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

//Distributed classical conditional two-qubit gates
inline StateVector apply_dcifcx(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_cx(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifcy(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_cy(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifcz(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_cz(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

inline StateVector apply_dcifecr(StateVector& statevector, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[0]) {
        meas_out meas = apply_measure(statevector, {qubits[0], -1, -1});
        int measurement = meas.measure;

        comm_qpus.client.connect(comm_qpus.comm_endpoints[1]);

        zmq::message_t message(sizeof(int));
        std::memcpy(message.data(), &measurement, sizeof(int));

        comm_qpus.client.send(message);


    } else if (comm_qpus.my_endpoint == comm_qpus.comm_endpoints[1]) {
        zmq::message_t message;
        comm_qpus.server.recv(message);

        int measurement;
        std::memcpy(&measurement, message.data(), sizeof(int));

        if (measurement == 1) {
            statevector = apply_ecr(statevector, {qubits[1], qubits[2], -1});
        }

    } else {
        //TODO: Logger
        std::cout << "Error! This QPU has no Id " << comm_qpus.comm_endpoints[0] << " nor " << comm_qpus.comm_endpoints[1] << "\n"; 
    }

    return statevector;

}

#else

    std::cout << "In CunqaSimulator: No QPU-communication MACRO defined." << "\n"; 

#endif