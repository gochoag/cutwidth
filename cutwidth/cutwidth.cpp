#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <fstream>

// Clase para manejar el grafo y la solución
class Grafo {
public:
    int nodos;
    int aristas;
    std::vector<std::pair<int, int>> listaAristas;
    std::vector<int> f;  // Para manejar las posiciones de los nodos
    // f -> posiciones en el diagrama de clase

    // Método para leer el grafo desde un archivo
    void leerEntrada(const std::string& nombreArchivo) {
        std::ifstream infile(nombreArchivo);

        if (!infile) {
            std::cerr << "No se pudo abrir el archivo." << std::endl;
            return;
        }

        // Leer la primera línea correctamente
        infile >> nodos >> nodos >> aristas;

        // Inicializando vector de posiciones {1, 2, 3, ..., n}
        f.resize(nodos);
        for (int i = 0; i < nodos; i++) {
            f[i] = i + 1;
        }

        // Leer las aristas
        listaAristas.resize(aristas);
        for (int i = 0; i < aristas; i++) {
            int u, v;
            infile >> u >> v;

            // Verificación de nodos dentro del rango permitido
            if (u < 1 || u > nodos || v < 1 || v > nodos) {
                std::cerr << "Error: Nodo fuera de rango. u: " << u << ", v: " << v << std::endl;
                return;
            }

            // Convertir a índice 0-based para la lógica interna
            listaAristas[i] = { u - 1, v - 1 };
        }
    }
};

// Clase para manejar la solución y encontrar la mejor solución aleatoria
class Solucion {
public:
    std::vector<int> generarSolucionAleatoria(int nodos) {
        std::vector<int> solucion(nodos);
        // Llenar el vector con los valores {1, 2, ..., nodos}
        for (int i = 0; i < nodos; ++i) {
            solucion[i] = i + 1;
        }
        // Barajar el vector para obtener una solución aleatoria
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(solucion.begin(), solucion.end(), g);
        return solucion;
    }

    int calcularValorCorte(const std::vector<std::pair<int, int>>& aristas, const std::vector<int>& f, int nodos) {
        int maxCorte = 0;  // Inicializar con 0 porque queremos encontrar el valor máximo de cortes
        std::vector<int> anchuraCorte(nodos - 1, 0);

        // Revisar todas las aristas y determinar si cruzan el corte
        for (const auto& a : aristas) {
            int fa = std::min(f[a.first], f[a.second]);
            int fb = std::max(f[a.first], f[a.second]);

            // fa y fb ahora representan las posiciones en la permutación
            for (int i = 0; i < nodos - 1; i++) {
                // Si la arista cruza el intervalo entre i e i+1, incrementa el corte
                if (fa <= i + 1 && fb > i + 1) {
                    ++anchuraCorte[i];
                }
            }
        }

        // Encontrar el máximo ancho de corte
        for (int i = 0; i < anchuraCorte.size(); i++) {
            maxCorte = std::max(maxCorte, anchuraCorte[i]);
        }

        return maxCorte;
    }

    void encontrarMejorSolucionAleatoria(int nodos, const std::vector<std::pair<int, int>>& aristas) {
        int mejorValorCorte = std::numeric_limits<int>::max();  // Inicialmente, el peor valor posible
        std::vector<int> mejorSolucion;

        for (int i = 0; i < 1000; ++i) {
            std::vector<int> solucionActual = generarSolucionAleatoria(nodos);
            int valorCorteActual = calcularValorCorte(aristas, solucionActual, nodos);

            // Si encontramos una mejor solución, la guardamos
            if (valorCorteActual < mejorValorCorte) {
                mejorValorCorte = valorCorteActual;
                mejorSolucion = solucionActual;
            }
        }

        // Reportar la mejor solución encontrada
        std::cout << "Mejor solucion encontrada: ";
        for (int i = 0; i < mejorSolucion.size(); ++i) {
            std::cout << mejorSolucion[i] << " ";
        }
        std::cout << "\nValor de la funcion objetivo (valor de corte): " << mejorValorCorte << std::endl;
    }
};

int main() {
    srand(time(NULL));

    Grafo grafo;
    Solucion solucion;

    grafo.leerEntrada("instancia.txt");

    // Generar y reportar la mejor solución aleatoria
    solucion.encontrarMejorSolucionAleatoria(grafo.nodos, grafo.listaAristas);

    return 0;
}
