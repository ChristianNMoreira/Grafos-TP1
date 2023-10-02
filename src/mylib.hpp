#pragma once

#include <string>
#include <vector>

class Graph {
private:
    char representation_type; // 'm' for adjacency matrix, 'v' for adjacency vector
    std::vector<std::vector<int>> adjacency_matrix;
    std::vector<std::vector<int>> adjacency_vector;

    // Estrutura para representar a árvore de busca
    struct TreeNode {
        int parent; // Pai do vértice
        int level;  // Nível do vértice na árvore
    };

    std::vector<TreeNode> dfs_tree; // Vetor para armazenar informações sobre a árvore de busca

public:
    int num_vertices;
    int num_edges;
    int min_degree;
    int max_degree;
    double avg_degree;
    int median_degree;

    Graph(std::string filepath, char rp);

    void SetGraph(std::string filepath);
    void CalculateGraphStats();
    void PrintRepresentation();

    // Adicione os métodos para busca em profundidade
    void DepthFirstSearch(int start_vertex);
    void DFS(int v, int parent, int level, std::vector<bool>& visited);

    // Método para imprimir a árvore de busca em profundidade
    void PrintDFSResult();
};
