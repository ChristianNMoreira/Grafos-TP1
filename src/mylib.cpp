#include "mylib.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>

struct markedNode {
    char marked = '0';
    int level = 0;
    int father = 0;
};


Graph::Graph(std::string filepath, char rp) {
    representation_type = rp;
    SetGraph(filepath);
};

void Graph::SetGraph(std::string filepath) {
    std::fstream graph_file;
    std::string line;
    graph_file.open(filepath, std::ios::in);
    if (graph_file.is_open()) {
        if (getline(graph_file, line)) {
            num_vertices = stoi(line);
        }
    }
    if (representation_type == 'm') {  // matriz de adjacência
        static char** M = (char**)malloc(num_vertices * sizeof(char*));
        matrix_pointer = M;
        for (int k = 0; k < num_vertices; k++) {
            M[k] = (char*)malloc(num_vertices * sizeof(char));
            std::memset(M[k], '0', num_vertices * sizeof(char));
        }

        if (graph_file.is_open()) {
            std::string s;
            int i = 0;
            int j = 0;
            while(getline(graph_file, line)) {
                std::stringstream ss(line);
                while (getline(ss, s, ' ')) {
                    (i == 0) ? i = stoi(s) : j = stoi(s);
                }
                M[i-1][j-1] = '1';
                M[j-1][i-1] = '1';
                i = 0;
            }
        }
    }
    else {  // vetor de adjacência
        static std::vector<int> *V = new std::vector<int>[num_vertices];
        vector_pointer = V;

        if (graph_file.is_open()) {
            std::string line;
            std::string s;
            int i = 0;
            int j = 0;
            while(getline(graph_file, line)) {
                std::stringstream ss(line);
                while (getline(ss, s, ' ')) {
                    (i == 0) ? i = stoi(s) : j = stoi(s);
                }
                V[i-1].push_back(j);
                V[j-1].push_back(i);
                i = 0;
            }
        }
    }
    graph_file.close();
}

void Graph::PrintRepresentation() {
    if (representation_type == 'm') {
        char** M = matrix_pointer;
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                std::cout<<M[i][j]<<'\t';
            }
            std::cout<<'\n';
        }
    }
    else {
        std::vector<int> *V = vector_pointer;
        for (int i = 0; i < num_vertices; i++) {
            std::cout<<i+1<<" -> ";
            for (int j = 0; j < V[i].size(); j++) {
                std::cout<<V[i][j]<<' ';
            }
            std::cout<<'\n'<<'\n';
        }
    }
    
}

void Graph::CalculateGraphStats() {
    
    num_edges = 0;
    min_degree = num_vertices; 
    max_degree = 0;
    avg_degree = 0.0;
    std::vector<int> degrees;

    if (representation_type== 'm') {
        char** M = matrix_pointer;
    }
    else {
        std::vector<int> *V = vector_pointer;
        for (int i = 0; i < num_vertices; i++) {
            int degree = V[i].size();
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
    std::ofstream output_file("graph_output.txt");
    
    output_file << "Vértices: " << num_vertices << std::endl;
    output_file << "Arestas: " << num_edges << std::endl;
    output_file << "Grau mínimo: " << min_degree << std::endl;
    output_file << "Grau máximo: " << max_degree << std::endl;
    //output_file << "Grau médio: " << std::fixed << std::setprecision(2) << avg_degree << std::endl;
    output_file << "Mediana de Grau: " << median_degree << std::endl;
    output_file.close();

}

void Graph::BFS(int initial) {
    std::ofstream output_file("bfs.txt");

    
    std::queue<int> Q;
    int v;
    struct markedNode mN[num_vertices];
    mN[initial - 1].marked = '1';
    std::cout<<initial<<'\t';
    output_file <<initial<<'\t';
    mN[initial - 1].level = 0;
    std::cout<<0<<'\t';
    output_file <<0<<'\t';
    std::cout<<0<<'\n';
    output_file <<0<<'\n';
    Q.push(initial);
    while (Q.size() > 0) {
        v = Q.front(); // v -> primeiro nó na fila Q
        Q.pop();
        if (representation_type == 'm') {
            char** M = matrix_pointer;
            for (int j = 0; j < num_vertices; j++) {
                if (M[v - 1][j] == '1') {
                    // Nó w=(j+1) é vizinho de v
                    if (mN[j].marked != '1') {
                        Q.push(j + 1);
                        std::cout<<(j+1)<<'\t';
                        output_file <<(j+1)<<'\t';
                        mN[j].marked = '1';
                        mN[j].father = v;
                        std::cout<<v<<'\t';
                        output_file <<v<<'\t';
                        mN[j].level = mN[v-1].level + 1;
                        std::cout<<mN[j].level<<'\n';
                        output_file <<mN[j].level<<'\n';
                    }

                }
            }

        }
        else {
            std::vector<int> *V = vector_pointer;
            for (int j = 0; j < V[v-1].size(); j++) {
                int w = V[v-1][j];
                // Nó w é vizinho de v
                if (mN[w-1].marked != '1') {
                    Q.push(w);
                    std::cout<<(w)<<'\t';
                    output_file <<(w)<<'\t';
                    mN[w-1].marked = '1';
                    mN[w-1].father = v;
                    std::cout<<v<<'\t';
                    output_file <<v<<'\t';
                    mN[w-1].level = mN[v-1].level + 1;
                    std::cout<<mN[w-1].level<<'\n';
                    output_file <<mN[w-1].level<<'\n';
                }
            }
        }
    }
    output_file.close();
}

void Graph::DFS(int initial) {
    std::ofstream output_file("dfs.txt");
    std::vector<int> P;
    int v;
    struct markedNode mN[num_vertices];
    P.push_back(initial);
    while (P.size() > 0) {
        v = P.back(); // v -> último nó no vetor P, funcionando como pilha
        P.pop_back();
        if (mN[v-1].marked != '1') {
            mN[v-1].marked = '1';
            std::cout << v << '\t';
            output_file << v << '\t';
            std::cout << mN[v-1].father << '\t';
            output_file << mN[v-1].father << '\t';
            std::cout << mN[v-1].level << '\n';
            output_file << mN[v-1].level << '\n';
            if (representation_type == 'm') {
                char** M = matrix_pointer;
                for (int j = 0; j < num_vertices; j++) {
                    if (M[v - 1][j] == '1') {
                        // Nó w=(j+1) é vizinho de v
                        P.push_back(j+1);
                        mN[j].father = v;
                        mN[j].level = mN[v-1].level + 1;
                    }
                }
            }
            else {
                std::vector<int> *V = vector_pointer;
                for (int j = 0; j < V[v-1].size(); j++) {
                    int w = V[v-1][j];
                    // Nó w é vizinho de v
                    P.push_back(w);
                    mN[w-1].father = v;
                    mN[w-1].level = mN[v-1].level + 1;
                }
            }
        }
    }
}