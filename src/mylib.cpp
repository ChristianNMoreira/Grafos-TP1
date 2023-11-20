#include "mylib.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <limits>
#include <tuple>


Graph::Graph(std::string filepath, char rp, bool w, bool drc) {
    if (w == 1) {
        representation_type = 'v';
        weighted = true;
    }
    else {
        representation_type = rp;
        weighted = false;
    }
    directed = drc;
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
        if (weighted == 1) {
            std::vector<std::vector<std::tuple<int,float,float,bool>>> V;
            V.resize(num_vertices);
            if (graph_file.is_open()) {
                std::string line;
                std::string s;
                int i = 0;
                int j = 0;
                float k = 0.0;
                while(getline(graph_file, line)) {
                    std::stringstream ss(line);
                    while (getline(ss, s, ' ')) {
                        (i == 0) ? i = stoi(s) : (j == 0) ? j = stoi(s) : k = stof(s);
                    }
                    V[i-1].push_back(std::make_tuple(j, k, 0.0, 1));
                    if (directed == 0) {
                        V[j-1].push_back(std::make_tuple(i, k, 0.0, 1));
                    }
                    i = 0;
                    j = 0;
                }
                w_vector_pointer = V;
            }
        }
        else {
            std::vector<std::vector<int>> V;
            V.resize(num_vertices);
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
            vector_pointer = V;
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
        if (weighted == 1) {
            // std::vector<std::pair<int, float>> V[num_vertices] = w_vector_pointer;
            for (int i = 0; i < num_vertices; i++) {
                std::cout<<i+1<<" -> ";
                for (int j = 0; j < w_vector_pointer[i].size(); j++) {
                    std::cout<<std::get<0>(w_vector_pointer[i][j])<<"(";
                    std::cout<<std::get<1>(w_vector_pointer[i][j])<<") ";
                }
                std::cout<<'\n'<<'\n';
            }
        }
        else {
            std::vector<std::vector<int>> V = vector_pointer;
            for (int i = 0; i < num_vertices; i++) {
                std::cout<<i+1<<" -> ";
                for (int j = 0; j < V[i].size(); j++) {
                    std::cout<<V[i][j]<<' ';
                }
                std::cout<<'\n'<<'\n';
            }
        }
    }
    
}

void Graph::CalculateGraphStats() {
    num_edges = 0;
    min_degree = num_vertices;
    max_degree = 0;
    avg_degree = 0.0;
    median_degree = 0;

    if (representation_type == 'm') {
        char** M = matrix_pointer;
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                if (M[i][j] == '1') {
                    num_edges++;
                }
            }
            int degree = 0;
            for (int j = 0; j < num_vertices; j++) {
                if (M[i][j] == '1') {
                    degree++;
                }
            }
            min_degree = std::min(min_degree, degree);
            max_degree = std::max(max_degree, degree);
        }
        num_edges /= 2;  // Dividir por 2, pois cada aresta foi contada duas vezes.
    } else {
        std::vector<std::vector<int>> V = vector_pointer;
        for (int i = 0; i < num_vertices; i++) {
            int degree = V[i].size();
            num_edges += degree;
            min_degree = std::min(min_degree, degree);
            max_degree = std::max(max_degree, degree);
        }
        num_edges /= 2;  // Dividir por 2, pois cada aresta foi contada duas vezes.
    }

    if (num_vertices > 0) {
        avg_degree = static_cast<double>(num_edges) / num_vertices;
    }

    // Agora, calcule a mediana (para ambos os casos)
    std::vector<int> degrees;
    if (representation_type == 'm') {
        char** M = matrix_pointer;
        for (int i = 0; i < num_vertices; i++) {
            int degree = 0;
            for (int j = 0; j < num_vertices; j++) {
                if (M[i][j] == '1') {
                    degree++;
                }
            }
            degrees.push_back(degree);
        }
    } else {
        std::vector<std::vector<int>> V = vector_pointer;
        for (int i = 0; i < num_vertices; i++) {
            int degree = V[i].size();
            degrees.push_back(degree);
        }
    }

    if (!degrees.empty()) {
        std::sort(degrees.begin(), degrees.end());
        int middle = num_vertices / 2;
        if (num_vertices % 2 == 0) {
            median_degree = (degrees[middle - 1] + degrees[middle]) / 2;
        } else {
            median_degree = degrees[middle];
        }
    }

    std::ofstream output_file("graph_output.txt");
    output_file << "Vértices: " << num_vertices << std::endl;
    output_file << "Arestas: " << num_edges << std::endl;
    output_file << "Grau mínimo: " << min_degree << std::endl;
    output_file << "Grau máximo: " << max_degree << std::endl;
    output_file << "Grau médio: " << std::fixed << std::setprecision(2) << static_cast<double>(2 * num_edges) / num_vertices << std::endl;

    output_file << "Mediana de Grau: " << median_degree << std::endl;
    output_file.close();
}

int Graph::BFS(int initial, bool export_file, int final, bool set_tree, bool update_array) {
    std::ofstream output_file("bfs.txt");
    std::queue<int> Q;
    int v;
    int max_level = 0;
    std::vector<markedNode> mN;
    mN.resize(num_vertices);
    mN[initial - 1].marked = '1';
    if (update_array == 1) markedArray[initial - 1] = '1';
    mN[initial - 1].level = 0;
    if (export_file == 1) {
        std::cout<<initial<<'\t';
        output_file <<initial<<'\t';
        std::cout<<0<<'\t';
        output_file <<0<<'\t';
        std::cout<<0<<'\n';
        output_file <<0<<'\n';
    }
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
                        mN[j].marked = '1';
                        if (update_array == 1) markedArray[j] = '1';
                        mN[j].father = v;
                        mN[j].level = mN[v-1].level + 1;
                        if (mN[j].level > max_level) max_level = mN[j].level;
                        if (export_file == 1) {
                            std::cout<<(j+1)<<'\t';
                            output_file <<(j+1)<<'\t';
                            std::cout<<v<<'\t';
                            output_file <<v<<'\t';
                            std::cout<<mN[j].level<<'\n';
                            output_file <<mN[j].level<<'\n';
                        }
                        if (final == (j+1)) {
                            output_file.close();
                            if (set_tree == 1) tree = mN;
                            return mN[j].level;
                        }
                    }
                }
            }
        }
        else {
            std::vector<std::vector<int>> V = vector_pointer;
            for (int j = 0; j < V[v-1].size(); j++) {
                int w = V[v-1][j];
                // Nó w é vizinho de v
                if (mN[w-1].marked != '1') {
                    Q.push(w);
                    mN[w-1].marked = '1';
                    if (update_array == 1) markedArray[w-1] = '1';
                    mN[w-1].father = v;
                    mN[w-1].level = mN[v-1].level + 1;
                    if (mN[w-1].level > max_level) max_level = mN[w-1].level;
                    if (export_file == 1) {
                        std::cout<<(w)<<'\t';
                        output_file <<(w)<<'\t';
                        std::cout<<v<<'\t';
                        output_file <<v<<'\t';
                        std::cout<<mN[w-1].level<<'\n';
                        output_file <<mN[w-1].level<<'\n';
                    }
                    if (final == w) {
                        output_file.close();
                        if (set_tree == 1) tree = mN;
                        return mN[w-1].level;
                    }
                }
            }
        }
    }
    output_file.close();
    if (set_tree == 1) tree = mN;
    return max_level;
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
                std::vector<std::vector<int>> V = vector_pointer;
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

int Graph::Distance(int initial, int final) {
    return BFS(initial, 0, final, 0, 0);
}

void Graph::ConnectedComponents() {
    std::ofstream output_file("connected_components.txt");
    char *marked = (char*)malloc(num_vertices * sizeof(char));
    std::memset(marked, '0', num_vertices * sizeof(char));
    markedArray = marked;

    int initial = 0;
    int iter = 0;
    int min_concomp = num_vertices;
    int max_concomp = 0;
    int num_concomp = 0;
    int n = 0;
    while (iter < num_vertices) {
        if (markedArray[iter] == '0') {
            initial = iter+1;
            BFS(initial, 0, 0, 1, 1);
            for (int i = 0; i < num_vertices; i++) {
                if (tree[i].marked == '1') {
                    output_file <<i+1<<',';
                    n += 1;
                }
            }
            num_concomp += 1;
            output_file <<'\n'<<'\n';
            if (min_concomp > n) min_concomp = n;
            if (max_concomp < n) max_concomp = n;
        }
        n=0;
        iter += 1;
    }

    std::cout << "min: " << min_concomp;
    std::cout << " max: " << max_concomp;
    std::cout << " num: " << num_concomp;
    free(marked);
    output_file.close();
}
 
void Graph::Diametro() {
    int maxDiameter = -1;
    int bfs_limit;

    if (num_vertices < 5000)
        bfs_limit = num_vertices;
    else
        bfs_limit = 5000;

    // Initialize random seed if needed
    if (num_vertices > 5000)
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 1; i <= bfs_limit; i++) {
        int start_vertex;

        if (num_vertices > 5000)
            start_vertex = std::rand() % num_vertices;
        else
            start_vertex = i;

        int max_level = BFS(start_vertex, false, 0, false, false);
        
        if (max_level > maxDiameter) {
            maxDiameter = max_level;
        }
    }

    std::cout << "Diameter: " << maxDiameter << std::endl;
}

int minDistance(std::vector<float> dist, std::vector<bool> S, int num_vertices){ 
    float min = std::numeric_limits<float>::infinity();
    int min_i;
    for (int i = 0; i < num_vertices; i++) {
        if (S[i] == false && dist[i] < min) {
            min = dist[i];
            min_i = i;
        }
    }
    return min_i;
}

void Graph::Dijkstra(int start_node, int target_node, bool use_heap) {
    std::ofstream output_file_dijkstra("dijkstra.txt");

    std::vector<float> dist(num_vertices, std::numeric_limits<float>::infinity());
    std::vector<int> pai(num_vertices, -1);
    std::vector<bool> S(num_vertices, false);
    dist[start_node - 1] = 0;
    int count = 0;

    // Custom comparator for the priority queue (min heap)
    auto compare = [&](int u, int v) { return dist[u] > dist[v]; };
    std::priority_queue<int, std::vector<int>, decltype(compare)> H(compare);

    H.push(start_node - 1);

    if (use_heap) {
        while (!H.empty()) {
            int u = H.top();
            H.pop();

            if (S[u]) continue;
            S[u] = true;

            for (int j = 0; j < w_vector_pointer[u].size(); j++) {
                int v = std::get<0>(w_vector_pointer[u][j]);
                float w = std::get<1>(w_vector_pointer[u][j]);

                // v é filho de (u+1)

                if (w < 0 || dist[u] < 0) {
                    throw "Peso negativo!";
                }

                if (dist[v - 1] > dist[u] + w) {
                    dist[v - 1] = dist[u] + w;
                    pai[v - 1] = u;

                    if (!S[v - 1]) {
                        H.push(v - 1);
                    }
                }
            }
        }
    }
    else {
        while (count < num_vertices) {
            int u = minDistance(dist, S, num_vertices);

            if (S[u]) continue;
            S[u] = true;
            count++;

            for (int j = 0; j < w_vector_pointer[u].size(); j++) {
                int v = std::get<0>(w_vector_pointer[u][j]);
                float w = std::get<1>(w_vector_pointer[u][j]);

                // v é filho de (u+1)

                if (w < 0 || dist[u] < 0) {
                    throw "Peso negativo!";
                }

                if (dist[v - 1] > dist[u] + w) {
                    dist[v - 1] = dist[u] + w;
                    pai[v - 1] = u;
                }
            }
        }
    }

    // Backtrack to construct the shortest path
    std::vector<int> shortest_path;
    int current_node = target_node - 1;

    while (current_node != (start_node - 1)) {
        shortest_path.push_back(current_node + 1);
        current_node = pai[current_node];
    }
    shortest_path.push_back(start_node);

    // Reverse the path
    std::reverse(shortest_path.begin(), shortest_path.end());

    std::ofstream output_file("shortest_path.txt");

    // Save the shortest path to the output file
    for (int i = 0; i < shortest_path.size(); i++) {
        output_file << shortest_path[i] << " ";
        std::cout << shortest_path[i] << " ";
    }
    
    for (int i = 0; i < num_vertices; i++) {
        output_file_dijkstra << (i + 1) << " " << dist[i] << " " << pai[i] << "\n";
    }

    output_file.close();
    output_file_dijkstra.close();
    output_file << std::endl;
}

void FordFulkerson(int source, int target) {
    std::ofstream output_file("ford_fulkerson.txt");
    
    output_file.close();
}

void Graph::freeAll() {
    std::cout<<"\n...freeAll...\n";
    if (representation_type == 'm') {
        for (int i = 0; i < num_vertices; i++) {
            free(matrix_pointer[i]);
        }
        free(matrix_pointer);
    }
    if (weighted == false) free(markedArray);
}