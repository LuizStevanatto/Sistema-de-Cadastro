#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <time.h>
#define MAX_PARTICIPANTES 100 // Mudar o maximo de participantes por aqui.
//Ao colocar essa struct fora do main, pode ser utilizada por qualquer função

/* Constantes */
#define T_NOME 50
#define Q_LINHAS 10
#define Q_COLUNAS 10

/* Tipos */
typedef enum{VAZIA,OCUPADA}TStatus;

int cadeiras[Q_LINHAS][Q_COLUNAS];

typedef struct
{
  char nome_pessoa[T_NOME];
  TStatus status;
  int n_linha,n_coluna;
  int ativo;
}TCadeira;

int op,cad;
FILE *arquivo;
FILE *matriz;

struct tp_assento{
  int coluna;
  int linha;
  int valor;
}assento[100];

struct cadastro_participantes
    {
      int numero_cadastro;
      int cadeira;
      char nome[45];
      int cpf;
      char email[45];
      int telefone;
      int verifc;
      struct tp_assento assento[100];
      int ativo; //verificar se palestrante esta ativo ou n para poder substituir com outros dados.
    }
    cad_palestra[MAX_PARTICIPANTES]; //nome da struct.

//login do ADM
struct administrador{
    char login[30];
    char senha[30];
}adm[1];

//Matriz reserva de cadeiras para ser usanda no switch case dentro das reservas.
int cadeira_reserva [10][10] =
{1,2,3,4,5,6,7,8,9,10,
11,12,13,14,15,16,17,18,19,20,
21,22,23,24,25,26,27,28,29,30,
31,32,33,34,35,36,37,38,39,40,
41,42,43,44,45,46,47,48,49,50,
51,52,53,54,55,56,57,58,59,60,
61,62,63,64,65,66,67,68,69,70,
71,72,73,74,75,76,77,78,79,80,
81,82,83,84,85,86,87,88,89,90,
91,92,93,94,95,96,97,98,99,100};

//Matriz ocupação para ser usada na verificação se o assento esta ocupado ou não.
int ocupacao[10][10];

//criando funções
void inicial();
void menu();
void cadastar();
void listar();
void Alterar();
void pesquisar();

/* Prototipos*/
void reserva_deficiente(void);
void reserva_prof_conv(void);
void reserva_part_gerais(void);
void sorteio(void);

/* Procedimentos */
/* Limpa todas as cadeiras */
void LimpaCadeiras(TCadeira cadeiras[][Q_COLUNAS]);

/* Mostra todas as cadeiras */
void MostraCadeirasDef(TCadeira cadeiras[][Q_COLUNAS]);
void MostraCadeirasProfConv(TCadeira cadeiras[][Q_COLUNAS]);
void MostraCadeirasGerais(TCadeira cadeiras[][Q_COLUNAS]);

//Main principal inicializando a função menu
int main ( int agrc, char const *arg[]){
  inicial();
}

//Tela de login
void inicial()
{
  setlocale(LC_ALL, "Portuguese");
  char login[30];
  char senha[30];

  strcpy(adm[0].login, "Admin");
  strcpy(adm[0].senha,"admin");

  system("clear");
  //system("cls");
  printf("\nSistema de controle de participantes para  a palestra.\n");
  printf("\n\t\tLogin: ");
  scanf("%s%*c", login);
  printf("\n\t\tSenha: ");
  scanf("%s%*c", senha);

  if((strcmp(adm[0].login, login)==0) || (strcmp(senha,adm[0].senha)==0))
  {
    system("clear");
    //system("cls");
    menu();
  }
  else
  {
    printf("\n\n\t[!] Login e/ou senha incorretos\n");
    system("sleep 03");
    inicial();
  }
}

//Função Menu
void menu ()
{
  setlocale(LC_ALL, "Portuguese");
  do{
    system("clear");
    //system("cls");
    printf("                Menu Principal               \n");
    printf("\n    ------------------------------------------");
    printf("\n    |  1  | Cadastrar Participante           |");
    printf("\n    ------------------------------------------");
    printf("\n    |  2  | Listar Participantes             |");
    printf("\n    ------------------------------------------");
    printf("\n    |  3  | Pesquisar Tiket                  |");
    printf("\n    ------------------------------------------");
    printf("\n    |  4  | Alterar Cadastro                 |");
    printf("\n    ------------------------------------------");
    printf("\n    |  5  | Sorteio                          |");
    printf("\n    ------------------------------------------");
    printf("\n    |  0  | Sair                             |");
    printf("\n    ------------------------------------------\n");
    scanf("%d",&op);
    getchar(); //limpar o Buffer
    switch (op){
      case 1:
        cadastar();
      break;

      case 2:
        listar();
      break;

      case 3:
        pesquisar();
      break;

      case 4:
        Alterar();
      break;

      case 5:
        sorteio();
      break;
      }
    getchar();
  }while(op!=0);
}

//Função cadastar participantes
void cadastar()
{
  setlocale(LC_ALL, "Portuguese");
  system ("clear");
  //system("cls");

  //Parametros  a serem linkados depois na struct
  int verifc,op,i,k,y,x,n;
  char tecla;
  
  arquivo = fopen("cadastro.dat", "ab+" );// Abrir arquivo binario
  if (arquivo == NULL)
  {
    printf("ERRO! O arquivo não foi aberto!\n");
  }
  else
  {
    do
    {
      for (i =0; i < MAX_PARTICIPANTES; i++)
      {
        system("clear");
        //system("cls");
        if (cad_palestra[i].ativo==0)
        {
          fflush(stdin);
            printf("\n    ---------------------------------------");
            printf("\n    |        CADASTRO PARTICIPANTE        |");
            printf("\n    ---------------------------------------\n\n");

          printf("\tDigite o número do participante: ");
          scanf("%i%*c",&cad_palestra[i].numero_cadastro);
          fflush(stdin);

          printf("\n\tNome do participante: ");
          scanf("%[^\n]s%*c", cad_palestra[i].nome);
          fflush(stdin);

          printf("\n\tDigite o cpf do participante: ");
          scanf("%i%*c", &cad_palestra[i].cpf);
          fflush(stdin);

          printf("\n\tDigite o telefone do participante: ");
          scanf("%i%*c", &cad_palestra[i].telefone);
          fflush(stdin);

          printf("\n\tDigite o email do participante: ");
          scanf("%[^\n]s%*c", cad_palestra[i].email);
          fflush(stdin);
          system("clear");
          //system("cls");

          //Verificação do participante
          do{
            printf("\n    ----------------------------------------------------------------------");
            printf("\n    |                ESCOLHA UMA DAS OPÇÕES ABAIXO                       |");
            printf("\n    ----------------------------------------------------------------------");
            printf("\n    |  1  | Participante com necessidades especiais                      |");
            printf("\n    ----------------------------------------------------------------------");
            printf("\n    |  2  | Participante professor                                       |");
            printf("\n    ----------------------------------------------------------------------");
            printf("\n    |  3  | Participante convidado do palestrante                        |");
            printf("\n    ----------------------------------------------------------------------");
            printf("\n    |  4  | Participante geral                                           |");
            printf("\n    ----------------------------------------------------------------------\n");

            scanf(" %d%*c", &op);
            if (op ==1)
            {
              verifc =1;
              reserva_deficiente();
              k=1;
            }
            else if(op ==2)
            {
              verifc =2;
              reserva_prof_conv();
              k=1;
            }
            else if(op ==3)
            {
              verifc =3;
              reserva_prof_conv();
              k=1;
            }
            else if(op ==4)
            {
              verifc = 4;
              reserva_part_gerais();
              k=1;
            }
            else
            {
              printf("Digite uma das opções!");
              k=0;
            }
          }while(k==0);
          cad_palestra[i].verifc = verifc;
          fflush(stdin);

          // Salvar  cadastro com Status ativo
          cad_palestra[i].ativo = 1;
          fflush(stdin);

          fwrite(&cad_palestra[i],sizeof(struct cadastro_participantes), 1, arquivo); //salvar struct Cadastro do participante.
          break; // para n cadastrar repetidamente o mesmo aluno quando aluno = 0.
        }
      }
      printf("\n1- Continuar\n0- Sair \n");
      scanf("%d", &op);
  }while(op!=0);
fclose(arquivo);
  }
}
//Listar todos os participantes salvos no aruqivo.
void listar(){
  setlocale(LC_ALL, "Portuguese");
  int n,i;
  system("clear");
  //system("cls");
  FILE *arquivo = fopen("cadastro.dat","rb+");
  if (arquivo==NULL)
  {
    printf("ERRO! o arquivo não foi aberto");
  }
  else
  {
    //Colocar o ponteiro no começo do arquivo par ele começar a ler os cadastros.
    rewind(arquivo);
  do{
    n=fread(&cad_palestra,sizeof(struct cadastro_participantes),1,arquivo);
    if(n!=0)
    {
      for(i=0;i<MAX_PARTICIPANTES;i++)
      {
        if (cad_palestra[i].ativo==1){
        printf("Numero do cadastro: %d",cad_palestra[i].numero_cadastro);
        printf("\nNome: %s",cad_palestra[i].nome);
        printf("\nCPF: %d",cad_palestra[i].cpf );
        printf("\nTelefone: %d",cad_palestra[i].telefone);
        printf("\nEmail: %s",cad_palestra[i].email);
        if(cad_palestra[i].verifc==1)
        {
          printf("\nVerificação: Participante com necessidade de assento especial");
        }
        else if(cad_palestra[i].verifc==2)
        {
          printf("\nVerificação: Professor");
        }
        else if(cad_palestra[i].verifc==3)
        {
          printf("\nVerificação: Convidado do Palestrante");
        }
        else
        {
          printf("\nVerificação: Participante Geral");
        }
        printf("\nNúmero da cadeira: %d",cad_palestra[i].cadeira);
        printf("\nAtivo: %d",cad_palestra[i].ativo);
        printf("\n\n*_*_**_*_**_*_**_*_**_*_**_*_**_*_**_*_*\n\n");
        break;
        }
      }
    }


  }while(n!=0);
  fclose(arquivo);
  }
}
//Alterar o cadastro do participante atraves do nome.
void Alterar(){
  setlocale(LC_ALL, "Portuguese");
  int n, i, alterar, retSeek;
  char nome[45];
  system("clear");
  //system("cls");

  FILE *arquivo;
  arquivo = fopen("cadastro.dat","rb+");
  if (arquivo==NULL)
  {
    printf("ERRO! o arquivo não foi aberto");
  }
  else
  {
    rewind(arquivo);
    printf("\nDigite o nome do participante para pesquisar: ");
    scanf("%[^\n]s%*c",nome);
    do{
      n=fread(&cad_palestra,sizeof(struct cadastro_participantes),1,arquivo);
      if(n!=0){
        for (i = 0; i < MAX_PARTICIPANTES; i++)
        {
          if (strstr(cad_palestra[i].nome, nome)!=NULL){ //Função prar comparar variavel
              printf("Numero do cadastro: %d",cad_palestra[i].numero_cadastro);

              printf("\n Nome: %s",cad_palestra[i].nome);

              printf("\n CPF: %d",cad_palestra[i].cpf );

              printf("\n Telefone: %d",cad_palestra[i].telefone);

              printf("\n Email: %s",cad_palestra[i].email);

              if(cad_palestra[i].verifc==1)
              {
                printf("\nVerificação: Participante com necessidade de assento especial");
              }
              else if(cad_palestra[i].verifc==2)
              {
                printf("\nVerificação: Professor");
              }
              else if(cad_palestra[i].verifc==3)
              {
                printf("\nVerificação: Convidado do Palestrante");
              }
              else
              {
                printf("\nVerificação: Participante Geral");
              }

              printf("\nNúmero da cadeira %d",cad_palestra[i].cadeira);

              printf("\n Ativo: %d",cad_palestra[i].ativo);

              printf("\n\n*_*_**_*_**_*_**_*_**_*_**_*_**_*_**_*_*\n\n");

              printf("Deseja alterar? (1-sim/ 0-não) ");
              scanf("%d%*c", &alterar);

              if(alterar == 0)
              {
                menu();
              }
              else
              {
                retSeek = fseek(arquivo, (-1)*sizeof(struct cadastro_participantes),SEEK_CUR);
                if(retSeek == 0)
                {
                  printf("Digite os novos dados:\n\n");

                  printf("Numero do cadastro a ser alterado %d ",cad_palestra[i].numero_cadastro);   

                  printf("\nNome do participante.............: ");
                  scanf("%[^\n]s%*c", cad_palestra[i].nome);
                  fflush(stdin);

                  printf("\nDigite o cpf do participante........: ");
                  scanf("%i%*c", &cad_palestra[i].cpf);
                  fflush(stdin);

                  printf("\nDigite o telefone do participante........: ");
                  scanf("%i%*c", &cad_palestra[i].telefone);
                  fflush(stdin);

                  printf("\nDigite o email do participantes.............: ");
                  scanf("%[^\n]s%*c", cad_palestra[i].email);
                  fflush(stdin);

                  fwrite(&cad_palestra[i],sizeof(struct cadastro_participantes), 1, arquivo);
                }
              }
            }
          }
        }
      }while(n!=0);
    }
  }

//Pesquisar Ticket do participante pelo nome ou pelo assento. 
void pesquisar(){
    setlocale(LC_ALL, "Portuguese");

    char nome[45];
    int op, n, i, z, assento;
    do{
        // system("cls");
        system("clear");
        FILE *arquivo;
        arquivo = fopen("cadastro.dat", "ab+");
        if (arquivo == NULL)
        {
          printf("ERRO! O arquivo não foi aberto!\n");
        }
        else
        {  

            printf("\n    -------------------------------------");
            printf("\n    |  ESCOLHA UMA DAS OPÇÕES ABAIXO    |");
            printf("\n    -------------------------------------");
            printf("\n    |  1  | Pesquisar pelo nome         |");
            printf("\n    -------------------------------------");
            printf("\n    |  2  | Pesquisar pelo assento      |");
            printf("\n    -------------------------------------\n");

          scanf("%d",&z);
        
        if (z==1)
        {
          rewind(arquivo);
          // system("cls");
          system("clear");
          printf("\nDigite o nome do participante para pesquisar: ");
          scanf("%s%*c",nome);
            do{
              n=fread(&cad_palestra,sizeof(struct cadastro_participantes),1,arquivo);
              if(n!=0){
                //fflush(stdin); 
                 for (i = 0; i < MAX_PARTICIPANTES; i++){
                  
                  if (strstr(cad_palestra[i].nome, nome)!=NULL){ //Função prar comparar variavel

                  
                  printf("Numero do cadastro: %d",cad_palestra[i].numero_cadastro);

                  printf("\n Nome: %s",cad_palestra[i].nome);

                  printf("\n CPF: %d",cad_palestra[i].cpf );

                  printf("\n Telefone: %d",cad_palestra[i].telefone);

                  printf("\n Email: %s",cad_palestra[i].email);

                  if(cad_palestra[i].verifc==1)
                  {
                    printf("\nVerificação: Participante com necessidade de assento especial");
                  }
                  else if(cad_palestra[i].verifc==2)
                  {
                    printf("\nVerificação: Professor");
                  }
                  else if(cad_palestra[i].verifc==3)
                  {
                    printf("\nVerificação: Convidado do Palestrante");
                  }
                  else
                  {
                    printf("\nVerificação: Participante Geral");
                  }

                  printf("\nNúmero da cadeira %d",cad_palestra[i].cadeira);

                  printf("\n Ativo: %d",cad_palestra[i].ativo);

                  printf("\n\n***********************************************************\n\n");
                }
              }
            }
          }while(n!=0);
          //printf("User not found");
        }
        else if(z==2)
        {
          rewind(arquivo);
          printf("\nDigite o assento do participante para pesquisar:\n ");
          scanf("%d%*c",&assento);
          do{
            n=fread(&cad_palestra,sizeof(struct cadastro_participantes),1,arquivo);
            if(n!=0){
              for (i = 0; i < MAX_PARTICIPANTES; i++){
                if (cad_palestra[i].cadeira == assento){//Função prar comparar variavel
                  printf("\n\nNumero do cadastro: %d",cad_palestra[i].numero_cadastro);
                  printf("\n Nome: %s",cad_palestra[i].nome);
                  printf("\n CPF: %d",cad_palestra[i].cpf );
                  printf("\n Telefone: %d",cad_palestra[i].telefone);
                  printf("\n Email: %s",cad_palestra[i].email);
                  if(cad_palestra[i].verifc==1)
                  {
                    printf("\nVerificação: Participante com necessidade de assento especial");
                  }
                  else if(cad_palestra[i].verifc==2)
                  {
                    printf("\nVerificação: Professor");
                  }
                  else if(cad_palestra[i].verifc==3)
                  {
                    printf("\nVerificação: Convidado do Palestrante");
                  }
                  else
                  {
                    printf("\nVerificação: Participante Geral");
                  }
                  printf("\nNúmero da cadeira %d",cad_palestra[i].cadeira);
                  printf("\n Ativo: %d",cad_palestra[i].ativo);
                  printf("\nA palestra ocorrerá no dia 20/12/2020 as 22:00 (BR)\n");
                  printf("\nA Palestra ocorrera na Av. Comendador Enzo Ferrari, 280 - Swift, Campinas - SP, 13045-770 ");
                  printf("\n\n************************************************************\n\n");
                }
              }
            }
          }while(n!=0);
          printf(" [!] Usuário não encontrado\n\n");
          system("sleep 03");
          pesquisar();

        }
        else
        {
          printf("\n\nPor favor digite 1 ou 2...\n\n");
        }
        printf("\nDigite 0 para sair ou 1 para nova pesquisa ");
        scanf("%d",&op);
        getchar();
    }
}while(op!=0);
}

//Reserva para participantes que precisam de assentos com necessidades especiais
void reserva_deficiente()
{
  setlocale(LC_ALL, "Portuguese");
  int y,x,i,cadeira_teste,voltar;
  TCadeira cadeiras[Q_LINHAS][Q_COLUNAS];
  /* Inicia o programa */
  // LimpaCadeiras(cadeiras);
  /* Mostra as cadeiras */
  MostraCadeirasDef(cadeiras);
  /* Pega uma cadeira disponivel */
    if (cad_palestra[i].assento[i].valor==0 ){
      do
      {
        do
        {
          do
          {
            printf("\nDigite a coluna da cadeira a ser ocupada:\t");
            scanf("%i",&cad_palestra[i].assento[i].coluna);
            cad_palestra[i].assento[i].linha = 3;
            cad_palestra[i].assento[i].linha=3;
            fflush(stdin);
            x = cad_palestra[i].assento[i].coluna;
            y = cad_palestra[i].assento[i].linha;

            if(x<1||x>5){
              printf("\nValor de linha ou coluna invalidos.\n");
            }
            else{
              if(cadeiras[y][x-1].status==OCUPADA){
              printf("\nEssa cadeira jÃ¡ estÃ¡ ocupada.");
              printf("\nEscolha outra cadeira.\n");
              fflush(stdin);
              } 
            }
          }while(x<1||x>5);
        }while(cadeiras[y][x-1].status==OCUPADA);
        x--;
        y--;cadeiras[y][x].status=OCUPADA;
        if(ocupacao[y][x]==0)
        {
          printf("\nDigite o número do cadastro:\t");    
          scanf("%d",&cadeira_teste); 
          fflush(stdin);
        switch(cadeira_teste)
        {
        case 1 :
          cad_palestra[0].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 2 :
          cad_palestra[1].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 3 :
          cad_palestra[2].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 4 :
          cad_palestra[3].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 5 :
          cad_palestra[4].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 6 :
          cad_palestra[5].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 7 :
          cad_palestra[6].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 8 :
          cad_palestra[7].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 9 :
          cad_palestra[8].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 10 :
          cad_palestra[9].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 11 :
          cad_palestra[10].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 12 :
          cad_palestra[11].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 13 :
          cad_palestra[12].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 14 :
          cad_palestra[13].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 15 :
          cad_palestra[14].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 16 :
          cad_palestra[15].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 17 :
          cad_palestra[16].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 18 :
          cad_palestra[17].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 19 :
          cad_palestra[18].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 20 :
          cad_palestra[19].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 21 :
          cad_palestra[20].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 22 :
          cad_palestra[21].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 23 :
          cad_palestra[22].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 24 :
          cad_palestra[23].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 25 :
          cad_palestra[24].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 26 :
          cad_palestra[25].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 27 :
          cad_palestra[26].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 28 :
          cad_palestra[27].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 29 :
          cad_palestra[28].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 30 :
          cad_palestra[29].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 31 :
          cad_palestra[30].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 32 :
          cad_palestra[31].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 33 :
          cad_palestra[32].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 34 :
          cad_palestra[33].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 35 :
          cad_palestra[34].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 36 :
          cad_palestra[35].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 37 :
          cad_palestra[36].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 38 :
          cad_palestra[37].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 39 :
          cad_palestra[38].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 40 :
          cad_palestra[39].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 41 :
          cad_palestra[40].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 42 :
          cad_palestra[41].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 43 :
          cad_palestra[42].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 44 :
          cad_palestra[43].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 45 :
          cad_palestra[44].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 46 :
          cad_palestra[45].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 47 :
          cad_palestra[46].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 48 :
          cad_palestra[47].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 49 :
          cad_palestra[48].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 50 :
          cad_palestra[49].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 51 :
          cad_palestra[50].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 52 :
          cad_palestra[51].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 53 :
          cad_palestra[52].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 54 :
          cad_palestra[53].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 55 :
          cad_palestra[54].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 56 :
          cad_palestra[55].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 57 :
          cad_palestra[56].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 58 :
          cad_palestra[57].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 59 :
          cad_palestra[58].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 60 :
          cad_palestra[59].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 61 :
          cad_palestra[60].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 62 :
          cad_palestra[61].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 63 :
          cad_palestra[62].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 64 :
          cad_palestra[63].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 65 :
          cad_palestra[64].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 66 :
          cad_palestra[65].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 67 :
          cad_palestra[66].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 68 :
          cad_palestra[67].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 69 :
          cad_palestra[68].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 70 :
          cad_palestra[69].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 71 :
          cad_palestra[70].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 72 :
          cad_palestra[71].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 73 :
          cad_palestra[72].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 74 :
          cad_palestra[73].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 75 :
          cad_palestra[74].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 76 :
          cad_palestra[75].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 77 :
          cad_palestra[76].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 78 :
          cad_palestra[77].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 79 :
          cad_palestra[78].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 80 :
          cad_palestra[79].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 81 :
          cad_palestra[80].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 82 :
          cad_palestra[81].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 83 :
          cad_palestra[82].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 84 :
          cad_palestra[83].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 85 :
          cad_palestra[84].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 86 :
          cad_palestra[85].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 87 :
          cad_palestra[86].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 88 :
          cad_palestra[87].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 89 :
          cad_palestra[88].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 90 :
          cad_palestra[89].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 91 :
          cad_palestra[90].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 92 :
          cad_palestra[91].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 93 :
          cad_palestra[92].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 94 :
          cad_palestra[93].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 95 :
          cad_palestra[94].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 96 :
          cad_palestra[95].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 97 :
          cad_palestra[96].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 98 :
          cad_palestra[97].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 99 :
          cad_palestra[98].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 100 :
          cad_palestra[99].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
      }
    voltar = 1; 
    }
    else
    {
      printf("Ocupada, escolha outra.");
      voltar = 0;
    }
  }while(voltar == 0);
  /* Mostra as cadeiras */
  MostraCadeirasDef(cadeiras);
  getchar();
  }  
}

void LimpaCadeiras(TCadeira cadeiras[][Q_COLUNAS])
{
  setlocale(LC_ALL, "Portuguese");
  int y,x;
  for(y=0; y<Q_LINHAS; y++)
  {
    for(x=0; x<Q_COLUNAS; x++)
    {
      cadeiras[y][x].n_linha=y+1;
      cadeiras[y][x].n_coluna=x+1;
      cadeiras[y][x].status=VAZIA;
      cadeiras[y][x].nome_pessoa[0]=0;
    }
  }   
}
//Mostrar na tela os assentos do teatro. 
void MostraCadeirasDef(TCadeira cadeiras[Q_LINHAS][Q_COLUNAS])
{
  setlocale(LC_ALL, "Portuguese");
  char tecla;
  int y,x;
  
  /* Mostra as cadeiras em formato de diagrama */
  printf("\nCadairas ocupadas estão assinaladas com \"O\" e cadeiras vazias estão com \"V\"\n  ");
  for(x=1; x<=5; x++)
    printf("%i ",x);
  for(y=2; y<3; y++)
  {
    printf("\n%i:",y+1);
    for(x=0; x<5; x++){
      if(ocupacao[y][x] == 1)
      {
        printf("O ");
      }
      else
      {
        printf("V ");
      }       
    }      
  }
}

void reserva_prof_conv()
{
  setlocale(LC_ALL, "Portuguese");
  int y,x,voltar,cadeira_teste;
  TCadeira cadeiras[Q_LINHAS][Q_COLUNAS];
  /* Inicia o programa */
  LimpaCadeiras(cadeiras);
  /* Mostra as cadeiras */
  MostraCadeirasProfConv(cadeiras);
  /* Pega uma cadeira disponivel */
  do
  {
    do
    {
      do
      {
        printf("\nDigite a linha da cadeira a ser ocupada:\t");
        scanf("%i",&y);
        printf("\nDigite a coluna da cadeira a ser ocupada:\t");
        scanf("%i",&x);
        getchar();
        if((x<1||x>Q_COLUNAS)||(y<1||y>2))
          printf("\nValor de linha ou coluna invalidos.\n");
        else if(ocupacao[y][x]==1)
        {
          printf("\nEssa cadeira jÃ¡ estÃ¡ ocupada.");
          printf("\nEscolha outra cadeira.\n");
          getchar();
        }
      }while((x<1||x>Q_COLUNAS)||(y<1||y>2));
    }while(ocupacao[y][x] == 1);
    x--;
    y--;
    cadeiras[y][x].status=OCUPADA;
    /* Pega o numero da pessoa */
    if(ocupacao[y][x]==0)
    {
      printf("\nDigite o número do cadastro:\t");
      scanf("%d",&cadeira_teste);
      switch(cadeira_teste)
      {
        case 1 :
          cad_palestra[0].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 2 :
          cad_palestra[1].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 3 :
          cad_palestra[2].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 4 :
          cad_palestra[3].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 5 :
          cad_palestra[4].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 6 :
          cad_palestra[5].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 7 :
          cad_palestra[6].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 8 :
          cad_palestra[7].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 9 :
          cad_palestra[8].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 10 :
          cad_palestra[9].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 11 :
          cad_palestra[10].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 12 :
          cad_palestra[11].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 13 :
          cad_palestra[12].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 14 :
          cad_palestra[13].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 15 :
          cad_palestra[14].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 16 :
          cad_palestra[15].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 17 :
          cad_palestra[16].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 18 :
          cad_palestra[17].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 19 :
          cad_palestra[18].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 20 :
          cad_palestra[19].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 21 :
          cad_palestra[20].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 22 :
          cad_palestra[21].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 23 :
          cad_palestra[22].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 24 :
          cad_palestra[23].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 25 :
          cad_palestra[24].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 26 :
          cad_palestra[25].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 27 :
          cad_palestra[26].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 28 :
          cad_palestra[27].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 29 :
          cad_palestra[28].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 30 :
          cad_palestra[29].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 31 :
          cad_palestra[30].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 32 :
          cad_palestra[31].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 33 :
          cad_palestra[32].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 34 :
          cad_palestra[33].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 35 :
          cad_palestra[34].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 36 :
          cad_palestra[35].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 37 :
          cad_palestra[36].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 38 :
          cad_palestra[37].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 39 :
          cad_palestra[38].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 40 :
          cad_palestra[39].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 41 :
          cad_palestra[40].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 42 :
          cad_palestra[41].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 43 :
          cad_palestra[42].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 44 :
          cad_palestra[43].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 45 :
          cad_palestra[44].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 46 :
          cad_palestra[45].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 47 :
          cad_palestra[46].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 48 :
          cad_palestra[47].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 49 :
          cad_palestra[48].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 50 :
          cad_palestra[49].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 51 :
          cad_palestra[50].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 52 :
          cad_palestra[51].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 53 :
          cad_palestra[52].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 54 :
          cad_palestra[53].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 55 :
          cad_palestra[54].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 56 :
          cad_palestra[55].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 57 :
          cad_palestra[56].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 58 :
          cad_palestra[57].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 59 :
          cad_palestra[58].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 60 :
          cad_palestra[59].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 61 :
          cad_palestra[60].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 62 :
          cad_palestra[61].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 63 :
          cad_palestra[62].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 64 :
          cad_palestra[63].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 65 :
          cad_palestra[64].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 66 :
          cad_palestra[65].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 67 :
          cad_palestra[66].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 68 :
          cad_palestra[67].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 69 :
          cad_palestra[68].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 70 :
          cad_palestra[69].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 71 :
          cad_palestra[70].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 72 :
          cad_palestra[71].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 73 :
          cad_palestra[72].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 74 :
          cad_palestra[73].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 75 :
          cad_palestra[74].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 76 :
          cad_palestra[75].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 77 :
          cad_palestra[76].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 78 :
          cad_palestra[77].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 79 :
          cad_palestra[78].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 80 :
          cad_palestra[79].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 81 :
          cad_palestra[80].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 82 :
          cad_palestra[81].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 83 :
          cad_palestra[82].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 84 :
          cad_palestra[83].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 85 :
          cad_palestra[84].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 86 :
          cad_palestra[85].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 87 :
          cad_palestra[86].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 88 :
          cad_palestra[87].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 89 :
          cad_palestra[88].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 90 :
          cad_palestra[89].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 91 :
          cad_palestra[90].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 92 :
          cad_palestra[91].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 93 :
          cad_palestra[92].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 94 :
          cad_palestra[93].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 95 :
          cad_palestra[94].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 96 :
          cad_palestra[95].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 97 :
          cad_palestra[96].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 98 :
          cad_palestra[97].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 99 :
          cad_palestra[98].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 100 :
          cad_palestra[99].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
      }
      MostraCadeirasProfConv(cadeiras);
      getchar();
      voltar = 1;
    }
    else
    {
     printf("Ocupado, escolha outro.");
     voltar = 0;
    }
  }while(voltar==0);
}

void MostraCadeirasProfConv(TCadeira cadeiras[][Q_COLUNAS])
{
  setlocale(LC_ALL, "Portuguese");
  char tecla;
  int y,x;
  /* Mostra as cadeiras em formato de diagrama */
  printf("\nCadairas ocupadas estão assinaladas com \"O\" e cadeiras vazias estão com \"V\"\n  ");
  for(x=1; x<=Q_COLUNAS; x++){
    printf("%i ",x);
  }  
  for(y=0; y<2; y++)
  {
    printf("\n%i:",y+1);
    for(x=0; x<Q_COLUNAS; x++){
      if(ocupacao[y][x]==1)
      {
        printf("O ");
      }
      else
      {
        printf("V ");
      }
    }    
  }
}

void reserva_part_gerais(void)
{
  setlocale(LC_ALL, "Portuguese");
  int y,x,cadeira_teste,voltar;
  TCadeira cadeiras[Q_LINHAS][Q_COLUNAS];
  /* Inicia o programa */
  // LimpaCadeiras(cadeiras);
  /* Mostra as cadeiras */
  MostraCadeirasGerais(cadeiras);
  /* Pega uma cadeira disponivel */

  do
  {
    do
    {
      do
      {
        printf("\nDigite a linha da cadeira a ser ocupada:\t");
        scanf("%i",&y);
        if(y==3)
        {
          do
          {
            printf("\nDigite a coluna da cadeira a ser ocupada:\t");
            scanf("%i",&x);
            if(x<6||x>Q_COLUNAS)
            {
              printf("\n [!] Coluna inválida.\n");
            }
            else if(cadeiras[y-1][x-1].status==OCUPADA)
            {
              printf("\nEssa cadeira já está ocupada.");
              printf("\nEscolha outra cadeira.\n");
              getchar();
            }
          }while(x<6||x>Q_COLUNAS);
          
        }
        else
        {
          printf("\nDigite a coluna da cadeira a ser ocupada:\t");
          scanf("%i",&x);
          getchar();
          if((x<1||x>Q_COLUNAS)||(y<1||y>Q_LINHAS))
          {
           printf("\nValor de linha ou coluna invalidos.\n");
          }
          else if(cadeiras[y-1][x-1].status==OCUPADA)
          {
            printf("\nEssa cadeira já está ocupada.");
            printf("\nEscolha outra cadeira.\n");
            getchar();
          }
        }
      }while((x<1||x>Q_COLUNAS)||(y<1||y>Q_LINHAS));
    }while(cadeiras[y-1][x-1].status==OCUPADA);
    x--;
    y--;
    cadeiras[y][x].status=OCUPADA;
    if(ocupacao[y][x]==0)
    {
      printf("\nDigite o número do cadastro:\t");
      scanf("%d",&cadeira_teste);
      switch(cadeira_teste)
      {
        case 1 :
          cad_palestra[0].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 2 :
          cad_palestra[1].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 3 :
          cad_palestra[2].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 4 :
          cad_palestra[3].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 5 :
          cad_palestra[4].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 6 :
          cad_palestra[5].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 7 :
          cad_palestra[6].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 8 :
          cad_palestra[7].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 9 :
          cad_palestra[8].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 10 :
          cad_palestra[9].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 11 :
          cad_palestra[10].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 12 :
          cad_palestra[11].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 13 :
          cad_palestra[12].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 14 :
          cad_palestra[13].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 15 :
          cad_palestra[14].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 16 :
          cad_palestra[15].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 17 :
          cad_palestra[16].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 18 :
          cad_palestra[17].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 19 :
          cad_palestra[18].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 20 :
          cad_palestra[19].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 21 :
          cad_palestra[20].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 22 :
          cad_palestra[21].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 23 :
          cad_palestra[22].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 24 :
          cad_palestra[23].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 25 :
          cad_palestra[24].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 26 :
          cad_palestra[25].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 27 :
          cad_palestra[26].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 28 :
          cad_palestra[27].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 29 :
          cad_palestra[28].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 30 :
          cad_palestra[29].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 31 :
          cad_palestra[30].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 32 :
          cad_palestra[31].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 33 :
          cad_palestra[32].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 34 :
          cad_palestra[33].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 35 :
          cad_palestra[34].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 36 :
          cad_palestra[35].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 37 :
          cad_palestra[36].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 38 :
          cad_palestra[37].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 39 :
          cad_palestra[38].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 40 :
          cad_palestra[39].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 41 :
          cad_palestra[40].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 42 :
          cad_palestra[41].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 43 :
          cad_palestra[42].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 44 :
          cad_palestra[43].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 45 :
          cad_palestra[44].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 46 :
          cad_palestra[45].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 47 :
          cad_palestra[46].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 48 :
          cad_palestra[47].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 49 :
          cad_palestra[48].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 50 :
          cad_palestra[49].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 51 :
          cad_palestra[50].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 52 :
          cad_palestra[51].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 53 :
          cad_palestra[52].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 54 :
          cad_palestra[53].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 55 :
          cad_palestra[54].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 56 :
          cad_palestra[55].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 57 :
          cad_palestra[56].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 58 :
          cad_palestra[57].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 59 :
          cad_palestra[58].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 60 :
          cad_palestra[59].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 61 :
          cad_palestra[60].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 62 :
          cad_palestra[61].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 63 :
          cad_palestra[62].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 64 :
          cad_palestra[63].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 65 :
          cad_palestra[64].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 66 :
          cad_palestra[65].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 67 :
          cad_palestra[66].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 68 :
          cad_palestra[67].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 69 :
          cad_palestra[68].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 70 :
          cad_palestra[69].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 71 :
          cad_palestra[70].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 72 :
          cad_palestra[71].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 73 :
          cad_palestra[72].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 74 :
          cad_palestra[73].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 75 :
          cad_palestra[74].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 76 :
          cad_palestra[75].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 77 :
          cad_palestra[76].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 78 :
          cad_palestra[77].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 79 :
          cad_palestra[78].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 80 :
          cad_palestra[79].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 81 :
          cad_palestra[80].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 82 :
          cad_palestra[81].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 83 :
          cad_palestra[82].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 84 :
          cad_palestra[83].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 85 :
          cad_palestra[84].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 86 :
          cad_palestra[85].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 87 :
          cad_palestra[86].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 88 :
          cad_palestra[87].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 89 :
          cad_palestra[88].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 90 :
          cad_palestra[89].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 91 :
          cad_palestra[90].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 92 :
          cad_palestra[91].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 93 :
          cad_palestra[92].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 94 :
          cad_palestra[93].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 95 :
          cad_palestra[94].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;

        case 96 :
          cad_palestra[95].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 97 :
          cad_palestra[96].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 98 :
          cad_palestra[97].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 99 :
          cad_palestra[98].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
        
        case 100 :
          cad_palestra[99].cadeira = cadeira_reserva[y][x];
          ocupacao[y][x] =1;
        break;
      }
    voltar = 1;
    }
    else
    {
      printf("Ocupada,escolha outra\n");
      voltar =0;
    }  
  }while(voltar==0);
  /* Mostra as cadeiras */
  MostraCadeirasGerais(cadeiras);
  getchar();
}

void MostraCadeirasGerais(TCadeira cadeiras[][Q_COLUNAS])
{
  setlocale(LC_ALL, "Portuguese");
  char tecla;
  int y,x;

  
  /* Mostra as cadeiras em formato de diagrama */
  printf("\nCadeiras ocupadas estão assinaladas com \"O\" e cadeiras vazias estão com \"V\"\n\n ");
  // Mostrar o restante de assento da fileira 3 tirando os assentos de deficientes
  printf("Linha:  ");
  for(x=6; x <=Q_COLUNAS; x++)
  {
     printf("%i  ",x);
  }
  printf("\nColuna: ");
  for(y=2; y<3; y++)
  {
    printf("%i:",y+1);
    for(x=5; x<Q_COLUNAS; x++)
    {
      if(cadeiras[y][x].status==OCUPADA)
      {
        printf("O  ");
      }   
      else
      {
        printf("V  ");
      }     
    }    
  }
printf("\n\n  ");
//Mostrar o restante de assentos para alunos
for(x=1; x<=Q_COLUNAS; x++)
{
  printf("%i ",x);
}   
  for(y=3; y<Q_LINHAS; y++)
  {
    printf("\n%i:",y+1);
    for(x=0; x<Q_COLUNAS; x++)
    {
      if(cadeiras[y][x].status==OCUPADA)
      {
        printf("O ");
      }
      else
      {
        printf("V ");
      }    
    }   
  }
}
void sorteio(void)
{setlocale(LC_ALL, "Portuguese");
    // system("cls");
    int r,a,z;
    system("cls");
    system("clear");
    srand((unsigned)time(NULL));
    for(a=0;a<1;a++)
    {
      fflush(stdin);
      printf("\n    ---------------------------------------");
      printf("\n            |        Sorteio        |");
      printf("\n    ---------------------------------------\n\n");

      r=rand()%100;
      printf("     %d é a poltrona premiada!!!!!!",r);
    }

    printf("\n          Procurar poltrona premiada?      ");
    printf("\n     --------------------------------------");
    printf("\n    |  1  | Sim                            |");
    printf("\n     --------------------------------------");
    printf("\n    |  2  | Não                            |");
    scanf("%d",&z);
    if(z==1)
    {
      pesquisar();
    }
    else{
     menu();
    }
    putchar('\n');
}


