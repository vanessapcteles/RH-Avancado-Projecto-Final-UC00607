#ifndef REPORTS_H
#define REPORTS_H

// Incluir as dependências essenciais para as funções usarem os tipos Colaborador, etc.
#include "colaborador.h" 
#include "calendario.h" 
#include <vector>
#include <string>

// --- DECLARAÇÕES DE FUNÇÕES ---

// Gera um dashboard de resumo anual/ferias
void dashboardResumido(const std::vector<Colaborador>& lista);

// Gera um relatorio de ferias/faltas por mes
void relatorioMensal(const std::vector<Colaborador>& lista);

// Gera estatisticas de ausencias por departamento
void estatisticasDepartamento(const std::vector<Colaborador>& lista);

// Menu para exportacao de dados (colaborador ou departamento)
void exportarDados(const std::vector<Colaborador>& lista);


#endif // REPORTS_H