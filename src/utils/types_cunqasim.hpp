#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using QuantumCircuit = std::vector<json>; 
using DistributedQuantumCircuit = std::vector<json>;
using complex = std::complex<double>;
using StateVector = std::vector<complex>;
using Params = std::vector<double>;

#if defined(QPU_MPI)
        using type_comm = std::array<int, 2>;
        using qpu_comm_type = std::array<int, 2>;
#elif defined(QPU_ZMQ)
    #include "zmq.hpp"
    struct zmq_info
    {
        std::array<std::string, 2> comm_endpoints;
        zmq::socket_t client;
        zmq::socket_t server;
        std::string my_endpoint;
    };

        using type_comm = std::array<std::string, 2>;
        using qpu_comm_type = std::array<std::string, 2>;
#endif

struct meas_out {
    StateVector statevector;
    int measure;
};

struct Counts {
    std::vector<int> counts_list;
    json counts;
};

