#pragma once

#include <complex>
#include <unordered_map>

inline std::complex<double> imag(0.0, 1.0);

inline double inverse_sqrt_2 = 1.0 / std::sqrt(2.0);

inline const std::vector<std::string> instructions = {"unitary", "measure", "id", "x", "y", "z", "h", "sx", "rx", "ry", "rz", "cx", "cy", "cz", "crx", "cry", "crz", "ecr", "c_if_x","c_if_y", "c_if_z", "c_if_h", "c_if_sx", "c_if_rx", "c_if_ry", "c_if_rz", "c_if_cx", "c_if_cy", "c_if_cz"};

enum Instructions {
    unitary,
    measure,
    id,    
    x,  
    y,
    z,
    h,
    sx,
    rx,
    ry,
    rz,
    cx,
    cy,
    cz,
    crx,
    cry,
    crz,
    ecr,
    c_if_x,
    c_if_y,
    c_if_z,
    c_if_h,
    c_if_sx,
    c_if_rx,
    c_if_ry,
    c_if_rz,
    c_if_cx,
    c_if_cy,
    c_if_cz,
    c_if_ecr,
};

inline std::unordered_map<std::string, int> instructions_map = {
    {"unitary", unitary},
    {"measure", measure},
    {"id", id},
    {"x", x},
    {"y", y},
    {"z", z},
    {"h", h},
    {"sx", sx},
    {"rx", rx},
    {"ry", ry},
    {"rz", rz},
    {"cx", cx},
    {"cy", cy},
    {"cz", cz},
    {"crx", crx},
    {"cry", cry},
    {"crz", crz},
    {"ecr", ecr},
    {"c_if_x", c_if_x},
    {"c_if_y", c_if_y},
    {"c_if_z", c_if_z},
    {"c_if_h", c_if_h},
    {"c_if_sx", c_if_sx},
    {"c_if_rx", c_if_rx},
    {"c_if_ry", c_if_ry},
    {"c_if_rz", c_if_rz},
    {"c_if_cx", c_if_cx},
    {"c_if_cy", c_if_cy},
    {"c_if_cz", c_if_cz},
    {"c_if_ecr", c_if_ecr},
};