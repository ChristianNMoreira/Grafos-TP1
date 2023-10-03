#include "src/mylib.hpp"
#include <iostream>
#include <string>

int main() {
    Graph my_graph((std::string)"grafo_3.txt", 'v'); // Cria a instância e atribue a uma variável // v=vetor, m=matriz
    //my_graph.CalculateGraphStats(); 
    //my_graph.ConnectedComponents();
    //my_graph.BFS(1, 1, 0, 1, 1);
    //my_graph.DFS(1);
    //my_graph.Distance(1, 2);
    //my_graph.PrintRepresentation();
    my_graph.Diametro();
  
    return 0;
}
