#include "io.h" 
#include "colaborador.h"
#include "calendario.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cctype>
#include <string>
#include <map>
#include "cores.h"

// Constante para a chave da Cifra de César
const int CHAVE_CESAR = 3; 

// --- Implementação das Funções de Cifra de César ---

std::string encriptar(const std::string& texto, int chave) {
    std::string resultado = texto;
    for (char& c : resultado) {
        if (std::isalpha(c)) { 
            char base = std::isupper(c) ? 'A' : 'a';
            c = char((c - base + chave) % 26 + base);
        }
    }
    return resultado;
}

std::string desencriptar(const std::string& texto, int chave) {
    std::string resultado = texto;
    for (char& c : resultado) {
        if (std::isalpha(c)) { 
            char base = std::isupper(c) ? 'A' : 'a';
            c = char((c - base - chave + 26) % 26 + base);
        }
    }
    return resultado;
}

// --- Implementação das Funções de Conversão de Tipo de Marcação ---

// Função para converter TipoMarcacao para string
std::string tipoParaString(TipoMarcacao tipo) {
    switch (tipo) {
        case TipoMarcacao::FERIAS: return "F";
        case TipoMarcacao::FALTA: return "X";
        case TipoMarcacao::FIM_SEMANA: return "S"; // Incluir FIM_SEMANA para consistência, embora não seja usual guardar
        default: return " ";
    }
}

// Função para converter string para TipoMarcacao
TipoMarcacao stringParaTipo(const std::string& str) {
    if (str == "F") return TipoMarcacao::FERIAS;
    if (str == "X") return TipoMarcacao::FALTA;
    return TipoMarcacao::LIVRE;
}

// --- Funções de Ficheiros (Guardar e Carregar) ---
std::string getNextToken(std::stringstream& ss, char delimitador) {
    std::string token;
    // O getline lê até encontrar o delimitador ou o fim da stream
    if (std::getline(ss, token, delimitador)) {
        return token;
    }
    return "";
}

void guardarDados(const std::vector<Colaborador>& colaboradores, const std::string& nomeFicheiro, const std::string& chave) {
    std::ofstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cerr << COR_VERMELHA << "ERRO: Nao foi possivel abrir o ficheiro " << nomeFicheiro << " para escrita.\n" << RESET_COR;
        return;
    }

    // A chave de encriptação é CHAVE_CESAR
    int chave_atual = CHAVE_CESAR; 
    
    for (const auto& colab : colaboradores) {
        // Encriptar Nome e Departamento
        std::string nomeCifrado = encriptar(colab.nome, chave_atual);
        std::string deptoCifrado = encriptar(colab.departamento, chave_atual);

        // Serializar o Calendário
        std::string calendarioStr;
        // O calendário pode ir até 366 dias.
        // Se o mapa não tiver o dia, assumimos LIVRE (' ')
        for (int dia = 1; dia <= 366; ++dia) { 
            auto it = colab.calendario.find(dia);
            TipoMarcacao tipo = (it != colab.calendario.end()) ? it->second : TipoMarcacao::LIVRE;
            
            // Apenas guardamos FÉRIAS ou FALTA para economizar espaço e evitar guardar ' '
            if (tipo == TipoMarcacao::FERIAS || tipo == TipoMarcacao::FALTA) {
                 calendarioStr += std::to_string(dia) + ":" + tipoParaString(tipo) + ",";
            }
        }

        // Remover a última vírgula se existir
        if (!calendarioStr.empty() && calendarioStr.back() == ',') {
            calendarioStr.pop_back();
        }

        // 3. Escrever a linha: Nome;Depto;ID;Calendario
        ficheiro << nomeCifrado << ";"
                 << deptoCifrado << ";"
                 << colab.id << ";"
                 << calendarioStr << "\n";
    }

    std::cout << COR_VERDE << "[INFO] Dados de " << colaboradores.size() << " colaboradores guardados com sucesso em " << nomeFicheiro << ".\n" << RESET_COR;
}
// Função para carregar dados de colaboradores a partir de um ficheiro
void carregarDados(std::vector<Colaborador>& lista, const std::string& nomeFicheiro) {
    lista.clear();
    std::ifstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cerr << COR_AMARELA << "[AVISO] Nao foi possivel abrir o ficheiro " << nomeFicheiro << " para leitura. O ficheiro pode nao existir ainda.\n" << RESET_COR;
        return;
    }

    int chave_atual = CHAVE_CESAR;
    std::string linha;
    while (std::getline(ficheiro, linha)) {
        
        std::stringstream ss(linha);
        std::string nomeCifrado = getNextToken(ss, ';');
        std::string deptoCifrado = getNextToken(ss, ';');
        std::string idStr = getNextToken(ss, ';');
        std::string calendarioEncodedStr = getNextToken(ss, ';');
        
        try {
            Colaborador colab;
            
            // Desencriptar Nome e Departamento
            colab.nome = desencriptar(nomeCifrado, chave_atual);
            colab.departamento = desencriptar(deptoCifrado, chave_atual);
            colab.id = std::stoi(idStr);

            // Deserializar o Calendário (Formato: Dia:Tipo,Dia:Tipo,...)
            std::stringstream ssCalendario(calendarioEncodedStr);
            std::string marcacaoEncoded;
            
            while (std::getline(ssCalendario, marcacaoEncoded, ',')) {
                size_t pos_delimitador = marcacaoEncoded.find(':');
                if (pos_delimitador != std::string::npos) {
                    int diaDoAno = std::stoi(marcacaoEncoded.substr(0, pos_delimitador));
                    std::string tipoStr = marcacaoEncoded.substr(pos_delimitador + 1);
                    colab.calendario[diaDoAno] = stringParaTipo(tipoStr);
                }
            }

            lista.push_back(colab);
        } catch (const std::exception& e) {
             std::cerr << COR_VERMELHA << "[ERRO] Erro ao carregar linha: " << e.what() << " -> Linha ignorada.\n" << RESET_COR;
        }
    }
    std::cout << COR_VERDE << "[INFO] " << lista.size() << " colaboradores carregados com sucesso.\n" << RESET_COR;
}

