#ifndef PAYLOAD_FORMATTER_H
#define PAYLOAD_FORMATTER_H

#include <string>

std::string format_payload_login(const std::string& userName, const std::string& password);
std::string format_payload_ambiente(const std::string& dataHora, int temperaturaAtual, int ambienteId);
std::string format_payload_bomba_condensador(bool ligado, const std::string& dataHora, float corrente, int frequencia, int bombaId);
std::string format_payload_condensador(bool ligado, const std::string& dataHora, float temperaturaEntrada, float umidadeRelativaEntrada, float temperaturaSaida, float umidadeRelativaSaida, float velocidadeArEntrada, float corrente, int frequencia, int condensadorId);
std::string format_payload_ventilador_condensador(const std::string& dataHora, float corrente, int frequencia, int ventiladorId);

#endif