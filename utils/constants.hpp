#pragma once

#include <complex>
#include <unordered_map>

std::complex<double> imag(0.0, 1.0);

const std::vector<std::string> instructions = {"measure", "id", "h", "x", "y", "z", "cx", "cy", "cz", "ecr", "c_if_x","c_if_y","c_if_z","c_if_rx","c_if_ry","c_if_rz","c_if_cx","c_if_cy","c_if_cz"};

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
    ecr,
    c_if_h,
    c_if_x,
    c_if_y,
    c_if_z,
    c_if_rx,
    c_if_ry,
    c_if_rz,
    c_if_cx,
    c_if_cy,
    c_if_cz,
    c_if_ecr,
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
    {"ecr", ecr},
    {"c_if_h", c_if_h},
    {"c_if_x", c_if_x},
    {"c_if_y", c_if_y},
    {"c_if_z", c_if_z},
    {"c_if_rx", c_if_rx},
    {"c_if_ry", c_if_ry},
    {"c_if_rz", c_if_rz},
    {"c_if_cx", c_if_cx},
    {"c_if_cy", c_if_cy},
    {"c_if_cz", c_if_cz},
    {"c_if_ecr", c_if_ecr},
};