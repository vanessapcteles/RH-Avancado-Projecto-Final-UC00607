#include "calendario.h"
#include "colaborador.h"
#include <iostream>
#include <iomanip>
#include <string>

// Cores para terminal (Códigos ANSI)
const std::string RESET_COR = "\033[0m"; // Reseta cor/estilo para o padrão
const std::string COR_FERIAS = "\033[34m"; // Azul (para Ferias 'F')
const std::string COR_FALTA = "\033[31m"; // Vermelho (para Faltas 'X')
const std::string COR_FIM_SEMANA = "\033[90m"; // Cinzento Claro (para Fins de Semana)

// Função para calcular o dia da semana
// Usa a congruência de Zeller para determinar o dia da semana de qualquer data.
// Retorna: 0 = Domingo, 1 = Segunda, ..., 6 = Sábado
int diaSemana(int diaA, int mesA, int anoA) {
    if(mesA < 3) { mesA += 12; anoA--; }
    int k = anoA % 100;
    int j = anoA / 100;
    // Congruência de Zeller
    int h = (diaA + 13*(mesA+1)/5 + k + k/4 + j/4 + 5*j) % 7;
    // Ajusta o resultado para 0=Domingo, 6=Sábado
    int d = (h + 6) % 7; 
    return d;
}

// Função para calcular o número de dias num mês
// Esta função trata corretamente anos bissextos.
int diasNoMes(int mes, int ano) {
    switch(mes){
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: // Fevereiro
            // Regra do ano bissexto: divisível por 4, mas não por 100, ou divisível por 400
            return ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) ? 29 : 28;
        default: 
            // Em caso de mês inválido (não deveria acontecer se a dataValida for usada)
            return 30;
    }
}

// Função auxiliar para obter o nome do mês
std::string nomeMes(int mes){
    const std::string meses[] = {"Janeiro","Fevereiro","Marco","Abril","Maio","Junho",
                                 "Julho","Agosto","Setembro","Outubro","Novembro","Dezembro"};
    // Se o mês for válido (1 a 12), retorna o nome. Caso contrário, retorna string vazia.
    if (mes >= 1 && mes <= 12) {
        return meses[mes-1];
    }
    return "";
}

// --- Funções de Validação e Conversão ---

// Verifica se a data é válida (mês dentro do intervalo, dia dentro do número de dias do mês)
bool dataValida(int dia, int mes, int ano) {
    // Restrição de ano para um intervalo prático
    if (mes < 1 || mes > 12 || ano < 1900 || ano > 2100) { 
        return false;
    }
    // Verifica se o dia existe no mês e ano dado
    if (dia < 1 || dia > diasNoMes(mes, ano)) {
        return false;
    }
    return true;
}

// Converte a data (dia, mês, ano) no dia do ano (1 a 366)
int dataParaDiaDoAno(int dia, int mes, int ano) {
    if (!dataValida(dia, mes, ano)) return -1; // Data inválida

    int diaDoAno = 0;
    for (int m = 1; m < mes; ++m) {
        diaDoAno += diasNoMes(m, ano);
    }
    diaDoAno += dia;
    return diaDoAno;
}

// --- Funções de Marcação ---

// Função auxiliar para obter o nome do TipoMarcacao para feedback
std::string tipoParaString(TipoMarcacao tipo) {
    switch (tipo) {
        case TipoMarcacao::FERIAS: return "Ferias";
        case TipoMarcacao::FALTA: return "Falta";
        case TipoMarcacao::LIVRE: return "Livre";
        case TipoMarcacao::FIM_SEMANA: return "Fim de Semana";
        default: return "Desconhecido";
    }
}

void marcarDia(Colaborador& colab, int dia, int mes, int ano, TipoMarcacao tipo) {
    if (!dataValida(dia, mes, ano)) {
        std::cout << "ERRO: Data invalida.\n";
        return;
    }

    // Apenas marca FERIAS ou FALTA, ignorando outros tipos se forem passados
    if (tipo != TipoMarcacao::FERIAS && tipo != TipoMarcacao::FALTA) {
        std::cout << "ERRO: Tipo de marcacao nao permitido (apenas Ferias ou Falta).\n";
        return;
    }

    int dSemana = diaSemana(dia, mes, ano);
    if (dSemana == 0 || dSemana == 6) { // 0=Domingo, 6=Sábado
        std::cout << "AVISO: Nao e possivel marcar " << tipoParaString(tipo) << " ao fim de semana.\n";
        return;
    }

    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
    if (diaDoAno != -1) {
        // O calendário armazena a marcação no mapa usando o dia do ano como chave.
        colab.calendario[diaDoAno] = tipo; 
        std::cout << "Marcacao (" << tipoParaString(tipo) << ") para " << dia << "/" << mes << "/" << ano << " registada.\n";
    }
}

void desmarcarDia(Colaborador& colab, int dia, int mes, int ano) {
    if (!dataValida(dia, mes, ano)) {
        std::cout << "ERRO: Data invalida.\n";
        return;
    }
    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
    if (diaDoAno != -1) {
        // A função erase do std::map retorna 1 se um elemento foi removido, 0 caso contrário.
        if (colab.calendario.erase(diaDoAno)) {
            std::cout << "Marcacao para " << dia << "/" << mes << "/" << ano << " removida.\n";
        } else {
            std::cout << "AVISO: Nao havia marcacao para " << dia << "/" << mes << "/" << ano << ".\n";
        }
    }
}

// --- Funções de Visualização ---

// Determina o char ('F', 'X', ou ' ') e a cor a usar para um dia específico
void obterMarcadorEcor(const Colaborador& colab, int diaDoAno, int diaSemAtual, char& marcador, std::string& cor) {
    marcador = ' '; // Por defeito: Dia Livre
    cor = RESET_COR; // Por defeito: Cor normal

    // 1. Verificar se é Fim de Semana
    if (diaSemAtual == 0 || diaSemAtual == 6) { // 0=Domingo, 6=Sábado
        cor = COR_FIM_SEMANA;
        return; // Não verificamos marcações em FDS, apenas cor
    }
    
    // 2. Verificar Marcação (apenas em dias úteis)
    auto it = colab.calendario.find(diaDoAno);

    if (it != colab.calendario.end()) {
        // Marcação encontrada. O tipo é um TipoMarcacao::TIPO (scoped enum)
        switch (it->second) {
            case TipoMarcacao::FERIAS:
                marcador = 'F';
                cor = COR_FERIAS;
                break;
            case TipoMarcacao::FALTA:
                marcador = 'X';
                cor = COR_FALTA;
                break;
            default:
                // Se for LIVRE ou FIM_SEMANA (não deveria estar no mapa)
                break;
        }
    }
}


void visualizarCalendario(const Colaborador& colab, int mes, int ano) {
    if (!dataValida(1, mes, ano)) { // Verificar o mês/ano (o dia 1 é usado como placeholder)
        std::cout << "ERRO: Mes/Ano invalido.\n";
        return;
    }

    int diasMax = diasNoMes(mes, ano);
    // 0=Domingo, 1=Segunda, ..., 6=Sábado (para o dia 1 do mês)
    int primeiroDiaSemana = diaSemana(1, mes, ano); 
    // Vamos usar esta variável para saber em que coluna estamos, 0=Dom, 6=Sab
    int colunaDiaSemana = primeiroDiaSemana; 

    std::cout << "\n*** Calendario de " << colab.nome << " - " << nomeMes(mes) << "/" << ano << " ***\n";
    std::cout << " Dom | Seg | Ter | Qua | Qui | Sex | Sab \n";
    std::cout << "----------------------------\n";

    // Espaços iniciais (preenche até ao dia em que o mês começa)
    for (int i = 0; i < primeiroDiaSemana; ++i) {
        std::cout << "   |"; // 3 espaços para alinhar com ' D |'
    }

    for (int dia = 1; dia <= diasMax; ++dia) {
        if (colunaDiaSemana == 7) { // Nova semana
            std::cout << "\n";
            colunaDiaSemana = 0;
        }

        // 1. Obter o Dia do Ano para a chave do mapa
        int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
        // 2. Obter o dia da semana atual (0-6)
        int diaSemAtual = diaSemana(dia, mes, ano);
        
        char marcador_marcacao;
        std::string cor_dia;
        
        // 3. Determinar o marcador e a cor (com base na refatoração)
        obterMarcadorEcor(colab, diaDoAno, diaSemAtual, marcador_marcacao, cor_dia);

        // --- Impressão ---
        std::cout << cor_dia; // Define a cor para o próximo dia
        
        // Impressão do dia (usando std::setw(2) para alinhamento)
        std::cout << std::setw(2) << dia;
        
        // Imprime o marcador real (F, X ou espaço)
        std::cout << marcador_marcacao;
        std::cout << RESET_COR; // Reseta a cor após imprimir o dia
        
        // Imprime a barra de separação
        std::cout << "|";
        
        colunaDiaSemana++;
    }
    
    std::cout << "\n---------------------------\n";
}