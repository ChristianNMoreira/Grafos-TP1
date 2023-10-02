#include <string>
#include <vector>

struct markedNode {
    char marked = '0';
    int level = 0;
    int father = 0;
};

class Graph {
    private:
        char representation_type; // "m" -> matriz de adjacência; "v" -> vetor de adjacência
        char** matrix_pointer;
        std::vector<int>* vector_pointer;
        markedNode *tree;
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

        int BFS(int initial, bool export_file=1, int final=0, bool set_tree=0);
        void DFS(int initial);
};
