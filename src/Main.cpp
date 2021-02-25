#include "grafo.hpp"

int main()
{
    int is_digraph = 1;
    int is_digraph1 = 0;
    //criando o grafo
    Graph *graph;
    graph = Create_Graph(5,5,0);
    //Inserindo no grafo
    insertEdges(graph,0,1,is_digraph,0);
    insertEdges(graph,1,3,is_digraph,0);
    insertEdges(graph,1,2,is_digraph,0);
    insertEdges(graph,2,4,is_digraph,0);
    insertEdges(graph,3,0,is_digraph,0);
    insertEdges(graph,3,4,is_digraph,0);
    insertEdges(graph,4,1,is_digraph,0);


    //criando o grafo 2 Para testar a arvore geradora mínima
    Graph *graph1;
    graph1 = Create_Graph(6,6,1);
    //Inserindo no grafo 2
    insertEdges(graph1,0,1,is_digraph1,6);
    insertEdges(graph1,0,2,is_digraph1,1);
    insertEdges(graph1,0,3,is_digraph1,5);
    insertEdges(graph1,1,2,is_digraph1,2);
    insertEdges(graph1,1,4,is_digraph1,5);
    insertEdges(graph1,2,3,is_digraph1,2);
    insertEdges(graph1,2,4,is_digraph1,6);
    insertEdges(graph1,2,5,is_digraph1,4);
    insertEdges(graph1,3,5,is_digraph1,4);
    insertEdges(graph1,4,5,is_digraph1,3);


    //------Buscas--------------
    // em profundidade
    int visited[5];
    deepSearch_Graph(graph, 0, visited);
    //em largura
    searchWidth_Graph(graph, 0, visited);
    //Busca pelo menor caminho
    int previous[5];
    float distance[5];
    searchShortestPath_Graph(graph, 0, previous, distance);


    //Árvore geradora Mínima
    //Algoritmo de Prim
    int father[6], i;
    algPRIM_Graph(graph1,0,father);
    for(i=0; i<6; i++)
        cout<<father[i]<<": "<<i<<endl;


    //Removendo no Grafo
    removeEdges(graph, 0,1,0);
    //Deletando o grafo
    Clean_Graph(graph);

    return 0;
}