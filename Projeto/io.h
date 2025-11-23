#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "colaborador.h" // Necessário para a estrutura Colaborador
#include "calendario.h" // Necessário para TipoMarcacao

// --- Constante da chave de codificação (Declarada como 'extern') ---
extern const int CHAVE_CESAR;

// --- Funções de Cifra de César ---
std::string encriptar(const std::string& texto, int chave);
std::string desencriptar(const std::string& texto, int chave);

// --- Funções de Conversão de Tipo de Marcação ---
std::string tipoParaString(TipoMarcacao tipo);
TipoMarcacao stringParaTipo(const std::string& str);

// --- Funções de Ficheiros (Guardar e Carregar) ---
void guardarDados(const std::vector<Colaborador>& colaboradores, const std::string& nomeFicheiro, const std::string& chave = "");
void carregarDados(std::vector<Colaborador>& colaboradores, const std::string& nomeFicheiro);

// Função para exportar dados 
void exportarDados(const std::vector<Colaborador>& colaboradores);

#endif // IO_H