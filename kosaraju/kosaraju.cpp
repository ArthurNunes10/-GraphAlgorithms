#include <iostream>
#include <stack>
#include <string.h>
#include <list>
#include <algorithm>
#include <iostream>
using namespace std;

class Grafo{
private:
	int numVertices;
	list<int>* listaAdjacencia; 

	void initAdj(int vertice, bool visit[], stack<int>& pilha){
		visit[vertice] = true;
		list<int>::iterator itrvertex;
		for(itrvertex = listaAdjacencia[vertice].begin(); itrvertex != listaAdjacencia[vertice].end(); itrvertex++){
			if(visit[*itrvertex] == false)
				initAdj(*itrvertex, visit, pilha);
		}
		pilha.push(vertice);
	}

	void BuscaProfundidade(int vertice, bool visit[], bool o, FILE *outFile){
		visit[vertice] = true;
        if(o){
            fprintf(outFile,"%d ",vertice);
        }
        else{
            printf("%d ",vertice);
        }
		list<int>::iterator itrvertex;
		for(itrvertex = listaAdjacencia[vertice].begin(); itrvertex != listaAdjacencia[vertice].end(); itrvertex++){
			if(visit[*itrvertex] == false){
                BuscaProfundidade(*itrvertex, visit, o, outFile);
            }	
		}
	}

public:

	Grafo(int numVertices){
		this->numVertices = numVertices;
		listaAdjacencia = new list<int>[numVertices];
	}

	void addEdges(int v1, int v2){
		listaAdjacencia[v1].push_back(v2);
	}

	Grafo transporGrafo(){
		Grafo gt(numVertices);
		for(int i = 0; i < numVertices; i++){
			list<int>::iterator itrVertex;
			for(itrVertex = listaAdjacencia[i].begin(); itrVertex != listaAdjacencia[i].end(); itrVertex++){
				gt.listaAdjacencia[*itrVertex].push_back(i);
			}
		}
		return gt;}

	void kosaraju(bool o, FILE *outFile){
		stack<int> pilha;
		bool * visit = new bool[numVertices];

		for(int i = 0; i < numVertices; i++){
            visit[i] = false;
        }
		for(int i = 0; i < numVertices; i++){
			if(visit[i] == false)
				initAdj(i, visit, pilha);
		}

		Grafo gt = transporGrafo();

		for(int i = 0; i < numVertices; i++)
			visit[i] = false;
        
        int count = 1;

		while(!pilha.empty()){
			int vert = pilha.top();
			pilha.pop();
			if(visit[vert] == false){
                if(o){
                    fprintf(outFile,"Componente conexa %d:\n",count);
                }
                else{
                    printf("Componente conexa %d: \n", count);
                }
                count++;
				gt.BuscaProfundidade(vert, visit, o, outFile);
                if(o){
                    fprintf(outFile," \n");
                }
                else{
                    printf(" \n");
                }
                
			}
		}
	}
};

int main(int argc, char *argv[]){
    FILE *inFile;
    FILE *outFile;
    inFile = fopen("arquivo-entrada.dat", "r");

    bool o = false;
    int sizeVertex = 0;
    int sizeEdge = 0;
    int v1 = 0;
    int v2 = 0;
    // Trata os argumentos
    if(argc > 1){
        for (int i = 0; i < argc; i++){
            if ((argc == 2) and !strcmp(argv[1],"-h")){
                printf("-h : mostra o help\n");
                printf("-o <arquivo> : redireciona a saida para o ‘‘arquivo’’\n");
                printf("-f <arquivo> : indica o ‘‘arquivo’’ que contém o grafo de entrada\n");
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
            }   
        }
    }

    //Faz a leitura do número de vértices e arestas
    if(!(inFile == NULL)){
        fscanf(inFile ,"%d %d", &sizeVertex, &sizeEdge);   
    }
    
    Grafo grafo(sizeVertex);

    //Armazena arestas
    for(int i = 0; i<sizeEdge; ++i){
		fscanf(inFile,"%i %i", &v1, &v2);
        grafo.addEdges(v1,v2);
	}

    if(o){
        fprintf(outFile,"Componentes fortemente conexas:\n");
    }
    else{
        printf("Componentes fortemente conexas:\n");
    }

    grafo.kosaraju(o,outFile);
	
	return 0;
}