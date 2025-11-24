#include "colaborador.h"
#include "calendario.h"
#include "io.h"
#include "reports.h"
#include "cores.h"

#include <ctime>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <cstddef>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>


// Função para limpar a consola (dependente do SO)
void limparConsola();

// Apresenta o menu principal 
void mostrarMenu();

// Função de ligação entre o menu e a marcação
void menuGerirMarcacoes(std::vector<Colaborador>& lista);
void menuVisualizarCalendario(const std::vector<Colaborador>& lista);

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
    std::cout << " Mini-Sistema RH - Menu Principal \n";
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
    std::string chave;
    std::cout << COR_AZUL << "\n--- Gerir Marcacoes ---\n" << RESET_COR;
    std::cout << "Coloque o nome do colaborador (ou ID): ";
    std::getline(std::cin >> std::ws, chave); // Usa getline para pegar o ID/Nome

    // Verifica se a chave fornecida é puramente numérica (ID)
    bool isID = std::all_of(chave.begin(), chave.end(), ::isdigit);
    // Usa a versão mais completa da função, assumida em io.cpp
    int indice = encontrarColaborador(lista, chave, isID); 

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
    std::cout << COR_AMARELA << " Opcao: " << RESET_COR;
    void limparConsola();

    // Lê a opção e as partes da data
    if (!(std::cin >> op >> dia >> mes >> ano)) {
        std::cout << COR_VERMELHA << "ERRO: Input invalido para Opcao, Dia, Mes ou Ano.\n" << RESET_COR;
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

    std::string chave;
    std::cout << COR_AZUL << "\n--- Visualizar Calendario ---\n" << RESET_COR;
    std::cout << "Coloque o nome do colaborador (ou ID) para o calendario: ";
    std::getline(std::cin >> std::ws, chave);

    bool isID = std::all_of(chave.begin(), chave.end(), ::isdigit);
    int indice = encontrarColaborador(lista, chave, isID);

    if (indice == -1) {
        std::cout << COR_VERMELHA << "ERRO: Colaborador nao encontrado.\n" << RESET_COR;
        return;
    }

    int mes, ano;
    std::cout << "Mes (mm): ";
    // Lê o mes e ano na mesma linha
    if (!(std::cin >> mes)) { 
        std::cout << COR_VERMELHA << "ERRO: Mes contem caracteres nao numericos.\n" << RESET_COR;
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; 
    }
    std::cout << "Ano (aaaa): "; 
    if (!(std::cin >> ano)) {
        std::cout << COR_VERMELHA << "ERRO: Ano contem caracteres nao numericos.\n" << RESET_COR;
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return; 
    }
    
    // Validar se a data é válida (função dataValida assumida em calendario.cpp)
    if (!dataValida(1, mes, ano)) {
        std::cout << COR_VERMELHA << "ERRO: Mes ou Ano invalido.\n" << RESET_COR;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpar buffer
        return;
    }

    visualizarCalendario(lista[static_cast<size_t>(indice)], mes, ano);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpar buffer após a leitura dos números
}

int main() {
    std::vector<Colaborador> listaColaboradores;
    const std::string FICHEIRO_DADOS = "rh_data.txt";
    
    // Carregar dados ao iniciar
    carregarDados(listaColaboradores, FICHEIRO_DADOS);

    int opcao;

    while (true) {
        mostrarMenu();

        // Validação de input numérico e tratamento de buffer
        if (!(std::cin >> opcao)) {
            std::cout << COR_AMARELA << "Input invalido. Por favor, digite um numero (0-10).\n" << RESET_COR;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        if (opcao != 0 && opcao != 7 && opcao != 9) { 
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        limparConsola();

        switch (opcao) {
            case 1:
                adicionarColaborador(listaColaboradores);
                limparConsola();
                break;
            case 2:
                menuGerirMarcacoes(listaColaboradores);
                limparConsola();
                break;
            case 3:
                menuVisualizarCalendario(listaColaboradores);
                limparConsola();
                break;
            case 4:
                listarColaboradores(listaColaboradores);
                std::cout << "Pressione ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                limparConsola();
                break;
            case 5: // Procurar Colaborador
                procurarColaborador(listaColaboradores);
                limparConsola();
                break;
            case 6: // Gerir Formações e Notas
                menuGestaoColaborador(listaColaboradores);
                limparConsola();
                break;
            case 7: // Dashboard Resumido
                dashboardResumido(listaColaboradores);
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                limparConsola();
                break;
            case 8: // Relatório Mensal
                relatorioMensal(listaColaboradores);
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.get();
                limparConsola();
                break;
            case 9: // Estatísticas de Departamento 
                estatisticasDepartamento(listaColaboradores);
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                limparConsola();
                break;
            case 10: // Exportação
                exportarDados(listaColaboradores);
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.get();
                limparConsola();
                break;
            case 0:
                // Guardar dados ao sair
                guardarDados(listaColaboradores, FICHEIRO_DADOS);
                std::cout << COR_VERDE << "Dados guardados. Obrigado!\n" << RESET_COR;
                return 0;
            default:
                std::cout << COR_VERMELHA << "Opcao invalida. Tente novamente.\n"<< RESET_COR;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Pressione ENTER para continuar...";
                std::cin.get();
                limparConsola();
                break;
        }
    }
}