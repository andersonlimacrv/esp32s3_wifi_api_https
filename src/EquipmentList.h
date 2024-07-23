#ifndef EQUIPMENTLIST_H
#define EQUIPMENTLIST_H

#include <vector>

// Definition of a structure to store equipment IDs
struct Equipment {
    int unidadeId;
    int condensadorId;
    int condensacaoId;
    std::vector<int> bombaIds;
    std::vector<int> ventiladorIds;
    std::vector<int> ambienteIds;
    std::vector<int> compressorIds;
    std::vector<int> regimeIds;

};

// Template function to ensure unique initialization
template<typename T>
struct Singleton {
    static T& instance() {
        static T instance;
        return instance;
    }
};

// Declaration of a global instance of the Equipment structure
extern Equipment& equipment;

#endif // EQUIPMENTLIST_H
