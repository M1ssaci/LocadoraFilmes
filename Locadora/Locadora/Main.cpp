#define limpar_tela "cls"
#pragma warning(disable : 4996)



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 80


typedef struct filmes
{
	int id;
	int id_diretores;
	char nome[MAX];
} l_filmes;


typedef struct diretores
{
	int id;
	char nome[MAX];
} l_diretores;



char menu();

//Cadastro de diretores
void cadastrar_diretores();

//Cadastro de filmes
void cadastrar_filmes();

//Mostra todos os filmes cadastrados
void listar_filmes();

//Mostra todos os diretores cadastrados
void listar_diretores();


//Procura um filme pelo id do filmes
l_filmes* retorna_filme(FILE* arq_filmes, int id_filmes);


// Procura um diretor pelo seu id
l_diretores* retorna_diretor(FILE* arq_diretores, int id_diretores);


// Pesquisa um filme e exibe suas informações
void pesquisa_filme();

//Pesquisa um diretor e exibe suas informações
void pesquisa_diretor();

//Verifica se o filme existe
int existe_filmes_boolean(FILE* arq_filmes, int id_filmes);

//Verifica se o diretor existe
int existe_diretores_boolean(FILE* arq_diretores, int id_diretores);

void altera_filme();
void altera_diretor();

void exclui_filmes();

void exclui_diretor();

int str_somente_numeros(char str[]);

int main(int argc, char* argv[])
{
	char op;

	
	while (1)
	{
		
		op = menu();

		
		if (op == '1')
			cadastrar_filmes();
		else if (op == '2')
			cadastrar_diretores();
		else if (op == '3')
			listar_filmes();
		else if (op == '4')
			listar_diretores();
		else if (op == '5')
			pesquisa_filme();
		else if (op == '6')
			pesquisa_diretor();
		else if (op == '7')
			exclui_filmes();
		else if (op == '8')
			exclui_diretor();
		else if (op == '9')
			altera_filme();
		else if (op == '0')
			altera_diretor();
		else
		{
			printf("\nOpcao invalida!");
			scanf("%*c");
			fseek(stdin, 0, SEEK_END); 
		}
		system(limpar_tela);
	}

	return 0;
}


char menu()
{
	char op[2];


	printf("\n\n1 - Cadastrar filmes\n");
	printf("2 - Cadastrar diretor\n");
	printf("3 - Listar todos os filmes\n");
	printf("4 - Listar todos os diretores\n");
	printf("5 - Pesquisar um filme\n");
	printf("6 - Pesquisar um diretor\n");
	printf("7 - Excluir um filme\n");
	printf("8 - Excluir um diretor\n");
	printf("9 - Altera um filme\n");
	printf("0 - Altera um diretor\n");
	printf("Selecione uma opcao: ");
	scanf("%1s%*c", op); 


	fseek(stdin, 0, SEEK_END); 


	return op[0];
}


int str_somente_numeros(char str[])
{
	int i = 0;
	int len_str = strlen(str);

	for (i = 0; i < len_str; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return 0;
	}
	return 1;
}



void cadastrar_diretores()
{
	//Abrindo arquivo para escrita
	FILE* arq_diretores = fopen("diretores.bin", "a+b");


	if (arq_diretores == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); 
	}

	int cont_bytes = 0;

	fseek(arq_diretores, 0, SEEK_END);

	cont_bytes = ftell(arq_diretores);

	l_diretores diretores;

	if (cont_bytes == 0)
	{
		
		diretores.id = 1;
	}
	else
	{
		l_diretores ultimo_diretores;
		
		fseek(arq_diretores, cont_bytes - sizeof(l_diretores), SEEK_SET);

		fread(&ultimo_diretores, sizeof(l_diretores), 1, arq_diretores);

		diretores.id = ultimo_diretores.id + 1;
	}
	
	printf("\nDigite o nome do diretor: ");
	scanf("%99[^\n]%*c", diretores.nome);

	
	fseek(stdin, 0, SEEK_END); 

	
	fseek(arq_diretores, 0, SEEK_END);

	fwrite(&diretores, sizeof(l_diretores), 1, arq_diretores);


	fclose(arq_diretores);

	printf("\ndiretor \"%s\" cadastrado com sucesso!\n", diretores.nome);
	printf("\nPressione uma tecla para continuar...");
	scanf("%*c"); 


	fseek(stdin, 0, SEEK_END); 
}


void cadastrar_filmes()
{
	// abre o arquivo para escrita
	FILE* arq_filmes = fopen("filmes.bin", "a+b");
	FILE* arq_diretores = fopen("diretores.bin", "a+b");

	
	if (arq_filmes && arq_diretores == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); 
	}
	l_filmes filmes;

	int cont_bytes = 0;

	fseek(arq_filmes, 0, SEEK_END);

	cont_bytes = ftell(arq_filmes);

	if (cont_bytes == 0)
	{
		// se for 0, então não existe filmes cadastrado
		filmes.id = 1;
	}
	else
	{
		l_filmes ultimo_filmes;


		fseek(arq_filmes, cont_bytes - sizeof(l_filmes), SEEK_SET);

		// leitura dos filmes
		fread(&ultimo_filmes, sizeof(l_filmes), 1, arq_filmes);

		// o ID do filme é o ID do último filme mais 1
		filmes.id = ultimo_filmes.id + 1;
	}

	int aux = 0;
	printf("\nDigite o nome do filme: ");
	scanf("%99[^\n]%*c", filmes.nome);
	printf("\nDigite o ID do diretor: ");
	scanf("%d", &aux);

	
	if (retorna_diretor(arq_diretores, aux) != NULL) {

		filmes.id_diretores = aux;
	}
	else {
		printf("\nO diretor nao encontrado! ");
		fseek(stdin, 0, SEEK_END);
		printf("\nPressione qualquer tecla para continuar...");
		scanf("%*c");
		fclose(arq_diretores);
		return;
	}


	fseek(stdin, 0, SEEK_END); 

	// Não escreve nada se o ponteiro não estiver no final do arquivo
	fseek(arq_filmes, 0, SEEK_END);
	
	// escreve no arquivo
	fwrite(&filmes, sizeof(l_filmes), 1, arq_filmes);

	// fecha o arquivo
	fclose(arq_filmes);
	fclose(arq_diretores);

	printf("\nfilme \"%s\" cadastrado com sucesso!\n", filmes.nome);
	printf("\nPressione qualquer tecla para continuar...");
	scanf("%*c"); 

	
	fseek(stdin, 0, SEEK_END); 
}


// função para listar todos os diretores
void listar_diretores()
{
	FILE* arq_diretores = fopen("diretores.bin", "rb");

	if (arq_diretores == NULL)
	{
		printf("\nFalha na abertura do(s) arquivo(s) ou ");
		printf("nenhum diretor encontrado!\n");
		printf("\nPressione qualquer tecla para continuar...");
		scanf("%*c"); 

		
		fseek(stdin, 0, SEEK_END); 
		return;
	}

	int achou_diretor = 0;
	l_diretores diretores;

	printf("\nListando os diretores...\n");
	
	while (1)
	{
		
		size_t result = fread(&diretores, sizeof(l_diretores), 1, arq_diretores);

		
		if (result == 0)
			break;

		achou_diretor = 1;

		//mostra os dados do diretores
		printf("\nID do diretor: %d\n", diretores.id);
		printf("Nome do diretor: %s\n", diretores.nome);
	}

	if (achou_diretor == 0)
		printf("\nNenhum diretor encontrado!\n");

	fclose(arq_diretores);

	printf("\nPressione qualquer tecla para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END); 
}



l_diretores* retorna_diretor(FILE* arq_diretores, int id_diretores)
{
	
	rewind(arq_diretores);

	l_diretores* diretores;

	
	diretores = (l_diretores*)malloc(sizeof(l_diretores));
	while (1)
	{

		
		size_t result = fread(diretores, sizeof(l_diretores), 1, arq_diretores);

		
		if (result == 0)
		{
			free(diretores); 
			return NULL;
		}
		if (diretores->id == id_diretores)
			break;
	}
	return diretores;
}



void listar_filmes()
{

	FILE* arq_filmes = fopen("filmes.bin", "rb");
	FILE* arq_diretores = fopen("diretores.bin", "rb");

	if (arq_filmes == NULL)
	{
		printf("\nFalha na abertura do(s) arquivo(s) ou ");
		printf("nenhum filme encontrado!\n");
		printf("\nPressione qualquer tecla para continuar...");
		scanf("%*c"); 

	
		fseek(stdin, 0, SEEK_END); 
		return;
	}
	
	int achou_filme = 0;
	printf("\nListando todos os filmes...\n");
	
	l_filmes filmes;
	while (1)
	{
		
		size_t result = fread(&filmes, sizeof(l_filmes), 1, arq_filmes);

		
		if (result == 0)
			break;

		achou_filme = 1;

		//mostra os dados do filmes
		printf("\nID do filme: %d\n", filmes.id);
		printf("Nome do filme: %s\n", filmes.nome);
		printf("Diretor do filme: %d\n", filmes.id_diretores);
	
	}

	if (achou_filme == 0)
		printf("\nNenhum filme encontrado!\n");

	if (arq_diretores != NULL)
	fclose(arq_diretores); 
	fclose(arq_filmes);

	printf("\nPressione qualquer tecla para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END); 
}


void pesquisa_filme()
{
	int id = 0;
	int achou_filme = 0;

	FILE* arq_filmes = fopen("filmes.bin", "rb");
	FILE* arq_diretores = fopen("diretores.bin", "rb");


	if (arq_filmes == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); 
	}

	printf("\nDigite o ID do filme: ");
	scanf("%d", &id);

	printf("\nfilme com o ID \"%d\":\n\n", id);
	l_filmes filmes;
	l_diretores diretores;
	while (1)
	{
		
		size_t result = fread(&filmes, sizeof(l_filmes), 1, arq_filmes);


		if (result == 0)
			break;

		int id_aux;
		
		id_aux = filmes.id;

	
		if (id_aux == id)
		{
			//mostra os dados do filmes
			printf("Nome do filme: %s\n", filmes.nome);
			printf("ID do diretor: %d\n", filmes.id_diretores);
			int idDi_aux = filmes.id_diretores;
			
			size_t result = fread(&diretores, sizeof(l_diretores), 1, arq_diretores);

			if (result == 0)
				break;

			int idDi_auxX = 0;
	
			idDi_auxX = diretores.id;

		
			if (idDi_aux == idDi_auxX)
				{
				  printf("Nome do diretor: %s\n\n", diretores.nome);
				}
			achou_filme = 1;

			fseek(stdin, 0, SEEK_END);
			printf("Pressione qualquer tecla para continuar...");
			scanf("%*c");

			fclose(arq_filmes);
			fclose(arq_diretores);
			return;
		}
	}

	if (achou_filme == 0)
		printf("Nenhum filme encontrado.\n\n");

	fclose(arq_filmes);
	fclose(arq_diretores);

	printf("Pressione qualquer tecla para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END); 
}



void pesquisa_diretor()
{
	int idDi = 1;
	int achou_diretor = 0;

	FILE* arq_diretores = fopen("diretores.bin", "rb");

	if (arq_diretores == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); 
	}

	printf("\nDigite o ID do diretor: ");
	scanf("%d", &idDi);

	printf("\ndiretor com o ID \"%d\":\n\n", idDi);
	l_diretores diretores;
	while (1)
	{
		
		size_t result = fread(&diretores, sizeof(l_diretores), 1, arq_diretores);

		
		if (result == 0)
			break;

		int idDi_aux = 0;
		
		idDi_aux = diretores.id;

		
		if (idDi_aux == idDi)
		{
		
			printf("Nome do diretor: %s\n\n", diretores.nome);
			achou_diretor = 1;

			fseek(stdin, 0, SEEK_END);
			printf("Pressione qualquer tecla para continuar...");
			scanf("%*c");

			fclose(arq_diretores);
			return;
		}
	}

	if (achou_diretor == 0)
		printf("Nenhum diretor encontrado.\n\n");

	fclose(arq_diretores);

	printf("Pressione qualquer tecla para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END); 
}



int existe_filmes_boolean(FILE* arq_filmes, int id_filmes)
{
	
	rewind(arq_filmes);

	l_filmes filmes;

	while (1)
	{

		
		size_t result = fread(&filmes, sizeof(l_filmes), 1, arq_filmes);

		
		if (result == 0)
			break;

		
		if (filmes.id == id_filmes)
			return 1;
	}

	
	return 0;
}



int existe_diretores_boolean(FILE* arq_diretores, int id_diretores)
{
	
	rewind(arq_diretores);

	l_diretores diretores;

	while (1)
	{
		
		size_t result = fread(&diretores, sizeof(l_diretores), 1, arq_diretores);

		
		if (result == 0)
			break;

		
		if (diretores.id == id_diretores)
			return 1;
	}

	
	return 0;
}



l_filmes* retorna_filme(FILE* arq_filmes, int id_filmes)
{
	
	rewind(arq_filmes);

	
	l_filmes* filmes;

	
	filmes = (l_filmes*)malloc(sizeof(l_filmes));
	while (1)
	{

		
		size_t result = fread(filmes, sizeof(l_filmes), 1, arq_filmes);

		
		if (result == 0)
			break;

		
		if (filmes->id == id_filmes)
			return filmes;
	}
	free(filmes); 
	return NULL;
}

void altera_filme() {
	
	char str_id_filmes[10];
	int id_filmes;

	printf("\nDigite o ID do filme: ");
	scanf("%10s%*c", str_id_filmes);

	fseek(stdin, 0, SEEK_END);


	if (str_somente_numeros(str_id_filmes) == 1)
	{

		sscanf(str_id_filmes, "%d", &id_filmes);


		FILE* arq_filmes = fopen("filmes.bin", "rb");

		if (arq_filmes == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1);
		}


		if (existe_filmes_boolean(arq_filmes, id_filmes) == 1)
		{
			char nome_filmes[MAX];

			//Abrindo um arquivo temporário
			FILE* arq_temp = fopen("temp_filmes.bin", "a+b");
			if (arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_filmes);
				exit(1);
			}
			rewind(arq_filmes);

			l_filmes filmes;
			int idDiretor = 0;
			while (1)
			{


				size_t result = fread(&filmes, sizeof(l_filmes), 1, arq_filmes);


				if (result == 0)
					break;


				if (filmes.id != id_filmes)
				{
					
					fwrite(&filmes, sizeof(l_filmes), 1, arq_temp);
				}
				else
					idDiretor = filmes.id_diretores;
					strcpy(nome_filmes, filmes.nome);
			}

			int cont_bytes = 0;

			fseek(arq_temp, 0, SEEK_END);

			cont_bytes = ftell(arq_temp);


			l_filmes ultimo_filmes;


			fseek(arq_temp, cont_bytes - sizeof(l_filmes), SEEK_SET);

				
			fread(&ultimo_filmes, sizeof(l_filmes), 1, arq_temp);

				
			filmes.id = id_filmes;
			

			int aux = 0;
			printf("\nDigite o novo nome do filme: ");
			scanf("%99[^\n]%*c", filmes.nome);
			filmes.id_diretores = idDiretor;

			fseek(stdin, 0, SEEK_END);

			
			fseek(arq_temp, 0, SEEK_END);

			
			fwrite(&filmes, sizeof(l_filmes), 1, arq_temp);

			fseek(stdin, 0, SEEK_END);
			

			fclose(arq_filmes);
			fclose(arq_temp);

			if (remove("filmes.bin") != 0)
				printf("\nErro ao deletar o arquivo \"filmes.bin\"\n");
			else
			{

				int r = rename("temp_filmes.bin", "filmes.bin");
				if (r != 0)
				{
					printf("\nNão foi possivel renomear o arquivo!\n");
					printf("Feche o programa e o arquivo \"temp_filmes.bin\" e renomeie para \"filmes.bin\"\n");
				}
				else
					printf("\nfilme \"%s\" alterado com sucesso!\n", nome_filmes);
			}
		}
		else
		{
			fclose(arq_filmes);
			printf("\nNenhum filme com esse ID: \"%d\".\n", id_filmes);
		}
	}
	else
		printf("\nID so contem numeros!\n");

	printf("\nPressione qualquer tecla para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END);

}

void altera_diretor() {

	char str_id_diretor[10];
	int id_diretor;

	printf("\nDigite o ID do diretor: ");
	scanf("%10s%*c", str_id_diretor);

	fseek(stdin, 0, SEEK_END);


	if (str_somente_numeros(str_id_diretor) == 1)
	{

		sscanf(str_id_diretor, "%d", &id_diretor);


		FILE* arq_diretores = fopen("diretores.bin", "rb");

		if (arq_diretores == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1);
		}


		if (existe_diretores_boolean(arq_diretores, id_diretor) == 1)
		{
			char nome_diretor[MAX];

			//Abrindo um arquivo temporário
			FILE* arq_temp = fopen("temp_diretor.bin", "a+b");
			if (arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_diretores);
				exit(1);
			}
			rewind(arq_diretores);

			l_diretores diretores;
			int idDiretor = 0;
			while (1)
			{


				size_t result = fread(&diretores, sizeof(l_diretores), 1, arq_diretores);


				if (result == 0)
					break;


				if (diretores.id != id_diretor)
				{

					fwrite(&diretores, sizeof(l_diretores), 1, arq_temp);
				}
				else
					id_diretor = diretores.id;
					strcpy(nome_diretor, diretores.nome);
			}

			int cont_bytes = 0;

			fseek(arq_temp, 0, SEEK_END);

			cont_bytes = ftell(arq_temp);


			l_diretores ultimo_diretor;


			fseek(arq_temp, cont_bytes - sizeof(l_diretores), SEEK_SET);


			fread(&ultimo_diretor, sizeof(l_diretores), 1, arq_temp);


			diretores.id = id_diretor;


			int aux = 0;
			printf("\nDigite o novo nome do diretor: ");
			scanf("%99[^\n]%*c", diretores.nome);
			diretores.id = idDiretor;

			fseek(stdin, 0, SEEK_END);


			fseek(arq_temp, 0, SEEK_END);


			fwrite(&diretores, sizeof(l_diretores), 1, arq_temp);

			fseek(stdin, 0, SEEK_END);


			fclose(arq_diretores);
			fclose(arq_temp);

			if (remove("diretores.bin") != 0)
				printf("\nErro ao deletar o arquivo \"diretores.bin\"\n");
			else
			{

				int r = rename("temp_diretor.bin", "diretores.bin");
				if (r != 0)
				{
					printf("\nNão foi possivel renomear o arquivo!\n");
					printf("Feche o programa e o arquivo \"temp_diretor.bin\" e renomeie para \"diretores.bin\"\n");
				}
				else
					printf("\nDiretor \"%s\" alterado com sucesso!\n", nome_diretor);
			}
		}
		else
		{
			fclose(arq_diretores);
			printf("\nNenhum diretor com esse ID: \"%d\".\n", id_diretor);
		}
	}
	else
		printf("\nID so contem numeros!\n");

	printf("\nPressione qualquer tecla para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END);


}

//função para excluir filmes
void exclui_filmes()
{
	char str_id_filmes[10];
	int id_filmes;

	printf("\nDigite o ID do filme: ");
	scanf("%10s%*c", str_id_filmes);

	fseek(stdin, 0, SEEK_END); 

	
	if (str_somente_numeros(str_id_filmes) == 1)
	{
		
		sscanf(str_id_filmes, "%d", &id_filmes);

		
		FILE* arq_filmes = fopen("filmes.bin", "rb");

		if (arq_filmes == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1); 
		}

		
		if (existe_filmes_boolean(arq_filmes, id_filmes) == 1)
		{
			char nome_filmes[MAX];

			//Abrindo um arquivo temporário
			FILE* arq_temp = fopen("temp_filmes.bin", "a+b");
			if (arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_filmes);
				exit(1);
			}
			rewind(arq_filmes); 

			l_filmes filmes;
			while (1) 
			{

				
				size_t result = fread(&filmes, sizeof(l_filmes), 1, arq_filmes);

				
				if (result == 0)
					break;

				
				if (filmes.id != id_filmes)
				{
					
					fwrite(&filmes, sizeof(l_filmes), 1, arq_temp);
				}
				else
					strcpy(nome_filmes, filmes.nome);
			}

			
			fclose(arq_filmes);
			fclose(arq_temp);

			if (remove("filmes.bin") != 0)
				printf("\nErro ao deletar o arquivo \"filmes.bin\"\n");
			else
			{
				
				int r = rename("temp_filmes.bin", "filmes.bin");
				if (r != 0)
				{
					printf("\nNão foi possivel renomear o arquivo!\n");
					printf("Feche o programa e o arquivo \"temp_filmes.bin\" e renomeie para \"filmes.bin\"\n");
				}
				else
					printf("\nfilme \"%s\" removido com sucesso!\n", nome_filmes);
			}
		}
		else
		{
			fclose(arq_filmes);
			printf("\nNenhum filme com esse ID: \"%d\".\n", id_filmes);
		}
	}
	else
		printf("\nID so contem numeros!\n");

	printf("\nPressione qualquer tecla para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); 
}

void exclui_diretor()
{
	char ch_id_diretor[10];
	int id_diretor;

	printf("\nDigite o ID do diretor: ");
	scanf("%10s%*c", ch_id_diretor);

	fseek(stdin, 0, SEEK_END); 

	
	if (str_somente_numeros(ch_id_diretor) == 1)
	{
		
		sscanf(ch_id_diretor, "%d", &id_diretor);

		
		FILE* arq_diretores = fopen("diretores.bin", "rb");

		if (arq_diretores == NULL)
		{
			printf("\nFalha ao abrir o(s) arquivo(s)!\n");
			exit(1);
		}

		
		if (existe_diretores_boolean(arq_diretores, id_diretor) == 1)
		{
			char nome_diretor[MAX];
			
			FILE* arq_tempP = fopen("temp_diretores.bin", "a+b");
			if (arq_tempP == NULL)
			{
				printf("\nFalha ao criar o arquivo temporario!\n");
				fclose(arq_diretores);
				exit(1); 
			}
			rewind(arq_diretores); 

			l_diretores diretores;
			while (1) 
			{

				
				size_t result = fread(&diretores, sizeof(l_filmes), 1, arq_diretores);

				
				if (result == 0)
					break;

		
				if (diretores.id != id_diretor)
				{
					
					fwrite(&diretores, sizeof(l_filmes), 1, arq_tempP);
				}
				else
					strcpy(nome_diretor, diretores.nome);
			}

			fclose(arq_diretores);
			fclose(arq_tempP);

			if (remove("diretores.bin") != 0)
				printf("\nErro ao deletar o arquivo \"diretores.bin\"\n");
			else
			{
				// renomeia o arquivo
				int r = rename("temp_diretores.bin", "diretores.bin");
				if (r != 0)
				{
					printf("\nNao foi possivel renomear o arquivo!\n");
					printf("Feche o programa e o arquivo \"temp_diretores.bin\" e renomeie para \"diretores.bin\"\n");
				}
				else
					printf("\ndiretor removido com sucesso!\n");
			}
		}
		else
		{
			fclose(arq_diretores);
			printf("\nNao existe diretor com o ID \"%d\".\n", id_diretor);
		}
	}
	else
		printf("\nO ID so contem numeros!\n");

	printf("\nPressione qualquer tecla para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); 
}