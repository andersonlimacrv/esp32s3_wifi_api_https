#include "payload_formatter.h"


std::string format_payload_login(const std::string& username, const std::string& password) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"username\": \"%s\",\n\"password\": \"%s\"\n}", username.c_str(), password.c_str());
    return std::string(json);
}

std::string format_payload_ambiente(const std::string& dataHora, float temperaturaAtual, int ambienteId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"dataHora\": \"%s\",\n\"temperaturaAtual\": %.2f,\n\"ambienteId\": %d\n}", dataHora.c_str(), temperaturaAtual, ambienteId);
    return std::string(json);
}

std::string format_payload_chiller(bool ligado, const std::string& dataHora, float temperaturaEntrada, int chillerId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"ligado\": %s,\n\"dataHora\": \"%s\",\n\"temperaturaEntrada\": %.2f,\n\"chillerId\": %d\n}", ligado ? "true" : "false", dataHora.c_str(), temperaturaEntrada, chillerId);
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
    snprintf(json, sizeof(json), "{ \n\"ligado\": %s,\n\"dataHora\": \"%s\",\n\"corrente\": %f,\n\"frequencia\": %d,\n\"ventiladorId\": %d\n}", ligado ? "true" : "false", dataHora.c_str(), corrente, frequencia, ventiladorId);
    return std::string(json);
}

std::string format_payload_compressor(bool ligado, const std::string& dataHora, float pressaoSuccao, float pressaoDescarga, float pressaoOleoReservatorio, float pressaoOleoAposFiltro, float temperaturaSuccao, float temperaturaDescarga, float temperaturaOleo, float corrente, int horas, int frequencia, int slider, int compressorId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"ligado\": %s,\n\"dataHora\": \"%s\", \n\"pressaoSuccao\": %f,\n\"pressaoDescarga\": %f,\n\"pressaoOleoReservatorio\": %f,\n\"pressaoOleoAposFiltro\": %f,\n\"temperaturaSuccao\": %f,\n\"temperaturaDescarga\": %f,\n\"temperaturaOleo\": %f,\n\"corrente\": %f,\n\"horas\": %d,\n\"frequencia\": %d,\n\"slide\": %d,\n\"compressorId\": %d\n}",
             ligado ? "true" : "false",
             dataHora.c_str(), pressaoSuccao, pressaoDescarga, pressaoOleoReservatorio, pressaoOleoAposFiltro, temperaturaSuccao, temperaturaDescarga, temperaturaOleo, corrente, horas, frequencia, slider, compressorId);
    return std::string(json);
}

std::string format_payload_regime(const std::string& dataHora, float pressaoAtual, int regimeId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"dataHora\": \"%s\",\n\"pressaoAtual\": %f,\n\"regimeId\": %d\n}", dataHora.c_str(), pressaoAtual, regimeId);
    return std::string(json);
}

std::string format_payload_condensacao(const std::string& dataHora, float pressaoAtual, int regimeId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"dataHora\": \"%s\",\n\"pressaoAtual\": %f,\n\"regimeId\": %d\n}", dataHora.c_str(), pressaoAtual, regimeId);
    return std::string(json);
}

std::string format_payload_energia(bool ligado, const std::string& dataHora, float demandaAtiva, float demandaReativa, float fatorPotencia, float consumoAtivoTotal, float consumoReativoTotal, float consumoAtivoForaDePontaIndutivo, float consumoAtivoForaDePontaCapacitivo, float consumoAtivoPonta, int periodoMedicao, int unidadeId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"ligado\": %s,\n\"dataHora\": \"%s\", \n\"demandaAtiva\": %f,\n\"demandaReativa\": %f,\n\"fatorPotencia\": %f,\n\"consumoAtivoTotal\": %f,\n\"consumoReativoTotal\": %f,\n\"consumoAtivoForaDePontaIndutivo\": %f,\n\"consumoAtivoForaDePontaCapacitivo\": %f,\n\"consumoAtivoPonta\": %f,\n\"periodoMedicao\": %d,\n\"unidadeId\": %d\n}",
             ligado ? "true" : "false",
             dataHora.c_str(), demandaAtiva, demandaReativa, fatorPotencia, consumoAtivoTotal, consumoReativoTotal, consumoAtivoForaDePontaIndutivo, consumoAtivoForaDePontaCapacitivo, consumoAtivoPonta, periodoMedicao, unidadeId);
    return std::string(json);
}

std::string format_payload_linha_liquido(const std::string& dataHora, float temperatura, int unidadeId) {
    char json[512];
    snprintf(json, sizeof(json), "{\n\"dataHora\": \"%s\",\n\"temperatura\": %f,\n\"unidadeId\": %d\n}",
             dataHora.c_str(), temperatura, unidadeId);
    return std::string(json);
}