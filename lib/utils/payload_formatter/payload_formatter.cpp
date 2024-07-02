#include "payload_formatter.h"
#include <cstdio>

std::string format_payload_login(const std::string& userName, const std::string& password) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"userName\": \"%s\",\n\"password\": \"%s\"\n}", userName.c_str(), password.c_str());
    return std::string(json);
}

std::string format_payload_ambiente(const std::string& dataHora, int temperaturaAtual, int ambienteId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"dataHora\": \"%s\",\n\"temperaturaAtual\": %d,\n\"ambienteId\": %d\n}", dataHora.c_str(), temperaturaAtual, ambienteId);
    return std::string(json);
}

std::string format_payload_condensador(bool ligado, const std::string& dataHora, float temperaturaEntrada, float umidadeRelativaEntrada, float temperaturaSaida, float umidadeRelativaSaida, float velocidadeArEntrada, float corrente, int frequencia, int condensadorId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"ligado\": %s,\n\"dataHora\": \"%s\",\n\"temperaturaEntrada\": %.2f,\n\"umidadeRelativaEntrada\": %.2f,\n\"temperaturaSaida\": %.2f,\n\"umidadeRelativaSaida\": %.2f,\n\"velocidadeArEntrada\": %f,\n\"corrente\": %f,\n\"frequencia\": %d,\n\"condensadorId\": %d\n}",
             ligado ? "true" : "false",
             dataHora.c_str(), temperaturaEntrada, umidadeRelativaEntrada, temperaturaSaida, umidadeRelativaSaida, velocidadeArEntrada, corrente, frequencia, condensadorId);
    return std::string(json);
}

std::string format_payload_bomba_condensador(bool ligado, const std::string& dataHora, float corrente, int frequencia, int bombaId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"ligado\": %s,\n\"dataHora\": \"%s\",\n\"corrente\": %f,\n\"frequencia\": %d,\n\"bombaId\": %d\n}", ligado ? "true" : "false", dataHora.c_str(), corrente, frequencia, bombaId);
    return std::string(json);
}

std::string format_payload_ventilador_condensador(bool ligado, const std::string& dataHora, float corrente, int frequencia, int ventiladorId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"dataHora\": \"%s\",\n\"corrente\": %f,\n\"frequencia\": %d,\n\"ventiladorId\": %d\n}", ligado ? "true" : "false", dataHora.c_str(), corrente, frequencia, ventiladorId);
    return std::string(json);
}





