#pragma once

#include <complex>
#include <unordered_map>

#include "types_cunqasim.hpp"

#if defined(USE_DOUBLE_PRECISION_MACRO)
    inline double ONE = 1.0;
    inline double ZERO = 0.0;
#elif defined(USE_FLOAT_PRECISION_MACRO)
    inline float ONE = 1.0f;
    inline float ZERO = 0.0f;
#endif


inline std::complex<Precision> imag(0.0, 1.0);

inline Precision inverse_sqrt_2 = 1.0 / std::sqrt(2.0);

inline const std::vector<std::string> instructions = {"gate", "parametric_gate", "measure", "id", "x", "y", "z", "h", "sx", "rx", "ry", "rz", "swap", "cx", "cy", "cz", "crx", "cry", "crz", "ecr", "unitary", "c_if_x","c_if_y", "c_if_z", "c_if_h", "c_if_sx", "c_if_rx", "c_if_ry", "c_if_rz", "c_if_cx", "c_if_cy", "c_if_cz", "c_if_unitary"};

enum Instructions {
    gate,
    parametric_gate,
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
    swap,
    cx,
    cy,
    cz,
    crx,
    cry,
    crz,
    ecr,
    unitary,
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
    c_if_unitary
};

inline std::unordered_map<std::string, int> instructions_map = {
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
    {"swap", swap},
    {"cx", cx},
    {"cy", cy},
    {"cz", cz},
    {"crx", crx},
    {"cry", cry},
    {"crz", crz},
    {"ecr", ecr},
    {"unitary", unitary},
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
    {"c_if_unitary", c_if_unitary},
};

inline std::unordered_map<std::string, int> case_map = {
    {"measure", measure},
    {"id", gate},
    {"x", gate},
    {"y", gate},
    {"z", gate},
    {"h", gate},
    {"sx", gate},
    {"rx", parametric_gate},
    {"ry", parametric_gate},
    {"rz", parametric_gate},
    {"swap", gate},
    {"cx", gate},
    {"cy", gate},
    {"cz", gate},
    {"crx", parametric_gate},
    {"cry", parametric_gate},
    {"crz", parametric_gate},
    {"ecr", gate},
    {"unitary", unitary},
    {"c_if_x", gate},
    {"c_if_y", gate},
    {"c_if_z", gate},
    {"c_if_h", gate},
    {"c_if_sx", gate},
    {"c_if_rx", parametric_gate},
    {"c_if_ry", parametric_gate},
    {"c_if_rz", parametric_gate},
    {"c_if_cx", parametric_gate},
    {"c_if_cy", parametric_gate},
    {"c_if_cz", parametric_gate},
    {"c_if_ecr", gate},
    {"c_if_unitary", c_if_unitary}
};