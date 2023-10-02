#include <string>
#include <vector>

class Graph {
    private:
        char representation_type; // "m" -> matriz de adjacência; "v" -> vetor de adjacência
        char** matrix_pointer;
        std::vector<int>* vector_pointer;
    public:
        int num_vertices;
        int num_edges;
        int min_degree;
        int max_degree;
        int avg_degree;
        int median_degree;

        Graph(std::string filepath, char rp);

        void SetGraph(std::string filepath);
        void PrintRepresentation();

        void CalculateGraphStats();

        void BFS(int initial);
        void DFS(int initial);
};