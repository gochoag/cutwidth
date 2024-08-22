#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <fstream>
#include <chrono>

// Método para generar una solución aleatoria
std::vector<int> generateRandomSolution(int nodes) {
    std::vector<int> solution(nodes);
    // Llenar el vector con los valores {1, 2, ..., nodes}
    for (int i = 0; i < nodes; ++i) {
        solution[i] = i + 1;
    }
    // Barajar el vector para obtener una solución aleatoria
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(solution.begin(), solution.end(), g);
    return solution;
}

// Función para calcular el valor de corte (cut value)
int cutValue(const std::vector<std::pair<int, int>>& edge, const std::vector<int>& f, int nodes) {
    int maxCut = 0; // Inicializar con 0 porque queremos encontrar el valor máximo de cortes
    std::vector<int> cutwidth(nodes - 1, 0);

    // Revisar todas las aristas y determinar si cruzan el corte
    for (const auto& e : edge) {
        int fa = std::min(f[e.first], f[e.second]);
        int fb = std::max(f[e.first], f[e.second]);

        // fa y fb ahora representan las posiciones en la permutación
        for (int i = 0; i < nodes - 1; i++) {
            // Si la arista cruza el intervalo entre i e i+1, incrementa el corte
            if (fa <= i + 1 && fb > i + 1) {
                ++cutwidth[i];
            }
        }
    }

    // Encontrar el máximo ancho de corte
    for (int i = 0; i < cutwidth.size(); i++) {
        maxCut = std::max(maxCut, cutwidth[i]);
    }

    return maxCut;
}


// Función para leer el grafo desde un archivo
void readInput(std::vector<std::pair<int, int>>& edge, std::vector<int>& f, int& nodes, int& edges) {
    std::ifstream infile("instancia.txt");

    if (!infile) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return;
    }

    // Leer la primera línea correctamente
    infile >> nodes >> nodes >> edges;



    // Inicializando vector de posiciones {1, 2, 3, ..., n}
    f.resize(nodes);
    for (int i = 0; i < nodes; i++) {
        f[i] = i + 1;
    }

    // Leer las aristas
    edge.resize(edges);
    for (int i = 0; i < edges; i++) {
        int u, v;
        infile >> u >> v;

        // Verificación de nodos dentro del rango permitido
        if (u < 1 || u > nodes || v < 1 || v > nodes) {
            std::cerr << "Error: Nodo fuera de rango. u: " << u << ", v: " << v << std::endl;
            return;
        }

        // Convertir a índice 0-based para la lógica interna
        edge[i] = { u - 1, v - 1 };


    }
}

// Función que evalúa 1000 soluciones aleatorias y reporta la mejor
void findBestRandomSolution(int nodes, const std::vector<std::pair<int, int>>& edge) {
    int bestCutValue = std::numeric_limits<int>::max(); // Inicialmente, el peor valor posible
    std::vector<int> bestSolution;

    for (int i = 0; i < 1000; ++i) {
        std::vector<int> currentSolution = generateRandomSolution(nodes);
        int currentCutValue = cutValue(edge, currentSolution, nodes);

        // Si encontramos una mejor solución, la guardamos
        if (currentCutValue < bestCutValue) {
            bestCutValue = currentCutValue;
            bestSolution = currentSolution;
        }
    }

    // Reportar la mejor solución encontrada
    std::cout << "Mejor solucion encontrada: ";
    for (int i = 0; i < bestSolution.size(); ++i) {
        std::cout << bestSolution[i] << " ";
    }
    std::cout << "\nValor de la funcion objetivo (cut value): " << bestCutValue << std::endl;
}

int main() {
    srand(time(NULL));

    int nodes, edges;
    std::vector<std::pair<int, int>> edge;
    std::vector<int> f;

    readInput(edge, f, nodes, edges);

    // Generar y reportar la mejor solución aleatoria
    findBestRandomSolution(nodes, edge);

    return 0;
}
