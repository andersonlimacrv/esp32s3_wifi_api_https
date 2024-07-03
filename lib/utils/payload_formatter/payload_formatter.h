#ifndef PAYLOAD_FORMATTER_H
#define PAYLOAD_FORMATTER_H

#include <string>

std::string format_payload_login(const std::string& userName, const std::string& password);
std::string format_payload_ambiente(const std::string& dataHora, float temperaturaAtual, int ambienteId);
std::string format_payload_bomba_condensador(bool ligado, const std::string& dataHora, float corrente, int frequencia, int bombaId);
std::string format_payload_condensador(bool ligado, const std::string& dataHora, float temperaturaEntrada, float umidadeRelativaEntrada, float temperaturaSaida, float umidadeRelativaSaida, float velocidadeArEntrada, float corrente, int frequencia, int condensadorId);
std::string format_payload_ventilador_condensador(bool ligado, const std::string& dataHora, float corrente, int frequencia, int ventiladorId);
std::string format_payload_compressor(bool ligado, const std::string& dataHora, float pressaoSuccao, float pressaoDescarga, float pressaoOleoReservatorio, float pressaoOleoAposFiltro, float temperaturaSuccao, float temperaturaDescarga, float temperaturaOleo, float corrente, int horas, int frequencia, int slide, int compressorId);
std::string format_payload_regime(const std::string& dataHora, float pressaoAtual, int regimeId);

#endif
