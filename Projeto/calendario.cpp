#include "calendario.h"
#include "colaborador.h"
#include "reports.h"
#include "io.h"
#include "cores.h"
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <array>


// Função para marcar um dia específico no calendário do colaborador
void marcarDia(Colaborador& colab, int dia, int mes, int ano, TipoMarcacao tipo) {
    if (!dataValida(dia, mes, ano)) {
        std::cout << COR_VERMELHA << "ERRO: Data invalida. Operacao cancelada.\n" << RESET_COR;
        return;
    }

    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
    int diaSemAtual = diaSemana(dia, mes, ano);

    // Verificar se é Fim de Semana
    if (diaSemAtual == 0 || diaSemAtual == 6) { // 0=Domingo, 6=Sábado
        std::cout << COR_VERMELHA << "ERRO: Nao e possivel marcar Fins de Semana. Operacao cancelada.\n" << RESET_COR;
        return;
    }

    colab.calendario[diaDoAno] = tipo;
    std::cout << COR_VERDE << "Dia marcado com sucesso.\n" << RESET_COR;
}
// Função para desmarcar um dia específico no calendário do colaborador
void desmarcarDia(Colaborador& colab, int dia, int mes, int ano) {
    if (!dataValida(dia, mes, ano)) {
        std::cout << COR_VERMELHA << "ERRO: Data invalida. Operacao cancelada.\n" << RESET_COR;
        return;
    }
    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
    colab.calendario[diaDoAno] = TipoMarcacao::LIVRE;
    std::cout << COR_VERDE << "Dia desmarcado com sucesso.\n" << RESET_COR;
}
// Função para visualizar o calendário mensal do colaborador
void visualizarCalendario(const Colaborador& colab, int mes, int ano) {
    if (mes < 1 || mes > 12) {
        std::cout << COR_VERMELHA << "ERRO: Mes invalido. Operacao cancelada.\n" << RESET_COR;
        return;
    }

    std::cout << COR_AZUL << "\n--- Calendario de " << colab.nome << " para " 
              << nomeMes(mes) << " de " << ano << " ---\n" << RESET_COR;

    int diasNoMesAtual = diasNoMes(mes, ano);
    int primeiroDiaSemana = diaSemana(1, mes, ano); // 0=Domingo, 1=Segunda, ..., 6=Sábado

    // Cabeçalho do calendário
    std::cout << "Dom Seg Ter Qua Qui Sex Sab\n";
    std::cout << std::string(primeiroDiaSemana * 4, ' '); // Espaços iniciais
    for (int dia = 1; dia <= diasNoMesAtual; ++dia) {
        int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
        int diaSemAtual = diaSemana(dia, mes, ano);
        char marcador = ' ';
        std::string cor;
        obterMarcadorEcor(colab, diaDoAno, diaSemAtual, marcador, cor);
        std::cout << cor << std::setw(2) << dia << marcador << " " << RESET_COR;
        if ((diaSemAtual + dia) % 7 == 0) {
            std::cout << "\n"; // Nova linha ao fim da semana
        }
    }
    std::cout << "\n";
}
// Função para contar ausências anuais
void contarAusencias(const Colaborador& colab, int ano, int& totalFerias, int& totalFaltas) {
    for (const auto& entrada : colab.calendario) {
        int diaDoAno = entrada.first;
        TipoMarcacao tipo = entrada.second;
        // Converter diaDoAno para mês e ano
        int mesAtual = 1;
        int anoAtual = ano;
        int diaAcumulado = 0;
        while (mesAtual <= 12) {
            int diasNoMesAtual = diasNoMes(mesAtual, anoAtual);
            if (diaDoAno <= diaAcumulado + diasNoMesAtual) {
                break;
            }
            diaAcumulado += diasNoMesAtual;
            mesAtual++;
            if (mesAtual > 12) {
                mesAtual = 1;
                anoAtual++;
            }
        }
        if (anoAtual == ano) {
            if (tipo == TipoMarcacao::FERIAS) {
                totalFerias++;
            } else if (tipo == TipoMarcacao::FALTA) {
                totalFaltas++;
            }
        }
    }
}
// Função para contar ausências mensais
void contarAusenciasMensal(const Colaborador& colab, int mes, int ano, int& totalFerias, int& totalFaltas) {
    for (const auto& entrada : colab.calendario) {
        int diaDoAno = entrada.first;
        TipoMarcacao tipo = entrada.second;
        // Converter diaDoAno para mês e ano
        int mesAtual = 1;
        int anoAtual = ano;
        int diaAcumulado = 0;
        while (mesAtual <= 12) {
            int diasNoMesAtual = diasNoMes(mesAtual, anoAtual);
            if (diaDoAno <= diaAcumulado + diasNoMesAtual) {
                break;
            }
            diaAcumulado += diasNoMesAtual;
            mesAtual++;
            if (mesAtual > 12) {
                mesAtual = 1;
                anoAtual++;
            }
        }
        if (anoAtual == ano && mesAtual == mes) {
            if (tipo == TipoMarcacao::FERIAS) {
                totalFerias++;
            } else if (tipo == TipoMarcacao::FALTA) {
                totalFaltas++;
            }
        }
    }
}
// Função para verificar conflito de férias no departamento
bool verificarConflitoFerias(const Colaborador& colab, int dia, int mes, int ano) {
    int diaDoAno = dataParaDiaDoAno(dia, mes, ano);
    // Aqui você precisaria de acesso à lista de colaboradores do mesmo departamento
    // Esta função atualmente não tem esse acesso, então retornamos false por padrão
    return false; // Implementação de conflito depende do contexto externo
}
// Função auxiliar para obter marcador e cor para o calendário

void obterMarcadorEcor(const Colaborador& colab, int diaDoAno, int diaSemAtual, char& marcador, std::string& cor) {

    auto it = colab.calendario.find(diaDoAno);

    if (it != colab.calendario.end()) {

        switch (it->second) {
            case TipoMarcacao::FERIAS:
                marcador = 'F';
                cor = COR_VERDE;
                break;
            case TipoMarcacao::FALTA:
                marcador = 'X';
                cor = COR_ROXO;
                break;
            case TipoMarcacao::LIVRE:
                marcador = ' ';
                cor = "";
                break;
            default:
                marcador = ' ';
                cor = "";
                break;
        }
    } else {
        // Verificar se é fim de semana
        if (diaSemAtual == 0 || diaSemAtual == 6) {
            marcador = 'S';
            cor = COR_AMARELA;
        } else {
            marcador = ' ';
            cor = "";
        }
    }
}
