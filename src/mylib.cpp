#include "mylib.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


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
            M[k] = (char*)calloc(num_vertices, sizeof(char));
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
                M[i-1][j-1] = (char)1;
                i = 0;
            }
        }
    }
    else {  // vetor de adjacência
        static std::vector<int> *v = new std::vector<int>[num_vertices];
        vector_pointer = v;
        //static int** v = (int**)malloc(num_vertices * sizeof(std::vector<int>*));

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
                v[i-1].push_back(j);
                v[j-1].push_back(i);
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
        std::vector<int> *v = vector_pointer;
        for (int i = 0; i < num_vertices; i++) {
            std::cout<<i+1<<'\n';
            for (int j = 0; j < v[i].size(); j++) {
                std::cout<<v[i][j]<<'\t';
            }
            std::cout<<'\n'<<'\n';
        }
    }
}