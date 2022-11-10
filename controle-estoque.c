#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


const int MAX=50;// limitando o tamanho do vetor
int ordenado = 0;// variável p/ controlar a ordenação dos dados

// Tipo de dado especial (Registro)
typedef struct TProduto{
    long int codigo;
    int grupo, quantidade;
    char nome[40], desc[40];
    float v_compra, v_venda;
}Tproduto;
// Escopo do programa
void leitura(Tproduto estoque[], int *tamanho);//gera o arquivo .dat na primeira vez
void gravacao(Tproduto estoque[], int tamanho);// realiza a gravação dos dado no arquivo
int pesquisa(Tproduto estoque[], int codigo, int *tamanho);// busca e retorna a posição do produto no vetor
int pesquisabinaria(Tproduto estoque[], int chave, int tamanho);
int vazio(int tamanho);// função auxiliar para da pesquisa binária
void ordena(Tproduto estoque[], int tamanho); //ordena o cadastro dos produtos por código
void cadastrar(Tproduto estoque[], int *tamanho); //faz o cadastro dos produtos
void mostra(Tproduto estoque[], int chave); //exibe os dados do produto
void atualizar(Tproduto estoque[], int *tamanho); //Altera os dados do produto
void consultar(Tproduto estoque[], int *tamanho); //faz a busca pelo produto
void relatorio(Tproduto estoque[], int *tamanho);
//void confirmacao(Tproduto estoque[], int *tamanho, char textoConfirma[50]);

int main(){
	setlocale(LC_ALL,"");
	Tproduto estoque[MAX];
    int tamanho=0, opcao, opc;
    char nome[20];
    leitura(estoque, &tamanho);// abre o arquivo da base de dados
    do{
        printf ("\n 1- CADASTRAR\n");
        printf (" 2- ATUALIZAR\n");
        printf (" 3- EXCLUIR\n");
        printf (" 4- CONSULTAR\n");
    	printf (" 5- VENDER\n");
    	printf (" 6- RELATORIO\n");
        printf (" 0- SAIR\n");
        printf ("\nESCOLHA A OPCAO DESEJADA:\n");
        scanf ("%d",&opcao);
        system("cls");
        switch (opcao){
            case 1:{
            	cadastrar(estoque, &tamanho);
            	gravacao(estoque, tamanho);
				break;
			}        	
            case 2:{
            	atualizar(estoque, &tamanho);
            	gravacao(estoque, tamanho);
				break;
			}
			case 3:{
				excluir(estoque, &tamanho);
				gravacao(estoque, tamanho);
				break;
			}
			case 4:{
				consultar(estoque, &tamanho);
				break;
			}
			case 5:{
				
				break;
			}
			case 6:{
				relatorio(estoque, &tamanho);
				break;
			}	
            case 0: 
				printf("Obrigado por usar nosso sistema.\n");
				system("pause");
				exit(0);
				break;
            default: printf("OPCAO INVALIDA!");
        }
    }while (opcao!=0);
             gravacao(estoque, tamanho);// grava após realizada as operações
	return 0;
}

void leitura(Tproduto estoque[], int *tamanho){
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");	   /* abrimos para anexar, binário */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
//le os dados do arquivo
    while(!feof(arquivo)){
       fread (&estoque[*tamanho], sizeof (Tproduto) , 1, arquivo);
       (*tamanho)++;
    }
	(*tamanho)--;
//fecha o arquivo
	fclose(arquivo);
	return;
}

void gravacao(Tproduto estoque[], int tamanho){
    FILE *arquivo;
    int i;
    arquivo= fopen("estoque.dat", "w+b");	   /* abre e apaga o conteúdo do arquivo,binário */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
// grava a estrutura no arquivo
    for(i=0;i<tamanho;i++)  
       fwrite(&estoque[i], sizeof(Tproduto), 1, arquivo);
// Fecha o arquivo de dados
	fclose(arquivo);
	return;
}

int pesquisabinaria(Tproduto estoque[], int chave, int tamanho){
    if(vazio(tamanho)) //vetor vazio
       return -1;       
    if (! ordenado){
        ordena(estoque,tamanho);
        ordenado=1;
    }
    int inicio=0,final=tamanho, meio;
    while (inicio<=final){
        meio=(int)(inicio+final)/2;
        if (estoque[meio].codigo==chave)
           return meio; // encontrou
        if (estoque[meio].codigo<chave)
           inicio=meio+1;
        else
           final=meio-1;
    }
    return -1; // não encontrou
}

int pesquisa(Tproduto estoque[], int codigo, int *tamanho){
	if(*tamanho == 0){
		return -2;
	}
	int i;
	for(i; i < *tamanho; i++){
		if(estoque[i].codigo == codigo){
			return 1;
		}
	}
	return -1;
}

int vazio(int tamanho){
     if(tamanho==0){
        printf("\nREGISTRO VAZIO!\n");
        return 1;
     }
     return 0;
}

void ordena(Tproduto estoque[], int tamanho){
	int i,j;
    Tproduto aux;
    for(i=0;i<tamanho-1;i++)
        for(j=i+1;j<tamanho;j++)
          	if (estoque[i].codigo>estoque[j].codigo){
              	aux=estoque[i];
              	estoque[i]=estoque[j];
              	estoque[j]=aux;
            }
}

void cadastrar(Tproduto estoque[], int *tamanho){
	Tproduto aux;
	
	//Verifica se o arquivo está cheio
	if(*tamanho == MAX){
		printf("Arquivo cheio! \n");
		return;
	}
	
	// solicita o codigo do produto
	do{
		printf("Código do produto--------------------------: ");
		scanf("%ld", &aux.codigo);
		fflush(stdin);
		
		if(aux.codigo < 0){
			printf("O código não pode ser negativo.\n");
		}	
		
	}while(aux.codigo < 0);
	
	// Verifica se já existe um produto com o mesmo código
	if(pesquisa(estoque, aux.codigo, tamanho) > 0){
		printf("\nPRODUTO JA EXISTE\n");
		return;	
	}
	
	printf("Nome do produto----------------------------: ");
	gets(&aux.nome);
	fflush(stdin);
	
	printf("Descrição do produto-----------------------: ");
	gets(aux.desc);
	fflush(stdin);
	
	do{
		printf("Quantidade de produto em estoque-----------: ");
		scanf("%i", &aux.quantidade);
		fflush(stdin);
		if(aux.quantidade < 0){
			printf("A quantidade não pode ser negativa.\n");	
		}
	}while(aux.quantidade < 0);
	
	
	printf("Grupo do produto---------------------------: ");
	scanf("%i", &aux.grupo);
	fflush(stdin);

	do{
		printf("Preco de compra do produto-----------------: ");
		scanf("%f", &aux.v_compra);
		fflush(stdin);
		if(aux.v_compra < 0){
			printf("O preço de compra não pode ser negativo.\n");
		}
	}while(aux.v_compra < 0);
	
	aux.v_venda = aux.v_compra * 1.9;
	printf("Preco de venda do produto-----------------: %.2f\n", aux.v_venda);
	
	
	int validaCadastro;
	printf("As informações estão corretas?\n 1 - Sim   0 - Não \n");
	scanf("%i", &validaCadastro);
	fflush(stdin);
	if(validaCadastro == 1){
		
		estoque[*tamanho] = aux;
		(*tamanho) ++;
		ordenado = 0;
		
		printf("\nCadastro Efetuado com sucesso!\n\n");
	}
}

void mostra(Tproduto estoque[], int chave){
	printf("\n======================\n");
	printf("Código:           %ld \n", estoque[chave].codigo);
	printf("Nome:             %s \n", estoque[chave].nome);
	printf("Descricao:        %s \n", estoque[chave].desc);
	printf("Quantidade:       %i \n", estoque[chave].quantidade);
	printf("Grupo:            %i \n", estoque[chave].grupo);
	printf("Valor de compra:  R$ %.2f \n", estoque[chave].v_compra);
	printf("Valor de venda:   R$ %.2f \n", estoque[chave].v_venda);	
	printf("\n======================\n");
	return;
}

void atualizar(Tproduto estoque[], int *tamanho){
	
	Tproduto aux;
	
	int pos, cod, opc;
	char correto = "N";
	
	printf("Código: ");
	scanf("%i", &cod);
	fflush(stdin);
	
	pos = pesquisabinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		aux = estoque[pos];
		mostra(estoque, pos);
		
		printf("O que deseja alterar? \n");
		printf("1 - Nome do produto: \n");
		printf("2 - Descrição do produto: \n");
		printf("3 - Quantidade do produto: \n");
		printf("4 - Grupo do produto: \n");
		printf("5 - Valor de compra do produto: \n");
		printf("0 - Sair: \n");
		scanf("%i", &opc);
		fflush(stdin);
		
		switch(opc){
		case 1:
			printf("\nDigite o novo nome do produto: ");
			
			gets(&aux.nome);
			fflush(stdin);
			
			{
				int validaCadastro;
				printf("Confirmar alteração?\n 1 - Sim   0 - Não \n");
				scanf("%i", &validaCadastro);
				fflush(stdin);
				if(validaCadastro == 1){
					
					estoque[*tamanho] = aux;
					(*tamanho) ++;
					ordenado = 0;
					
					printf("\nNome atualizado!\n\n");
				}
			}
			
			break;
		case 2:
			printf("\nDigite a nova descrição do produto: ");
			gets(aux.desc);
			fflush(stdin);
			
			{
				int validaCadastro;
				printf("Confirmar alteração?\n 1 - Sim   0 - Não \n");
				scanf("%i", &validaCadastro);
				fflush(stdin);
				if(validaCadastro == 1){
					
					estoque[*tamanho] = aux;
					(*tamanho) ++;
					ordenado = 0;
					
					printf("\nDescrição atualizada!\n\n");
				}
			}
			break;
		case 3:
			do{
				printf("Quantidade de produto em estoque-----------: ");
				scanf("%i", &aux.quantidade);
				fflush(stdin);
				if(aux.quantidade < 0){
					printf("A quantidade não pode ser negativa.\n");	
				}
			}while(aux.quantidade < 0);
			
			{
				int validaCadastro;
				printf("Confirmar alteração?\n 1 - Sim   0 - Não \n");
				scanf("%i", &validaCadastro);
				fflush(stdin);
				if(validaCadastro == 1){
					
					estoque[*tamanho] = aux;
					(*tamanho) ++;
					ordenado = 0;
					
					printf("\nQuantidade atualizada!\n\n");
				}
			}
			
			break;
		case 4:
			printf("\nDigite o grupo do produto: ");
			scanf("%i", &aux.grupo);
			fflush(stdin);
			
			{
				int validaCadastro;
				printf("Confirmar alteração?\n 1 - Sim   0 - Não \n");
				scanf("%i", &validaCadastro);
				fflush(stdin);
				if(validaCadastro == 1){
					
					estoque[*tamanho] = aux;
					(*tamanho) ++;
					ordenado = 0;
					
					printf("\nGrupo atualizado!\n\n");
				}
			}
			break;
		case 5:
			do{
				printf("Preco de compra do produto-----------------: ");
				scanf("%f", &aux.v_compra);
				fflush(stdin);
				if(aux.v_compra < 0){
					printf("O preço de compra não pode ser negativo.\n");
				}
			}while(aux.v_compra < 0);
	
			aux.v_venda = aux.v_compra * 1.9;
			
			{
				int validaCadastro;
				printf("Confirmar alteração?\n 1 - Sim   0 - Não \n");
				scanf("%i", &validaCadastro);
				fflush(stdin);
				if(validaCadastro == 1){
					
					estoque[*tamanho] = aux;
					(*tamanho) ++;
					ordenado = 0;
					
					printf("\nPreço de compra atualizado!\n\n");
				}
			}

//			confirmacao(estoque, *tamanho, "Preço de compra atualizado");
			
			break;
		case 0:
			printf("Sair");
			break;
		default:
			printf("Opção Inválida");
			break; 
		}
	} else {
		printf("nao cadastrado");
	}
}

//void confirmacao(Tproduto estoque[], int *tamanho, char textoConfirma[50]){
//	int validaCadastro;
//	printf("Confirmar alteração?\n 1 - Sim   0 - Não \n");
//	scanf("%i", &validaCadastro);
//	fflush(stdin);
//	if(validaCadastro == 1){
//		
//		estoque[*tamanho] = aux;
//		(*tamanho) ++;
//		ordenado = 0;
//		
//		printf("\n%s!\n\n", textoConfirma);
//	}
//}	
	
void excluir(Tproduto estoque[], int *tamanho){
	if(*tamanho == 0){
		printf("Arquivo Vazio");
		return;
	}
	int pos, cod, opc;
	char correto = "N";
	
	printf("Código do produto que deseja excluir: ");
	scanf("%i", &cod);
	
	pos = pesquisabinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		mostra(estoque, pos);
		
		{
			int validaCadastro;
			printf("Confirmar exclusão?\n 1 - Sim   0 - Não \n");
			scanf("%i", &validaCadastro);
			fflush(stdin);
			if(validaCadastro == 1){
				
				int i;
				for(i = 0; i < (*tamanho) -1; i++){
					estoque[i] = estoque[i+1];
				}
				printf("\nProduto excluido\n");
			} else {
				printf("\nNão foi possivel remover o produto\n");
			}
		}		
	}
}

void consultar(Tproduto estoque[], int *tamanho){
	
	int pos, cod, opc;
	char correto = "N";
	
	printf("Pesquisar produto: \n");
	printf("1 - Pesquisar produto por código \n");
	printf("2 - Pesquisar produtos por nome \n");
	printf("0 - Sair \n");
	scanf("%i", &opc);
	fflush(stdin);
	
	switch(opc){
	case 1:
					
		{
			printf("Digite o código para pesquisa: ");
			scanf("%i", &cod);
			fflush(stdin);
			
			pos = pesquisabinaria(estoque, cod, *tamanho);
			if(pos >= 0){;				
				mostra(estoque, pos);
				system("pause");
				system("cls");
			} else {
				printf("Código não cadastrado");
			}
		}
		
		break;
	case 2:
		
		{
			printf("Digite o código para pesquisa: ");
			scanf("%i", &cod);
			fflush(stdin);
			
			pos = pesquisabinaria(estoque, cod, *tamanho);
			if(pos >= 0){;				
				mostra(estoque, pos);
				system("pause");
				system("cls");
			} else {
				printf("Código não cadastrado");
			}
		}
		
		break;
	case 0:
		printf("Sair");
		break;
	default:
		printf("Opção Inválida");
		break; 
	}
		
}

void relatorio(Tproduto estoque[], int *tamanho){
	int chave, pos, cod, opc;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	
	for(chave = 0; chave < *tamanho; chave++){
		mostra(estoque, chave);
	}
	
}
