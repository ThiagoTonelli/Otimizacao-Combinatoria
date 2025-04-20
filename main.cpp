#include <iostream>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <float.h>
#include <cstring>
#include "estruturas.h"
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MIN3(a, b, c) (MIN(MIN(a, b), c))


#define DIST(x1, y1, x2, y2) ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2))

solucao sol[1000];

int main() {
    clock_t inicio, fim;
    double tempo_total[5];
    char arquivo[30];
	double tempo_medio = 0;
    memset(matriz_custo, 0, sizeof(matriz_custo));
    memset(vet_dist, 0, sizeof(vet_dist));
    srand(time(NULL));
    std::cout << "Digite o numero de hubs: ";
    std::cin >> numeroHubs;
    std::cout << "\nDigite o nome do arquivo: ";
    std::cin >> arquivo;
    lerEntrada(arquivo);
    preencheMatriz();
    insertionSortPosicoes();
	sprintf(nome_saida, "hubs_%d_%s", numeroHubs, arquivo);
	
    for (int i = 0; i < 3; i++){
        inicio = clock();

        VNS(5, 3);

        fim = clock();
        
        tempo_total[i] = static_cast<double>(fim - inicio) / CLOCKS_PER_SEC;
        std::cout << "\nTempo de execução: " << tempo_total[i] << " segundos\n";
    	solAtual++;
    }
    tempo_medio = (tempo_total[0]+tempo_total[1]+tempo_total[2])/3;
    escreverInformacoes(tempo_medio);
    //imprimirSolucaoTela(sol[solAtual]);

    //printf("%lf", sol[solAtual].fo);

    //tempo_total = static_cast<double>(fim - inicio) / CLOCKS_PER_SEC;
    //std::cout << "\nTempo de execução: " << tempo_total << " segundos\n";

    return 0;
}

// Função para ler instância
void lerEntrada(char* arq) {
    FILE* f = fopen(arq, "r");
    fscanf(f, "%d", &numeroNos);
    for (int i = 0; i < numeroNos; i++) {
        fscanf(f, "%lf %lf", &matriz_cood[i][0], &matriz_cood[i][1]);
    }
    fclose(f);
}

// Função que preenche matriz de adjacencia
void preencheMatriz() {
    double aux = 0;
    for (int i = 0; i < numeroNos; i++) {
        for (int j = i; j < numeroNos; j++) {
            aux = sqrt(DIST(matriz_cood[i][0], matriz_cood[i][1], matriz_cood[j][0], matriz_cood[j][1]));
            matriz_custo[i][j] = aux;
            matriz_custo[j][i] = aux;
            vet_dist[i] += aux;
            if (i != j) {
                vet_dist[j] += aux;
            }
        }
    }
}

// Função para ordenar distancias e achar o elemento central
void insertionSortPosicoes() {
    int posKey;
    double key;
    int j = 0;

    for (int i = 0; i < numeroNos; i++) {
        vet_posicoes[i] = i;
    }
    for (int i = 1; i < numeroNos; i++) {
        posKey = vet_posicoes[i];
        key = vet_dist[posKey];
        j = i - 1;

        while (j >= 0 && vet_dist[vet_posicoes[j]] > key) {
            vet_posicoes[j + 1] = vet_posicoes[j];
            j--;
        }
        vet_posicoes[j + 1] = posKey;
    }
}

// Mapeamento dos hubs para facilitar a verificação posteriormente
void inicializaHubs(solucao &sol) {
    for (int i = 0; i < numeroHubs; i++) {
        hubSet.insert(sol.hubs[i]);
    }
}

// Verifica se o nó é um hub
bool verificaHub(int no) {
    return hubSet.count(no);
}

void inicializaNaoHub(solucao &sol){
    for(int i = 0; i < numeroNos; i++){
        if(!verificaHub(i)){
            vet_naohubs[i] = i;
        }
    }
}

// Construtiva
void construtiva(solucao &sol) {


    for (int i = 0; i < (numeroHubs / 2); i++) {
        sol.hubs[i] = vet_posicoes[i];
        sol.hubs[numeroHubs - (i + 1)] = vet_posicoes[numeroNos - (i + 1)];
    }
    if ((numeroHubs % 2) != 0) {
        sol.hubs[(numeroHubs / 2)] = vet_posicoes[(numeroNos / 2)];
    }


}

// Cria a solução de maneira gulosa e calcula a FO
void calculoFO(solucao &sol) {
    N = numeroNos;
    P = numeroHubs;
    sol.fo = 0;
    int hubA, hubB, melhorHub1, melhorHub2, index;
    double menorCustoPar, custoA, custoB, custoTotal;

    hubSet.clear();
    inicializaHubs(sol);

    //Teste com pontos conhecidos
    //sol->hubs[0] = 3;
    //sol->hubs[1] = 5;
    //sol->hubs[2] = 16;
    //sol->hubs[3] = 13;
    //inicializaHubs(sol);

    std::vector<double> menorDistancia(numeroNos, DBL_MAX);
    std::vector<int> hub1(numeroNos, -1);
    std::vector<int> hub2(numeroNos, -1);

    // pre calculo das dist entre nó e hubs
    double custosNosHubs[numeroNos][numeroHubs];

    for (int i = 0; i < numeroNos; i++) {
        for (int k = 0; k < numeroHubs; k++) {
            hubA = sol.hubs[k];
            custosNosHubs[i][k] = matriz_custo[i][hubA];
            if (verificaHub(i)) {
                custosNosHubs[i][k] -= matriz_custo[i][hubA] * 0.25;
            }
        }
    }

    for (int i = 0; i < numeroNos; i++) {
        menorDistancia[i] = DBL_MAX;
    }

    for (int i = 0; i < numeroNos; i++) {
        for (int j = i; j < numeroNos; j++) {
            menorCustoPar = DBL_MAX;
            melhorHub1 = -1;
            melhorHub2 = -1;


            // verifica caminho para um hub
            for (int k = 0; k < numeroHubs; k++) {
                hubA = sol.hubs[k];
                custoTotal = custosNosHubs[i][k] + custosNosHubs[j][k];

                if (custoTotal < menorCustoPar) {
                    menorCustoPar = custoTotal;
                    melhorHub1 = hubA;
                    melhorHub2 = hubA; // continua no mesmo hub
                }
            }

            // verifica caminho para dois hubs
            for (int k = 0; k < numeroHubs; k++) {
                hubA = sol.hubs[k];
                custoA = custosNosHubs[i][k];

                for (int l = 0; l < numeroHubs; l++) {
                    hubB = sol.hubs[l];

                    if (hubA == hubB) {
                        continue;
                    }

                    custoB = custosNosHubs[j][l];

                    custoTotal = custoA + (matriz_custo[hubA][hubB] * 0.75) + custoB;

                    if (custoTotal < menorCustoPar) {
                        menorCustoPar = custoTotal;
                        melhorHub1 = hubA;
                        melhorHub2 = hubB;
                    }
                }
            }

            //guarda o menor custo e os hubs correspondentes
            menorDistancia[i] = menorCustoPar;
            hub1[i] = melhorHub1;
            hub2[i] = melhorHub2;

            // Compara e guarda a maior FO
            if (menorCustoPar >= sol.fo) {
                sol.fo = menorCustoPar;
            }

            index = i * numeroNos + j;

            sol.matrizSol[index][0] = i;
            sol.matrizSol[index][3] = j;
            sol.matrizSol[index][1] = hub1[i];
            sol.matrizSol[index][2] = hub2[i];
            sol.matrizSol[index][4] = menorDistancia[i];

            index = j * numeroNos + i;
            sol.matrizSol[index][0] = j;
            sol.matrizSol[index][3] = i;
            sol.matrizSol[index][1] = hub2[i];
            sol.matrizSol[index][2] = hub1[i];
            sol.matrizSol[index][4] = menorDistancia[i];

        }
    }
}


// Função para imprimir uma solução na tela
void imprimirSolucaoTela(solucao &sol) {

    printf("n: %d\tp: %d\n", N, P);

    printf("FO:	%lf\n", sol.fo);

    printf("HUBS:\t[%d", sol.hubs[0]);
    for (int i = 1; i < numeroHubs; i++) {
        printf(", %d", sol.hubs[i]);
    }
    printf("]\n\n");

    printf("OR\t\tH1\t\tH2\t\tDS\t\tCUSTO\n");

	for(int i = 0; i < (numeroNos*numeroNos); i++){
          printf("%d\t\t%d\t\t%d\t\t%d\t\t%lf\n", (int )sol.matrizSol[i][0], (int )sol.matrizSol[i][1], (int )sol.matrizSol[i][2], (int )sol.matrizSol[i][3], sol.matrizSol[i][4]);
    }
}

// Função para imprimrir uma solução em arquivo
void imprimirSolucaoArq(solucao &sol){

    FILE* arquivo = fopen(nome_saida, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(arquivo, "n: %d\tp: %d\n", N, P);

    fprintf(arquivo, "FO:\t%lf\n", sol.fo);

    fprintf(arquivo, "HUBS:\t[%d", sol.hubs[0]);
    for (int i = 1; i < numeroHubs; i++) {
        fprintf(arquivo, ", %d", sol.hubs[i]);
    }
    fprintf(arquivo, "]\n\n");

    fprintf(arquivo, "OR\t\tH1\t\tH2\t\tDS\t\tCUSTO\n");

	for(int i = 0; i < (numeroNos*numeroNos); i++){
          fprintf(arquivo, "%d\t\t%d\t\t%d\t\t%d\t\t%lf\n", (int )sol.matrizSol[i][0], (int )sol.matrizSol[i][1], (int )sol.matrizSol[i][2], (int )sol.matrizSol[i][3], sol.matrizSol[i][4]);
     }
    fclose(arquivo);
}

// Função que copia uma outra solução
void clonaSolucao(solucao *sol1, solucao *sol2)
{
    memcpy(sol2, sol1, sizeof(solucao));
}

// Função que le uma solução e armazena na estrutura
void lerSolucao(solucao &sol){
	FILE *arq = fopen(nome_saida, "r");
	fscanf(arq, "n: %d\tp: %d\n", &N, &P);
	int no = N;
	int max_linha = no*no;
	fscanf(arq, "FO:\t%lf\n", &sol.fo);
	fscanf(arq, "HUBS:\t[%d", &sol.hubs[0]);
	for(int i = 1; i < P; i++){
		fscanf(arq, ", %d", &sol.hubs[i]);
	}
	fscanf(arq, "]\n\n");
	fscanf(arq, "OR\t\tH1\t\tH2\t\tDS\t\tCUSTO\n");
	for(int i = 0; i < max_linha; i++){
		fscanf(arq, "%lf\t\t%lf\t\t%lf\t\t%lf\t\t%lf\n", &sol.matrizSol[i][0], &sol.matrizSol[i][1], &sol.matrizSol[i][2], &sol.matrizSol[i][3], &sol.matrizSol[i][4]);
	}
	fclose(arq);
}


void melhorMelhora(solucao &s){
    int melhor_fo, fo_original, flag, hub_original, posicao_hub_ruim, melhor_hub;

    melhor_fo = s.fo;

    while(true){
        flag = 0;
        fo_original = s.fo;

        for(int i = 0; i < numeroHubs; i++){
            hub_original = s.hubs[i];

            for(int j = 0; j < numeroNos; j++){
                if(j != hub_original){
                    s.hubs[i] = j;
                    calculoFO(s);

                    if(s.fo < melhor_fo){
                        melhor_fo = s.fo;
                        posicao_hub_ruim = i;
                        melhor_hub = j;
                        flag = 1;
                    }
                }
            }
            s.hubs[i] = hub_original;
        }

        if(flag){
            s.hubs[posicao_hub_ruim] = melhor_hub;
        }
        else{
            break;
        }
    }
}

void geraVizinho(solucao &sol, int opcao){
	if (opcao == 1){
        inicializaNaoHub(sol);
        int num_ale_nos = rand() % ((numeroNos - numeroHubs)+ 1);
        int num_ale_hubs = rand() % (numeroHubs + 1);
        sol.hubs[num_ale_hubs] = vet_naohubs[num_ale_nos];
    }
    else if (opcao == 2){
        double porcentagem = 50.0 + ((rand() / (double)RAND_MAX) * 40.0); // Gera um número entre 50.0 e 90.0
        int resultado = (int)((porcentagem / 100.0) * numeroHubs); // Calcula o valor relativo
        for (int j = 0; j < resultado; j++){
            sol.hubs[j] = vet_naohubs[j];
        }
    }
    else {
        double porcentagem = 10.0 + ((rand() / (double)RAND_MAX) * 20.0);
        int resultado = (int)((porcentagem / 100.0) * numeroHubs);
        for (int j = 0; j < resultado; j++){
            sol.hubs[j] = vet_naohubs[j];
        }
    }
    calculoFO(sol);
}

void VNS(int VNSmax, int viz){
    double tempo_corrido = 0.0;
    const double tempo_maximo = VNSmax * 60;
    int k = 0, i = 0;

    construtiva(sol[solAtual]);
    calculoFO(sol[solAtual]);
    fo_inicial[solAtual] = sol[solAtual].fo;

    clonaSolucao(&sol[solAtual], &sol[solAtual+1]);

    clock_t tempo_inicial = clock();
    tempo_corrido = (double)(clock() - tempo_inicial) / CLOCKS_PER_SEC;

    while (tempo_corrido <= tempo_maximo){
        k = 1;
        while(k <= viz){
      
           geraVizinho(sol[solAtual+1], k);

           melhorMelhora(sol[solAtual+1]);
           calculoFO(sol[solAtual+1]);
           tempo_corrido = (double)(clock() - tempo_inicial) / CLOCKS_PER_SEC;

           if((sol[solAtual+1].fo) < (sol[solAtual].fo)){
                tempo_corrido = (double)(clock() - tempo_inicial) / CLOCKS_PER_SEC;
                clonaSolucao(&sol[solAtual+1], &sol[solAtual]);
                k = 1;
                saidas_tempo.push_back(tempo_corrido);
                saidas_fo.push_back(sol[solAtual].fo);
                saidas_exec.push_back(solAtual);
           }
           else if(tempo_corrido > 300.00){
           		goto fim;
           }
           else{
			    k = k + 1;
           }
          
       }
       tempo_corrido = (double)(clock() - tempo_inicial) / CLOCKS_PER_SEC;
    }
    fim:
}

void escreverInformacoes(double tempo_medio){
    sprintf(nome_saida_info, "INFO_%s", nome_saida);
    FILE* arquivo = fopen(nome_saida_info, "w");
    fprintf(arquivo, "INFORMAÇÕES SOBRE A SAIDA.\n\n");
    fprintf(arquivo, "Solucao Inicial: %lf.\n\n", fo_inicial[solAtual-1]);
    fprintf(arquivo, "Tempo Médio: %lf.\n\n", tempo_medio);
    for (size_t i = 0; i < saidas_fo.size(); ++i) {
        fprintf(arquivo, "Execução: %d\tFO: %lf\tTEMPO: %lf\n", saidas_exec[i], saidas_fo[i], saidas_tempo[i]);
    }
    fclose(arquivo);
    if(sol[0].fo < sol[1].fo && sol[0].fo < sol[2].fo){
    	imprimirSolucaoArq(sol[0]);
    }
    else if(sol[1].fo < sol[0].fo && sol[1].fo < sol[2].fo){
    	imprimirSolucaoArq(sol[1]);
    }
    else {
    	imprimirSolucaoArq(sol[2]);
    }
}



