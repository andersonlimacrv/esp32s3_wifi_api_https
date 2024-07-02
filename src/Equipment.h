#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <vector>

// Definition of a structure to store equipment IDs
struct Equipment {
    int condensadorId;
    std::vector<int> bombaIds;
    std::vector<int> ventiladorIds;

    Equipment() {
        // Initialization of equipment IDs
        condensadorId = 1;
        bombaIds = {1, 2}; // Pump IDs
        ventiladorIds = {1, 2}; // Fan IDs
    }
};

// Template function to ensure unique initialization
template<typename T>
struct Singleton {
    static T& instance() {
        static T instance;
        return instance;
    }
};

// Definition of a global instance of the Equipment structure
#define equipment Singleton<Equipment>::instance()

#endif // EQUIPMENT_H
