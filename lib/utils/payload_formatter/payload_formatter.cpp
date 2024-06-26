#include "payload_formatter.h"
#include <cstdio>

std::string format_payload_login(const std::string& userName, const std::string& password) {
    char json[512];
    snprintf(json, sizeof(json), "{\"userName\": \"%s\",\"password\": \"%s\"}", userName.c_str(), password.c_str());
    return std::string(json);
}

std::string format_payload_ambiente(const std::string& dataHora, int temperaturaAtual, int ambienteId) {
    char json[512];
    snprintf(json, sizeof(json), "{\"dataHora\": \"%s\",\"temperaturaAtual\": %d,\"ambienteId\": %d}", dataHora.c_str(), temperaturaAtual, ambienteId);
    return std::string(json);
}

std::string format_payload_bomba_condensador(bool ligado, const std::string& dataHora, float corrente, int frequencia, int bombaId) {
    char json[512];
    snprintf(json, sizeof(json), "{ \"ligado\": %s, \"dataHora\": \"%s\",\"corrente\": %f,\"frequencia\": %d,\"bombaId\": %d}", dataHora.c_str(), corrente, frequencia, bombaId);
    return std::string(json);
}

std::string format_payload_condensador(bool ligado, const std::string& dataHora, float temperaturaEntrada, float umidadeRelativaEntrada, float temperaturaSaida, float umidadeRelativaSaida, float velocidadeArEntrada, float corrente, int frequencia, int condensadorId) {
    char json[512];
    printf("CondensadorId: %d\n", condensadorId);
    printf("Ligado: %s\n", ligado ? "true" : "false");
    printf("Corrente: %f\n", corrente);
    printf("Frequencia: %d\n", frequencia);
    printf("Data: %s\n", dataHora.c_str());
    printf("TemperaturaEntrada: %f\n", temperaturaEntrada);
    printf("UmidadeRelativaEntrada: %f\n", umidadeRelativaEntrada);
    printf("TemperaturaSaida: %f\n", temperaturaSaida);
    printf("UmidadeRelativaSaida: %f\n", umidadeRelativaSaida);
    printf("VelocidadeArEntrada: %f\n", velocidadeArEntrada);
    snprintf(json, sizeof(json), "{\"ligado\": %s,\"dataHora\": \"%s\",\"temperaturaEntrada\": %.2f,\"umidadeRelativaEntrada\": %.2f,\"temperaturaSaida\": %.2f,\"umidadeRelativaSaida\": %.2f,\"velocidadeArEntrada\": %f,\"corrente\": %f,\"frequencia\": %d,\"condensadorId\": %d}",
             ligado ? "true" : "false",
             dataHora.c_str(), temperaturaEntrada, umidadeRelativaEntrada, temperaturaSaida, umidadeRelativaSaida, velocidadeArEntrada, corrente, frequencia, condensadorId);
    return std::string(json);
}



std::string format_payload_ventilador_condensador(const std::string& dataHora, float corrente, int frequencia, int ventiladorId) {
    char json[512];
    snprintf(json, sizeof(json), "{\"dataHora\": \"%s\",\"corrente\": %f,\"frequencia\": %d,\"ventiladorId\": %d}", dataHora.c_str(), corrente, frequencia, ventiladorId);
    return std::string(json);
}
