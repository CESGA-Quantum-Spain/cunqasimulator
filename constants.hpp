#pragma once

#include <complex>
#include <unordered_map>

std::complex<double> imag(0.0, 1.0);

enum Instructions {
    measure,
    id,
    h,    
    x,  
    y,
    z,
    rx,
    ry,
    rz,
    cx,
    cy,
    cz,
    ecr
};

std::unordered_map<std::string, int> instructions_map = {
    {"measure", measure},
    {"id", id},
    {"h", h},
    {"x", x},
    {"y", y},
    {"z", z},
    {"rx", rx},
    {"ry", ry},
    {"rz", rz},
    {"cx", cx},
    {"cy", cy},
    {"cz", cz},
    {"ecr", ecr}
};