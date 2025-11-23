#ifndef IO_H
#define IO_H

#include "colaborador.h" 
#include "calendario.h" // Necessário para TipoMarcacao
#include "reports.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector> // Inclua se usar std::vector, etc., no .h

// Chave de Cifra de César (definida em io.cpp)
extern const int CHAVE_CESAR; 

// Funções de Cifra
std::string encriptar(const std::string &texto, int chave);
std::string desencriptar(const std::string &texto, int chave);

// Funções Auxiliares de Serialização/Desserialização
std::string tipoParaString(TipoMarcacao tipo);
TipoMarcacao stringParaTipo(const std::string& str);
std::string getNextToken(std::stringstream& ss, char delimitador);

// Funções de Persistência
void guardarDados(const std::vector<Colaborador>& lista, const std::string& nomeFicheiro);
void carregarDados(std::vector<Colaborador>& lista, const std::string& nomeFicheiro);

#endif // IO_H