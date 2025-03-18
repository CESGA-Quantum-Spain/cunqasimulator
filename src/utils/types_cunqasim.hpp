#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using QuantumCircuit = std::vector<json>; 
using DistributedQuantumCircuit = std::vector<json>;
using complex = std::complex<double>;
using StateVector = std::vector<complex>;
using Params = std::vector<double>;

struct meas_out {
    StateVector statevector;
    int measure;
};

struct Counts {
    std::vector<int> counts_list;
    json counts;
};

#ifdef QPU_ZMQ
#include "zmq.hpp"

struct ZMQSockets
{
    zmq::socket_type::client client;
    zmq::socket_type::server server;
    std::string zmq_endpoint;
};
#endif