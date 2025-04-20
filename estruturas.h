#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED
#define MAX_TAM 40000
#define MAX_HUBS 50 
#define MAX_NO 200


//estrutura para armazenar solucoes
typedef struct Solucao{
    double fo;
    int hubs[MAX_HUBS];
    double matrizSol[MAX_TAM][5];
}solucao;

int N, P;

//var para guardar nome da saida
char nome_saida[100];
char nome_saida_info[100];


//vetor de distancia
double vet_dist[MAX_NO];

//variaveis para controlar numero de hubs e nos da instancia atual
int numeroHubs, numeroNos;

//var para controlar numero de solucoes
int solAtual = 0;

//
std::unordered_set<int> hubSet;
std::unordered_set<int> naoHub;
std::vector<double> saidas_fo;
std::vector<double> saidas_tempo;
std::vector<int> saidas_exec;
double fo_inicial[50];


//matriz que guarda valores de entrada
double matriz_cood[MAX_NO][2];

//matriz de adjacencia
double matriz_custo[MAX_NO][MAX_NO];

//vetor que guarda os nós de forma crescente de acordo com a soma das distancias
int vet_posicoes[MAX_NO];

int vet_naohubs[MAX_NO];

void lerEntrada(char *arq);
void preencheMatriz();
void imprimeMatriz();
void insertionSortPosicoes();
void imprimeVetor();
void calculoFO(solucao &sol);
void clonaSolucao(solucao *sol1, solucao *sol2);
void imprimirSolucaoTela(solucao &sol);
void imprimirSolucaoArq(solucao &sol);
bool verificaHub(int no);
void inicializaHubs(solucao &sol);
void inicializaNaoHub(solucao &sol);
void lerSolucao(solucao &sol);
void construtiva(solucao &sol);
void melhorMelhora(solucao &s);
void geraVizinho(solucao &sol);
void VNS(int VNSmax, int viz);
void escreverInformacoes(double tempo_medio);

extern solucao sol[];
#endif // ESTRUTURAS_H_INCLUDED

