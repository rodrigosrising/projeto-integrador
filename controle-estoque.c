#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>


#define MAX 50 // limitando o tamanho do vetor
int ordenado = 0; // variável p/ controlar a ordenação dos dados

// Tipo de dado especial (Produto)
typedef struct Produto{
    long int codigo;
    int grupo, lucro;
    char nomeProduto[41], unidade[3], fornecedor[41];
    float quantidade, quantidadeDisponivel, estoque_min, custoInicial, custoTemporario, receita, precoCompra, precoVenda, valorLucro, vendidos;
}produto;

// Tipo de dado especial (Fornecedor)
typedef struct Fornecedor{
    long int codigo;
    char nomeFornecedor[40];
}Fornecedor;

// Inicializa o ID do próximo fornecedor a ser cadastrado
int proximoIDFornecedor = 1;

// Escopo do programa
void leitura(produto estoque[], int *tamanho);	// gera o arquivo .dat na primeira vez
void gravacao(produto estoque[], int tamanho);	// realiza a gravação dos dado no arquivo
int pesquisa(produto estoque[], int codigo, int *tamanho);	// busca e retorna a posição do produto no vetor
void ordena(produto estoque[], int tamanho);	// ordena o cadastro dos produtos por código
void ordenaPreco(produto estoque[], int tamanho);	// ordena o cadastro dos produtos por preço de venda
int pesquisaBinaria(produto estoque[], int index, int tamanho);
int pesquisaBinariaPreco(produto estoque[], int index, int tamanho);
int vazio(int tamanho);	// função auxiliar para da pesquisa binária
void camposCadastro(int largura, char titulo[]); // Formata campos do cadastro
void alinhaTexto(int largura, char titulo[]); // alinha titulos
char* uppercase(char *uppertext); // Deixa os caracteres maiusculos

//CRUD
void cadastrar(produto estoque[], int *tamanho);	// Faz o cadastro dos produtos
void atualizar(produto estoque[], int *tamanho); 	// Altera os dados do produto
void excluir (produto estoque[], int *tamanho);	// Remove o produto selecionado

//Consulta de produtos
void consultar(produto estoque[], int *tamanho);	
void pesquisaCodigo(produto estoque[], int *tamanho);	// consulta por codigo
void pesquisaDescricao(produto estoque[], int *tamanho);	// consulta por descricao

//Relatorios
void relatorio(produto estoque[], int *tamanho);
void relatorioGeral(produto estoque[], int *tamanho);	// Relatório Geral
void relatorioPreco(produto estoque[], int *tamanho);	// Relatório por Preço

//Relatorios Especiais
void relatorioEspecial(produto estoque[], int *tamanho);
void margemLucroMinima(produto estoque[], int *tamanho);
void estoqueAbaixoMinimo(produto estoque[], int *tamanho);
void produtosFornecedor(produto estoque[], int *tamanho);
void produtosUnidade(produto estoque[], int *tamanho);
void produtosComPrejuizo(produto estoque[], int *tamanho);

// Movimentação
void movimentacao(produto estoque[], int *tamanho);
void vender(produto estoque[], int *tamanho);
void comprar(produto estoque[], int *tamanho);

// Exibir dados
void mostraFicha(produto estoque[], int index); 	//exibe os dados do produto em formato de ficha
void mostraLista(produto estoque[], int index); 	//exibe os dados do produto em formato de lista
void mostraListaVenda(produto estoque[], int index);
void mostraListaCompra(produto estoque[], int index);

//Paginação
// 1 - mostraFicha(estoque, index)
// 2 - mostraLista(estoque, index)
void paginacao(produto estoque[], int numProdutos, int itensPagina, int tipoLista, char titulo[]); //tipoLista define qual modelo será exibido

int main(){
	setlocale(LC_ALL,"");
	produto estoque[MAX];
    int tamanho = 0, n_resultados = 0, opcao;
    leitura(estoque, &tamanho);// abre o arquivo da base de dados
    
    do{
    	
		alinhaTexto(80, "CONTROLE DE ESTOQUE");
        printf ("1 - CADASTRAR\n");
        printf ("2 - ATUALIZAR\n");
        printf ("3 - EXCLUIR\n");
        printf ("4 - CONSULTAR\n");
    	printf ("5 - RELATORIOS\n");
    	printf ("6 - RELATORIOS ESPECIAIS\n");
    	printf ("7 - MOVIMENTAÇÃO\n");
        printf ("0 - SAIR\n");
        printf ("\nESCOLHA A OPCAO DESEJADA:\n");
        scanf ("%d",&opcao);
        system("cls");
        switch (opcao){
            case 1:{
            	cadastrar(estoque, &tamanho); // ok
            	gravacao(estoque, tamanho);
				break;
			}        	
            case 2:{
            	atualizar(estoque, &tamanho); // ok
            	gravacao(estoque, tamanho);
				break;
			}
			case 3:{
				excluir(estoque, &tamanho); // ok
				gravacao(estoque, tamanho); 
				break;
			}
			case 4:{
				//Pesquisar produtos
				//1 - pesquisar por codigo
				//2 - pesquisar por titulo e mostrar todos os itens buscados
				consultar(estoque, &tamanho); // ok
				break;
			}
			case 5:{
				// Relatorios comuns
				// 1 - relatorio em ficha mostrando todos os produtos do estoque 2x2
				// 2 - relatorio de preços mostrando todos os produtos de 15x15
				relatorio(estoque, &tamanho); // ok
				
				break;
			}
			case 6:{
				// Relatorios especiais
				// 1 - Produtos com margem de lucro abaixo da minima
				
				relatorioEspecial(estoque, &tamanho);
				break;
			}
			case 7:{
				// Movimentação
				// 1 - Vendas
				movimentacao(estoque, &tamanho);
				break;
			}	
            case 0:
            	alinhaTexto(80, "CONTROLE DE ESTOQUE");
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

void leitura(produto estoque[], int *tamanho){
    FILE *arquivo;
  	arquivo= fopen("estoque.dat", "a+b");	   /* abrimos para anexar, binário */ 
  	if (!arquivo){
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
//le os dados do arquivo
    while(!feof(arquivo)){
       fread (&estoque[*tamanho], sizeof (produto) , 1, arquivo);
       (*tamanho)++;
    }
	(*tamanho)--;
//fecha o arquivo
	fclose(arquivo);
	return;
}

void gravacao(produto estoque[], int tamanho){
    FILE *arquivo;
    int i;
    arquivo= fopen("estoque.dat", "w+b");	   /* abre e apaga o conteúdo do arquivo,binário */ 
  	if (!arquivo){
  		alinhaTexto(80, "CONTROLE DE ESTOQUE");
  		printf("Erro ao abrir arquivo!");
  		return;
  	}
// grava a estrutura no arquivo
    for(i = 0; i < tamanho; i++)  
       fwrite(&estoque[i], sizeof(produto), 1, arquivo);
// Fecha o arquivo de dados
	fclose(arquivo);
	return;
}

int pesquisaBinaria(produto estoque[], int index, int tamanho){
    if(vazio(tamanho)) //vetor vazio
       return -1;       
    if (! ordenado){
        ordena(estoque,tamanho);
        ordenado = 1;
    }
    int inicio = 0, final = tamanho, meio;
    while (inicio <= final){
        meio = (int)(inicio + final) / 2;
        if (estoque[meio].codigo == index)
           return meio; // encontrou
        if (estoque[meio].codigo < index)
           inicio = meio + 1;
        else
           final = meio - 1;
    }
    return -1; // não encontrou
}

int pesquisaBinariaPreco(produto estoque[], int index, int tamanho){
    if(vazio(tamanho)) //vetor vazio
       return -1;       
    if (! ordenado){
        ordenaPreco(estoque,tamanho);
        ordenado = 1;
    }
    int inicio = 0, final = tamanho, meio;
    while (inicio <= final){
        meio = (int)(inicio + final) / 2;
        if (estoque[meio].codigo == index)
           return meio; // encontrou
        if (estoque[meio].codigo < index)
           inicio = meio + 1;
        else
           final = meio - 1;
    }
    return -1; // não encontrou
}

int pesquisa(produto estoque[], int codigo, int *tamanho){
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
     	alinhaTexto(80, "CONTROLE DE ESTOQUE");
        printf("REGISTRO VAZIO!\n");
        system("pause");
		system("cls");
        return 1;
     }
     return 0;
}

void ordena(produto estoque[], int tamanho){
	int i,j;
    produto aux;
    for(i=0;i<tamanho-1;i++)
        for(j=i+1;j<tamanho;j++)
          	if (estoque[i].codigo>estoque[j].codigo){
              	aux=estoque[i];
              	estoque[i]=estoque[j];
              	estoque[j]=aux;
            }
}

void ordenaPreco(produto estoque[], int tamanho){
	int i,j;
    produto aux;
    for(i=0;i<tamanho-1;i++)
        for(j=i+1;j<tamanho;j++)
          	if (estoque[i].precoVenda>estoque[j].precoVenda){
              	aux=estoque[i];
              	estoque[i]=estoque[j];
              	estoque[j]=aux;
            }
}

// Função responsável pelo cadastro de novos produtos
void cadastrar(produto estoque[], int *tamanho){
	produto aux;
	aux.vendidos = 0;
	//Verifica se o arquivo está cheio
	if(*tamanho == MAX){
		alinhaTexto(80, "CONTROLE DE ESTOQUE");
		printf("Arquivo cheio! \n");
		return;
	}
	
	alinhaTexto(80, "CADASTRAR PRODUTO");	
	// solicita o codigo do produto
	do{
		camposCadastro(50, "Código do produto");
		scanf("%ld", &aux.codigo);
		fflush(stdin);
		
		if(aux.codigo <= 0){
			printf("O código não pode ser igual a 0 ou negativo.\n");
		}	
		
	}while(aux.codigo <= 0);
	
	// Verifica se já existe um produto com o mesmo código
	if(pesquisa(estoque, aux.codigo, tamanho) > 0){
		system("cls");
		alinhaTexto(80, "CONTROLE DE ESTOQUE");
		printf("O CODIGO JÁ ESTÁ CADASTRADO EM OUTRO PRODUTO.\n");
		system("pause");
		system("cls");
		return;	
	}
	
	// Nome do produto
	do{
		camposCadastro(50, "Nome");
		uppercase(gets(aux.nomeProduto));
		fflush(stdin);
		if(aux.nomeProduto[0] == '\0'){
			printf("O campo descricao não pode ser vazio. \n");	
		}
	}while(aux.nomeProduto[0] == '\0');
	
	
	// Quantidade do produto
	do{
		camposCadastro(50, "Quantidade de produto em estoque");
		scanf("%f", &aux.quantidade);
		fflush(stdin);
		if(aux.quantidade < 0){
			printf("A quantidade não pode ser negativa.\n");	
		}
	}while(aux.quantidade < 0);
	
	
	// Unidade do produto
	do{
		camposCadastro(50, "Unidade [KG, PC, UN, LT]");
		uppercase(gets(aux.unidade));
		fflush(stdin);
		if(aux.unidade[0] == '\0'){
			printf("O campo unidade não pode ser vazio. \n");	
		}
	}while(aux.unidade[0] == '\0');
	
	
	// Estoque minimo
	do{
        camposCadastro(50, "Estoque minimo");
		scanf("%f", &aux.estoque_min);
		fflush(stdin);
		if(aux.estoque_min < 0){
			printf("A estoque minimo não pode ser negativo.\n");	
		}
	}while(aux.estoque_min < 0);
	
	
	// Grupo do produto
	do{
		printf("Grupo:\n[1] Ração\n[2] Medicamentos\n[3] Acessórios\n[4] Higiene\n[5] Brinquedos\n");
        camposCadastro(50, "Selecione uma Categoria");
		scanf("%i", &aux.grupo);
		fflush(stdin);

		if(aux.grupo < 1 || aux.grupo > 5){
			printf("Digite uma categoria válida\n");
		}
			
	}while(aux.grupo < 1 || aux.grupo > 5);
	
	// Fornecedor do produto
	do{
        camposCadastro(50, "Fornecedor");
		uppercase(gets(aux.fornecedor));
		fflush(stdin);
		if(aux.fornecedor[0] == '\0'){
			printf("O campo fornecedor não pode ser vazio. \n");	
		}
	}while(aux.fornecedor[0] == '\0');
	
	//Preço de compra do produto
	do{
        camposCadastro(50, "Preco de compra do produto");
		scanf("%f", &aux.precoCompra);
		fflush(stdin);
		if(aux.precoCompra < 0){
			printf("O preço de compra não pode ser negativo.\n");
		}
	}while(aux.precoCompra < 0);


	//Preço de venda do produto
	do{
        camposCadastro(50, "Preco de venda do produto");
		scanf("%f", &aux.precoVenda);
		fflush(stdin);
		if(aux.precoVenda < 0){
			printf("O preço de venda não pode ser negativo.\n");
		}
	}while(aux.precoVenda < 0);

	// o código a seguir faz o calculo inicial para retornar o lucro minimo em porcentagem.
	{
		aux.quantidadeDisponivel = aux.quantidade;
		aux.custoInicial = aux.precoCompra * aux.quantidade;
		
		aux.receita = 0;
		aux.custoTemporario = aux.custoInicial;
		aux.valorLucro = aux.receita - aux.custoTemporario;
	
		aux.lucro = aux.valorLucro/aux.custoTemporario * 100;		
	}
	
	int validaCadastro;
	printf("As informações estão corretas?\n 1 - Sim   0 - Não \n");
	scanf("%i", &validaCadastro);
	fflush(stdin);
	if(validaCadastro == 1){	
		estoque[*tamanho] = aux;
		(*tamanho) ++;
		ordenado = 0;
		system("cls");
		alinhaTexto(80, "CADASTRAR PRODUTO");
		printf("Cadastro Efetuado com sucesso!\n\n");
		system("pause");
		system("cls");
	} else {
		system("cls");
		alinhaTexto(80, "CADASTRAR PRODUTO");
		printf("Aperte ENTER para voltar ao menu\n");
		getchar();
		system("cls");
		return;
	}
}

// Função responsável pela atualização de alguns dados dos produtos já cadastrados
void atualizar(produto estoque[], int *tamanho){

 	//Não atualizar os seguintes campos: preço de compra, quantidade, fornecedor (esses itens fazem parte de outras funcionalidades do PI que foram sorteadas para outros grupos)
 	//Atualizar descricao, unidade, estoque_min, precoVenda
	produto aux;
	int pos, cod, opc;
	
	alinhaTexto(80, "EDITAR PRODUTO");
	
    camposCadastro(50, "Código do produto");
	scanf("%i", &cod);
	fflush(stdin);
	
	system("cls");
	alinhaTexto(80, "EDITAR PRODUTO");
	
	pos = pesquisaBinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		aux = estoque[pos];
		mostraFicha(estoque, pos);
		
		printf("\nO que deseja alterar? \n");
		printf("1 - Nome do produto: \n");
		printf("2 - Unidade do produto: \n");
		printf("3 - Grupo do produto: \n");
		printf("4 - Estoque minimo: \n");
		printf("5 - Valor de venda do produto: \n");
		printf("0 - Voltar ao menu\n");
		scanf("%i", &opc);
		fflush(stdin);
		
		switch(opc){
		case 1:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			// Nome do produto
			do{
                camposCadastro(50, "Digite o nova Nome");
				uppercase(gets(aux.nomeProduto));
				fflush(stdin);
				if(aux.nomeProduto[0] == '\0'){
					printf("O campo descricao não pode ser vazio. \n");	
				}
			}while(aux.nomeProduto[0] == '\0');
						
			break;
		case 2:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			// Unidade do produto
			do{
                camposCadastro(50, "Unidade [KG, PC, UN, LT, CX]");
				uppercase(gets(aux.unidade));
				fflush(stdin);
				if(aux.unidade[0] == '\0'){
					printf("O campo unidade não pode ser vazio. \n");	
				}
			}while(aux.unidade[0] == '\0');
			
			break;
		case 3:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			// Grupo do produto
			do{
				printf("Grupo:\n[1] Ração\n[2] Medicamentos\n[3] Acessórios\n[4] Higiene\n[5] Brinquedos\n");
                camposCadastro(50, "Selecione uma Categoria");
				scanf("%i", &aux.grupo);
				fflush(stdin);

				if(aux.grupo < 1 || aux.grupo > 5){
					system("cls");
					alinhaTexto(76, "EDITAR PRODUTO");
					printf("Digite uma categoria válida\n\n");
				}
					
			}while(aux.grupo < 1 || aux.grupo > 5); //aux.grupo != 1 && aux.grupo != 2 && aux.grupo != 3 && aux.grupo != 4 && aux.grupo != 5
			
			break;
		case 4:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			// Estoque minimo
			do{
                camposCadastro(50, "Estoque minimo");
				scanf("%f", &aux.estoque_min);
				fflush(stdin);
				if(aux.estoque_min < 0){
					printf("A estoque minimo não pode ser negativo.\n");	
				}
			}while(aux.estoque_min < 0);
			
			break;
			
		case 5:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			//Preço de venda do produto
			do{
                camposCadastro(50, "Preco de venda do produto");
				scanf("%f", &aux.precoVenda);
				fflush(stdin);
				if(aux.precoVenda < 0){
					printf("O preço de venda não pode ser negativo.\n");
				}
			}while(aux.precoVenda < 0);
			
			break;
		case 0:
			system("cls");
			return;
			
			break;
		default:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			printf("Opção Inválida");
			system("pause");
			system("cls");
			
			break; 
		}
		int validaCadastro;
		printf("As informações estão corretas?\n 1 - Sim   0 - Não \n");
		scanf("%i", &validaCadastro);
		fflush(stdin);
		if(validaCadastro == 1){
			estoque[pos] = aux;
			
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			printf("Cadastro atualizado com sucesso!\n");
			system("pause");
			system("cls");
		} else {
			system("cls");
		}
	} else {
		system("cls");
		alinhaTexto(80, "EDITAR PRODUTO");
		printf("nao existem produtos com esse código\n");
		system("pause");
		system("cls");
	}
}

// Função responsável pela exclusão de um produto.	
void excluir (produto estoque[], int *tamanho){
	if(*tamanho == 0){
		printf("\nREGISTRO VAZIO!\n\n");
		return;
	}
	int posicao, i, codigo;
	
	alinhaTexto(80, "EXCLUIR PRODUTO");
    camposCadastro(50, "Código do produto a ser excluido");

	scanf("%d", &codigo);
	fflush(stdin);
	
	system("cls");
	alinhaTexto(80, "EXCLUIR PRODUTO");
	posicao = pesquisaBinaria(estoque, codigo, *tamanho);
     
	if (posicao >= 0) {//encontrou registro
	    mostraFicha(estoque, posicao);
		
		int validaCadastro;
		printf(" Deseja excluir esse produto?\n 1 - Sim   0 - Não \n");
		scanf("%i", &validaCadastro);
		fflush(stdin);
		if(validaCadastro == 1){
			for (i = posicao; i < (*tamanho)-1; i++){
				estoque[i] = estoque[i + 1];
	    		(*tamanho)--;
			} // copia os dados do proximo registro para o anterior
		
            system("cls");
			alinhaTexto(80, "EXCLUIR PRODUTO"); // copia os dados do proximo registro para o anterior  
			printf("Produto removido!\n\n");
			system("pause");
			system("cls");
		} else {
			system("cls");
			alinhaTexto(80, "EXCLUIR PRODUTO");
	        printf("O registro não foi excluido!\n\n");
	        printf("Aperte ENTER para voltar ao menu\n");
			getchar();
			system("cls");
			return;
		}
		
	}else{
		system("cls");
		alinhaTexto(80, "EXCLUIR PRODUTO");
		printf("Registro não localizado!\n\n");
		printf("Aperte ENTER para voltar ao menu\n");
		getchar();
		system("cls");
		return;
	}       
}

// Consutar produtos (por código ou por texto)
void consultar(produto estoque[], int *tamanho){
	
	int opc;
	
	alinhaTexto(80, "PESQUISAR PRODUTO");
	
	printf("1 - Pesquisar produto por código \n");
	printf("2 - Pesquisar produtos por Nome \n");
	printf("0 - Voltar ao menu\n");
	scanf("%i", &opc);
	fflush(stdin);
	system("cls");
	switch(opc){
	case 1:	
		pesquisaCodigo(estoque, tamanho);
		
		break;
	case 2:
		pesquisaDescricao(estoque, tamanho);
		
		break;
	case 0:
		system("cls");
		return;
		
		break;
	default:
		system("cls");
		alinhaTexto(80, "PESQUISAR PRODUTO");
		printf("Opção Inválida\n");
		system("pause");
		system("cls");
		break; 
	}
		
}

// consulta por codigo
void pesquisaCodigo(produto estoque[], int *tamanho){
	
	int index, pos, cod, opc;
	
	alinhaTexto(80, "PESQUISAR POR CÓDIGO");
	printf("Digite o código para pesquisa: ");
	scanf("%i", &cod);
	fflush(stdin);
	system("cls");
	alinhaTexto(80, "PESQUISAR POR CÓDIGO");
	pos = pesquisaBinaria(estoque, cod, *tamanho);
	if(pos >= 0){			
		mostraFicha(estoque, pos);
		system("pause");
		system("cls");
	} else {
		system("cls");
		alinhaTexto(80, "PESQUISAR POR CÓDIGO");
		printf("Código não cadastrado \n");
		system("pause");
		system("cls");
	}
}

// consulta por descricao
void pesquisaDescricao(produto estoque[], int *tamanho){
	
	produto resultados[MAX];
	int n_resultados = 0;
	
	int index;
	
	alinhaTexto(80, "PESQUISAR POR NOME");
		
	char buscaNome[41];
	char *pesquisa;
	bool buscaResultado = false;

	printf("Buscar por: ");
	scanf("%s", &buscaNome);
	fflush(stdin);
	system("cls");
	
	alinhaTexto(80, "PESQUISAR POR NOME");

	for(index = 0; index < *tamanho; index++){
		pesquisa = strstr(estoque[index].nomeProduto, uppercase(buscaNome));
	
		if(pesquisa){
			resultados[n_resultados] = estoque[index];
			n_resultados++;
		}
	}
	
	for(index = 0; index < n_resultados; index++){
		buscaResultado = true;
		paginacao(resultados, n_resultados, 2, 1, "PESQUISAR POR NOME");
	}	
	
	if(buscaResultado == false){
		printf ("Sua busca por '%s' não encontrou nenhum produto.\n", uppercase(buscaNome));
		system("pause");
		system("cls");
	}
}

// Relatórios (geral e por preço)
void relatorio(produto estoque[], int *tamanho){
	
	int opc;
	
	alinhaTexto(80, "RELATÓRIOS");
	
	printf("1 - Relatório geral\n");
	printf("2 - Relatório por preços\n");
	printf("0 - Voltar ao menu\n");
	scanf("%i", &opc);
	fflush(stdin);
	system("cls");
	switch(opc){
	case 1:
		relatorioGeral(estoque, tamanho);
		
		break;
	case 2:
		relatorioPreco(estoque, tamanho);
		
		break;
	case 0:
		system("cls");
		return;
		
		break;
	default:
		system("cls");
		alinhaTexto(80, "RELATÓRIOS");
		printf("Opção Inválida\n");
		system("pause");
		system("cls");
		break; 
	}
}

// Relatorio Geral
void relatorioGeral(produto estoque[], int *tamanho){
	
	int index, pos, cod, opc;
	pos = pesquisaBinaria(estoque, cod, *tamanho);

	alinhaTexto(80, "RELATÓRIO GERAL");
				
	for(index = 0; index < *tamanho; index++){
		paginacao(estoque, *tamanho, 2, 1, "RELATÓRIO GERAL");
	}
	system("pause");
	system("cls");
}

// Relatorio por Preço
void relatorioPreco(produto estoque[], int *tamanho){
		
	int index, pos, cod;
	pos = pesquisaBinaria(estoque, cod, *tamanho);
	
	alinhaTexto(80, "RELATÓRIO POR PREÇO");
			
	printf("Código  Nome                                                      Preço     \n\n");

	for(index = 0; index < *tamanho; index++){
		paginacao(estoque, *tamanho, 5, 2, "RELATÓRIO POR PREÇO");
	}
	system("pause");
	system("cls");
}

// Relatórios Especiais (Listar produtos com margem de lucro abaixo da mínima)
void relatorioEspecial(produto estoque[], int *tamanho){
	int chave, pos, cod, opcao;
	pos = pesquisaBinaria(estoque, cod, *tamanho);
	
	alinhaTexto(80, "RELATÓRIOS ESPECIAIS");
	
	printf("1 - Produtos com margem de lucro abaixo da mínima\n");
	printf("2 - Produtos com estoque abaixo do mínimo\n");
	printf("3 - Produtos que estão sendo vendidos com prejuízo\n");
	printf("4 - Produtos fornecidos por um fornecedor\n");
	printf("5 - Produtos de acordo com sua unidade de venda\n");
	printf("0 - Voltar ao menu\n");
	scanf("%i", &opcao);
	fflush(stdin);
	system("cls");
	switch(opcao){
	case 1:
		margemLucroMinima(estoque, tamanho);
		break;
		
	case 2:
		estoqueAbaixoMinimo(estoque, tamanho);
		break;
		
	case 3:
		produtosComPrejuizo(estoque, tamanho);
		break;
		
	case 4: 
		produtosFornecedor(estoque, tamanho);
		break;
		
	case 5: 
		produtosUnidade(estoque, tamanho);
		break;
		
	case 0:
		system("cls");
		return;
		
		break;
		
	default:
		system("cls");
		alinhaTexto(80, "RELATÓRIOS ESPECIAIS");
		printf("Opção Inválida\n");
		system("pause");
		system("cls");
		break; 
	}
}

// Produtos com margem de lucro abaixo da mínima
void margemLucroMinima(produto estoque[], int *tamanho){
	
	produto resultados[MAX];
	int n_resultados = 0;
	
	int index, pos, cod;
	alinhaTexto(80, "Produtos com margem de lucro abaixo da mínima");
						
	for(index = 0; index < *tamanho; index++){
				
		if(estoque[index].lucro < 0){
			resultados[n_resultados] = estoque[index];
			n_resultados++;
		}
	}
	
	for(index = 0; index < n_resultados; index++){
		// buscaResultado = true;
		paginacao(resultados, n_resultados, 2, 1, "Produtos com estoque abaixo do mínimo");
	}
	
	system("pause");
	system("cls");
}

// Produtos com estoque abaixo do mínimo
void estoqueAbaixoMinimo(produto estoque[], int *tamanho){
	
	produto resultados[MAX];
	int n_resultados = 0;
	
	int index, pos, cod;
	alinhaTexto(80, "Produtos com estoque abaixo do mínimo");
						
	for(index = 0; index < *tamanho; index++){
		if(estoque[index].quantidadeDisponivel < estoque[index].estoque_min){
			resultados[n_resultados] = estoque[index];
			n_resultados++;
		}
	}
	
	for(index = 0; index < n_resultados; index++){
		//buscaResultado = true;
		paginacao(resultados, n_resultados, 2, 1, "Produtos com estoque abaixo do mínimo");
	}
	
	system("pause");
	system("cls");
}

// Produtos que estão sendo vendidos com prejuízo
void produtosComPrejuizo(produto estoque[], int *tamanho){
	
	produto resultados[MAX];
	int n_resultados = 0;
	
	int index, pos, cod;
	alinhaTexto(80, "Produtos que estão sendo vendidos com prejuízo");
						
	for(index = 0; index < *tamanho; index++){
		if(estoque[index].precoVenda < estoque[index].precoCompra){
			resultados[n_resultados] = estoque[index];
            n_resultados++;
		}
	}
	
	for(index = 0; index < n_resultados; index++){
		//buscaResultado = true;
		paginacao(resultados, n_resultados, 2, 1, "Produtos que estão sendo vendidos com prejuízo");
	}
		
	system("pause");
	system("cls");
}

// Produtos por fornecedor
void produtosFornecedor(produto estoque[], int *tamanho){
	
	produto resultados[MAX];
	int n_resultados = 0;
	
	int index, pos, cod;
	alinhaTexto(80, "PESQUISAR POR FORNECEDOR");
		
	{
		char buscaFornecedor[41];
		char *pesquisa;
		bool buscaResultado = false;
	
		printf("Buscar por: ");
		scanf("%s", &buscaFornecedor);
		fflush(stdin);
		system("cls");
		
		alinhaTexto(80, "Produtos fornecidos por um fornecedor");

		for(index = 0; index < *tamanho; index++){
			pesquisa = strstr(estoque[index].fornecedor, uppercase(buscaFornecedor));
			
			if(pesquisa){
        		resultados[n_resultados] = estoque[index];
            	n_resultados++;
			}
		}
		
		for(index = 0; index < n_resultados; index++){
			buscaResultado = true;
			paginacao(resultados, n_resultados, 2, 1, "Produtos fornecidos por um fornecedor");
		}
		
		if(buscaResultado == false){
			printf ("Sua busca por '%s' não encontrou nenhum fornecedor.\n", uppercase(buscaFornecedor));
			system("pause");
			system("cls");
		}
	
	}
}

// Produtos por Unidade
void produtosUnidade(produto estoque[], int *tamanho){
	
	produto resultados[MAX];
	int n_resultados = 0;
	
	int index, pos, cod;
	alinhaTexto(80, "PESQUISAR POR UNIDADE");
		
	{
		char buscaUnidade[2];
		char *pesquisa;
		bool buscaResultado = false;
	
		printf("Buscar por [KG, PC, UN, LT, CX]: ");
		scanf("%s", &buscaUnidade);
		fflush(stdin);
		system("cls");
		
		alinhaTexto(80, "Produtos de acordo com sua unidade de venda");	
		
		
		for(index = 0; index < *tamanho; index++){
			pesquisa = strstr(estoque[index].unidade, uppercase(buscaUnidade));
			//pesquisa = strstr(estoque[index].unidade, buscaUnidade);
			
			if(pesquisa){
        		resultados[n_resultados] = estoque[index];
            	n_resultados++;
			}		
		}
		
		for(index = 0; index < n_resultados; index++){
			buscaResultado = true;
			paginacao(resultados, n_resultados, 2, 1, "Produtos de acordo com sua unidade de venda");
		}
		
		if(buscaResultado == false){
			printf ("Sua busca por '%s' não encontrou nenhum resultado.\n", uppercase(buscaUnidade));
			system("pause");
			system("cls");
		}
	}
}

// Movimentação (vender e comprar produtos)
void movimentacao(produto estoque[], int *tamanho){
	int chave, pos, cod, opc;
	pos = pesquisaBinaria(estoque, cod, *tamanho);
	
	alinhaTexto(80, "MOVIMENTACAO");
	
	printf("1 - Vender\n");
	printf("2 - Comprar\n");
	printf("0 - Voltar ao menu\n");
	scanf("%i", &opc);
	fflush(stdin);
	system("cls");
	switch(opc){
	case 1:
		{
			vender(estoque, tamanho);
			gravacao(estoque, *tamanho);
		}
		
		break;
	case 2:
		{
			comprar(estoque, tamanho);
			gravacao(estoque, *tamanho);
		}
		break;
	case 0:
		system("cls");
		return;
		
		break;
	default:
		system("cls");
		alinhaTexto(80, "MOVIMENTACAO");
		printf("Opção Inválida\n");
		system("pause");
		system("cls");
		break; 
	}
}

// funcão responsável por realizar a venda de produtos
void vender(produto estoque[], int *tamanho){
	
	int pos, cod;
	float qtdVendida, valorVenda;
	produto aux;
	
	alinhaTexto(80, "VENDER PRODUTO");
	
	printf("Código: ");
	scanf("%i", &cod);
	fflush(stdin);
	system("cls");
	alinhaTexto(80, "VENDER PRODUTO");
	
	pos = pesquisaBinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		aux = estoque[pos];
		printf("Nome                                               Preço     Quantidade\n");
		printf("----------------------------------------------------------------------------\n");
		mostraListaVenda(estoque, pos);
		
		do{
			camposCadastro(50, "Quantos produtos deseja vender?");
			scanf("%f", &qtdVendida);
			printf("\n");
			fflush(stdin);
			if(qtdVendida > aux.quantidadeDisponivel){
				printf("A quantidade informada é maior que nosso estoque.\n");
			}
			if(qtdVendida <= 0){
				printf("A quantidade informada não pode ser menor que 0.\n");
			}
		}while(qtdVendida <= 0 || qtdVendida > aux.quantidadeDisponivel);
		
		valorVenda = qtdVendida * aux.precoVenda;
		
		printf("Itens (qtd): %.2f  Valor a receber: R$ %.2f \n", qtdVendida, valorVenda);
		
		aux.quantidadeDisponivel = aux.quantidadeDisponivel - qtdVendida; // atualiza a quantidade descontando os itens vendidos sem interferir na quantidade comprada inicialmente.
		aux.custoTemporario = aux.custoTemporario - valorVenda;		
		aux.valorLucro = aux.receita - aux.custoInicial + valorVenda;
		aux.receita = aux.receita + valorVenda;
		aux.lucro = aux.valorLucro/aux.custoInicial * 100;
		
		{
			int validaCadastro;
			printf("Confirmar venda?\n 1 - Sim   0 - Não \n");
			scanf("%i", &validaCadastro);
			fflush(stdin);
			if(validaCadastro == 1){
				estoque[pos] = aux;
				
				system("cls");
				alinhaTexto(80, "VENDER PRODUTO");
				printf("Venda realizada!\n");
				system("pause");
				system("cls");
			} else {
				system("cls");
				alinhaTexto(80, "VENDER PRODUTO");
				printf("Venda cancelada!\n");
				system("pause");
				system("cls");
			}
		}
		
	} else {
		system("cls");
		alinhaTexto(80, "VENDER PRODUTO");
		printf("nao cadastrado\n");
		system("pause");
		system("cls");
	}
}

// funcão responsável por realizar a compra de produtos
void comprar(produto estoque[], int *tamanho){
	
	int pos, cod;
	float qtdCompra, valorCompra;
	produto aux;
	
	alinhaTexto(80, "COMPRAR PRODUTO");
	
	printf("Código: ");
	scanf("%i", &cod);
	fflush(stdin);
	system("cls");
	alinhaTexto(80, "COMPRAR PRODUTO");
	
	pos = pesquisaBinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		aux = estoque[pos];
		mostraListaCompra(estoque, pos);
		
		// Fornecedor do produto
		do{
			camposCadastro(50, "Fornecedor");
			uppercase(gets(aux.fornecedor));
			fflush(stdin);
			if(aux.fornecedor[0] == '\0'){
				printf("O campo fornecedor não pode ser vazio. \n");	
			}
		}while(aux.fornecedor[0] == '\0');
		
		//Preço de compra do produto
		do{
			camposCadastro(50, "Preco de compra do produto");
			scanf("%f", &aux.precoCompra);
			fflush(stdin);
			if(aux.precoCompra < 0){
				printf("O preço de compra não pode ser negativo.\n");
			}
		}while(aux.precoCompra < 0);
		
		do{
			camposCadastro(50, "Quantos produtos deseja comprar?");
			scanf("%f", &qtdCompra);
			printf("\n");
			fflush(stdin);

			if(qtdCompra <= 0){
				printf("A quantidade informada não pode ser menor que 0.\n");
			}
			
		}while(qtdCompra <= 0);
		
		aux.quantidadeDisponivel = aux.quantidadeDisponivel + qtdCompra;
		aux.quantidade = aux.quantidade + qtdCompra;
		
		valorCompra = qtdCompra * aux.precoCompra;
		
		aux.custoInicial = aux.custoInicial + valorCompra;
		
		// Calcula o novo lucro
		double novoLucro = aux.receita - aux.custoInicial;
		
		// Atualiza a porcentagem de lucro do produto
		aux.lucro = (novoLucro / aux.custoInicial) * 100;
		
		
		system("cls");
		alinhaTexto(80, "COMPRAR PRODUTO");
		
		printf("Nome: %-51s Quantidade: %7.2f\n", aux.nomeProduto, qtdCompra);
		printf("Fornecedor: %-39s Valor a pagar R$: %7.2f\n", aux.fornecedor, valorCompra);
		printf("Novo estoque após a compra: %.2f\n", aux.quantidadeDisponivel);
		printf("----------------------------------------------------------------------------\n\n");
		
		{
			int validaCadastro;
			printf("Confirmar compra?\n 1 - Sim   0 - Não \n");
			scanf("%i", &validaCadastro);
			fflush(stdin);
			if(validaCadastro == 1){
				estoque[pos] = aux;
				
				system("cls");
				alinhaTexto(80, "COMPRAR PRODUTO");
				printf("Compra realizada!\n\n");
				system("pause");
				system("cls");
			} else {
				system("cls");
				alinhaTexto(80, "COMPRAR PRODUTO");
				printf("Compra cancelada!\n\n");
				system("pause");
				system("cls");
			}
		}
		
	} else {
		system("cls");
		alinhaTexto(80, "COMPRAR PRODUTO");
		printf("nao cadastrado\n");
		system("pause");
		system("cls");
	}
}

// Lista de produtos em formato de ficha
void mostraFicha(produto estoque[], int index){
	
	printf("Código: %-57ld Grupo: %3i\n", estoque[index].codigo, estoque[index].grupo);
	printf("Descricao: %-52s Unidade: %3s\n", estoque[index].nomeProduto, estoque[index].unidade);
	printf("Fornecedor: %s\n", estoque[index].fornecedor);
	
	//Mostra a linha de preços e lucro com cores vermelha para lucros abaixo de 0% e verde para lucro 0% ou maiores
	(estoque[index].lucro > 0) ? 
	printf("Preço Compra: R$ %-9.2f  Preço Venda: R$ %-9.2f   Lucro Mínimo: \033[1;32m%5i%%\033[0m\n", estoque[index].precoCompra, estoque[index].precoVenda, estoque[index].lucro):
	printf("Preço Compra: R$ %-9.2f  Preço Venda: R$ %-9.2f   Lucro Mínimo: \033[1;31m%5i%%\033[0m\n", estoque[index].precoCompra, estoque[index].precoVenda, estoque[index].lucro);
	
	//Mostra o estoque em vermelho caso fique abaixo do estoque minimo
	(estoque[index].quantidadeDisponivel == 0) ? 
	printf("Quantidade em Estoque: \033[1;31m%-25.2f\033[0m   Quantidade Mínima: %6.2f\n", estoque[index].quantidadeDisponivel, estoque[index].estoque_min):
	(estoque[index].quantidadeDisponivel < estoque[index].estoque_min) ? 
	printf("Quantidade em Estoque: \033[1;33m%-25.2f\033[0m   Quantidade Mínima: %6.2f\n", estoque[index].quantidadeDisponivel, estoque[index].estoque_min):
	printf("Quantidade em Estoque: %-25.2f   Quantidade Mínima: %6.2f\n", estoque[index].quantidadeDisponivel, estoque[index].estoque_min);
	
	//Mostra a linha de preços e lucro com cores vermelha para lucros abaixo de 0% e verde para lucro 0% ou maiores
	(estoque[index].receita > estoque[index].custoInicial) ? 
	printf("Investimento em R$: %-31.2f  Receita em R$: \033[1;32m%8.2f\033[0m\n", estoque[index].custoInicial, estoque[index].receita) :
	printf("Investimento em R$: %-31.2f  Receita em R$: \033[1;31m%8.2f\033[0m\n", estoque[index].custoInicial, estoque[index].receita);
		
	printf("----------------------------------------------------------------------------\n");
	return;
}

// Lista de produtos por preço
void mostraLista(produto estoque[], int index){
	printf("%-7ld %-57s R$ %6.2f\n", estoque[index].codigo, estoque[index].nomeProduto, estoque[index].precoVenda);
	printf("----------------------------------------------------------------------------\n");
	return;
}

// Usado em conjunto com a função de venda de produtos
void mostraListaVenda(produto estoque[], int index){
	printf("%-55s R$ %-7.2f %.2f %2s\n", estoque[index].nomeProduto, estoque[index].precoVenda, estoque[index].quantidadeDisponivel, estoque[index].unidade);
	printf("----------------------------------------------------------------------------\n");
	return;
}

// Usado em conjunto com a função de compra de produtos
void mostraListaCompra(produto estoque[], int index){
	printf("Nome: %-57s Unidade: %3s\n", estoque[index].nomeProduto, estoque[index].unidade);
	printf("Fornecedor: %-39s Último preço R$: %7.2f\n", estoque[index].fornecedor, estoque[index].precoCompra);
	printf("----------------------------------------------------------------------------\n\n");
	return;
}

// Converte o texto digitado em caractéres maíusculos
char* uppercase(char *uppertext){
	char *texto = uppertext;
	int i;
	for (i = 0; texto[i]!='\0'; i++) {
		texto[i] = toupper(texto[i]);
	}
	return texto;
}

// Campos cadastro
void camposCadastro(int largura, char titulo[]){
    int tamanhoTitulo = strlen(titulo);
    int espaco = largura - tamanhoTitulo;
    
    printf("%s%*.*s: ", titulo, espaco, espaco, " --------------------------------------------------");
}

// Alinha o titulo das seçoes
void alinhaTexto(int largura, char titulo[]){
    int tamanhoTitulo = strlen(titulo);
    int espaco = (tamanhoTitulo >= largura) ? 0 : (largura - tamanhoTitulo) / 2;
    
    printf ("----------------------------------------------------------------------------\n");
    printf("%*.*s%s\n", espaco, espaco, " ", titulo);
	printf ("----------------------------------------------------------------------------\n\n");
}


//Paginação
void paginacao(produto estoque[], int numProdutos, int itensPagina, int tipoLista, char titulo[]){
	int totalPaginas = (numProdutos + itensPagina - 1) / itensPagina;
    int paginaAtiva = 0;
    
    while(1){    	
    	// calcula o inicio e final dos itens para a pagina atual.
        int posicaoInicial = paginaAtiva * itensPagina;
        int posicaoFinal = posicaoInicial + itensPagina;
        
        // Mostra o resultado
        int index;
        for (index = posicaoInicial; index < posicaoFinal && index < numProdutos; index++) {
        	
        	switch(tipoLista){
				case 1:
					mostraFicha(estoque, index);
					break;
				case 2:
					mostraLista(estoque, index);
					break;
				default:
					mostraFicha(estoque, index);
			}
        }
        
        printf ("\n\n");
        printf("Página %d/%d:\n", paginaAtiva + 1, totalPaginas);
        printf ("----------------------------------------------------------------------------\n");
        // Mostra as opções de navegação
        if (paginaAtiva > 0) {
            printf("(A) Anterior ");
        }
        if (paginaAtiva < totalPaginas - 1) {
            printf("(P) Próxima ");
        }
        printf("(S) Sair ");
        
        // Lê a opção do usuário
        char pagina;
        scanf(" %c", &pagina);

        // Executa a opção do usuário
        switch (pagina) {
            case 'A':
            case 'a':
                if (paginaAtiva > 0) {
                    paginaAtiva--;
                    system("cls");
                    alinhaTexto(80, titulo);
                    
                    if(tipoLista == 2){
                    	printf("Código  Nome                                                      Preço     \n\n");
					}
                }
                break;
            case 'P':
            case 'p':
                if (paginaAtiva < totalPaginas - 1) {
                    paginaAtiva++;
                    system("cls");
                    alinhaTexto(80, titulo);
                    
                    if(tipoLista == 2){
                    	printf("Código  Nome                                                      Preço     \n\n");
					}
                }
                break;
            case 'S':
            case 's':
				system("cls");
				main();
				break;
            default:
                printf("Opção Inválida.\n");
        }
	}	
}
