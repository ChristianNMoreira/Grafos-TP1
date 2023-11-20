#include <string>
#include <vector>
#include <utility>
#include <tuple>

struct markedNode {
    char marked = '0';
    int level = 0;
    int father = 0;
};

class Graph {
    private:
        char representation_type; // "m" -> matriz de adjacência; "v" -> vetor de adjacência
        bool weighted;
        bool directed;
        char** matrix_pointer;
        std::vector<std::vector<int>> vector_pointer;
        std::vector<std::vector<std::tuple<int,float,float,bool>>> w_vector_pointer;
        std::vector<markedNode> tree;
        char* markedArray;
    public:
        int num_vertices;
        int num_edges;
        int min_degree;
        int max_degree;
        int avg_degree;
        int median_degree;

        Graph(std::string filepath, char rp='v', bool w=0, bool drc=0);

        void SetGraph(std::string filepath);
        void PrintRepresentation();
        void Diametro();
        void CalculateGraphStats();

        int BFS(int initial, bool export_file=1, int final=0, bool set_tree=0, bool update_array=0);
        void DFS(int initial);

        int Distance(int initial, int final);
        void ConnectedComponents();

        void Dijkstra(int start_node, int target_node, bool use_heap = false);

        void FordFulkerson(int source, int target);

        void freeAll();
};
