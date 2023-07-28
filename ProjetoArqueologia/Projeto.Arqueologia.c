#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <locale.h>
#define MAXINPUT 100
#define MAX_EQUIPAS 10

int **matriz; //declaração da matriz
int linha, coluna;//declaraçao das variaveis linha e coluna

 
struct Jogador
{
  char nome[MAXINPUT];     // Campo para armazenar o nome do jogador
  int pontos;             // Campo para armazenar a pontuação do jogador
  int penalizacao;       // Campo para armazenar a penalização do jogador
  struct Jogador* next; // Ponteiro para o próximo jogador na lista encadeada
};

struct Equipa
{
  char nome[MAXINPUT];            // Campo para armazenar o nome da equipa
  struct Jogador* headJogadores; // Ponteiro para o primeiro jogador da equipa
  struct Equipa* next;          // Ponteiro para a próxima equipa na lista ligada
};


struct Equipa* Head = NULL;

void insertEquipaTop(struct Equipa newNode)
{
    struct Equipa* novaEquipa = (struct Equipa*)malloc(sizeof(struct Equipa)); // Alocando memória para uma nova estrutura Equipa
    strcpy(novaEquipa->nome, newNode.nome);                                   // Copiando o nome do novo nó para a nova equipa 
    novaEquipa->headJogadores = NULL;                                        // Definindo o ponteiro 'headJogadores' da nova equipa como NULL
    novaEquipa->next = Head;                                                // Aponta o ponteiro 'next' da nova equipa para o valor atual de 'Head'
    Head = novaEquipa;                                                     // Atualiza 'Head' para apontar para a nova equipa
}


//insere numa lista um jogador e define os parametros do mesmo
void insertJogadorTop(struct Equipa* equipa, struct Jogador newJogador)
{
    struct Jogador* novoJogador = (struct Jogador*)malloc(sizeof(struct Jogador)); // Aloca memória para o novo jogador
    strcpy(novoJogador->nome, newJogador.nome);                                   // Copia o nome e os pontos do novo jogador para o novo jogador alocado
    novoJogador->pontos = newJogador.pontos;
    novoJogador->penalizacao = 0;                                               // Define a penalização do novo jogador como 0
    novoJogador->next = equipa->headJogadores;                                 // Define o próximo jogador como o jogador atual no topo da lista de jogadores da equipa
     equipa->headJogadores = novoJogador;                                     // Atualiza o topo da lista de jogadores da equipa para o novo jogador
}

//faz a verificaçao no ficheiro txt quando é um número ou é uma string
int IsNumber(char* String)
{
    int i, len;
    int result = 1;

    len = strlen(String);           // Obtém o comprimento da string
    for (i = 0; i < len; i++)      // Loop através de cada caractere da string
    {
        if (!isdigit(String[i])) // Verifica se o caractere atual não é um dígito numérico
        {
            result = 0;        // Define o resultado como 0 para indicar que a string não é um número
            break;            // Interrompe o loop, pois um caractere não numérico foi encontrado
        }
    }
    return result;         // Retorna o resultado (1 se a string for um número, 0 caso contrário)
} 


//criaçao do menu que vai aparecer na linha de comando
void criaMenu() {
  printf("********************************\n");
  printf("********************************\n");
  printf("*      Jogo Arqueologia        *\n");
  printf("********************************\n");
  printf("*         Comandos             *\n");
  printf("********************************\n");
  printf("* LE - Listar Equipas em Jogo  *\n");
  printf("* RI - Riqueza                 *\n");
  printf("* TR - Terreno                 *\n");
  printf("* EE - Estrela da Equipa       *\n");
  printf("* CE - Escavação               *\n");
  printf("* CR - Chamar Reforço          *\n");
  printf("* AE - Adicionar Equipa        *\n");
  printf("* A - Ajuda                    *\n");
  printf("* S - Saida                    *\n");
  printf("* FP - Fechar Programa         *\n");
  printf("* O que pretende fazer?        *\n");
};

//Da setup do mapa criando uma matriz e colocando os tessouros dentro da matriz
void Setupmapa(){
  // Mensagem de boas-vindas e instruções para definir o tamanho do terreno e esconder os tesouros
  printf("Bem Vindo ao Concurso de Arqueologia!\nAntes de começar a jogar vamos definir o tamanho do terreno e esconder os tesouros!\n");
  
  // Solicita ao utilizador o número de linhas do terreno e armazena o valor em 'linha'
  printf("Digite o número de linhas do terreno: ");
  scanf("%d", &linha);

  // Solicita ao utilizador o número de colunas do terreno e armazena o valor em 'coluna'
  printf("Digite o número de colunas do terreno: ");
  scanf("%d", &coluna);

  // Aloca memória para a matriz com o tamanho definido pelo usuário
  matriz = (int **)malloc(linha * sizeof(int *));
  for(int i = 0; i < linha ; i++){
    matriz[i] = (int *)malloc(coluna * sizeof(int));
  }

  // Loop para preencher a matriz com os tesouros, com validação para valores negativos
  for(int i = 0; i < linha ; i++){
    for(int j = 0; j < coluna; j++){
      do {
        // Solicita ao utilizador o tesouro na posição [i+1, j+1] e armazena o valor em matriz[i][j]
        printf("Introduza o tesouro da posição [%i , %i]: ", i+1, j+1);
        scanf("%d",&matriz[i][j]);
        
        // Verifica se o valor é negativo e exibe uma mensagem de validação
        if(matriz[i][j] < 0)
          printf("0 - sem riqueza, > 0 - com riqueza.\n");
      } while (matriz[i][j] < 0);
    }
  }
}


//Calcula a riqueza total da matriz
int calcularriqueza(int **matriz, int linha, int coluna)
{
    // Variável para armazenar o total de riqueza
    int total = 0;
    
    // Loop para percorrer cada posição da matriz
    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            // Verifica se o valor na posição [i][j] é maior que 0
            if (matriz[i][j] > 0)
                // Se for maior que 0, adiciona o valor à variável total
                total += matriz[i][j];
        }
    }
    
    // Retorna o total de riqueza
    return total;
}


//Mostra o estado do terreno
void mostrarterreno(){
  // Mensagem indicando que o terreno será exibido
  printf("O terreno encontra-se neste estado:\n");
  
  // Loop para percorrer cada posição da matriz e exibir o estado do terreno
  for (int i = 0; i < linha; i++) {
    for (int j = 0; j < coluna; j++) {
      // Verifica se o valor na posição [i][j] da matriz é menor ou igual a 0
      if (matriz[i][j] <= 0 )  
        // Se for menor ou igual a 0, exibe um caractere indicando que não há riqueza
        printf("| - |");
      else
        // Se for maior que 0, exibe um caractere indicando a presença de riqueza
        printf("| * |");
    }
    // Quebra de linha após exibir cada linha da matriz
    printf("\n");
  }
}

//Cria uma nova equipa no ficheiro equipas.txt
void registarEquipa() {
    // Variáveis para armazenar o número de jogadores e o nome da equipa
    int numeroJogadores;
    char nomeEquipa[MAXINPUT];

    // Solicita ao utilizador o número de jogadores da equipa e armazena o valor em 'numeroJogadores'
    printf("Digite o número de jogadores da equipa: ");
    scanf("%d", &numeroJogadores);

    // Solicita ao utilizador o nome da equipa e armazena o valor em 'nomeEquipa'
    printf("Digite o nome da equipa: ");
    scanf(" %[^\n]", nomeEquipa);

    // Abre o arquivo "equipas.txt" em modo de adição (append)
    FILE* file = fopen("equipas.txt", "a");
    if (file == NULL) {
        // Verifica se houve erro ao abrir o arquivo
        printf("Erro ao abrir o ficheiro equipas.txt.\n");
        return;
    }

    // Escreve no arquivo o número de jogadores e o nome da equipa
    fprintf(file, "%d\n%s\n", numeroJogadores, nomeEquipa);

    // Loop para solicitar o nome de cada jogador e escrever no arquivo
    for (int i = 0; i < numeroJogadores; i++) {
        char nomeJogador[MAXINPUT];
        printf("Digite o nome do jogador %d: ", i+1);
        scanf(" %[^\n]", nomeJogador);
        fprintf(file, "%s", nomeJogador);
    }

    // Escreve uma quebra de linha para separar as equipas no arquivo
    fprintf(file, "\n");

    // Fecha o arquivo
    fclose(file);

    // Exibe uma mensagem de sucesso
    printf("Equipa registada com sucesso!\n");
}

//Remove o jogador da lista e liberta em memória esse jogador
struct Jogador* removeJogador(struct Jogador* head, const char* nome)
{
    // Verifica se a lista está vazia
    if (head == NULL)
        return NULL;

    // Verifica se o jogador a ser removido é o primeiro da lista
    if (strcmp(head->nome, nome) == 0)
    {
        // Guarda o próximo jogador após o jogador removido
        struct Jogador* nextJogador = head->next;
        
        // Libera a memória ocupada pelo jogador removido
        free(head);
        
        // Retorna o próximo jogador como a nova cabeça da lista
        return nextJogador;
    }

    // Percorre a lista para encontrar o jogador a ser removido
    struct Jogador* currentJogador = head;
    while (currentJogador->next != NULL)
    {
        if (strcmp(currentJogador->next->nome, nome) == 0)
        {
            // Guarda o jogador a ser removido
            struct Jogador* jogadorRemovido = currentJogador->next;
            
            // Atualiza o ponteiro para o próximo jogador após o jogador removido
            currentJogador->next = jogadorRemovido->next;
            
            // Libera a memória ocupada pelo jogador removido
            free(jogadorRemovido);
            
            // Retorna a cabeça da lista inalterada
            return head;
        }
        currentJogador = currentJogador->next;
    }

    // Se o jogador a ser removido não foi encontrado, retorna a cabeça da lista inalterada
    return head;
}

//comando responsável pela escavação 
void escavacao(int** matriz, int linha, int coluna)
{
    char equipaNome[MAXINPUT];
    printf("Digite o nome da equipa que vai iniciar a escavação: ");
    scanf(" %[^\n]", equipaNome);

    // Procura a equipa na lista de equipas
    struct Equipa* ptrEquipa = Head;
    while (ptrEquipa != NULL)
    {
        if (strcmp(ptrEquipa->nome, equipaNome) == 0)
        {
            printf("Equipa: %s\n", ptrEquipa->nome);
            struct Jogador* ptrJogador = ptrEquipa->headJogadores;

            // Verifica se existem jogadores na equipa
            if (ptrJogador == NULL)
            {
                printf("A equipa não possui jogadores.\n");
                return;
            }

            char jogadorNome[MAXINPUT];
            printf("Digite o nome do jogador que deseja escavar: ");
            scanf(" %[^\n]", jogadorNome);

            // Procura o jogador na equipa
            while (ptrJogador != NULL)
            {
                if (strcmp(ptrJogador->nome, jogadorNome) == 0)
                    break;
                ptrJogador = ptrJogador->next;
            }

            // Verifica se o jogador foi encontrado
            if (ptrJogador == NULL)
            {
                printf("Jogador '%s' não encontrado na equipa '%s'.\n", jogadorNome, ptrEquipa->nome);
                return;
            }

            int linhaescava, colunaescava;
            printf("Digite a linha da matriz que deseja escavar: ");
            scanf("%d", &linhaescava);
            printf("Digite a coluna da matriz que deseja escavar: ");
            scanf("%d", &colunaescava);

            // Verifica se as coordenadas estão dentro dos limites da matriz
            if (linhaescava < 1 || linhaescava > linha || colunaescava < 1 || colunaescava > coluna)
            {
                // Desqualifica o jogador
                printf("O jogador '%s' foi desqualificado por cavar fora dos limites da matriz.\n", ptrJogador->nome);
                ptrEquipa->headJogadores = removeJogador(ptrEquipa->headJogadores, ptrJogador->nome);

                // Verifica se a equipa não possui mais jogadores após a remoção
                if (ptrEquipa->headJogadores == NULL)
                {
                    printf("Equipa '%s' não possui mais jogadores.\n", ptrEquipa->nome);
                    ptrEquipa->headJogadores = NULL;  // Atualiza o ponteiro da equipa
                    return;
                }

                return;
            }

            // Verifica se a posição já foi escavada
            if (matriz[linhaescava - 1][colunaescava - 1] == -1)
            {
                printf("A posição [%d, %d] já foi escavada anteriormente.\n", linhaescava, colunaescava);
                ptrJogador->penalizacao++;
                printf("O jogador %s recebeu uma penalização. Penalizações atuais: %d\n", ptrJogador->nome, ptrJogador->penalizacao);

                return;
            }

            // Obtém o valor na posição da matriz
            int tesouro = matriz[linhaescava - 1][colunaescava - 1];

            if (tesouro > 0)
            {
                // Atribui o tesouro ao jogador
                ptrJogador->pontos += tesouro;
                printf("Tesouro encontrado! O jogador %s escavou o tesouro na posição [%d, %d].\n", ptrJogador->nome, linhaescava, colunaescava);
                printf("Pontos do jogador %s: %d\n", ptrJogador->nome, ptrJogador->pontos);
            }
            else
            {
                printf("Nenhum tesouro encontrado na posição [%d, %d].\n", linhaescava, colunaescava);
            }

            // Marca a posição como escavada (-1)
            matriz[linhaescava - 1][colunaescava - 1] = -1;

            return;  // Encontrou a equipa, sair da função
        }
        ptrEquipa = ptrEquipa->next;
    }

    printf("Equipa '%s' não encontrada.\n", equipaNome);
}

//comando para listar todas as equipas e os seus repetivos jogadores com os pontos e as penalizações
void printEquipas()
{
    struct Equipa* ptrEquipa = Head;
    printf("Lista de equipas em jogo:\n");

    // Percorre a lista de equipas
    while (ptrEquipa != NULL)
    {
        printf("Nome da equipa: %s\n", ptrEquipa->nome);
        struct Jogador* ptrJogador = ptrEquipa->headJogadores;

        // Verifica se a equipa possui jogadores
        if (ptrJogador == NULL)
        {
            printf("Equipa sem jogadores.\n");
        }
        else
        {
            printf("Jogadores:\n");

            // Percorre a lista de jogadores da equipa
            while (ptrJogador != NULL)
            {
                printf(" - %s (Pontos: %d | Penalizações: %d)\n", ptrJogador->nome, ptrJogador->pontos, ptrJogador->penalizacao);
                ptrJogador = ptrJogador->next;
            }
        }

        ptrEquipa = ptrEquipa->next;
    }
}

//Comando responsável por ver qual é o jogador da equipa com mais pontos
void estrelaequipa()
{
    char teamName[MAXINPUT];
    printf("Digite o nome da equipa: ");
    scanf("%s", teamName);

    struct Equipa* ptrEquipa = Head;

    // Percorre a lista de equipas
    while (ptrEquipa != NULL)
    {
        // Verifica se o nome da equipa atual é igual ao nome digitado pelo utilizador
        if (strcmp(ptrEquipa->nome, teamName) == 0)
        {
            struct Jogador* ptrJogador = ptrEquipa->headJogadores;
            struct Jogador* jogadorMaxPontos = NULL;
            int maxPontos = -1;

            // Percorre a lista de jogadores da equipa
            while (ptrJogador != NULL)
            {
                // Verifica se o jogador atual tem mais pontos que o máximo atual
                // ou se tem a mesma quantidade de pontos, mas um nome alfabeticamente menor
                if (ptrJogador->pontos > maxPontos || (ptrJogador->pontos == maxPontos && strcmp(ptrJogador->nome, jogadorMaxPontos->nome) < 0))
                {
                    maxPontos = ptrJogador->pontos;
                    jogadorMaxPontos = ptrJogador;
                }
                ptrJogador = ptrJogador->next;
            }

            if (jogadorMaxPontos != NULL)
            {
                printf("Estrela da equipa '%s':\n", teamName);
                printf("Nome: %s\n", jogadorMaxPontos->nome);
                printf("Pontos: %d\n", jogadorMaxPontos->pontos);
            }
            else
            {
                printf("A equipa '%s' não possui jogadores.\n", teamName);
            }

            return;  // Encontrou a equipa, sair da função
        }
        ptrEquipa = ptrEquipa->next;
    }

    printf("Equipa '%s' não encontrada.\n", teamName);
}

//Comando responsável por adicionar um novo jogador à equipa
void chamarreforco()
{
    char teamName[MAXINPUT];
    printf("Digite o nome da equipa para adicionar um novo jogador: ");
    scanf("%s", teamName);

    struct Equipa* ptrEquipa = Head;

    // Percorre a lista de equipas
    while (ptrEquipa != NULL)
    {
        // Verifica se o nome da equipa atual é igual ao nome digitado pelo utilizador
        if (strcmp(ptrEquipa->nome, teamName) == 0)
        {
            char playerName[MAXINPUT];
            printf("Digite o nome do novo jogador: ");
            scanf("%s", playerName);

            // Aloca memória para um novo jogador
            struct Jogador* novoJogador = (struct Jogador*)malloc(sizeof(struct Jogador));

            // Copia o nome do novo jogador para a estrutura
            strcpy(novoJogador->nome, playerName);

            // Define os pontos e penalizações do novo jogador como 0
            novoJogador->pontos = 0;
            novoJogador->penalizacao = 0;

            // Define o próximo jogador como o atual cabeça da lista de jogadores da equipa
            novoJogador->next = ptrEquipa->headJogadores;

            // Atualiza o ponteiro da cabeça da lista de jogadores para o novo jogador
            ptrEquipa->headJogadores = novoJogador;

            printf("Novo jogador adicionado à equipa '%s'.\n", teamName);

            return;  // Encontrou a equipa, sair da função
        }
        ptrEquipa = ptrEquipa->next;
    }

    printf("Equipa '%s' não encontrada.\n", teamName);
}

//função que calcula a pontuação de cada equipa
int calcularPontuacaoEquipa(struct Equipa *equipa)
{
    int pontuacao = 0;

    // Inicializa um ponteiro para percorrer a lista de jogadores da equipa
    struct Jogador *ptrJogador = equipa->headJogadores;

    // Percorre a lista de jogadores e acumula a pontuação de cada jogador na variável "pontuacao"
    while (ptrJogador != NULL) {
        pontuacao += ptrJogador->pontos;
        ptrJogador = ptrJogador->next;
    }

    // Retorna a pontuação total da equipa
    return pontuacao;
}

//Comando sair que vai mostrar as classificações das equipas no jogo
void sair(int **matriz, int linha, int coluna)
{
    int tesouros = 0;

    // Verificar se ainda há tesouros enterrados na matriz
    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            if (matriz[i][j] > 0) {
                tesouros++;
                break;
            }
        }
    }

    if (tesouros > 0) {
        printf("Ainda existem tesouros enterrados na matriz.\n");
    } else {
        printf("Todos os tesouros foram encontrados.\n");
    }

    // Obter a pontuação de todas as equipas
    int numEquipas = 0;
    struct Equipa *ptrEquipa = Head;
    while (ptrEquipa != NULL) {
        numEquipas++;
        ptrEquipa = ptrEquipa->next;
    }

    // Criar um array para armazenar as pontuações das equipas
    int pontuacoes[numEquipas];
    ptrEquipa = Head;
    for (int i = 0; i < numEquipas; i++) {
        pontuacoes[i] = calcularPontuacaoEquipa(ptrEquipa);
        ptrEquipa = ptrEquipa->next;
    }

    // Ordenar as pontuações das equipas em ordem decrescente
    for (int i = 0; i < numEquipas - 1; i++) {
        for (int j = 0; j < numEquipas - i - 1; j++) {
            if (pontuacoes[j] < pontuacoes[j + 1]) {
                int temp = pontuacoes[j];
                pontuacoes[j] = pontuacoes[j + 1];
                pontuacoes[j + 1] = temp;
            }
        }
    }

    // Mostrar a classificação das equipas por pontos (começando pela equipa com mais pontos)
    printf("Classificação das equipas por pontos:\n");
    for (int i = 0; i < numEquipas; i++) {
        ptrEquipa = Head;
        while (ptrEquipa != NULL) {
            if (calcularPontuacaoEquipa(ptrEquipa) == pontuacoes[i]) {
                if (pontuacoes[i] > 0) {
                    printf("Equipa: %s - Pontos: %d\n", ptrEquipa->nome, pontuacoes[i]);
                }
                break;
            }
            ptrEquipa = ptrEquipa->next;
        }
    }
}



int main(void) {
  // main function variables
  char line[MAXINPUT] = "";
  char ch;
  int numequipasjogo = 0;

  char nomesEquipas[MAX_EQUIPAS][100];

  Setupmapa();


  struct Equipa nodeEquipa;
  struct Jogador nodeJogador;

  // open file
  FILE* file;
  file = fopen("equipas.txt", "r");

  // check file error
  if (!file)
  {
      printf("Error opening the teams.dat file.\n");
  }
  else
  {
    while (fscanf(file, "%[^\n]\n", line) != EOF)
    {
      if (IsNumber(line))
      {
        fscanf(file, "%[^\n]\n", line);
        strcpy(nodeEquipa.nome, line);

        insertEquipaTop(nodeEquipa);
      }
      else
      {
        strcpy(nodeJogador.nome, line);

        insertJogadorTop(Head, nodeJogador);
      }
    }
    printf("Equipas Lidas com sucesso e registadas no Jogo\n");
  }

  char comando[3];

  // COMANDOS
  char saida[] = "S";
  char help[] = "A";
  char Riqueza[] = "RI";
  char Terreno[] = "TR";
  char EstrelaEquipa[] = "EE";
  char comandoEscavacao[] = "CE";
  char ChamarReforco[] = "CR";
  char AdicionarEquipa[] = "AE";
  char FecharPrograma[] = "FP";
  char ListarEquipas[] = "LE";

  criaMenu();

  while (strcmp(comando, FecharPrograma) != 0) {
    scanf(" %2s", comando);

    if (!strcmp(Riqueza, comando)){
      int riqueza = calcularriqueza(matriz, linha, coluna);
      printf("A riqueza total do terreno é: %d\n", riqueza);
    }
    else if (!strcmp(Terreno, comando)) {
      mostrarterreno();
    }else if (!strcmp(ListarEquipas, comando)) {
      printEquipas();
    }
    else if (!strcmp(EstrelaEquipa, comando)) {
      estrelaequipa();
    }
    else if (!strcmp(comandoEscavacao, comando)) {
      escavacao(matriz, linha, coluna);
    }
    else if (!strcmp(ChamarReforco, comando)) {
      chamarreforco();
    }
    else if (!strcmp(AdicionarEquipa, comando)) {
      registarEquipa();
    }
    else if (!strcmp(saida, comando)) {
      sair(matriz,linha,coluna);
    } else if (!strcmp(help, comando)) {
      criaMenu();
    } else {
      printf("****\n Comando invalido! \n****\n\n\n");
    }
  }
}