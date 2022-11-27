#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>


const int MAX=50; // limitando o tamanho do vetor
int ordenado = 0; // variável p/ controlar a ordenação dos dados

// Tipo de dado especial (Registro)
typedef struct TProduto{
    long int codigo;
    int grupo, lucro;
    char descricao[41], unidade[3], fornecedor[41];
    float quantidade, quantidadeDisponivel, estoque_min, custoInicial, custoTemporario, receita, pr_compra, pr_venda, valorLucro, vendidos;
}Tproduto;


// Escopo do programa
void leitura(Tproduto estoque[], int *tamanho);	// gera o arquivo .dat na primeira vez
void gravacao(Tproduto estoque[], int tamanho);	// realiza a gravação dos dado no arquivo
int pesquisa(Tproduto estoque[], int codigo, int *tamanho);	// busca e retorna a posição do produto no vetor
void ordena(Tproduto estoque[], int tamanho);	// ordena o cadastro dos produtos por código
int pesquisabinaria(Tproduto estoque[], int chave, int tamanho);
int vazio(int tamanho);	// função auxiliar para da pesquisa binária
void alinhaTexto(int largura, char titulo[]); // alinha titulos
char* uppercase(char *uppertext);

//CRUD
void cadastrar(Tproduto estoque[], int *tamanho);	//faz o cadastro dos produtos
void atualizar(Tproduto estoque[], int *tamanho); 	//Altera os dados do produto
void excluir (Tproduto estoque[], int *tamanho);	//Remove o produto selecionado
void consultar(Tproduto estoque[], int *tamanho);	//faz a busca pelo produto

//Relatorios
void relatorio(Tproduto estoque[], int *tamanho);
void relatorioEspecial(Tproduto estoque[], int *tamanho);

//Relatorios Especiais
void margemLucroMinima(Tproduto estoque[], int *tamanho);
void estoqueAbaixoMinimo(Tproduto estoque[], int *tamanho);
void produtosFornecedor(Tproduto estoque[], int *tamanho);
void produtosUnidade(Tproduto estoque[], int *tamanho);
void produtosComPrejuizo(Tproduto estoque[], int *tamanho);

// Movimentação
void movimentacao(Tproduto estoque[], int *tamanho);
void vender(Tproduto estoque[], int *tamanho);
void comprar(Tproduto estoque[], int *tamanho);

// Exibir dados
void mostraFicha(Tproduto estoque[], int chave); 	//exibe os dados do produto em formato de ficha
void mostraLista(Tproduto estoque[], int chave); 	//exibe os dados do produto em formato de lista
void mostraListaVenda(Tproduto estoque[], int chave);
void mostraListaCompra(Tproduto estoque[], int chave);

int main(){
	setlocale(LC_ALL,"");
	Tproduto estoque[MAX];
    int tamanho=0, opcao, opc;
    char nome[20];
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
  		alinhaTexto(80, "CONTROLE DE ESTOQUE");
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
     	alinhaTexto(80, "CONTROLE DE ESTOQUE");
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

// Função responsável pelo cadastro de novos produtos
void cadastrar(Tproduto estoque[], int *tamanho){
	Tproduto aux;
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
		printf("Código do produto--------------------------: ");
		scanf("%ld", &aux.codigo);
		fflush(stdin);
		
		if(aux.codigo < 0){
			printf("O código não pode ser negativo.\n");
		}	
		
	}while(aux.codigo < 0);
	
	// Verifica se já existe um produto com o mesmo código
	if(pesquisa(estoque, aux.codigo, tamanho) > 0){
		system("cls");
		printf("\n PRODUTO JA EXISTE\n");
		system("pause");
		system("cls");
		return;	
	}
	
	// Descrição do produto
	do{
		printf("Descrição----------------------------------: ");
//		gets(aux.descricao);
		fgets(aux.descricao, 40, stdin);
		fflush(stdin);
		if(aux.descricao[0] == '\0'){
			printf("O campo descricao não pode ser vazio. \n");	
		}
	}while(aux.descricao[0] == '\0');
	
	
	// Quantidade do produto
	do{
		printf("Quantidade de produto em estoque-----------: ");
		scanf("%f", &aux.quantidade);
		fflush(stdin);
		if(aux.quantidade < 0){
			printf("A quantidade não pode ser negativa.\n");	
		}
	}while(aux.quantidade < 0);
	
	
	// Unidade do produto
	do{
		printf("Unidade [KG, PC, UN, LT] ------------------: ");
		uppercase(gets(aux.unidade));
//		gets(aux.unidade);
		fflush(stdin);
		if(aux.unidade[0] == '\0'){
			printf("O campo unidade não pode ser vazio. \n");	
		}
	}while(aux.unidade[0] == '\0');
	
	
	// Estoque minimo
	do{
		printf("Estoque minimo-----------------------------: ");
		scanf("%f", &aux.estoque_min);
		fflush(stdin);
		if(aux.estoque_min < 0){
			printf("A estoque minimo não pode ser negativo.\n");	
		}
	}while(aux.estoque_min < 0);
	
	
	// Grupo do produto
	do{
		printf("Grupo:\n[1] Ração\n[2] Medicamentos\n[3] Acessórios\n[4] Higiene\n[5] Brinquedos\n");
		printf("Selecione uma Categoria--------------------: ");
		scanf("%i", &aux.grupo);
		fflush(stdin);
//		if(aux.grupo != 1 && aux.grupo != 2 && aux.grupo != 3 && aux.grupo != 4 && aux.grupo != 5){
//			printf(" Digite uma categoria válida\n");
//		}
		if(aux.grupo < 1 || aux.grupo > 5){
			printf("Digite uma categoria válida\n");
		}
			
	}while(aux.grupo < 1 || aux.grupo > 5);
	
	// Fornecedor do produto
	do{
		printf("Fornecedor---------------------------------: ");
		// gets(aux.fornecedor);
		fgets(aux.fornecedor, 40, stdin);
		fflush(stdin);
		if(aux.fornecedor[0] == '\0'){
			printf("O campo fornecedor não pode ser vazio. \n");	
		}
	}while(aux.fornecedor[0] == '\0');
	
	//Preço de compra do produto
	do{
		printf("Preco de compra do produto-----------------: ");
		scanf("%f", &aux.pr_compra);
		fflush(stdin);
		if(aux.pr_compra < 0){
			printf("O preço de compra não pode ser negativo.\n");
		}
	}while(aux.pr_compra < 0);


	//Preço de venda do produto
	do{
		printf("Preco de venda do produto------------------: ");
		scanf("%f", &aux.pr_venda);
		fflush(stdin);
		if(aux.pr_venda < 0){
			printf("O preço de venda não pode ser negativo.\n");
		}
	}while(aux.pr_venda < 0);

	// o código a seguir faz o calculo inicial para retornar o lucro minimo em porcentagem.
	{
		aux.quantidadeDisponivel = aux.quantidade;
		aux.custoInicial = aux.pr_compra * aux.quantidade;
		
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
void atualizar(Tproduto estoque[], int *tamanho){

 	//Não atualizar os seguintes campos: preço de compra, quantidade, fornecedor (esses itens fazem parte de outras funcionalidades do PI que foram sorteadas para outros grupos)
 	//Atualizar descricao, unidade, estoque_min, pr_venda
	
	Tproduto aux;
	
	int pos, cod, opc;
	int correto='n';
	
	alinhaTexto(80, "EDITAR PRODUTO");
	
	printf("Código do produto--------------------------: ");
	scanf("%i", &cod);
	fflush(stdin);
	
	system("cls");
	alinhaTexto(80, "EDITAR PRODUTO");
	
	pos = pesquisabinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		aux = estoque[pos];
		mostraFicha(estoque, pos);
		
		printf("\nO que deseja alterar? \n");
		printf("1 - Descrição do produto: \n");
		printf("2 - Unidade do produto: \n");
		printf("3 - Grupo do produto: \n");
		printf("4 - Estoque minimo: \n");
		printf("4 - Valor de venda do produto: \n");
		printf("0 - Voltar ao menu\n");
		scanf("%i", &opc);
		fflush(stdin);
		
		switch(opc){
		case 1:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			// Descrição do produto
			do{
				printf("Digite o nova descrição------------------------: ");
				// gets(aux.descricao);
				fgets(aux.descricao, 40, stdin);
				fflush(stdin);
				if(aux.descricao[0] == '\0'){
					printf("O campo descricao não pode ser vazio. \n");	
				}
			}while(aux.descricao[0] == '\0');
						
			break;
		case 2:
			system("cls");
			alinhaTexto(80, "EDITAR PRODUTO");
			// Unidade do produto
			do{
				printf("Unidade [KG, PC, UN, LT] ----------------------: ");
//				gets(aux.unidade);
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
				printf("Selecione uma Categoria--------------------: ");
				scanf("%i", &aux.grupo);
				fflush(stdin);
//				if(aux.grupo != 1 && aux.grupo != 2 && aux.grupo != 3 && aux.grupo != 4 && aux.grupo != 5){
//					printf(" Digite uma categoria válida\n");
//				}
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
				printf("Estoque minimo---------------------------------: ");
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
				printf("Preco de venda do produto----------------------: ");
				scanf("%f", &aux.pr_venda);
				fflush(stdin);
				if(aux.pr_venda < 0){
					printf("O preço de venda não pode ser negativo.\n");
				}
			}while(aux.pr_venda < 0);
			
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
			printf("\n Cadastro atualizado com sucesso!\n\n");
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
void excluir (Tproduto estoque[], int *tamanho){
	if(*tamanho==0){
		printf("\nREGISTRO VAZIO!\n\n");
		return;
	}
	int posicao, i, codigo;
	char confirma='n';
	
	alinhaTexto(80, "EXCLUIR PRODUTO");

	printf("Código do produto a ser excluido ----------: ");

	scanf("%d", &codigo);
	fflush(stdin);
	
	system("cls");
	alinhaTexto(80, "EXCLUIR PRODUTO");
	posicao=pesquisabinaria(estoque, codigo, *tamanho);
     
	if (posicao>=0) {//encontrou registro
	    mostraFicha(estoque, posicao);
		
		int validaCadastro;
		printf(" Deseja excluir esse produto?\n 1 - Sim   0 - Não \n");
		scanf("%i", &validaCadastro);
		fflush(stdin);
		if(validaCadastro == 1){
			for (i=posicao;i<(*tamanho)-1;i++){
				estoque[i]=estoque[i+1];
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
void consultar(Tproduto estoque[], int *tamanho){
	
	int index, pos, cod, opc;
	
	alinhaTexto(80, "PESQUISAR PRODUTO");
	
	printf("1 - Pesquisar produto por código \n");
	printf("2 - Pesquisar produtos por descrição \n");
	printf("0 - Voltar ao menu\n");
	scanf("%i", &opc);
	fflush(stdin);
	system("cls");
	switch(opc){
	case 1:	
		alinhaTexto(80, "PESQUISAR POR CÓDIGO");
		{
			printf("Digite o código para pesquisa: ");
			scanf("%i", &cod);
			fflush(stdin);
			system("cls");
			alinhaTexto(80, "PESQUISAR POR CÓDIGO");
			pos = pesquisabinaria(estoque, cod, *tamanho);
			if(pos >= 0){			
				mostraFicha(estoque, pos);
				system("pause");
				system("cls");
			} else {
				printf("Código não cadastrado \n");
				system("pause");
				system("cls");
			}
		}
		
		break;
	case 2:
		alinhaTexto(80, "PESQUISAR POR DESCRIÇÃO");
		
		{
			char buscaDescricao[41];
			char *pesquisa;
			bool buscaResultado = false;
		
			printf("Buscar por: ");
			scanf("%s", &buscaDescricao);
			fflush(stdin);
			system("cls");
			
			alinhaTexto(80, "PESQUISAR POR DESCRIÇÃO");

			int porPagina = 1;
			for(index = 0; index < *tamanho; index++){
				pesquisa = strstr(estoque[index].descricao, buscaDescricao);
				if(pesquisa){
					buscaResultado = true;
					mostraFicha(estoque, index);
					porPagina++;
				}
				
				if(porPagina > 2){
					system("pause");
					system("cls");
					alinhaTexto(80, "PESQUISAR POR DESCRIÇÃO");
					porPagina = 1;
				}
			}	
			
			if(buscaResultado == false){
				printf ("Sua busca por '%s' não encontrou nenhum produto.\n", buscaDescricao);
				system("pause");
				system("cls");
				porPagina = 0;
			}
			//corrige para não mostrar esse sistem pause se não retornar resultados
			if(porPagina != 0){
				system("pause");
				system("cls");
			}
		}
		
		break;
	case 0:
		system("cls");
		return;
		
		break;
	default:
		printf("Opção Inválida\n");
		break; 
	}
		
}

// Relatórios (geral e por preço)
void relatorio(Tproduto estoque[], int *tamanho){
	int chave, pos, cod, opc;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	
	alinhaTexto(80, "RELATÓRIOS");
	
	printf("1 - Relatório geral\n");
	printf("2 - Relatório por preços\n");
	printf("0 - Voltar ao menu\n");
	scanf("%i", &opc);
	fflush(stdin);
	system("cls");
	switch(opc){
	case 1:
		{
			alinhaTexto(80, "RELATÓRIO GERAL");
						
			int porPagina = 1;
			for(chave = 0; chave < *tamanho; chave++){
				mostraFicha(estoque, chave);
				porPagina++;
				if(porPagina > 2){
					system("pause");
					system("cls");
					alinhaTexto(80, "RELATÓRIO GERAL");
					porPagina = 1;
				}
			}
			system("pause");
			system("cls");
			}
		
		break;
	case 2:
		{
			alinhaTexto(80, "RELATÓRIO POR PREÇO");
			
			printf("Código  Descrição                                                 Preço     \n");
			printf("----------------------------------------------------------------------------\n");

			int porPagina = 1;
			for(chave = 0; chave < *tamanho; chave++){
				mostraLista(estoque, chave);
				
				porPagina++;
				if(porPagina > 15){
					system("pause");
					system("cls");
					alinhaTexto(80, "RELATÓRIO POR PREÇO");
					porPagina = 1;
				}
			}
			system("pause");
			system("cls");
		}
		break;
	case 0:
		system("cls");
		return;
		
		break;
	default:
		printf("Opção Inválida\n");
		break; 
	}
}

// Relatórios Especiais (Listar produtos com margem de lucro abaixo da mínima)
void relatorioEspecial(Tproduto estoque[], int *tamanho){
	int chave, pos, cod, opc;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	
	alinhaTexto(80, "RELATÓRIOS ESPECIAIS");
	
	printf("1 - Produtos com margem de lucro abaixo da mínima\n");
	printf("2 - Produtos com estoque abaixo do mínimo\n");
	printf("3 - Produtos que estão sendo vendidos com prejuízo\n");
	printf("4 - Produtos fornecidos por um fornecedor\n");
	printf("5 - Produtos de acordo com sua unidade de venda\n");
	printf("0 - Voltar ao menu\n");
	scanf("%i", &opc);
	fflush(stdin);
	system("cls");
	switch(opc){
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
void margemLucroMinima(Tproduto estoque[], int *tamanho){
	
	int index, pos, cod;
	alinhaTexto(80, "Produtos com margem de lucro abaixo da mínima");
						
	int porPagina = 1;
	for(index = 0; index < *tamanho; index++){
		if(estoque[index].lucro < 0){
			mostraFicha(estoque, index);
			porPagina++;
			if(porPagina > 2){
				system("pause");
				system("cls");
				alinhaTexto(80, "Produtos com margem de lucro abaixo da mínima");
				porPagina = 1;
			}
		}
	}
	system("pause");
	system("cls");
}

// Produtos com estoque abaixo do mínimo
void estoqueAbaixoMinimo(Tproduto estoque[], int *tamanho){
	
	int index, pos, cod;
	alinhaTexto(80, "Produtos com estoque abaixo do mínimo");
						
	int porPagina = 1;
	for(index = 0; index < *tamanho; index++){
		if(estoque[index].quantidadeDisponivel < estoque[index].estoque_min){
			mostraFicha(estoque, index);
			porPagina++;
			if(porPagina > 2){
				system("pause");
				system("cls");
				alinhaTexto(80, "Produtos com estoque abaixo do mínimo");
				porPagina = 1;
			}
		}
	}
	system("pause");
	system("cls");
}

// Produtos que estão sendo vendidos com prejuízo
void produtosComPrejuizo(Tproduto estoque[], int *tamanho){
	
	int index, pos, cod;
	alinhaTexto(80, "Produtos que estão sendo vendidos com prejuízo");
						
	int porPagina = 1;
	for(index = 0; index < *tamanho; index++){
		if(estoque[index].pr_venda < estoque[index].pr_compra){
			mostraFicha(estoque, index);
			porPagina++;
			if(porPagina > 2){
				system("pause");
				system("cls");
				alinhaTexto(80, "Produtos que estão sendo vendidos com prejuízo");
				porPagina = 1;
			}
		}
	}
	system("pause");
	system("cls");
}

// Produtos por fornecedor
void produtosFornecedor(Tproduto estoque[], int *tamanho){
	
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
		int porPagina = 1;
		for(index = 0; index < *tamanho; index++){
			pesquisa = strstr(estoque[index].fornecedor, buscaFornecedor);
			if(pesquisa){
				buscaResultado = true;
				mostraFicha(estoque, index);
				porPagina++;
			}
			
			if(porPagina > 2){
				system("pause");
				system("cls");
				alinhaTexto(80, "Produtos fornecidos por um fornecedor");
				porPagina = 1;
			}	
		}
		
		if(buscaResultado == false){
			printf ("Sua busca por '%s' não encontrou nenhum fornecedor.\n", buscaFornecedor);
			system("pause");
			system("cls");
			porPagina = 0;
		}
		//corrige para não mostrar esse sistem pause se não retornar resultados
		if(porPagina != 0){
			system("pause");
			system("cls");
		}	
	}
}

// Produtos por Unidade
void produtosUnidade(Tproduto estoque[], int *tamanho){
	
	int index, pos, cod;
	alinhaTexto(80, "PESQUISAR POR UNIDADE");
		
	{
		char buscaUnidade[2];
		char *pesquisa;
		bool buscaResultado = false;
	
		printf("Buscar por [KG, PC, UN, LT]: ");
		scanf("%s", &buscaUnidade);
//			gets(buscaDescricao);
		fflush(stdin);
		system("cls");
		
		alinhaTexto(80, "Produtos de acordo com sua unidade de venda");	
		
		int porPagina = 1;
		for(index = 0; index < *tamanho; index++){
			pesquisa = strstr(estoque[index].unidade, uppercase(buscaUnidade));
			if(pesquisa){
				buscaResultado = true;
				mostraFicha(estoque, index);
				porPagina++;
			}
			
			if(porPagina > 2){
				system("pause");
				system("cls");
				alinhaTexto(80, "Produtos de acordo com sua unidade de venda");
				porPagina = 1;
			}
			
		}
		
		if(buscaResultado == false){
			printf ("Sua busca por '%s' não encontrou nenhum fornecedor.\n", buscaUnidade);
			system("pause");
			system("cls");
			porPagina = 0;
		}
		//corrige para não mostrar esse sistem pause se não retornar resultados
		if(porPagina != 0){
			system("pause");
			system("cls");
		}
	}
}

// Movimentação (vender e comprar produtos)
void movimentacao(Tproduto estoque[], int *tamanho){
	int chave, pos, cod, opc;
	pos = pesquisabinaria(estoque, cod, *tamanho);
	
	alinhaTexto(80, "MOVIMENTACAO");
	
	printf("1 - Vender\n");
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
		printf("Opção Inválida\n");
		break; 
	}
}

// funcão responsável por realizar a venda de produtos
void vender(Tproduto estoque[], int *tamanho){
	
	int pos, cod;
	float qtdVendida, valorVenda;
	char confirma='n';
	Tproduto aux;
	
	alinhaTexto(80, "VENDER PRODUTO");
	
	printf("Código: ");
	scanf("%i", &cod);
	fflush(stdin);
	system("cls");
	alinhaTexto(80, "VENDER PRODUTO");
	
	pos = pesquisabinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		aux = estoque[pos];
		printf("Descrição                                               Preço     Quantidade\n");
		printf("----------------------------------------------------------------------------\n");
		mostraListaVenda(estoque, pos);
		
		do{
			printf("Quantos produtos deseja vender? ");
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
		
		valorVenda = qtdVendida * aux.pr_venda;
		
		printf("Itens (qtd): %.2f  Valor a pagar: R$ %.2f \n", qtdVendida, valorVenda);
		
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
				printf("\nVenda realizada!\n\n");
				system("pause");
				system("cls");
			}
		}
		
	} else {
		printf("nao cadastrado");
	}
}

// funcão responsável por realizar a compra de produtos
void comprar(Tproduto estoque[], int *tamanho){
	
	int pos, cod;
	float qtdCompra, valorCompra;
	char confirma='n';
	Tproduto aux;
	
	alinhaTexto(80, "COMPRAR PRODUTO");
	
	printf("Código: ");
	scanf("%i", &cod);
	fflush(stdin);
	system("cls");
	alinhaTexto(80, "COMPRAR PRODUTO");
	
	pos = pesquisabinaria(estoque, cod, *tamanho);
	if(pos >= 0){
		aux = estoque[pos];
		mostraListaCompra(estoque, pos);
		
		// Fornecedor do produto
		do{
			printf("Fornecedor---------------------------------------: ");
			gets(aux.fornecedor);
			fflush(stdin);
			if(aux.fornecedor[0] == '\0'){
				printf("O campo fornecedor não pode ser vazio. \n");	
			}
		}while(aux.fornecedor[0] == '\0');
		
		//Preço de compra do produto
		do{
			printf("Preco de compra do produto-----------------: ");
			scanf("%f", &aux.pr_compra);
			fflush(stdin);
			if(aux.pr_compra < 0){
				printf("O preço de compra não pode ser negativo.\n");
			}
		}while(aux.pr_compra < 0);
		
		do{
			printf("Quantos produtos deseja comprar? ");
			scanf("%f", &qtdCompra);
			printf("\n");
			fflush(stdin);

			if(qtdCompra <= 0){
				printf("A quantidade informada não pode ser menor que 0.\n");
			}
			
		}while(qtdCompra <= 0);
		
		aux.quantidadeDisponivel = aux.quantidadeDisponivel + qtdCompra;
		aux.quantidade = aux.quantidade + qtdCompra;
		
		valorCompra = qtdCompra * aux.pr_compra;
		
		system("cls");
		alinhaTexto(80, "COMPRAR PRODUTO");
		
		printf("Produto: %s\n", aux.descricao);
		printf("Fornecedor: %s\n", aux.fornecedor);
		printf("Quantidade: %-36.2f  Valor a pagar: R$ %6.2f \n", qtdCompra, valorCompra);
		printf("Novo estoque: %.2f\n", aux.quantidadeDisponivel);
		
		{
			int validaCadastro;
			printf("Confirmar compra?\n 1 - Sim   0 - Não \n");
			scanf("%i", &validaCadastro);
			fflush(stdin);
			if(validaCadastro == 1){
				estoque[pos] = aux;
				
				system("cls");
				printf("\nVenda realizada!\n\n");
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

// Alinha o titulo das seçoes
void alinhaTexto(int largura, char titulo[]){
    int tamanhoTitulo = strlen(titulo);
    int espaco = (tamanhoTitulo >= largura) ? 0 : (largura - tamanhoTitulo) / 2;
    
    printf ("----------------------------------------------------------------------------\n");
    printf("%*.*s%s\n", espaco, espaco, " ", titulo);
	printf ("----------------------------------------------------------------------------\n\n");
}

// Lista de produtos em formato de ficha
void mostraFicha(Tproduto estoque[], int chave){
	printf("Código: %-57ld Grupo: %3i\n", estoque[chave].codigo, estoque[chave].grupo);
	printf("Descricao: %-52s Unidade: %3s\n", estoque[chave].descricao, estoque[chave].unidade);
	printf("Fornecedor: %s\n", estoque[chave].fornecedor);
	printf("Preço de Compra: R$ %-7.2f  Preço de Venda: R$ %-5.2f   Lucro Mínimo: %5i%%\n", estoque[chave].pr_compra, estoque[chave].pr_venda, estoque[chave].lucro);
	printf("Quantidade em Estoque: %-25.2f   Quantidade Mínima: %6.2f\n", estoque[chave].quantidadeDisponivel, estoque[chave].estoque_min);
	printf("----------------------------------------------------------------------------\n");
	return;
}

// Lista de produtos por preço
void mostraLista(Tproduto estoque[], int chave){
	printf("%-7ld %-58s R$ %6.2f\n", estoque[chave].codigo, estoque[chave].descricao, estoque[chave].pr_venda);
	printf("----------------------------------------------------------------------------\n");
	return;
}

// Usado em conjunto com a função de venda de produtos
void mostraListaVenda(Tproduto estoque[], int chave){
	printf("%-55s R$ %-7.2f %.2f %2s\n", estoque[chave].descricao, estoque[chave].pr_venda, estoque[chave].quantidadeDisponivel, estoque[chave].unidade);
	printf("----------------------------------------------------------------------------\n");
	return;
}

// Usado em conjunto com a função de compra de produtos
void mostraListaCompra(Tproduto estoque[], int chave){
	printf("Descrição: %s\n", estoque[chave].descricao);
	printf("Fornecedor: %-51s Unidade: %3s\n", estoque[chave].fornecedor, estoque[chave].unidade);
	printf("----------------------------------------------------------------------------\n");
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
