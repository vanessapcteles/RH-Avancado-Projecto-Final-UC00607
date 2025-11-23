#ifndef COLABORADOR_H
#define COLABORADOR_H

#include "calendario.h" // Apenas o cabeçalho é necessário para TipoMarcacao
#include <string>
#include <vector>
#include <map>

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

// Estrutura que representa um colaborador (Atualizada)
struct Colaborador {
    int id; // Novo: ID único (Requisito 2)
    std::string nome;
    std::string departamento; // Novo: Departamento

    // Calendário existente (int = DiaDoAno, TipoMarcacao = Férias/Falta)
    std::map<int, TipoMarcacao> calendario;

    // Novos: Listas de Formacoes e Notas 
    std::vector<Formacao> formacoes;
    std::vector<Nota> notas;

    // Campo calculado/resumo 
    int dias_ferias_restantes = 22; // Assumindo 22 dias por ano
};

// --- Declarações de Funções (Protótipos) ---

// Adiciona um novo colaborador à lista
void adicionarColaborador(std::vector<Colaborador>& lista);

// Lista todos os colaboradores existentes
void listarColaboradores(const std::vector<Colaborador>& lista);

// Encontra o índice de um colaborador pelo nome (ou ID)
int encontrarColaborador(const std::vector<Colaborador>& lista, const std::string& query, bool isID = false);

// Novo: Procura e mostra dados 
void procurarColaborador(const std::vector<Colaborador>& lista);

// Novo: Menu de Gestão do Colaborador (Formações/Notas)
void menuGestaoColaborador(std::vector<Colaborador>& lista);

// Novo: Funções CRUD de Formação 
void gerirFormacoes(Colaborador& colab);

// Novo: Funções CRUD de Notas 
void gerirNotas(Colaborador& colab);

#endif // COLABORADOR_H