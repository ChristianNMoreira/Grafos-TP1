#include "src/mylib.hpp"
#include <iostream>
#include <string>

int main() {
    Graph my_graph((std::string)"grafos_pesos_txt/grafo_W_5.txt", 'v', true); // Cria a instância e atribue a uma variável // v=vetor, m=matriz

    my_graph.Dijkstra(10, 30, true);
    my_graph.freeAll();
    
    return 0;
}
