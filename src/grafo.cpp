#include "grafo.hpp"


//-------------------------CRIAÇÃO E EXCLUSÃO DE GRAFOS-----------------------------------------
Graph *Create_Graph(int numb_vertices, int max_degree, int is_weighted)
{
    Graph *graph = new Graph;
    if(graph != NULL)
    {
        int i;
        graph->numb_vertices = numb_vertices;
        graph->max_degree = max_degree;
        graph->is_weighted = (is_weighted != 0)?1:0;
        graph->grade = (int*) calloc(numb_vertices,sizeof(int));
        graph->edges = (int**) malloc(numb_vertices * sizeof(int*));
        if(graph->is_weighted)
            graph->weights = (float**)malloc(numb_vertices * sizeof(float*));
        for(i = 0; i < numb_vertices; i++)
        {
            graph->edges[i] = (int*) malloc(max_degree * sizeof(int));
            if(graph->is_weighted)
            {
                graph->weights[i] = (float*) malloc(max_degree * sizeof(float));
            }
        }
    }
    return graph;
}

void Clean_Graph(Graph *graph)
{
    if(graph != NULL)
    {
        int i;
        for(i = 0; i < graph->numb_vertices; i++)
            delete graph->edges[i];
        delete graph->edges;

        if(graph->is_weighted)
        {
            for(i = 0; i < graph->numb_vertices; i++)
                delete graph->weights[i];
            delete graph->weights;
        }
        delete graph->grade; 
        delete graph;
    }
}

// ---------------------------------INSERÇÃO E REMOÇÃO DE ARESTAS -------------------------------------
int insertEdges(Graph *graph, int origin, int destiny, int is_digraph, float weight)
{
    if(graph == NULL)
        return 0;
    if(origin < 0 || origin >= graph->numb_vertices)
        return 0;
    if(destiny < 0 || destiny >= graph->numb_vertices)
        return 0;
    
    graph->edges[origin][graph->grade[origin]] = destiny; 
    if(graph->is_weighted)
    {
        graph->weights[origin][graph->grade[origin]] = weight;
    }

    graph->grade[origin]++;

    if(is_digraph == 0)
        insertEdges(graph, destiny, origin, 1, weight);
    
    return 1;
}
int removeEdges(Graph *graph, int origin, int destiny, int is_digraph)
{
    if(graph == NULL)
        return 0;
    if(origin < 0 || origin >= graph->numb_vertices)
        return 0;
    if(destiny < 0 || destiny >= graph->numb_vertices)
        return 0;
    
    //Descobrindo a posição na matriz de adjacencia do item a ser removido
    int i = 0;
    while(i < graph->grade[origin] && graph->edges[origin][i] != destiny)
        i++;
    if(i == graph->grade[origin])
        return 0;

    //Romeve o elemento (Pega a ultima posição e cola na posição que foi removida)
    graph->grade[origin]--;
    graph->edges[origin][i] = graph->edges[origin][graph->grade[origin]];
    if(graph->is_weighted)
        graph->weights[origin][i] = graph->weights[origin][graph->grade[origin]];
    if(is_digraph == 0)
        removeEdges(graph, destiny, origin, 1);
    return 1;
}

//------------------------------------FAZENDO BUSCAS-------------------------------------------
//Busca em Profundidade (explora o máximo possível de cada ramo antes de retornar 
//(encontra componentes conectados fortemente e serve para solucionar labirintos)
void deepSearch(Graph *graph, int origin, int *visited, int cont)
{
    visited[origin] = cont;
    for(int i = 0; i < graph->grade[origin]; i++)
    {
        if(!visited[graph->edges[origin][i]])
            deepSearch(graph, graph->edges[origin][i], visited, cont+1);
    }

}
void deepSearch_Graph(Graph *graph, int origin, int *visited)
{
    int cont = 1;
    for(int i = 0; i < graph->numb_vertices; i++)
        visited[i] = 0;
    deepSearch(graph, origin, visited, cont);
}

//Busca em Largura (explora todos os vertices vizinhos primeiro)
//(Achar componentes conectados e testar bipartição em grafos)
void searchWidth_Graph(Graph *graph, int origin, int *visited)
{
    int i, edge, NV, cont = 1, *fila, IF = 0, FF = 0;
    for(i = 0; i < graph->numb_vertices; i++)
        visited[i] = 0;
    NV = graph->numb_vertices;
    fila = (int*) malloc(NV*sizeof(int));
    FF++;
    fila[FF] = origin;
    visited[origin] = cont;

    while(IF != FF)
    {
        IF = (IF+1) % NV;
        edge = fila[IF];
        cont++;
        for (i = 0; i < graph->grade[edge]; i++)
        {
            if(!visited[graph->edges[edge][i]])
            {
                FF = (FF+1)%NV;
                fila[FF] = graph->edges[edge][i];
                visited[graph->edges[edge][i]] = cont;
            }
        }
    }
    delete fila;
}

//Busca pelo menor caminho 
int searchShortestPath(float *distance, int *visited, int NV)
{
    int i, smaller = -1, first = 1;
    for(i = 0; i < NV; i++)
    {
        if(distance[i] >= 0 && visited[i] == 0)
        {
            if(first)
            {
                smaller = i;
                first = 0;
            }
            else{
                if(distance[smaller] > distance[i])
                    smaller = i;
            }
        }
    }
    return smaller;
}
void searchShortestPath_Graph(Graph *graph, int origin, int *previous, float *distance)
{
    int i, cont, NV,ind, *visited, smaller;
    cont = NV = graph->numb_vertices;
    visited = new int[NV];
    for(i = 0; i < NV; i++)
    {
        previous[i] = -1;
        distance[i] = -1;
        visited[i] = 0;
    }
    distance[origin] = 0;
    while(cont > 0)
    {
        smaller = searchShortestPath(distance, visited, NV);
        if(smaller == -1)
            break;
        visited[smaller] = 1;
        cont--;
        for(i = 0; i < graph->grade[smaller]; i++)
        {
            ind = graph->edges[smaller][i];
            if(distance[ind] < 0)
            {
                distance[ind] = distance[smaller] + 1;
                //Ou podemos calcular pelo peso das arestas
                //distance[ind] = distance[smaller] + graph->weights[smaller][i];
                previous[ind] = smaller;
            }
            else{
                if(distance[ind] > distance[smaller] + 1)
                {
                    distance[ind] = distance[smaller] + 1;
                    //Ou podemos calcular pelo peso das arestas
                    //distance[ind] = distance[smaller] + graph->weights[smaller][i];
                    previous[ind] = smaller;
                }
            }
        }
    }
    delete visited;

}

//Arvore Geradora Mínima (cria um sub grafo com o menor número de arestas)
//As arestas são pondeiradas e o sub grafo mantém as de menores custos, 
void algPRIM_Graph(Graph *graph, int origin, int *father)
{
    int i, j, first, destiny, NV = graph->numb_vertices;
    double lowerWeight;
    for(i = 0; i < NV; i++)
        father[i] = -1;
    father[origin] = origin;
    while(true)
    {
        first = 1;
        for(i = 0; i < NV; i++)             //percorre todos os vertices
        {
            if(father[i] != -1)             //Achou vértices já visitados
            {
                for(j = 0; j < graph->grade[i]; j++)        //Percorre os vizinhos do vértice visitado
                {
                    if(father[graph->edges[i][j]] == -1)    //Encontrou vértice vizinho não visitado
                    {
                        if(first)                           //Procura aresta de menor custo
                        {
                            lowerWeight = graph->weights[i][j];
                            origin = i;
                            destiny = graph->edges[i][j];
                            first = 0;
                        }
                        else if(lowerWeight > graph->weights[i][j])
                        {
                            lowerWeight = graph->weights[i][j];
                            origin = i;
                            destiny = graph->edges[i][j];
                        }
                    }
                }
            }
        }
        if(first == 1)
            break;
        father[destiny] = origin;
    } 
}