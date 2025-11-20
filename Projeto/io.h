
#include "colaborador.h" // Necessário para aceder à estrutura Colaborador
#include "calendario.h" // Necessário para TipoMarcacao
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

// Implementação da Cifra de César
std::string encriptar(const std::string &texto, int chave) {
    std::string resultado = texto;
    for (char &c : resultado) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            c = (char)(base + (c - base + chave) % 26);
        }
    }
    return resultado;
}

std::string desencriptar(const std::string &texto, int chave) {
    // A desencriptação é o mesmo que encriptar com o inverso da chave (26 - chave)
    return encriptar(texto, 26 - (chave % 26));
}

// --- Funções Auxiliares de Serialização/Desserialização ---

// Função auxiliar para converter TipoMarcacao para string (para guardar)
std::string tipoParaString(TipoMarcacao tipo) {
    switch (tipo) {
        case TipoMarcacao::FERIAS: return "F";
        case TipoMarcacao::FALTA: return "L";
        default: return "U"; // Desconhecido/Não Marcado
    }
}

// Função auxiliar para converter string para TipoMarcacao (ao carregar)
TipoMarcacao stringParaTipo(const std::string& str) {
    if (str == "F") return TipoMarcacao::FERIAS;
    if (str == "L") return TipoMarcacao::FALTA;
    return TipoMarcacao::NAO_MARCADO;
}

// Guarda os dados dos colaboradores no ficheiro (CSV Encriptado)
void guardarDados(const std::vector<Colaborador>& lista, const std::string& nomeFicheiro) {
    std::ofstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o ficheiro " << nomeFicheiro << " para escrita." << std::endl;
        return;
    }

    for (const auto& colab : lista) {
        std::stringstream ss;
        // Dados base
        ss << colab.id << ";" << colab.nome << ";" << colab.departamento << ";" << colab.dias_ferias_restantes;

        // Formações
        ss << ";FORM";
        for (const auto& f : colab.formacoes) {
            ss << "|" << f.id_curso << "|" << f.nome_curso << "|" << f.data_conclusao;
        }

        // Notas
        ss << ";NOTA";
        for (const auto& n : colab.notas) {
            // Notas podem ter texto complexo, mas assumimos que o pipe '|' não é usado no texto da nota
            ss << "|" << n.id_nota << "|" << n.texto << "|" << n.data_criacao;
        }

        // Calendário
        ss << ";CAL";
        for (const auto& par : colab.calendario) {
            // Chave é DiaMesAno (int), Valor é TipoMarcacao (enum)
            ss << "|" << par.first << "|" << tipoParaString(par.second);
        }
        
        // Encripta a linha e escreve no ficheiro
        ficheiro << encriptar(ss.str(), CHAVE_CESAR) << "\n";
    }

    ficheiro.close();
    std::cout << "Dados guardados com sucesso em " << nomeFicheiro << " (encriptados).\n";
}

// Função auxiliar para ler um token até ao próximo delimitador (ou fim da string)
std::string getNextToken(std::stringstream& ss, char delimiter) {
    std::string token;
    std::getline(ss, token, delimiter);
    return token;
}

// Carrega os dados do ficheiro para o sistema
void carregarDados(std::vector<Colaborador>& lista, const std::string& nomeFicheiro) {
    std::ifstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cout << "Aviso: O ficheiro " << nomeFicheiro << " nao foi encontrado ou esta vazio. A iniciar com dados vazios.\n";
        return;
    }

    lista.clear();
    std::string linha_encriptada;

    while (std::getline(ficheiro, linha_encriptada)) {
        if (linha_encriptada.empty()) continue;

        std::string linha = desencriptar(linha_encriptada, CHAVE_CESAR);
        std::stringstream ss(linha);
        Colaborador colab;
        std::string segmento;
        
        // Dados base
        std::getline(ss, segmento, ';'); 
        std::stringstream ss_base(segmento);
        
        colab.id = std::stoi(getNextToken(ss_base, ';'));
        colab.nome = getNextToken(ss_base, ';');
        colab.departamento = getNextToken(ss_base, ';');
        colab.dias_ferias_restantes = std::stoi(getNextToken(ss_base, ';'));

        // Formações
        if (std::getline(ss, segmento, ';') && segmento.substr(0, 4) == "FORM") {
            std::stringstream ss_form(segmento.substr(4));
            std::string token;
            // O loop lê tokens separados por '|'. O primeiro token é lixo se a lista não for vazia.
            while (std::getline(ss_form, token, '|')) {
                Formacao f;
                try {
                    // ID | Nome | Data
                    f.id_curso = std::stoi(token);
                    f.nome_curso = getNextToken(ss_form, '|');
                    f.data_conclusao = getNextToken(ss_form, '|');
                    colab.formacoes.push_back(f);
                } catch (const std::exception& e) {
                    // Ignora se a formacao estiver mal formatada ou for o token de cabeçalho
                }
            }
        }

        // Notas 
        if (std::getline(ss, segmento, ';') && segmento.substr(0, 4) == "NOTA") {
            std::stringstream ss_nota(segmento.substr(4));
            std::string token;
            while (std::getline(ss_nota, token, '|')) {
                Nota n;
                try {
                    // ID | Texto | Data
                    n.id_nota = std::stoi(token);
                    n.texto = getNextToken(ss_nota, '|');
                    n.data_criacao = getNextToken(ss_nota, '|');
                    colab.notas.push_back(n);
                } catch (const std::exception& e) {
                    // Ignora se a nota estiver mal formatada
                }
            }
        }

        // Calendário 
        if (std::getline(ss, segmento, ';') && segmento.substr(0, 3) == "CAL") {
            std::stringstream ss_cal(segmento.substr(3));
            std::string token;
            while (std::getline(ss_cal, token, '|')) {
                try {
                    int diaMesAno = std::stoi(token);
                    std::string tipoStr = getNextToken(ss_cal, '|');
                    colab.calendario[diaMesAno] = stringParaTipo(tipoStr);
                } catch (const std::exception& e) {
                    // Ignora se a marcacao estiver mal formatada
                }
            }
        }

        lista.push_back(colab);
    }

    ficheiro.close();
    std::cout << lista.size() << " colaboradores carregados com sucesso de " << nomeFicheiro << ".\n";
}