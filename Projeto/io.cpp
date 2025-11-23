#include "io.h" // Inclui as declarações (assinaturas)
#include "colaborador.h"
#include "calendario.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cctype>
#include <string>
#include <map>

// Constante para a chave da Cifra de César
const int CHAVE_CESAR = 3; 

// --- Implementação das Funções ---

// Implementação da Cifra de César
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

// Função para converter TipoMarcacao para string
std::string tipoParaString(TipoMarcacao tipo) {
    switch (tipo) {
        case TipoMarcacao::FERIAS: return "F";
        case TipoMarcacao::FALTA: return "X";
        default: return " ";
    }
    return " ";
}

// ... Coloque aqui a implementação completa de stringParaTipo ...
TipoMarcacao stringParaTipo(const std::string& str) {
    if (str == "F") return TipoMarcacao::FERIAS;
    if (str == "X") return TipoMarcacao::FALTA;
    return TipoMarcacao::LIVRE;
}

// ... Coloque aqui a implementação completa de guardarDados ...
void guardarDados(const std::vector<Colaborador>& lista, const std::string& nomeFicheiro) {
    // ... TODO o corpo da função aqui ...
    std::ofstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o ficheiro " << nomeFicheiro << " para escrita.\n";
        return;
    }
    // ... resto do código ...
}

// ... Coloque aqui a implementação completa de getNextToken ...
std::string getNextToken(std::stringstream& ss, char delimitador) {
    std::string token;
    if (std::getline(ss, token, delimitador)) {
        return token;
    }
    return "";
}

void carregarDados(std::vector<Colaborador>& lista, const std::string& nomeFicheiro) {
    lista.clear(); // Garantir que a lista está vazia antes de carregar novos dados
    std::ifstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cerr << "AVISO: Nao foi possivel abrir o ficheiro " << nomeFicheiro << " para leitura. Pode ser que o ficheiro nao exista ainda.\n";
        return;
    }

    std::string linha;
    while (std::getline(ficheiro, linha)) {
        
        std::stringstream ss(linha);
        std::string nome = getNextToken(ss, ';');
        std::string departamento = getNextToken(ss, ';');
        int id = std::stoi(getNextToken(ss, ';'));
        Colaborador colab;
        colab.nome = desencriptar(nome, CHAVE_CESAR);
        colab.departamento = desencriptar(departamento, CHAVE_CESAR);
        colab.id = id;
        std::string calendarioStr = getNextToken(ss, ';');
        std::stringstream ssCalendario(calendarioStr);
        std::string marcacao;
        int dia = 0;
        while (std::getline(ssCalendario, marcacao, ',')) {
            colab.calendario[dia++] = stringParaTipo(marcacao);
        }
        lista.push_back(colab);
    }
}


