#include "colaborador.h"
#include "calendario.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <cctype>
#include <ctime>
#include <sstream>
#include <iomanip>

// Cores (Definições globais)
const std::string COR_AMARELA = "\033[33m";      // Avisos/Fim de Semana
const std::string COR_VERMELHA = "\033[31m";    // Erros
const std::string COR_VERDE = "\033[32m";       // Sucesso/Férias
const std::string COR_AZUL = "\033[34m";        // Títulos
const std::string COR_ROXO = "\033[35m";        // Faltas
const std::string RESET_COR = "\033[0m";        // Reset

// Retorna o próximo ID disponível
int getProximoId(const std::vector<Colaborador>& lista) {
    int maxId = 0;
    for (const auto& colab : lista) {
        if (colab.id > maxId) {
            maxId = colab.id;
        }
    }
    return maxId + 1;
}

// Verifica se o nome já existe na lista
bool verificarDuplicado(const std::vector<Colaborador>& lista, const std::string& nome) {
    for (const auto& colab : lista) {
        // Ignora a capitalização para a verificação de duplicado
        std::string nome_lower = nome;
        std::string colab_nome_lower = colab.nome;
        std::transform(nome_lower.begin(), nome_lower.end(), nome_lower.begin(), ::tolower);
        std::transform(colab_nome_lower.begin(), colab_nome_lower.end(), colab_nome_lower.begin(), ::tolower);

        if (colab_nome_lower == nome_lower) {
            return true;
        }
    }
    return false;
}

// Adiciona colaborador (Atualizado com ID e Departamento)
void adicionarColaborador(std::vector<Colaborador>& lista) {
    std::string nome, departamento;
    std::cout << COR_AZUL << "\n--- Adicionar Colaborador ---\n" << RESET_COR;

    // Nome
    std::cout << "Nome do novo colaborador: ";
    std::getline(std::cin >> std::ws, nome);

    if (verificarDuplicado(lista, nome)) {
        char confirmacao;
        std::cout << COR_VERMELHA << "ERRO: Colaborador com o nome '" << nome << "' ja existe. Deseja adicionar mesmo assim? (S/N): " << RESET_COR;
        std::cin >> confirmacao;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::tolower(confirmacao) != 's') {
            std::cout << COR_AMARELA << "Operacao cancelada.\n" << RESET_COR;
            return;
        }
    }

    // Departamento
    std::cout << "Departamento (Ex: Vendas, RH, IT): ";
    std::getline(std::cin >> std::ws, departamento);

    Colaborador novoColab;
    novoColab.id = getProximoId(lista);
    novoColab.nome = nome;
    novoColab.departamento = departamento;

    lista.push_back(novoColab);
    std::cout << COR_VERDE << "Colaborador '" << nome << "' (ID: " << novoColab.id << ") do departamento '" << departamento << "' adicionado com sucesso.\n" << RESET_COR;
}

// Lista os colaboradores
void listarColaboradores(const std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_VERMELHA << "Nao ha colaboradores registados.\n" << RESET_COR;
        return;
    }
    std::cout << COR_AZUL << "\n--- Lista de Colaboradores (" << lista.size() << ") ---\n" << RESET_COR;
    std::cout << std::left << std::setw(5) << "ID" << std::setw(30) << "Nome" << "Departamento\n";
    std::cout << "----------------------------------------------------\n";
    for (const auto& colab : lista) {
        std::cout << std::left << std::setw(5) << colab.id
                  << std::setw(30) << colab.nome
                  << colab.departamento << "\n";
    }
    std::cout << "----------------------------------------------------\n";
}

// Encontra o índice (por nome ou ID) do colaborador na lista
int encontrarColaborador(const std::vector<Colaborador>& lista, const std::string& query, bool isID) {
    if (isID) {
        try {
            int id = std::stoi(query);
            for (size_t i = 0; i < lista.size(); ++i) {
                if (lista[i].id == id) {
                    return static_cast<int>(i);
                }
            }
        } catch (const std::exception& e) {
            // Ignora se a conversão falhar, tenta procurar por nome
        }
    }
    // Procurar por nome (parcial e insensível a maiúsculas/minúsculas)
    std::string query_lower = query;
    std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);

    for (size_t i = 0; i < lista.size(); ++i) {
        std::string nome_lower = lista[i].nome;
        std::transform(nome_lower.begin(), nome_lower.end(), nome_lower.begin(), ::tolower);

        if (nome_lower.find(query_lower) != std::string::npos) { // Usa find para procurar parte do nome
            return static_cast<int>(i);
        }
    }
    return -1; // Não encontrado
}

// Procura e mostra dados do colaborador
void procurarColaborador(const std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha colaboradores para buscar.\n" << RESET_COR;
        return;
    }

    std::string query;
    std::cout << COR_AZUL << "\n--- Buscar Colaborador ---\n" << RESET_COR;
    std::cout << "Digite o nome (parcial ou completo) ou ID do colaborador: ";
    std::getline(std::cin >> std::ws, query);

    bool isID = std::all_of(query.begin(), query.end(), ::isdigit);
    int indice = encontrarColaborador(lista, query, isID);

    if (indice == -1) {
        std::cout << COR_VERMELHA << "ERRO: Colaborador nao encontrado com a query: '" << query << "'.\n" << RESET_COR;
        return;
    }

    const Colaborador& colab = lista[static_cast<size_t>(indice)];

    // Mostrar dados básicos
    std::cout << "\n============================================\n";
    std::cout << COR_AZUL << "DADOS DO COLABORADOR: " << colab.nome << RESET_COR << " (ID: " << colab.id << ")\n";
    std::cout << "============================================\n";
    std::cout << "Departamento: " << colab.departamento << "\n";
    std::cout << "Ferias Restantes (Ano): " << colab.dias_ferias_restantes << " dias\n";
    std::cout << "Total de Formacoes: " << colab.formacoes.size() << "\n";
    std::cout << "Total de Notas: " << colab.notas.size() << "\n";
    std::cout << "--------------------------------------------\n";

    // Mostrar calendário do mês atual
    std::time_t t = std::time(nullptr);
    std::tm* tm_local = std::localtime(&t);
    int mesAtual = tm_local->tm_mon + 1;
    int anoAtual = tm_local->tm_year + 1900;

    visualizarCalendario(colab, mesAtual, anoAtual);
}


// --- Funções CRUD de FORMAÇÃO ---

// Retorna o próximo ID de formação para um colaborador
int getProximoIdFormacao(const Colaborador& colab) {
    int maxId = 0;
    for (const auto& f : colab.formacoes) {
        if (f.id_curso > maxId) {
            maxId = f.id_curso;
        }
    }
    return maxId + 1;
}

void listarFormacoes(const Colaborador& colab) {
    if (colab.formacoes.empty()) {
        std::cout << COR_AMARELA << "Nao ha formacoes registadas para " << colab.nome << ".\n" << RESET_COR;
        return;
    }
    std::cout << COR_AZUL << "\n--- Formacoes de " << colab.nome << " ---\n" << RESET_COR;
    std::cout << std::left << std::setw(5) << "ID" << std::setw(40) << "Nome do Curso" << "Conclusao\n";
    std::cout << "---------------------------------------------------------\n";
    for (const auto& form : colab.formacoes) {
        std::cout << std::left << std::setw(5) << form.id_curso
                  << std::setw(40) << form.nome_curso
                  << form.data_conclusao << "\n";
    }
    std::cout << "---------------------------------------------------------\n";
}

void adicionarFormacao(Colaborador& colab) {
    std::string nome, data;
    std::cout << COR_AZUL << "\n--- Adicionar Formacao/Curso ---\n" << RESET_COR;
    std::cout << "Nome do Curso: ";
    std::getline(std::cin >> std::ws, nome);
    std::cout << "Data de Conclusao (dd/mm/aaaa): ";
    std::getline(std::cin >> std::ws, data);

    Formacao novaForm;
    novaForm.id_curso = getProximoIdFormacao(colab);
    novaForm.nome_curso = nome;
    novaForm.data_conclusao = data;

    colab.formacoes.push_back(novaForm);
    std::cout << COR_VERDE << "Formacao '" << nome << "' adicionada com sucesso (ID: " << novaForm.id_curso << ").\n" << RESET_COR;
}

void editarFormacao(Colaborador& colab) {
    if (colab.formacoes.empty()) { listarFormacoes(colab); return; }
    listarFormacoes(colab);
    int id_curso;
    std::cout << "Digite o ID da formacao a editar: ";
    if (!(std::cin >> id_curso)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto it = std::find_if(colab.formacoes.begin(), colab.formacoes.end(),
                           [id_curso](const Formacao& f){ return f.id_curso == id_curso; });

    if (it != colab.formacoes.end()) {
        std::cout << "Editar Nome (Atual: " << it->nome_curso << "): ";
        std::getline(std::cin >> std::ws, it->nome_curso);
        std::cout << "Editar Data de Conclusao (Atual: " << it->data_conclusao << "): ";
        std::getline(std::cin >> std::ws, it->data_conclusao);
        std::cout << COR_VERDE << "Formacao ID " << id_curso << " editada com sucesso.\n" << RESET_COR;
    } else {
        std::cout << COR_VERMELHA << "ERRO: ID de formacao nao encontrado.\n" << RESET_COR;
    }
}

void removerFormacao(Colaborador& colab) {
    if (colab.formacoes.empty()) { listarFormacoes(colab); return; }
    listarFormacoes(colab);
    int id_curso;
    std::cout << "Digite o ID da formacao a remover: ";
    if (!(std::cin >> id_curso)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto it = std::remove_if(colab.formacoes.begin(), colab.formacoes.end(),
                             [id_curso](const Formacao& f){ return f.id_curso == id_curso; });

    if (it != colab.formacoes.end()) {
        colab.formacoes.erase(it, colab.formacoes.end());
        std::cout << COR_VERDE << "Formacao ID " << id_curso << " removida com sucesso.\n" << RESET_COR;
    } else {
        std::cout << COR_VERMELHA << "ERRO: ID de formacao nao encontrado.\n" << RESET_COR;
    }
}

void gerirFormacoes(Colaborador& colab) {
    int op;
    do {
        std::cout << COR_AZUL << "\n--- GERIR FORMACOES de " << colab.nome << " (ID: " << colab.id << ") ---\n" << RESET_COR;
        std::cout << "1. Listar Formacoes\n";
        std::cout << "2. Adicionar Curso\n";
        std::cout << "3. Editar Curso\n";
        std::cout << "4. Remover Curso\n";
        std::cout << "0. Voltar ao Menu Anterior\n";
        std::cout << "Opcao: ";

        if (!(std::cin >> op)) {
            std::cout << COR_AMARELA << "Input invalido. Tente novamente.\n" << RESET_COR;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (op) {
            case 1: listarFormacoes(colab); break;
            case 2: adicionarFormacao(colab); break;
            case 3: editarFormacao(colab); break;
            case 4: removerFormacao(colab); break;
            case 0: break;
            default: std::cout << COR_VERMELHA << "Opcao invalida.\n" << RESET_COR;
        }
    } while (op != 0);
}

// --- Funções CRUD de NOTAS (Requisito 7) ---

// Retorna o próximo ID de nota para um colaborador
int getProximoIdNota(const Colaborador& colab) {
    int maxId = 0;
    for (const auto& n : colab.notas) {
        if (n.id_nota > maxId) {
            maxId = n.id_nota;
        }
    }
    return maxId + 1;
}

// Retorna a data atual como string dd/mm/aaaa
std::string getDataAtual() {
    std::time_t t = std::time(nullptr);
    std::tm* tm_local = std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(tm_local, "%d/%m/%Y");
    return ss.str();
}

void listarNotas(const Colaborador& colab) {
    if (colab.notas.empty()) {
        std::cout << COR_AMARELA << "Nao ha notas registadas para " << colab.nome << ".\n" << RESET_COR;
        return;
    }
    std::cout << COR_AZUL << "\n--- Notas Internas de " << colab.nome << " ---\n" << RESET_COR;
    for (const auto& nota : colab.notas) {
        std::cout << "  ID " << nota.id_nota << " (" << nota.data_criacao << "):\n";
        std::cout << "    - " << nota.texto << "\n";
    }
    std::cout << "-----------------------------------\n";
}


void adicionarNota(Colaborador& colab) {
    std::string texto;
    std::cout << COR_AZUL << "\n--- Adicionar Nota Interna ---\n" << RESET_COR;
    std::cout << "Texto da Nota: ";
    std::getline(std::cin >> std::ws, texto);

    Nota novaNota;
    novaNota.id_nota = getProximoIdNota(colab);
    novaNota.texto = texto;
    novaNota.data_criacao = getDataAtual();

    colab.notas.push_back(novaNota);
    std::cout << COR_VERDE << "Nota adicionada com sucesso (ID: " << novaNota.id_nota << ", Data: " << novaNota.data_criacao << ").\n" << RESET_COR;
}

void editarNota(Colaborador& colab) {
    if (colab.notas.empty()) { listarNotas(colab); return; }
    listarNotas(colab);
    int id_nota;
    std::cout << "Digite o ID da nota a editar: ";
    if (!(std::cin >> id_nota)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto it = std::find_if(colab.notas.begin(), colab.notas.end(),
                           [id_nota](const Nota& n){ return n.id_nota == id_nota; });

    if (it != colab.notas.end()) {
        std::cout << "Editar Texto (Atual: " << it->texto << "): ";
        std::getline(std::cin >> std::ws, it->texto);
        // A data de criação é mantida
        std::cout << COR_VERDE << "Nota ID " << id_nota << " editada com sucesso.\n" << RESET_COR;
    } else {
        std::cout << COR_VERMELHA << "ERRO: ID de nota nao encontrado.\n" << RESET_COR;
    }
}

void removerNota(Colaborador& colab) {
    if (colab.notas.empty()) { listarNotas(colab); return; }
    listarNotas(colab);
    int id_nota;
    std::cout << "Digite o ID da nota a remover: ";
    if (!(std::cin >> id_nota)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto it = std::remove_if(colab.notas.begin(), colab.notas.end(),
                             [id_nota](const Nota& n){ return n.id_nota == id_nota; });

    if (it != colab.notas.end()) {
        colab.notas.erase(it, colab.notas.end());
        std::cout << COR_VERDE << "Nota ID " << id_nota << " removida com sucesso.\n" << RESET_COR;
    } else {
        std::cout << COR_VERMELHA << "ERRO: ID de nota nao encontrado.\n" << RESET_COR;
    }
}

void gerirNotas(Colaborador& colab) {
    int op;
    do {
        std::cout << COR_AZUL << "\n--- GERIR NOTAS de " << colab.nome << " (ID: " << colab.id << ") ---\n" << RESET_COR;
        std::cout << "1. Listar Notas\n";
        std::cout << "2. Adicionar Nota\n";
        std::cout << "3. Editar Nota\n";
        std::cout << "4. Remover Nota\n";
        std::cout << "0. Voltar ao Menu Anterior\n";
        std::cout << "Opcao: ";

        if (!(std::cin >> op)) {
            std::cout << COR_AMARELA << "Input invalido. Tente novamente.\n" << RESET_COR;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (op) {
            case 1: listarNotas(colab); break;
            case 2: adicionarNota(colab); break;
            case 3: editarNota(colab); break;
            case 4: removerNota(colab); break;
            case 0: break;
            default: std::cout << COR_VERMELHA << "Opcao invalida.\n" << RESET_COR;
        }
    } while (op != 0);
}

// Menu de Gestão do Colaborador (Formações/Notas) - Ligação principal do main.cpp
void menuGestaoColaborador(std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha colaboradores para gerir.\n" << RESET_COR;
        return;
    }

    listarColaboradores(lista);
    std::string query;
    std::cout << "\nDigite o nome ou ID do colaborador para gerir Formacoes/Notas: ";
    std::getline(std::cin >> std::ws, query);

    bool isID = std::all_of(query.begin(), query.end(), ::isdigit);
    int indice = encontrarColaborador(lista, query, isID);

    if (indice == -1) {
        std::cout << COR_VERMELHA << "ERRO: Colaborador nao encontrado.\n" << RESET_COR;
        return;
    }

    Colaborador& colab = lista[static_cast<size_t>(indice)];
    int op;

    do {
        std::cout << COR_AZUL << "\n--- GESTAO DE " << colab.nome << " ---\n" << RESET_COR;
        std::cout << "1. Gerir Formacoes / Cursos (CRUD)\n";
        std::cout << "2. Gerir Notas / Observacoes Internas (CRUD)\n";
        std::cout << "0. Voltar ao Menu Principal\n";
        std::cout << "Opcao: ";

        if (!(std::cin >> op)) {
            std::cout << COR_AMARELA << "Input invalido. Tente novamente.\n" << RESET_COR;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (op) {
            case 1: gerirFormacoes(colab); break;
            case 2: gerirNotas(colab); break;
            case 0: break;
            default: std::cout << COR_VERMELHA << "Opcao invalida.\n" << RESET_COR;
        }
    } while (op != 0);
}