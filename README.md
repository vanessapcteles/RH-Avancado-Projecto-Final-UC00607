# 🏢 Sistema de Gestão de Recursos Humanos

> **Projeto Final - UC 00607: Desenvolver Programas Complexos em Linguagem Estruturada**  
> Desenvolvido em C++ | ATEC - Centro de Formação Profissional

---

## 📋 Sobre o Projeto

Sistema completo de gestão de recursos humanos desenvolvido em C++ que permite gerir colaboradores, marcações de férias e faltas, formações, notas internas e gerar relatórios detalhados. O sistema inclui funcionalidades avançadas como encriptação de dados, interface colorida e exportação de relatórios.

### ✨ Características Principais

- 🔐 **Segurança de Dados**: Encriptação de nomes e departamentos usando Cifra de César
- 🎨 **Interface Colorida**: Sistema de cores para melhor experiência do utilizador
- 📊 **Relatórios Avançados**: Dashboards, relatórios mensais e estatísticas por departamento
- 📅 **Gestão de Calendário**: Visualização mensal com marcações de férias e faltas
- 📝 **CRUD Completo**: Gestão de formações e notas internas
- 💾 **Persistência de Dados**: Armazenamento automático em ficheiro
- 📤 **Exportação**: Exportar calendários e relatórios para ficheiros de texto

---

## 🚀 Começar

### Pré-requisitos

- **Compilador C++**: GCC/G++ (MinGW para Windows)
- **Sistema Operativo**: Windows, Linux ou macOS
- **Terminal**: PowerShell, CMD ou terminal Unix

### Compilação

```bash
g++ main.cpp calendario.cpp colaborador.cpp cores.cpp io.cpp reports.cpp -o main.exe
```

### Execução

```bash
./main.exe
```

---

## 📖 Funcionalidades

### 1️⃣ Gestão de Colaboradores

- **Adicionar Colaborador**: Registo de novos colaboradores com ID automático
- **Listar Colaboradores**: Visualização de todos os colaboradores registados
- **Procurar Colaborador**: Pesquisa por nome ou ID com exibição de dados e calendário
- **Gestão de Formações**: CRUD completo de cursos e formações
- **Gestão de Notas**: CRUD completo de observações internas

### 2️⃣ Gestão de Marcações

- **Marcar Férias**: Registo de dias de férias com validação
- **Marcar Faltas**: Registo de faltas
- **Desmarcar Dias**: Remoção de marcações
- **Deteção de Conflitos**: Aviso de conflitos de férias no departamento
- **Validação de Fins de Semana**: Impedimento de marcações em sábados e domingos

### 3️⃣ Visualização de Calendário

- **Calendário Mensal**: Visualização gráfica do mês com marcações
- **Legenda de Cores**: 
  - 🔵 **F** = Férias
  - 🔴 **X** = Falta
  - 🔷 Fim de Semana

### 4️⃣ Relatórios e Estatísticas

- **Dashboard Resumido**: Visão geral anual de todos os colaboradores
- **Relatório Mensal**: Ausências detalhadas por colaborador num mês específico
- **Estatísticas de Departamento**: Análise de ausências por departamento
- **Identificação de Padrões**: Departamento com mais ausências

### 5️⃣ Exportação de Dados

- **Exportar Calendário**: Ficheiro CSV com todas as marcações de um colaborador
- **Exportar Relatório de Departamento**: Ficheiro CSV com estatísticas anuais

---

## 🗂️ Estrutura do Projeto

```
Projeto/
├── main.cpp              # Ponto de entrada e menu principal
├── colaborador.h/cpp     # Gestão de colaboradores, formações e notas
├── calendario.h/cpp      # Gestão de calendário e marcações
├── cores.h/cpp           # Definições de cores para interface
├── io.h/cpp              # Entrada/saída, encriptação e persistência
├── reports.h/cpp         # Geração de relatórios e estatísticas
├── rh_data.txt           # Ficheiro de dados (encriptado)
└── README.md             # Este ficheiro
```

---

## 🔒 Segurança

### Encriptação de Dados

O sistema utiliza a **Cifra de César** com chave 3 para proteger informações sensíveis:

- ✅ **Encriptado**: Nomes e departamentos
- ❌ **Não encriptado**: IDs, dias de férias, calendário

**Exemplo:**
```
"Teste" → "Whvwh"
"IT"    → "LW"
```

### Formato do Ficheiro de Dados

```
Nome_Encriptado;Departamento_Encriptado;ID;Dias_Ferias;Calendario
```

**Exemplo:**
```
Whvwh;LW;1;22;328:F,329:F
```

---

## 🎨 Interface

### Sistema de Cores

| Cor | Utilização |
|-----|------------|
| 🔵 **Azul** | Títulos e cabeçalhos |
| 🟢 **Verde** | Mensagens de sucesso |
| 🟡 **Amarelo** | Avisos |
| 🔴 **Vermelho** | Erros |
| 🟣 **Roxo** | Informações especiais |
| 🔷 **Ciano** | Destaques |

### Menu Principal

```
============================================
 Mini-Sistema RH - Menu Principal 
============================================
 1. Adicionar Colaborador
 2. Gerir Marcacoes (Ferias/Faltas)
 3. Visualizar Calendario Mensal
 4. Listar Todos os Colaboradores
 5. Procurar Colaborador (Dados e Calendario)
 6. Gerir Formacoes e Notas (CRUD)
--------------------------------------------
 7. Dashboard (Resumo Anual)
 8. Relatorio Mensal (Ausencias por Colaborador)
 9. Estatisticas de Departamento
10. Exportar Dados (Calendario/Relatorio)
--------------------------------------------
0. Guardar e Sair
--------------------------------------------
```

---

## 📊 Exemplos de Utilização

### Adicionar um Colaborador

1. Escolher opção **1** no menu
2. Inserir nome do colaborador
3. Inserir departamento
4. Sistema atribui ID automaticamente

### Marcar Férias

1. Escolher opção **2** no menu
2. Selecionar colaborador (por nome ou ID)
3. Escolher opção **1** (Marcar Férias)
4. Inserir data no formato: `Dia Mes Ano`
   - Exemplo: `15 12 2025`

### Gerar Relatório Mensal

1. Escolher opção **8** no menu
2. Inserir mês (1-12)
3. Inserir ano (ex: 2025)
4. Visualizar relatório com férias e faltas de todos os colaboradores

---

## 🛠️ Tecnologias Utilizadas

- **Linguagem**: C++ (Standard C++11 ou superior)
- **Bibliotecas Standard**:
  - `<iostream>` - Entrada/saída
  - `<vector>` - Estruturas de dados dinâmicas
  - `<map>` - Mapeamento de calendário
  - `<fstream>` - Manipulação de ficheiros
  - `<algorithm>` - Algoritmos standard
  - `<iomanip>` - Formatação de output
  - `<ctime>` - Manipulação de datas

---

## 📝 Notas Técnicas

### Validações Implementadas

- ✅ Validação de datas (dias, meses, anos)
- ✅ Deteção de anos bissextos
- ✅ Impedimento de marcações em fins de semana
- ✅ Verificação de conflitos de férias
- ✅ Validação de input numérico
- ✅ Verificação de duplicados de nomes

### Algoritmos Destacados

- **Algoritmo de Zeller**: Cálculo do dia da semana
- **Cifra de César**: Encriptação/desencriptação de dados
- **Gestão de Calendário**: Mapeamento de 366 dias do ano

---

## 👨‍💻 Autor

**Projeto desenvolvido para a UC 00607**  
**Autor:** Vanessa Teles

---

## 📄 Licença

Este projeto foi desenvolvido para fins educacionais no âmbito da formação profissional.

---

## 🆘 Suporte

Para questões ou problemas:
1. Verificar se todos os ficheiros `.cpp` e `.h` estão no mesmo diretório
2. Confirmar que o compilador C++ está corretamente instalado
3. Verificar permissões de leitura/escrita no diretório do projeto

---

## 🎯 Requisitos Cumpridos

- ✅ **Requisito 1**: Relatórios mensais de ausências
- ✅ **Requisito 2**: Pesquisa de colaboradores com calendário
- ✅ **Requisito 3**: Estatísticas por departamento
- ✅ **Requisito 4**: Avisos de conflitos de férias
- ✅ **Requisito 5**: Dashboard resumido
- ✅ **Requisito 6**: CRUD de formações e cursos
- ✅ **Requisito 7**: CRUD de notas internas
- ✅ **Requisito 8**: Exportação de calendário e relatórios
- ✅ **Requisito 9**: Interface melhorada com cores
- ✅ **Requisito 10**: Encriptação de dados sensíveis

---

<div align="center">

**Desenvolvido com** ❤️ **em C++**

</div>
