#include <unordered_map>


enum Gates {
    id,
    h,    
    x,  
    y,
    z,
    cx,
    cy,
    cz
};

std::unordered_map<std::string, int> gates_map = {
    {"id", id},
    {"h", h},
    {"x", x},
    {"y", y},
    {"z", z},
    {"cx", cx},
    {"cy", cy},
    {"cz", cz}
};