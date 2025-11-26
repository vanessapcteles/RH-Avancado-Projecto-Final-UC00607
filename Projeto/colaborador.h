#ifndef COLABORADOR_H
#define COLABORADOR_H

#include "calendario.h" 
#include <string>
#include <vector>
#include <map>

// ===============================================
// ESTRUTURAS DE DADOS ADICIONAIS
// ===============================================

// Estrutura para Formacoes/Cursos
struct Formacao {
    int id_curso = 0; // ID único para a formação/nota dentro do colaborador
    std::string nome_curso;
    std::string data_conclusao; // Formato dd/mm/aaaa
};

// Estrutura para Notas Internas 
struct Nota {
    int id_nota = 0;
    std::string texto;
    std::string data_criacao; // Data em que a nota foi adicionada
};

// ===============================================
// ESTRUTURA PRINCIPAL
// ===============================================

// Estrutura que representa um colaborador 
struct Colaborador {
    int id = 0; // ID único
    std::string nome;
    std::string departamento; // Departamento

    // Calendário existente (int = DiaDoAno, TipoMarcacao = Férias/Falta)
    std::map<int, TipoMarcacao> calendario;

    //  Listas de Formacoes e Notas 
    std::vector<Formacao> formacoes;
    std::vector<Nota> notas;

    // Campo para o Dashboard
    int dias_ferias_restantes = 22; // Assumindo 22 dias por ano
};

// ===============================================
// DECLARAÇÕES DE FUNÇÕES
// ===============================================

// Adiciona um novo colaborador à lista
void adicionarColaborador(std::vector<Colaborador>& lista);

// Remove um colaborador da lista
void removerColaborador(std::vector<Colaborador>& lista);

// Lista todos os colaboradores existentes (inclui ID e Departamento)
void listarColaboradores(const std::vector<Colaborador>& lista);

// Encontra o índice de um colaborador pelo nome ou ID
int encontrarColaborador(const std::vector<Colaborador>& lista, const std::string& query, bool isID = false);

// Procura, mostra dados básicos e o calendário do mês atual
void procurarColaborador(const std::vector<Colaborador>& lista);

// Menu de Gestão do Colaborador (Formações/Notas)
void menuGestaoColaborador(std::vector<Colaborador>& lista);

// Funções CRUD de Formação (usadas internamente ou no menuGestao)
void gerirFormacoes(Colaborador& colab);

// Funções CRUD de Notas (usadas internamente ou no menuGestao)
void gerirNotas(Colaborador& colab);

#endif // COLABORADOR_H