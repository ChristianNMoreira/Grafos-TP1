#pragma once

#include <string>
#include <vector>

class Graph {
private:
    char representation_type; // 'm' for adjacency matrix, 'v' for adjacency vector
    std::vector<std::vector<int>> adjacency_matrix;
    std::vector<std::vector<int>> adjacency_vector;

public:
    int num_vertices;
    int num_edges;
    int min_degree;
    int max_degree;
    double avg_degree;
    int median_degree;
    

    Graph(std::string filepath, char rp);

    void SetGraph(std::string filepath);
    void PrintRepresentation();
    void CalculateGraphStats();
};
