/*
 * Paralelização do Problema do Caixeiro Viajante (TSP) utilizando OpenMP.
 *
 * Estratégia de Paralelização:
 * 1. Granulação: Focamos a paralelização no laço 'for' principal da função 'main'.
 *    Cada thread assume a responsabilidade de processar a busca do menor caminho a partir de 
 *    uma cidade de origem diferente.
 * 2. Balanceamento de Carga: Como a função recursiva faz cortes (pruning) quando a rota 
 *    atual já supera o menor caminho encontrado, o tempo de execução para cada 
 *    cidade inicial é muito variável. Por isso, utilizamos 'schedule(dynamic)' para que as 
 *    threads ociosas assumam novas iterações dinamicamente.
 * 3. Prevenção de Condições de Corrida: 
 *    - O vetor global 'ok' foi removido e instanciado localmente dentro da região 
 *      paralela ('local_ok'). Isso garante que cada thread tenha seu próprio estado de cidades visitadas.
 *    - A variável 'D', que guarda a menor distância de todo o grafo, é atualizada 
 *      dentro de uma região '#pragma omp critical' para garantir exclusão mútua.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

//#define DEBUG

int tsp(int matrix[51][51], int cities, int distance, int initial, int current,
		int ok[51], int total, int shortest) {

	int i, d;

	if (total == cities)
		return distance + matrix[current][initial];

	if (shortest < distance)
		return distance;

	for (i = 1; i <= cities; i++)
		if (i != current && !ok[i]) {
			ok[i] = 1;
			d = tsp(matrix, cities, distance + matrix[current][i], initial, i,
					ok, total + 1, shortest);
			if (d < shortest)
				shortest = d;
			ok[i] = 0;
		}
	return shortest;
}

int main(int argc, char *argv[]) {

	int c, e;
	int i, j, w, v;
	int matrix[51][51], d, D;
	int k;

	memset(matrix, 0, sizeof(matrix));

	FILE *file;
	if ((file = fopen("tsp.in", "r")) == NULL) {
		perror("tsp.in");
		exit(1);
	}

	fscanf(file, "%d%d", &c, &e);

#ifdef DEBUG
	printf("size: %d\nedges: %d\n", c, e);
	fflush(stdout);
#endif

	for (k = 0; k < e; k++) {
		fscanf(file, "%d%d%d", &i, &j, &w);
		matrix[i][j] = w;
		matrix[j][i] = w;
	}

	D = 1000000;

	// Inicia a região paralela
	#pragma omp parallel private(d)
	{
		// Vetor local para cada thread
		int local_ok[51];
		memset(local_ok, 0, sizeof(local_ok));

		// Distribuição dinâmica das tarefas
		#pragma omp for schedule(dynamic)
		for (v = 1; v <= c; v++) {

#ifdef DEBUG
			printf("Thread %d rodando cidade: %d\n", omp_get_thread_num(), v);
			fflush(stdout);
#endif

			local_ok[v] = 1;
			d = tsp(matrix, c, 0, v, v, local_ok, 1, D);
			local_ok[v] = 0;
			
			// Protege a escrita da menor distância global
			#pragma omp critical
			{
				if (d < D) {
					D = d;
				}
			}
		}
	}

	fclose(file);

	if ((file = fopen("tsp.out", "w")) == NULL) {
		perror("tsp.out");
		exit(1);
	}
	fprintf(file, "%d\n", D);
	fclose(file);

	return EXIT_SUCCESS;
}