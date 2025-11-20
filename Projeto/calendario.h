#ifndef CALENDARIO_H
#define CALENDARIO_H

#include <string>
#include <map>
#include <iostream>
#include <iomanip>

// --- Enumeração para Tipo de Marcação (Usando enum class para tipagem forte) ---
// Define os tipos de marcação de um dia, evitando colisões de nomes
enum class TipoMarcacao {
    LIVRE,       //' ' - Dia normal de trabalho
    FERIAS,     // 'F' - Dia de férias
    FALTA,      // 'X' - Dia de falta
    FIM_SEMANA  // 'S' - Fim de semana (normalmente não é marcável pelo utilizador)
};

// Declaração antecipada da estrutura Colaborador para uso nas funções
struct Colaborador;

// --- Funções de Datas e Conversão ---

// Retorna o dia da semana (ex: 0=Domingo, 1=Segunda, etc.)
int diaSemana(int dia, int mes, int ano);

// Retorna o número de dias num determinado mês (considerando anos bissextos)
int diasNoMes(int mes, int ano);

// Retorna o nome do mês em formato string
std::string nomeMes(int mes);

// --- Funções de Validação e Conversão ---

// Verifica se a data é válida (ex: 30 de Fev, 31 em Abril)
bool dataValida(int dia, int mes, int ano);

// Converte a data (dia, mês, ano) no dia do ano (1 a 366)
int dataParaDiaDoAno(int dia, int mes, int ano);

// --- Funções de Marcação e Visualização ---

// Marcar um dia específico para um colaborador com um tipo de marcação
// (Principal ponto para validar se o dia é um Fim de Semana)
void marcarDia(Colaborador& colab, int dia, int mes, int ano, TipoMarcacao tipo);

// Desmarcar um dia, voltando-o a LIVRE
void desmarcarDia(Colaborador& colab, int dia, int mes, int ano);

// Visualiza o calendário mensal detalhado do colaborador
void visualizarCalendario(const Colaborador& colab, int mes, int ano);

#endif // CALENDARIO_H