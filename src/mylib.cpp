#include "mylib.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

Graph::Graph(std::string filepath, char rp) {
    representation_type = rp;
    SetGraph(filepath);
    CalculateGraphStats();
}

void Graph::SetGraph(std::string filepath) {
    std::ifstream graph_file(filepath);

    if (!graph_file.is_open()) {
        std::cerr << "Error: Unable to open the file: " << filepath << std::endl;
        return;
    }

    std::string line;

    // Read the number of vertices from the first line
    if (getline(graph_file, line)) {
        num_vertices = std::stoi(line);
    } else {
        std::cerr << "Error: Unable to read the number of vertices." << std::endl;
        graph_file.close();
        return;
    }

    adjacency_matrix.resize(num_vertices, std::vector<int>(num_vertices, 0));
    adjacency_vector.resize(num_vertices);

    while (getline(graph_file, line)) {
        int i, j;
        std::istringstream ss(line);

        if (!(ss >> i >> j)) {
            std::cerr << "Error: Unable to parse line: " << line << std::endl;
            continue;
        }

        if (i >= 1 && i <= num_vertices && j >= 1 && j <= num_vertices) {
            adjacency_matrix[i - 1][j - 1] = 1;
            adjacency_matrix[j - 1][i - 1] = 1;

            adjacency_vector[i - 1].push_back(j);
            adjacency_vector[j - 1].push_back(i);
        } else {
            std::cerr << "Error: Vertex indices out of bounds in line: " << line << std::endl;
        }
    }

    graph_file.close();
}

void Graph::CalculateGraphStats() {
    num_edges = 0;
    min_degree = num_vertices; 
    max_degree = 0;
    avg_degree = 0.0;
    std::vector<int> degrees;

    for (int i = 0; i < num_vertices; i++) {
        int degree = adjacency_vector[i].size();
        num_edges += degree;
        min_degree = std::min(min_degree, degree);
        max_degree = std::max(max_degree, degree);
        degrees.push_back(degree);
    }

    // Corrigindo a contagem de arestas dividindo por 2
    num_edges /= 2;

    if (!degrees.empty()) {
        std::sort(degrees.begin(), degrees.end());
        int middle = num_vertices / 2;
        if (num_vertices % 2 == 0) {
            median_degree = (degrees[middle - 1] + degrees[middle]) / 2;
        } else {
            median_degree = degrees[middle];
        }
        avg_degree = static_cast<double>(num_edges) / num_vertices;
    }
}

void Graph::PrintRepresentation() {
    std::ofstream output_file("graph_output.txt");

    if (!output_file.is_open()) {
        std::cerr << "Error: Unable to create output file." << std::endl;
        return;
    }

    output_file << "Number of vertices: " << num_vertices << std::endl;
    output_file << "Number of edges: " << num_edges << std::endl;
    output_file << "Minimum degree: " << min_degree << std::endl;
    output_file << "Maximum degree: " << max_degree << std::endl;
    output_file << "Average degree: " << std::fixed << std::setprecision(2) << avg_degree << std::endl;
    output_file << "Median degree: " << median_degree << std::endl;

    if (representation_type == 'm') {
        output_file << "Adjacency Matrix:" << std::endl;
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                output_file << adjacency_matrix[i][j] << ' ';
            }
            output_file << std::endl;
        }
    } else if (representation_type == 'v') {
        output_file << "Adjacency Vector:" << std::endl;
        for (int i = 0; i < num_vertices; i++) {
            output_file << i + 1 << " -> ";
            for (int j = 0; j < adjacency_vector[i].size(); j++) {
                output_file << adjacency_vector[i][j] << ' ';
            }
            output_file << std::endl;
        }
    } else {
        output_file << "Error: Unknown representation type." << std::endl;
    }

    output_file.close();
}

void Graph::DepthFirstSearch(int start_vertex) {
    dfs_tree.resize(num_vertices);
    std::vector<bool> visited(num_vertices, false); // Vector para controlar os vértices visitados
    DFS(start_vertex, -1, 0, visited); // Chame a função de busca em profundidade
}

void Graph::DFS(int v, int parent, int level, std::vector<bool>& visited) {
    // Marque o vértice como visitado e registre o pai e o nível
    visited[v] = true;
    dfs_tree[v].parent = parent;
    dfs_tree[v].level = level;

    // Recorra pelos vizinhos do vértice
    for (int neighbor : adjacency_vector[v]) {
        if (!visited[neighbor]) { // Verifique se o vizinho não foi visitado ainda
            DFS(neighbor, v, level + 1, visited);
        }
    }
}




void Graph::PrintDFSResult() {
    std::ofstream output_file("dfs_tree_output.txt");
    if (!output_file.is_open()) {
        std::cerr << "Error: Unable to create output file." << std::endl;
        return;
    }

    for (int i = 0; i < num_vertices; i++) {
        output_file << "Vertex " << i + 1 << ": Parent = " << dfs_tree[i].parent << ", Level = " << dfs_tree[i].level << std::endl;
    }

    output_file.close();
}