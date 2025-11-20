
#include "colaborador.h" // Já inclui calendario.h
#include "calendario.h"
#include "io.h"
#include "reports.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

const std::string COR_VERDE = "\033[32m"; // Verde (para Sucesso/Confirmações)
const std::string COR_AMARELA = "\033[33m";       // Amarelo (Serve para avisos)
const std::string COR_VERMELHA = "\033[31m";    // Vermelho (serve para erros)
const std::string RESET_COR = "\033[0m";    // Reseta cor/estilo para o padrão

// --- Funções de Cifra ---

// Função simples de encriptação (Cifra de César)
std::string encriptar(const std::string& texto, int chave) {
    std::string resultado = texto;

    for (size_t i = 0; i < texto.size(); ++i) {
        char c = texto[i];

        if (std::isalpha(c)) { // apenas letras
            char base = std::isupper(c) ? 'A' : 'a';
            resultado[i] = char((c - base + chave) % 26 + base);
        } else {
            resultado[i] = c; // mantém espaços e pontuação
        }
    }
    return resultado;
}

// Função de desencriptação (reverte o processo)
std::string desencriptar(const std::string &texto, int chave) {
    return encriptar(texto, 26 - (chave % 26)); // desloca no sentido inverso
}
// --- Funções de Persistência ---

// Formato de gravação por linha: NOME_CIFRADO|DiaDoAno:Tipo,DiaDoAno:Tipo,...
void guardarDados(const std::vector<Colaborador>& lista, const std::string& nomeFicheiro) {
    std::ofstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cerr << COR_VERMELHA << "ERRO: Nao foi possivel abrir o ficheiro '" << nomeFicheiro << "' para escrita.\n" << RESET_COR;
        return;
    }

    for (const auto& colab : lista) {
        // 1. Grava o Nome Cifrado
        ficheiro << encriptar(colab.nome, CHAVE_CESAR) << "|";

        std::string calendario_str;
        for (const auto& par : colab.calendario) {
            // par.first = diaDoAno (int); par.second = TipoMarcacao (enum)
            char tipo_char = (par.second == FERIAS) ? 'F' : 'X';
            calendario_str += std::to_string(par.first) + ":" + std::string(1, tipo_char) + ",";
        }
        if (!calendario_str.empty()) {
            // Remove a última vírgula
            calendario_str.pop_back();
        }
        ficheiro << calendario_str << "\n";
    }
    ficheiro.close();
    std::cout << COR_VERDE << "Dados guardados em '" << nomeFicheiro << "' (" << lista.size() << " colaboradores).\n" << RESET_COR;
}

void carregarDados(std::vector<Colaborador>& lista, const std::string& nomeFicheiro) {
    std::ifstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cout << COR_AMARELA << "AVISO: Ficheiro de dados '" << nomeFicheiro << "' nao encontrado. Iniciando sistema vazio.\n" << RESET_COR;
        return;
    }

    std::string linha;
    while (std::getline(ficheiro, linha)) {
        if (linha.empty()) continue;

        Colaborador novoColab;
        std::stringstream ss(linha);
        std::string parte;

        // Ler Nome Cifrado e dados do Calendário (separados por '|')
        if (std::getline(ss, parte, '|')) {
            novoColab.nome = desencriptar(parte, CHAVE_CESAR);
        } else {
            // Se não houver '|', assume que a linha é apenas o nome cifrado (sem calendário)
            novoColab.nome = desencriptar(linha, CHAVE_CESAR);
            lista.push_back(novoColab);
            continue;
        }

        // Ler e processar o Calendário (string de marcações)
        std::string calendario_str;
        if (std::getline(ss, calendario_str)) {
            std::stringstream ss_cal(calendario_str);
            std::string marcacao;
            while (std::getline(ss_cal, marcacao, ',')) {
                if (marcacao.empty()) continue;

                size_t pos_dois_pontos = marcacao.find(':');
                if (pos_dois_pontos != std::string::npos) {
                    try {
                        int diaDoAno = std::stoi(marcacao.substr(0, pos_dois_pontos));
                        char tipo_char = marcacao[pos_dois_pontos + 1];

                        TipoMarcacao tipo = LIVRE;
                        if (tipo_char == 'F') tipo = FERIAS;
                        else if (tipo_char == 'X') tipo = FALTA;

                        if (tipo != LIVRE) {
                            novoColab.calendario[diaDoAno] = tipo;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << COR_VERMELHA << "ERRO de parsing em linha: " << linha << std::endl << RESET_COR;
                    }
                }
            }
        }
        lista.push_back(novoColab);
    }
    ficheiro.close();
    std::cout << COR_VERDE << "Dados de '" << nomeFicheiro << "' carregados com sucesso (" << lista.size() << " colaboradores).\n" << RESET_COR;
}