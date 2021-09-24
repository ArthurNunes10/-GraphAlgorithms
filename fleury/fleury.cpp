#include <string.h>
#include <list>
#include <algorithm>
#include <iostream>
using namespace std;

//Estrutura o grafo
class Grafo {
    // Número total de vértices
	int numVertices;
    //Lista de adjacência 
	list<int>* adjacencia; 
public:
    // Cria uma estrutura de grafo com base no número de vértices
	Grafo (int numVertices){
		this->numVertices = numVertices;
		adjacencia = new list<int>[numVertices];
	}

	~Grafo() {
        delete[] adjacencia;
    }

	// Adiciona arestas
	void adicionarAresta(int v1, int v2){
		adjacencia[v1].push_back(v2);
		adjacencia[v2].push_back(v1);
	}

    // Remove arestas
	void removerAresta(int v1, int v2){
        list<int>::iterator iteratorV2 = find(adjacencia[v1].begin(), adjacencia[v1].end(), v2);
        *iteratorV2 = -1;
        list<int>::iterator iteratorV1 = find(adjacencia[v2].begin(), adjacencia[v2].end(), v1);
	    *iteratorV1 = -1;
    }

    // Conta vértices acessiveis
    int contarVerticesAcessiveis(int vertice, bool visitados[]){
        int contador = 1;
        visitados[vertice] = true;
        list<int>::iterator ivertice;

        for(ivertice = adjacencia[vertice].begin(); ivertice != adjacencia[vertice].end(); ++ivertice){
            if(*ivertice != -1 && !visitados[*ivertice]){
                contador += contarVerticesAcessiveis(*ivertice, visitados);
            }
        }
        return contador;
    }

    // Verifica se aresta pode ser considerada a aresta seguinte do percurso Euleriano
    bool verificaAresta(int v1, int v2){
        int contador = 0;
        list<int>::iterator ivertice;
        for(ivertice = adjacencia[v1].begin(); ivertice != adjacencia[v1].end(); ++ivertice){
            if(*ivertice != -1){
                contador++;
            }
        }
        if(contador == 1){
            return true;
        }

        bool visitados[numVertices];
        memset(visitados, false, numVertices);
	    int numVerticesAcessiveis = contarVerticesAcessiveis(v1, visitados);

        removerAresta(v1,v2);
        memset(visitados, false, numVertices);
        int numVerticesAcessiveis02 = contarVerticesAcessiveis(v1, visitados);

        adicionarAresta(v1,v2);
        if(numVerticesAcessiveis > numVerticesAcessiveis02){
            return false;
        }
        else{
            return true;
        }
    }

    //Imprime caminho
    void imprimeCaminho(int v1,bool writeFile, FILE *outFile){
        list<int>::iterator ivertice;
        for(ivertice = adjacencia[v1].begin(); ivertice != adjacencia[v1].end(); ++ivertice){
            int v2 = *ivertice;
            if (v2 != -1 && verificaAresta(v1,v2)){
				if (writeFile){
					fprintf(outFile,"(%d, %d) ", v1,v2);
				}
				else{
					printf("(%d, %d) ", v1,v2);
				}
                removerAresta(v1,v2);
                imprimeCaminho(v2,writeFile,outFile);
            }
        }
    }
    
	void fleury(bool writeFile, FILE *outFile){
        int verticeGrauImpar = 0;
        for (int i = 0; i < numVertices; i++){
            if((adjacencia[i].size() % 2) != 0){
                verticeGrauImpar = i;
                break;
            }
        }
        imprimeCaminho(verticeGrauImpar, writeFile, outFile);
        cout << endl;    
    }
};


int main(int argc, char const *argv[])
{
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
    
    Grafo grafo(sizeVertex+1);

    //Armazena arestas
    for(int i = 0; i<sizeEdge; ++i){
		fscanf(inFile,"%i %i", &v1, &v2);
        grafo.adicionarAresta(v1,v2);
	}

    grafo.fleury(o, outFile);
	return 0;
}
