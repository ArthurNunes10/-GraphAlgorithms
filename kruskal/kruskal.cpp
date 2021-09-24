#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

// Definindo um limite de vértices para o grafo
#define MAX_VETEX 100

// Estrutura das arestas (dois vértices e seu respectivo peso)
struct edge{
    int v1, v2;
    int weight;
};

// Estrutura utilizada evitar criação de ciclos
// Cada elemento do vetor consiste de um vértice fixo
// Caso no momento da escolha da aresta ela contenha um vértice pertencente a esse array, a aresta é desconsiderado
int cycles[MAX_VETEX];

// Vértice âncora que ajudará na detecção de ciclos
int anchor(int vertex){
    if (cycles[vertex] == vertex){
        return vertex;
	}
    cycles[vertex] = anchor(cycles[vertex]);
    return cycles[vertex];
}

// Realiza a união entre os vértices
void joinVertex(int v1, int v2){
    cycles[anchor(v1)] = anchor(v2);
}

// Ordena arestas em ordem crescente de peso
void weightSort(edge *E, int arraySize ){  
  
    int index=0;  

    // Determinamos um limite para o peso
    stack <edge> weight[MAX_VETEX];
    
    // Vetor weight recebe arestas e pesos.
    // wight[peso] = aresta
	for(int i = 0; i<arraySize; ++i){
		weight[E[i].weight].push(E[i]);
	}  
    // Percorre o vetor de pesos
	for(int i=0; i<arraySize; i++){
        //Se vazio continue, caso contrário armazena peso e aresta no topo array de arestas
		while(weight[index].empty()){ 
			index++;
		}
		E[i] = weight[index].top();
		weight[index].pop();
		index--;
	}
}

int main(int argc, char const *argv[]){
    FILE *inFile;
    FILE *outFile;

    inFile = fopen("arquivo-entrada.dat", "r");

    int sizeVertex = 0;
    int sizeEdge = 0;
    int costEdge = 0;
    int firstVertex = 0;
    int lastVertex = 0;
    bool s = false;
    bool o = false;
    bool init = false;
    bool last = false;
    
    // Trata os argumentos
    if(argc > 1){
        for (int i = 0; i < argc; i++){
            if ((argc == 2) and !strcmp(argv[1],"-h")){
                printf("-h : mostra o help\n");
                printf("-o <arquivo> : redireciona a saida para o ‘‘arquivo’’\n");
                printf("-f <arquivo> : indica o ‘‘arquivo’’ que contém o grafo de entrada\n");
                printf("-s : mostra a solução (em ordem crescente)\n");
                return 0;
            }
            else {
                if (!strcmp(argv[i],"-o")){
                    o = true;
                    outFile = fopen(argv[++i], "wb");
                }
                else if (!strcmp(argv[i],"-f")){
                    inFile = fopen(argv[++i], "r");
                }
                else if (!strcmp(argv[i],"-s")){
                    s = true;
                }
                else if (!strcmp(argv[i],"-i")){
                    init = true;
                    cout << "Parâmetro não disponível para esse algoritmo\n";
                    return 0;
                    firstVertex = atoi(argv[++i]);
                }
                else if (!strcmp(argv[i],"-l")){
                    last = true;
                    cout << "Parâmetro não disponível para esse algoritmo\n";
                    return 0;
                    lastVertex = atoi(argv[++i]);
                }
            }   
        }
    }
    
    //Faz a leitura do número de vértices e arestas
    if(!(inFile == NULL)){
        fscanf(inFile ,"%d %d", &sizeVertex, &sizeEdge);   
    }
    
    //printf("%d %d\n",sizeVertex,sizeEdge);

    edge graphEdges[sizeEdge];

    // Incluindo vértices no vetor de ciclos
    for(int i = 0; i<sizeVertex; ++i){
        cycles[i] = i;
    }

    // Ler as arestas e armazena no vetor
	for(int i = 0; i<sizeEdge; ++i){
		fscanf(inFile,"%d %d %d", &graphEdges[i].v1, &graphEdges[i].v2, &graphEdges[i].weight);
	}

    // Ordena as arestas pelo peso
    weightSort(graphEdges, sizeEdge);

    // Execução do algoritmo de Kruskal
    for(int i = 0; i < sizeEdge; ++i){
        if(anchor(graphEdges[i].v1) != anchor(graphEdges[i].v2)){
            costEdge += graphEdges[i].weight;
            // Imprimindo árvore
            if (s){
                if(o){
                    if (i==0){
                        fprintf(outFile,"Árvore em ordem: ");
                    }
                    if (graphEdges[i].v1 < graphEdges[i].v2){
                     fprintf(outFile,"(%d,%d) ", graphEdges[i].v1, graphEdges[i].v2);
                    }
                    else{
                        fprintf(outFile,"(%d,%d) ", graphEdges[i].v2, graphEdges[i].v1);
                    }
                }
                else{
                    if (i==0){
                        cout << "Árvore em ordem: ";
                    }
                    if (graphEdges[i].v1 < graphEdges[i].v2){
                     printf("(%d,%d) ", graphEdges[i].v1, graphEdges[i].v2);
                    }
                    else{
                        printf("(%d,%d) ", graphEdges[i].v2, graphEdges[i].v1);
                    }
                }
            }
            joinVertex(graphEdges[i].v1, graphEdges[i].v2);
        }
    }
    if(o){
        if(s){
            fprintf(outFile,"\n");
        }
        fprintf(outFile, "Custo da árvore: %d\n", costEdge);
    }
    else{
        if(s){
            printf("\n");
        }
        printf("Custo da árvore: %d\n", costEdge);
    }

    fclose(outFile);
    fclose(inFile);
    return 0;
}
