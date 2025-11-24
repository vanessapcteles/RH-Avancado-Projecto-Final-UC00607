#include "reports.h"
#include "colaborador.h" // Necessário para aceder à estrutura Colaborador
#include "io.h"        // Necessário para encriptar/desencriptar
#include "calendario.h" // Necessário para funções de calendário
#include "cores.h"    // Necessário para cores de output
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <limits>

extern const std::string COR_AMARELA;
extern const std::string COR_VERMELHA;
extern const std::string COR_VERDE;
extern const std::string COR_AZUL;
extern const std::string COR_ROXO;
extern const std::string RESET_COR;

// --- Implementação do Dashboard Resumido  ---
void dashboardResumido(const std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha colaboradores para gerar o Dashboard.\n" << RESET_COR;
        return;
    }

    // Obter o ano atual para calcular faltas/férias anuais
    std::time_t t = std::time(nullptr);
    std::tm* tm_local = std::localtime(&t);
    int anoAtual = tm_local->tm_year + 1900;
    const int DIAS_TOTAIS_FERIAS = 22; // Limite de férias anual

    std::cout << COR_AZUL << "\n--- DASHBOARD RESUMIDO (" << anoAtual << ") ---\n" << RESET_COR;
    std::cout << "Total de Ferias Anuais Disponiveis: " << DIAS_TOTAIS_FERIAS << " dias\n";
    std::cout << "==================================================================\n";
    std::cout << std::left
              << std::setw(30) << "Nome"
              << std::setw(10) << "Ferias (F)"
              << std::setw(10) << "Faltas (X)"
              << std::setw(15) << "Restantes (F)"
              << "Departamento\n";
    std::cout << "------------------------------------------------------------------\n";

    for (const auto& colab : lista) {
        int total_ferias_contadas = 0;
        int total_faltas_contadas = 0;

        // Contar o total de ausências no ano
        contarAusencias(colab, anoAtual, total_ferias_contadas, total_faltas_contadas);

        std::cout << std::left
                  << std::setw(30) << colab.nome
                  << std::setw(10) << total_ferias_contadas
                  << std::setw(10) << total_faltas_contadas
                  << std::setw(15) << colab.dias_ferias_restantes
                  << colab.departamento << "\n";
    }
    std::cout << "==================================================================\n";
}

// --- Implementação dos Relatórios Mensais (Requisito 1) ---
void relatorioMensal(const std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha colaboradores para gerar relatorios.\n" << RESET_COR;
        return;
    }

    int mes, ano;
    std::cout << COR_AZUL << "\n--- RELATORIO MENSAL ---\n" << RESET_COR;
    std::cout << "Digite o Mes (mm): ";
    if (!(std::cin >> mes)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; }
    std::cout << "Digite o Ano (aaaa): ";
    if (!(std::cin >> ano)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(dataValida(1, mes, ano) == false ) {
        std::cout << COR_VERMELHA << "ERRO: Mes ou Ano invalidos.\n" << RESET_COR;
        return;
    }

    std::cout << "\n============================================\n";
    std::cout << COR_AZUL << " RELATORIO DE AUSENCIAS - " << std::setw(2) << std::setfill('0') << mes << "/" << ano << RESET_COR << "\n";
    std::cout << "============================================\n";
    std::cout << std::left << std::setw(30) << "Colaborador" << std::setw(10) << "Ferias (F)" << "Faltas (X)\n";
    std::cout << "--------------------------------------------\n";

    for (const auto& colab : lista) {
        int ferias = 0;
        int faltas = 0;
        contarAusenciasMes(colab, mes, ano, ferias, faltas);

        std::cout << std::left
                  << std::setw(30) << colab.nome
                  << std::setw(10) << ferias
                  << faltas << "\n";
    }
    std::cout << "============================================\n";
}


// --- Implementação das Estatísticas de Departamento (Requisito 3) ---
void estatisticasDepartamento(const std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha colaboradores para gerar estatisticas.\n" << RESET_COR;
        return;
    }

    // Obter o ano atual para a estatística
    std::time_t t = std::time(nullptr);
    std::tm* tm_local = std::localtime(&t);
    int anoAtual = tm_local->tm_year + 1900;

    // Estrutura para acumular totais por departamento
    struct DeptStats {
        int total_ferias = 0;
        int total_faltas = 0;
        int total_ausencias() const { return total_ferias + total_faltas; }
    };

    std::map<std::string, DeptStats> stats;
    std::string deptMaisAusente = "N/A";
    int maxAusencias = -1;

    for (const auto& colab : lista) {
        int ferias = 0;
        int faltas = 0;
        // Contar ausências anuais para o departamento
        contarAusencias(colab, anoAtual, ferias, faltas);

        stats[colab.departamento].total_ferias += ferias;
        stats[colab.departamento].total_faltas += faltas;
    }

    std::cout << COR_AZUL << "\n--- ESTATISTICAS DE DEPARTAMENTO (" << anoAtual << ") ---\n" << RESET_COR;
    std::cout << "==================================================\n";
    std::cout << std::left << std::setw(25) << "Departamento" << std::setw(10) << "Ferias" << std::setw(10) << "Faltas" << "Total Ausencias\n";
    std::cout << "--------------------------------------------------\n";

    for (const auto& par : stats) {
        const std::string& dept = par.first;
        const DeptStats& s = par.second;

        std::cout << std::left
                  << std::setw(25) << dept
                  << std::setw(10) << s.total_ferias
                  << std::setw(10) << s.total_faltas
                  << s.total_ausencias() << "\n";

        // Determinar o departamento com mais ausências
        if (s.total_ausencias() > maxAusencias) {
            maxAusencias = s.total_ausencias();
            deptMaisAusente = dept;
        }
    }
    std::cout << "==================================================\n";
    if (maxAusencias > 0) {
        std::cout << COR_AMARELA << "Departamento com mais ausencias (" << maxAusencias << " dias): " << deptMaisAusente << RESET_COR << "\n";
    } else {
        std::cout << COR_AMARELA << "Nao foram encontradas ausencias registadas neste ano.\n" << RESET_COR;
    }
}


// --- Implementação da Exportação  ---
void exportarDados(const std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha dados para exportar.\n" << RESET_COR;
        return;
    }

    int op;
    std::cout << COR_AZUL << "\n--- EXPORTACAO DE DADOS ---\n" << RESET_COR;
    std::cout << "1. Exportar Calendario de UM Colaborador (CSV)\n";
    std::cout << "2. Exportar Relatorio de Departamento (CSV)\n";
    std::cout << "Opcao: ";

    if (!(std::cin >> op) || (op != 1 && op != 2)) {
        std::cout << COR_VERMELHA << "Opcao invalida. A exportacao foi cancelada.\n" << RESET_COR;
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    if (op == 1) { // Exportar Calendário de Colaborador
        std::string nome, nomeFicheiro;
        std::cout << "Nome do colaborador: ";
        std::getline(std::cin >> std::ws, nome);
        int indice = encontrarColaborador(lista, nome);

        if (indice == -1) {
            std::cout << COR_VERMELHA << "ERRO: Colaborador nao encontrado.\n" << RESET_COR;
            return;
        }
        const Colaborador& colab = lista[static_cast<size_t>(indice)];

        nomeFicheiro = colab.nome + "_calendario.csv";
        std::ofstream ficheiro(nomeFicheiro);

        ficheiro << "ID,Nome,Departamento,Data,Tipo_Marcacao\n";
        for (const auto& par : colab.calendario) {
            int chave = par.first; // YYYYMMDD
            int ano = chave / 10000;
            int mes = (chave % 10000) / 100;
            int dia = chave % 100;
            TipoMarcacao tipo = TipoMarcacao::LIVRE;
            // Converter int de volta para TipoMarcacao
            char tipo_char = ' ';
            if (par.second == TipoMarcacao::FERIAS) tipo_char = 'F';
            else if (par.second == TipoMarcacao::FALTA) tipo_char = 'X';
            if (tipo_char != ' ') {
                tipo = stringParaTipo(std::string(1, tipo_char));
            }


            std::string tipoMarcacao = (par.second == TipoMarcacao::FERIAS) ? "F" : "X";
            ficheiro << colab.id << ","
                     << colab.nome << ","
                     << colab.departamento << ","
                     << std::setw(2) << std::setfill('0') << dia << "/"
                        << std::setw(2) << std::setfill('0') << mes << "/"
                        << ano << ","
                        << tipoMarcacao << "\n";
        }
        ficheiro.close();
        std::cout << COR_VERDE << "Calendario de " << colab.nome << " exportado para " << nomeFicheiro << "\n" << RESET_COR;

    } else if (op == 2) { // Exportar Relatório de Departamento
        std::string dept;
        std::cout << "Nome do Departamento a exportar: ";
        std::getline(std::cin >> std::ws, dept);

        // Obter o ano atual para a estatística
        std::time_t t = std::time(nullptr);
        std::tm* tm_local = std::localtime(&t);
        int anoAtual = tm_local->tm_year + 1900;

        std::string nomeFicheiro = dept + "_relatorio_" + std::to_string(anoAtual) + ".csv";
        std::ofstream ficheiro(nomeFicheiro);

        ficheiro << "Departamento,Nome,Ferias_Ano,Faltas_Ano,Dias_Restantes_Ferias\n";

        bool foundDept = false;
        for (const auto& colab : lista) {
            if (colab.departamento == dept) {
                foundDept = true;
                int total_ferias = 0;
                int total_faltas = 0;
                contarAusencias(colab, anoAtual, total_ferias, total_faltas);

                ficheiro << colab.departamento << ","
                         << colab.nome << ","
                         << total_ferias << ","
                         << total_faltas << ","
                         << colab.dias_ferias_restantes << "\n";
            }
        }
        ficheiro.close();
        if (foundDept) {
            std::cout << COR_VERDE << "Relatorio anual do departamento '" << dept << "' exportado para " << nomeFicheiro << "\n" << RESET_COR;
        } else {
            std::cout << COR_AMARELA << "AVISO: Nao foi encontrado o departamento '" << dept << "'. Exportacao cancelada.\n" << RESET_COR;
        }
    }
}
