#include "calendario.h"
#include "colaborador.h" // Necessário para aceder à struct Colaborador
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>
#include "cores.h" 

// ===============================================
// FUNÇÕES AUXILIARES DE DATA
// ===============================================

// Verifica se o ano é bissexto
bool isBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

// Retorna o número de dias num determinado mês
int diasNoMes(int mes, int ano) {
    if (mes < 1 || mes > 12) return 0;
    if (mes == 2) {
        return isBissexto(ano) ? 29 : 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31;
}

// Retorna o nome do mês em formato string
std::string nomeMes(int mes) {
    switch (mes) {
        case 1: return "Janeiro";
        case 2: return "Fevereiro";
        case 3: return "Marco";
        case 4: return "Abril";
        case 5: return "Maio";
        case 6: return "Junho";
        case 7: return "Julho";
        case 8: return "Agosto";
        case 9: return "Setembro";
        case 10: return "Outubro";
        case 11: return "Novembro";
        case 12: return "Dezembro";
        default: return "Mes Invalido";
    }
}

// --- Funções de Validação e Conversão ---

// Verifica se a data é válida
bool dataValida(int dia, int mes, int ano) {
    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1) {
        return false;
    }
    return dia <= diasNoMes(mes, ano);
}

// Converte a data (dia, mês, ano) no dia do ano (1 a 366)
int dataParaDiaDoAno(int dia, int mes, int ano) {
    if (!dataValida(dia, mes, ano)) return -1;
    int diaDoAno = 0;
    for (int m = 1; m < mes; ++m) {
        diaDoAno += diasNoMes(m, ano);
    }
    diaDoAno += dia;
    return diaDoAno;
}

// Retorna o dia da semana (0=Domingo, 1=Segunda, ..., 6=Sábado) usando o Algoritmo Zeller
int diaSemana(int dia, int mes, int ano) {
    // Algoritmo Zeller para Geração Gregoriana do Calendário
    int Y = ano;
    int m = mes;
    int d = dia;
    if (m < 3) {
        m += 12;
        Y -= 1;
    }
    int C = Y / 100;
    int K = Y % 100;
    
    // Formula de Zeller: h = (d + floor((13*(m+1))/5) + K + floor(K/4) + floor(C/4) - 2*C) mod 7
    // O resultado mod 7: 0=Sábado, 1=Domingo, 2=Segunda, ..., 6=Sexta
    int h = (d + (13 * (m + 1)) / 5 + K + (K / 4) + (C / 4) - 2 * C) % 7;
    
    // Convertemos para o nosso padrão: 0=Domingo, 1=Segunda, ..., 6=Sábado
    // O valor resultante de h tem Sábado=0, Domingo=1...
    // O nosso objetivo é Domingo=0, Segunda=1, ...
    int dia_semana_padrao;
    if (h == 0) { // Sábado
        dia_semana_padrao = 6;
    } else { // Domingo (1) a Sexta (6)
        dia_semana_padrao = h - 1;
    }
    return dia_semana_padrao;
}

// ===============================================
// FUNÇÕES DE MARCAÇÃO
// ===============================================

// Marcar um dia específico para um colaborador com um tipo de marcação
void marcarDia(Colaborador& colab, int dia, int mes, int ano, TipoMarcacao tipo) {
    if (!dataValida(dia, mes, ano)) {
        std::cout << COR_VERMELHA << "ERRO: Data invalida. Nao foi possivel marcar o dia.\n" << RESET_COR;
        return;
    }

    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
    int diaDaSemana = diaSemana(dia, mes, ano);

    // O fim de semana (Domingo=0, Sábado=6) não deve ser marcável como Férias/Falta
    if (diaDaSemana == 0 || diaDaSemana == 6) {
        std::cout << COR_AMARELA << "AVISO: O dia " << dia << "/" << mes << " e um Fim de Semana. Nao e necessario marcar Ferias/Falta.\n" << RESET_COR;
        // Se for Fim de Semana, registamos no mapa como FIM_SEMANA
        colab.calendario[diaDoAno] = TipoMarcacao::FIM_SEMANA;
        return;
    }

    // Se o dia já estiver marcado, avisa
    if (colab.calendario.count(diaDoAno)) {
        TipoMarcacao atual = colab.calendario[diaDoAno];
        if (atual != TipoMarcacao::LIVRE) {
            std::cout << COR_AMARELA << "AVISO: O dia ja estava marcado. Sobrescrevendo...\n" << RESET_COR;
        }
    }
    
    // Verificação de férias restantes (apenas para FERIAS)
    if (tipo == TipoMarcacao::FERIAS) {
        if (colab.dias_ferias_restantes > 0) {
            colab.calendario[diaDoAno] = tipo;
            std::cout << COR_VERDE << "Dia " << dia << "/" << mes << " marcado como FERIAS.\n" << RESET_COR;
        } else {
            std::cout << COR_VERMELHA << "ERRO: Nao ha dias de ferias restantes para marcar.\n" << RESET_COR;
        }
    } else if (tipo == TipoMarcacao::FALTA) {
        colab.calendario[diaDoAno] = tipo;
        std::cout << COR_VERDE << "Dia " << dia << "/" << mes << " marcado como FALTA.\n" << RESET_COR;
    } else {
         colab.calendario[diaDoAno] = TipoMarcacao::LIVRE;
         std::cout << COR_VERDE << "Dia " << dia << "/" << mes << " marcado como LIVRE.\n" << RESET_COR;
    }
}

// Desmarcar um dia
void desmarcarDia(Colaborador& colab, int dia, int mes, int ano) {
    if (!dataValida(dia, mes, ano)) {
        std::cout << COR_VERMELHA << "ERRO: Data invalida.\n" << RESET_COR;
        return;
    }
    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);

    if (colab.calendario.count(diaDoAno)) {
        colab.calendario.erase(diaDoAno);
        std::cout << COR_VERDE << "Dia " << dia << "/" << mes << " desmarcado (removido do calendario).\n" << RESET_COR;
    } else {
        std::cout << COR_AMARELA << "AVISO: O dia " << dia << "/" << mes << " nao estava marcado.\n" << RESET_COR;
    }
}

// ===============================================
// FUNÇÕES DE VISUALIZAÇÃO E RELATÓRIO
// ===============================================

// Função para obter o caractere de exibição
char getCharMarcacao(TipoMarcacao tipo) {
    switch (tipo) {
        case TipoMarcacao::FERIAS: return 'F';
        case TipoMarcacao::FALTA: return 'X';
        case TipoMarcacao::FIM_SEMANA:
        case TipoMarcacao::LIVRE:
        case TipoMarcacao::NAO_MARCADO:
        default: return ' ';
    }
}

// Função para obter a cor do caractere de exibição
const char* getCorMarcacao(TipoMarcacao tipo) {
    switch (tipo) {
        case TipoMarcacao::FERIAS: return COR_AZUL.c_str();
        case TipoMarcacao::FALTA: return COR_VERMELHA.c_str();
        case TipoMarcacao::FIM_SEMANA: return COR_CIANO.c_str();
        case TipoMarcacao::LIVRE: return COR_VERDE.c_str();
        case TipoMarcacao::NAO_MARCADO:  
        default: return RESET_COR.c_str();
    }

}

// Visualiza o calendário mensal detalhado do colaborador
void visualizarCalendario(const Colaborador& colab, int mes, int ano) {
    if (!dataValida(1, mes, ano)) {
        std::cout << COR_VERMELHA << "ERRO: Mes/Ano invalido para visualizacao.\n" << RESET_COR;
        return;
    }

    int totalDias = diasNoMes(mes, ano);
    int primeiroDiaDoMes = diaSemana(1, mes, ano); // 0=Domingo, 1=Segunda
    int diaDoAnoInicial = dataParaDiaDoAno(1, mes, ano);

    std::cout << COR_CIANO << "\n--- Calendario do " << nomeMes(mes) << " do ano " << ano << " ---\n" << RESET_COR;
    std::cout << COR_AZUL << std::setw(4) << "Dom" << std::setw(4) << "Seg" << std::setw(4) << "Ter" 
              << std::setw(4) << "Qua" << std::setw(4) << "Qui" << std::setw(4) << "Sex" << std::setw(4) << "Sab" << RESET_COR << "\n";
    std::cout << "--------------------------------\n";

    int diaAtual = 1;
    
    // Preenche espaços vazios antes do primeiro dia (a nossa semana começa em Domingo=0)
    for (int i = 0; i < primeiroDiaDoMes; ++i) {
        std::cout << std::setw(4) << "";
    }
    
    // Ajusta o loop para começar no dia correto da semana
    int diaDaSemanaIndex = primeiroDiaDoMes;

    for (diaAtual = 1; diaAtual <= totalDias; ++diaAtual) {
        int diaDoAno = diaDoAnoInicial + diaAtual - 1;
        TipoMarcacao tipo = TipoMarcacao::LIVRE;
        
        // Verifica se há marcação
        auto it = colab.calendario.find(diaDoAno);
        if (it != colab.calendario.end()) {
            tipo = it->second;
        } else {
            // Se não está no mapa, verifica se é Fim de Semana (para o destacar)
            int d_semana = diaSemana(diaAtual, mes, ano);
            if (d_semana == 0 || d_semana == 6) {
                tipo = TipoMarcacao::FIM_SEMANA;
            } else {
                tipo = TipoMarcacao::LIVRE;
            }
        }

        char marcador = getCharMarcacao(tipo);
        const char* cor = getCorMarcacao(tipo);
        
        std::cout << cor << std::setw(2) << diaAtual;
        
        // Exibe o marcador
        if (marcador != ' ') {
            std::cout << "(" << marcador << ")" << RESET_COR;
        } else {
            std::cout << std::setw(2) << "" << RESET_COR;
        }

        // Nova linha após o Sábado 
        if (diaDaSemanaIndex % 7 == 6) {
            std::cout << "\n";
            diaDaSemanaIndex = 0; // Reinicia para Domingo
        } else {
            diaDaSemanaIndex++;
        }
    }
    
    // Finaliza a linha se necessário
    if (diaDaSemanaIndex != 0) {
        std::cout << "\n";
    }
    std::cout << "--------------------------------\n";
    std::cout << "Legenda: F=Ferias, X=Falta \n";
}

// Conta o total de ausências (Férias e Faltas) num determinado ano
void contarAusencias(const Colaborador& colab, int ano, int& totalFerias, int& totalFaltas) {
    totalFerias = 0;
    totalFaltas = 0;

    for (const auto& par : colab.calendario) {
        if (par.second == TipoMarcacao::FERIAS) {
            totalFerias++;
        } else if (par.second == TipoMarcacao::FALTA) {
            totalFaltas++;
        }
    }
}

// Conta o total de ausências num determinado mês e ano
void contarAusenciasMes(const Colaborador& colab, int mes, int ano, int& totalFerias, int& totalFaltas) {
    totalFerias = 0;
    totalFaltas = 0;

    if (!dataValida(1, mes, ano)) return;

    int diaDoAnoInicial = dataParaDiaDoAno(1, mes, ano);
    int totalDias = diasNoMes(mes, ano);

    for (int diaDoMes = 1; diaDoMes <= totalDias; ++diaDoMes) {
        int diaDoAno = diaDoAnoInicial + diaDoMes - 1;
        
        auto it = colab.calendario.find(diaDoAno);
        if (it != colab.calendario.end()) {
            if (it->second == TipoMarcacao::FERIAS) {
                totalFerias++;
            } else if (it->second == TipoMarcacao::FALTA) {
                totalFaltas++;
            }
        }
    }
}

// Conta o total de ausências num determinado mês e ano (alias)
void contarAusenciasMensal(const Colaborador& colab, int mes, int ano, int& totalFerias, int& totalFaltas) {
    contarAusenciasMes(colab, mes, ano, totalFerias, totalFaltas);
}

// Verificar Conflito de Férias
bool verificarConflitoFerias(const Colaborador& colab, int dia, int mes, int ano, const std::vector<Colaborador>& todosColaboradores) {
    if (!dataValida(dia, mes, ano)) return false;
    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
    // Verificar se outros colaboradores do mesmo departamento já têm férias marcadas neste dia
    for (const auto& outroColab : todosColaboradores) {
        // Ignorar o próprio colaborador
        if (outroColab.id == colab.id) continue;
        
        // Verificar apenas colaboradores do mesmo departamento
        if (outroColab.departamento == colab.departamento) {
            // Verificar se este colaborador tem férias marcadas no mesmo dia
            auto it = outroColab.calendario.find(diaDoAno);
            if (it != outroColab.calendario.end() && it->second == TipoMarcacao::FERIAS) {
                std::cout << COR_AMARELA << "AVISO: O colaborador '" << outroColab.nome 
                          << "' do mesmo departamento ja tem ferias marcadas neste dia.\n" << RESET_COR;
                std::cout << COR_AZUL << "Deseja proceder? (S/N): " << RESET_COR;
                char resposta;
                std::cin >> resposta;
                if (resposta != 'S' && resposta != 's')
                {
                    return false;
                }
                else
                {
                    return true;
                } 
            }
        }
    }
    return false; // Sem conflito
}