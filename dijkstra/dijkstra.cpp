#include <list>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <stack>
#include <iostream>
#include <string>
#include <list>

#define INF 100000002

using namespace std;

// Matriz de adjacência
int weights [251][251];
// Faz o controle dos vértices vizitados
list<int>  indexVisited;
list<int>::iterator it, aux;

int main(int argc, char const *argv[]){
    FILE *inFile;
    FILE *outFile;
    
    inFile = fopen("arquivo-entrada.dat", "r");

    int sizeVertex = 0;
    int sizeEdge = 0;
    int costEdge = 0;
    int firstVertex = 0;
    int lastVertex = 0;
    int weight = 0;
    int current = 0;
    int smallest = 0;
    int v1, v2;
    int cost = 0;
    int visit = 0;
    bool s = false;
    bool o = false;
    bool init = false;
    bool last = false;

    if(argc > 1){
        for (int i = 0; i < argc; i++){
            if ((argc == 2) and !strcmp(argv[1],"-h")){
                printf("-h : mostra o help\n");
                printf("-o <arquivo> : redireciona a saida para o ‘‘arquivo’’\n");
                printf("-f <arquivo> : indica o ‘‘arquivo’’ que contém o grafo de entrada\n");
                printf("-s : mostra a solução (em ordem crescente)\n");
                printf("-i : vértice inicial (dependendo do algoritmo)\n");
                printf("-l : vértice final (dependendo do algoritmo)\n");
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
                    firstVertex = atoi(argv[++i]);
                }
                else if (!strcmp(argv[i],"-l")){
                    last = true;
                    lastVertex = atoi(argv[++i]);
                }
            }   
        }
    }

    if(!init){
        printf("Erro: Insira um vértice inicial válido");
        return 0;
    }
    if(!last){
        printf("Erro: Insira um vértice final válido");
        return 0;
    }

    // Faz a leitura do número de vértices e arestas
    if(!(inFile == NULL)){
        fscanf(inFile ,"%d %d", &sizeVertex, &sizeEdge);   
    }

    // Caso o vértice inicial não seja passado
    if(!init){
        firstVertex = 0;
    }
    // Caso vértice final não seja passado
    if(!last){
        lastVertex = sizeVertex;
    }

    vector <int> adj[sizeVertex+1];
	int estCost[sizeVertex+1];
	bool vertexEnabled[sizeVertex+1];

    // Colocando estimativa de custo infinita para os vértices
	for(int i=0; i<sizeVertex+1; ++i){
		estCost[i] = INF;
		vertexEnabled[i] = true;
	}

    // Realiza leitura do grafo
    for(int i = 0; i<sizeEdge; ++i){
		fscanf(inFile,"%d %d %d", &v1, &v2, &weight);
        weights[v1][v2] = weight;
		weights[v2][v1] = weight;
        // Armazena as adjacências
        adj[v2].push_back(v1);
		adj[v1].push_back(v2);	
	}
	
    // Dijkstra:
	estCost[firstVertex] = 0;
	indexVisited.push_back(firstVertex);

	while(!indexVisited.empty()){
		//Etapa 1: Procura vértice aberto de menor estimativa de custo
		smallest = INF;
		for(it = indexVisited.begin(); it != indexVisited.end(); ++it){
			visit = (*it);
			if(estCost[visit] < smallest){
				smallest = estCost[visit];
				current = visit;
				aux = it;
			}			
		}
        //Etapa 2: Bloquear o vértice para impedir que ele seja visitado novamente
		indexVisited.erase(aux);
		vertexEnabled[current] = false;
 
 
		//Etapa 3: Atualizando custo previsto dos vértices vizinhos
		for(int i=0; i<int(adj[current].size()); ++i){
			visit = adj[current][i];

			if(vertexEnabled[visit]){
				//Calcula o custo até o vértice atual
				cost = weights[current][visit] + estCost[current];
                if(s){
                    if (o){
                        fprintf(outFile, "Estimativa de custo de %d para %d é %d=>(Total acumulado de %d até %d = %d)\n", current, visit, weights[current][visit], firstVertex,visit,cost);
                    }
                    else{
                        printf("Estimativa de custo de %d para %d é %d=>(Total acumulado de %d até %d = %d)\n", current, visit, weights[current][visit], firstVertex,visit,cost);
                        } 
                }
				if(estCost[visit] == INF){
					estCost[visit] = cost;
					indexVisited.push_back(visit);
				}
				else{
					if(cost < estCost[visit]){
						estCost[visit] = cost;
					}
				}
			}
		}
	}
    
	if(estCost[lastVertex] == INF){
        if(o){
            fprintf(outFile,"Não é possível chegar ao vértice final\n");
        }
        else{
            printf("Não é possível chegar ao vértice final\n");	
        }
	}
	else{
        if(o){
            fprintf(outFile,"Custo mínimo de %d a %d: %d\n",firstVertex, lastVertex, estCost[lastVertex]);	
        }
        else{
            printf("Custo mínimo de %d a %d: %d\n",firstVertex, lastVertex, estCost[lastVertex]);	
        }
	}
	indexVisited.clear();
    //fclose(inFile);
    //close(outFile);
}
