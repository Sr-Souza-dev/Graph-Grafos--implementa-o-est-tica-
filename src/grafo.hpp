#ifndef _GRAFO_HPP
#define _GRAFO_HPP
#endif

#include <iostream>
using namespace std;

typedef struct graph Graph;

struct graph{
    int is_weighted;                //Suas arestas são pondeiradas?
    int numb_vertices;              //Número de vertices máximo do grafo
    int max_degree;                 //Maior grau de ligações (qtd maxima de arestas ligadas a um unico vertice)
    int** edges;                    //Arestas do grafo (Matriz [vertices][qtd conexões])
    float** weights;                //Pesos do grafo (Caso vá trabalhar com grafos ponderados)
    int* grade;                     //Vetor que guarda quantas arestas estão conecetados em cada vertice
};

//Criação e exclusão do grafo
Graph *Create_Graph(int numb_vertices, int max_degree, int is_weighted);            //Cria o grafo alocando varios espaços de memória
void Clean_Graph(Graph *graph);                                                     //Deleta o grafo

//inserção e remoção
int insertEdges(Graph *graph, int origin, int destiny, int is_digraph, float weight);       //Insere elementos no grafo
int removeEdges(Graph *graph, int origin, int destiny, int is_digraph);                     //Remove o elemento colocando a ultima interação na posição a ser removida

//Buscas
//Busca em profundidade
void deepSearch_Graph(Graph *graph, int origin, int *visited);          //(Função principal) -> Faz interface com o usuario
void deepSearch(Graph *graph, int origin, int *visited, int cont);       //(Função auxiliar) -> Realiza o cálculo
//Busca em largura
void searchWidth_Graph(Graph *graph, int origin, int *visited);
//Busca pelo menor caminho
int searchShortestPath(float *distance, int *visited, int NV);                                     //Procura vertice com menor distância que não tenha sido visitado
void searchShortestPath_Graph(Graph *graph, int origin, int *previous, float *distance);

//Arvore Geradora Mínima (cria um sub grafo com o menor número de arestas)
//As arestas são pondeiradas e o sub grafo mantém as de menores custos, 
void algPRIM_Graph(Graph *graph, int origin, int *father);                      //Considera um vertice inicial para a construção da arvore (constroi uma unica arvore ao longo do tempo)

