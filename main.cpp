#include "src/mylib.hpp"
#include <iostream>
#include <string>

int main() {
    Graph my_graph((std::string)"grafos_pesos_txt/grafo_W_5.txt", 'v', true); // Cria a instância e atribue a uma variável // v=vetor, m=matriz
    //my_graph.CalculateGraphStats(); 
    //my_graph.ConnectedComponents();
    //my_graph.BFS(1, 1, 0, 1, 1);
    //my_graph.DFS(1);
    //my_graph.Distance(1, 2);
    // my_graph.PrintRepresentation();
    // my_graph.Diametro();

    my_graph.Dijkstra(10, 20, true);
    // my_graph.freeAll();
    return 0;
}
