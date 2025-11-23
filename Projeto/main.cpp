#include "colaborador.h"
#include "calendario.h"
#include "io.h"
#include "reports.h" // Inclui os novos relatórios
#include "calendario.cpp"
#include "io.cpp"
#include <limits>
#include <cstdlib>
#include <iostream>
#include <cstddef>
#include <sstream>
#include <algorithm>

// Cores (Definições globais)
const std::string COR_AMARELA = "\033[33m";      // Avisos/Fim de Semana
const std::string COR_VERMELHA = "\033[31m";    // Erros
const std::string COR_VERDE = "\033[32m";      // Sucesso/Férias
const std::string COR_AZUL = "\033[34m";        // Títulos
const std::string COR_ROXO = "\033[35m";        // Faltas
const std::string RESET_COR = "\033[0m";       // Reset

void limparConsola() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Apresenta o menu principal (Requisito 9)
void mostrarMenu() {
    std::cout << COR_AZUL << "\n============================================\n";
    std::cout << "  Mini-Sistema RH - Menu Principal \n";
    std::cout << "============================================\n" << RESET_COR;
    std::cout << " 1. Adicionar Colaborador\n";
    std::cout << " 2. Gerir Marcacoes (Ferias/Faltas)\n";
    std::cout << " 3. Visualizar Calendario Mensal\n";
    std::cout << " 4. Listar Todos os Colaboradores\n";
    std::cout << " 5. Procurar Colaborador (Dados e Calendario)\n"; 
    std::cout << " 6. Gerir Formacoes e Notas (CRUD)\n";
    std::cout << "--------------------------------------------\n";
    std::cout << " 7. DASHBOARD (Resumo Anual)\n"; 
    std::cout << " 8. Relatorio Mensal (Ausencias por Colaborador)\n";
    std::cout << " 9. Estatisticas de Departamento\n"; 
    std::cout << "10. Exportar Dados (Calendario/Relatorio)\n"; 
    std::cout << "--------------------------------------------\n";
    std::cout << COR_VERMELHA << "0. Guardar e Sair\n" << RESET_COR;
    std::cout << "--------------------------------------------\n";
    std::cout << "Escolha uma opcao (1 - 10 em que 0 - Sair): ";
}

// Função de ligação entre o menu e a marcação 
void menuGerirMarcacoes(std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha colaboradores para gerir marcacoes.\n" << RESET_COR;
        return;
    }

    listarColaboradores(lista);
    std::string nome;
    std::cout << COR_AZUL << "\n--- GERIR MARCACOES ---\n" << RESET_COR;
    std::cout << "Coloque o nome do colaborador (ou ID): ";
    std::getline(std::cin >> std::ws, nome);

    bool isID = std::all_of(nome.begin(), nome.end(), ::isdigit);
    int indice = encontrarColaborador(lista, nome, isID);

    if (indice == -1) {
        std::cout << COR_VERMELHA << "ERRO: Colaborador nao encontrado.\n" << RESET_COR;
        return;
    }

    Colaborador& colab = lista[static_cast<size_t>(indice)];
    int op, dia, mes, ano;

    std::cout << "\n--- Gerir Marcacoes para " << colab.nome << " ---\n";
    std::cout << " 1. Marcar Ferias (F)\n";
    std::cout << " 2. Marcar Falta (X)\n";
    std::cout << " 3. Desmarcar (Remover)\n";
    std::cout << " Opcao: ";

    // Validação de input numérico
    if (!(std::cin >> op)) {
        std::cout << COR_AMARELA << "Input invalido. Operacao cancelada.\n" << RESET_COR;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cout << "Dia (dd): "; std::cin >> dia;
    std::cout << "Mes (mm): "; std::cin >> mes;
    std::cout << "Ano (aaaa): "; std::cin >> ano;

    // Validar se o input da data foi numérico
    if (std::cin.fail()) {
        std::cout << COR_VERMELHA << "ERRO: Data contem caracteres nao numericos. Operacao cancelada.\n" << RESET_COR;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (op == 3) {
        desmarcarDia(colab, dia, mes, ano);
    } else if (op == 1 || op == 2) {
        TipoMarcacao tipoMarcacao = (op == 1) 
            ? TipoMarcacao::FERIAS 
            : TipoMarcacao::FALTA;

        // Verificar Conflito de Férias 
        if (tipoMarcacao == TipoMarcacao::FERIAS) {
            bool conflito = verificarConflitoFerias(colab, dia, mes, ano);
            if (conflito) {
                // Continua a marcar mesmo com conflito (conforme o requisito)
                std::cout << COR_AMARELA << "A marcacao vai prosseguir, apesar do conflito no departamento.\n" << RESET_COR;
            }
        }

        marcarDia(colab, dia, mes, ano, tipoMarcacao);
    } else {
        std::cout << COR_VERMELHA << "Opcao invalida. Operacao cancelada.\n" << RESET_COR;
    }

    // Limpar o buffer de input após a leitura de números
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void menuVisualizarCalendario(const std::vector<Colaborador>& lista) {
    if (lista.empty()) {
        std::cout << COR_AMARELA << "AVISO: Nao ha colaboradores para visualizar.\n" << RESET_COR;
        return;
    }
    listarColaboradores(lista);

    std::string nome;
    std::cout << COR_AZUL << "\n--- VISUALIZAR CALENDARIO ---\n" << RESET_COR;
    std::cout << "Coloque o nome do colaborador (ou ID) para o calendario: ";
    std::getline(std::cin >> std::ws, nome);

    bool isID = std::all_of(nome.begin(), nome.end(), ::isdigit);
    int indice = encontrarColaborador(lista, nome, isID);

    if (indice == -1) {
        std::cout << COR_VERMELHA << "ERRO: Colaborador nao encontrado.\n" << RESET_COR;
        return;
    }

    int mes, ano;
    std::cout << "Mes (mm): "; std::cin >> mes;
    std::cout << "Ano (aaaa): "; std::cin >> ano;

    // Validar se o input da data foi numérico e limpar o buffer
    if (std::cin.fail()) {
        std::cout << COR_VERMELHA << "ERRO: Data contem caracteres nao numericos.\n" << RESET_COR;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    visualizarCalendario(lista[static_cast<size_t>(indice)], mes, ano);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpar buffer
}

int main() {
    std::vector<Colaborador> listaColaboradores;
    const std::string FICHEIRO_DADOS = "rh_data.dat";

    // Carregar dados ao iniciar
    carregarDados(listaColaboradores, FICHEIRO_DADOS);

    int opcao;

    while (true) {
        mostrarMenu();

        // Validação de input numérico e tratamento de buffer
        if (!(std::cin >> opcao)) {
            std::cout << COR_AMARELA << "Input invalido. Por favor, digite um numero (1-11).\n" << RESET_COR;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // O cin.ignore após a leitura de 'opcao' é movido para dentro dos switch/case onde é necessário,
        // mas é garantido que o buffer está limpo após o loop de validação acima.

        limparConsola();

        switch (opcao) {
            case 1:
                adicionarColaborador(listaColaboradores);
                break;
            case 2:
                menuGerirMarcacoes(listaColaboradores);
                break;
            case 3:
                menuVisualizarCalendario(listaColaboradores);
                break;
            case 4:
                listarColaboradores(listaColaboradores);
                break;
            case 5: // Buscar Colaborador
                procurarColaborador(listaColaboradores);
                break;
            case 6: // Gerir Formações e Notas
                menuGestaoColaborador(listaColaboradores);
                break;
            case 7: // Dashboard
                dashboardResumido(listaColaboradores);
                break;
            case 8: // Relatório Mensal
                relatorioMensal(listaColaboradores);
                break;
            case 9: // Estatísticas de Departamento
                estatisticasDepartamento(listaColaboradores);
                break;
            case 10: // Exportação
                exportarDados(listaColaboradores);
                break;
            case 11:
                // Guardar dados ao sair
                guardarDados(listaColaboradores, FICHEIRO_DADOS);
                std::cout << COR_VERDE << "Dados guardados. Obrigado!\n" << RESET_COR;
                return 0;
            default:
                std::cout << COR_VERMELHA << "Opcao invalida. Tente novamente.\n"<< RESET_COR;
                break;
        }
    }
}