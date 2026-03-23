#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;
// ESTRUCTURA PARA MÉTRICAS
class PerformanceMetrics {
public:
    double executionTime;   // ms
    long long memoryUsage;  // bytes
    string algorithmName;

    PerformanceMetrics(const string& name)
        : executionTime(0), memoryUsage(0), algorithmName(name) {}

    void displayMetrics() const {
        cout << "\n" << string(55, '=') << endl;
        cout << "ALGORITMO: " << algorithmName << endl;
        cout << string(55, '=') << endl;
        cout << fixed << setprecision(3);
        cout << "Tiempo de ejecución: " << executionTime << " ms" << endl;
        cout << "Memoria aproximada: " << (memoryUsage / 1024.0 / 1024.0) << " MB" << endl;
        cout << string(55, '=') << endl;
    }
};


// FUNCIONES AUXILIARES DE MEMORIA

long long estimateStringVectorMemory(const vector<string>& data) {
    long long total = sizeof(vector<string>);
    total += static_cast<long long>(data.capacity()) * sizeof(string);

    for (const auto& s : data) {
        total += static_cast<long long>(s.capacity()) * sizeof(char);
    }
    return total;
}


// 1. QUICKSORT CON VECTOR

class QuickSortSolver {
private:
    vector<string> data;
    long long comparisons = 0;

    int partition(int low, int high) {
        // Pivot medio: para evitar peor caso en datos casi ordenados
        int mid = low + (high - low) / 2;
        swap(data[mid], data[high]);

        const string pivot = data[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            comparisons++;
            if (data[j] <= pivot) {
                i++;
                swap(data[i], data[j]);
            }
        }

        swap(data[i + 1], data[high]);
        return i + 1;
    }

    void quickSort(int low, int high) {
        if (low < high) {
            int pi = partition(low, high);
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
        }
    }

public:
    PerformanceMetrics sort(vector<string>& dataset) {
        PerformanceMetrics metrics("QuickSort (Vector)");
        data = dataset;
        comparisons = 0;

        auto start = chrono::high_resolution_clock::now();
        if (!data.empty()) {
            quickSort(0, static_cast<int>(data.size()) - 1);
        }
        auto end = chrono::high_resolution_clock::now();

        metrics.executionTime = chrono::duration<double, milli>(end - start).count();
        metrics.memoryUsage = estimateStringVectorMemory(data);

        dataset = data;
        return metrics;
    }
};

// 2. HEAPSORT CON BINARY HEAP

class HeapSortSolver {
private:
    vector<string> heap;
    long long comparisons = 0;

    void siftDown(int index, int size) {
        while (true) {
            int largest = index;
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;

            if (leftChild < size) {
                comparisons++;
                if (heap[leftChild] > heap[largest]) {
                    largest = leftChild;
                }
            }

            if (rightChild < size) {
                comparisons++;
                if (heap[rightChild] > heap[largest]) {
                    largest = rightChild;
                }
            }

            if (largest == index) {
                break;
            }

            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

    void buildMaxHeap() {
        for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; i--) {
            siftDown(i, static_cast<int>(heap.size()));
        }
    }

public:
    PerformanceMetrics sort(vector<string>& dataset) {
        PerformanceMetrics metrics("HeapSort (Binary Heap)");
        heap = dataset;
        comparisons = 0;

        auto start = chrono::high_resolution_clock::now();

        buildMaxHeap();

        for (int i = static_cast<int>(heap.size()) - 1; i > 0; i--) {
            swap(heap[0], heap[i]);
            siftDown(0, i);
        }

        auto end = chrono::high_resolution_clock::now();

        metrics.executionTime = chrono::duration<double, milli>(end - start).count();
        metrics.memoryUsage = estimateStringVectorMemory(heap);

        dataset = heap;
        return metrics;
    }
};

// 3. AVL TREE

struct AVLNode {
    string value;
    int count;     // para manejar duplicados
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const string& v)
        : value(v), count(1), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    AVLNode* root;
    long long comparisons = 0;
    long long nodeCount = 0;

    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(AVLNode* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    AVLNode* insertNode(AVLNode* node, const string& value) {
        if (!node) {
            nodeCount++;
            return new AVLNode(value);
        }

        comparisons++;

        if (value < node->value) {
            node->left = insertNode(node->left, value);
        } else if (value > node->value) {
            node->right = insertNode(node->right, value);
        } else {
            // duplicado
            node->count++;
            return node;
        }

        updateHeight(node);
        int balance = getBalance(node);

        // Left-Left
        if (balance > 1 && value < node->left->value) {
            return rotateRight(node);
        }

        // Right-Right
        if (balance < -1 && value > node->right->value) {
            return rotateLeft(node);
        }

        // Left-Right
        if (balance > 1 && value > node->left->value) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right-Left
        if (balance < -1 && value < node->right->value) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void inorder(AVLNode* node, vector<string>& result) {
        if (!node) return;

        inorder(node->left, result);

        for (int i = 0; i < node->count; i++) {
            result.push_back(node->value);
        }

        inorder(node->right, result);
    }

    void deleteTree(AVLNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr), comparisons(0), nodeCount(0) {}

    ~AVLTree() {
        deleteTree(root);
    }

    PerformanceMetrics sort(vector<string>& dataset) {
        PerformanceMetrics metrics("AVL Tree");

        deleteTree(root);
        root = nullptr;
        comparisons = 0;
        nodeCount = 0;

        auto start = chrono::high_resolution_clock::now();

        for (const string& word : dataset) {
            root = insertNode(root, word);
        }

        vector<string> result;
        result.reserve(dataset.size());
        inorder(root, result);

        auto end = chrono::high_resolution_clock::now();

        metrics.executionTime = chrono::duration<double, milli>(end - start).count();

        // estimación: nodos + strings del resultado
        metrics.memoryUsage =
            nodeCount * static_cast<long long>(sizeof(AVLNode))
            + estimateStringVectorMemory(result);

        dataset = result;
        return metrics;
    }
};

// FUNCIONES AUXILIARES

vector<string> loadDataset(const string& filename, int limit = 100000) {
    vector<string> dataset;
    dataset.reserve(limit);

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo: " << filename << endl;
        return dataset;
    }

    string word;
    while (getline(file, word) && static_cast<int>(dataset.size()) < limit) {
        size_t start = word.find_first_not_of(" \t\r\n");
        if (start == string::npos) continue;

        size_t end = word.find_last_not_of(" \t\r\n");
        word = word.substr(start, end - start + 1);

        if (!word.empty()) {
            dataset.push_back(word);
        }
    }

    file.close();
    return dataset;
}

bool isSorted(const vector<string>& data) {
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i] < data[i - 1]) {
            return false;
        }
    }
    return true;
}

void saveResults(const vector<string>& data, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "No se pudo guardar: " << filename << endl;
        return;
    }

    for (const string& word : data) {
        file << word << '\n';
    }
    file.close();
}

void printEqualityCheck(const vector<string>& a, const vector<string>& b, const string& nameA, const string& nameB) {
    cout << nameA << " vs " << nameB << ": "
         << ((a == b) ? "IGUALES" : "DIFERENTES") << endl;
}

void saveMetricsCSV(const vector<PerformanceMetrics>& metrics, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "No se pudo guardar " << filename << endl;
        return;
    }

    file << "Algoritmo,Tiempo_ms,Memoria_MB\n";
    for (const auto& m : metrics) {
        file << m.algorithmName << ","
             << m.executionTime << ","
             << (m.memoryUsage / 1024.0 / 1024.0) << "\n";
    }

    file.close();
}

// MAIN

int main() {
    cout << "ESTOY EJECUTANDO SORTING_ALGORITHMS.CPP" << endl;
    cout << "\n" << string(65, '=') << endl;
    cout << "ANALISIS DE ALGORITMOS DE ORDENAMIENTO" << endl;
    cout << "QuickSort vs HeapSort vs AVL Tree" << endl;
    cout << string(65, '=') << "\n" << endl;

    cout << "Cargando dataset..." << endl;
    vector<string> dataset = loadDataset("dataset.txt", 100000);

    if (dataset.empty()) {
        cerr << "Error: dataset vacio o no encontrado." << endl;
        cerr << "Asegurate de que dataset.txt este en la misma carpeta del programa." << endl;
        return 1;
    }

    cout << "Dataset cargado: " << dataset.size() << " palabras\n" << endl;

    vector<PerformanceMetrics> allMetrics;

    // QuickSort
    cout << "Ejecutando QuickSort..." << endl;
    vector<string> dataQS = dataset;
    QuickSortSolver qs;
    PerformanceMetrics metricsQS = qs.sort(dataQS);
    metricsQS.displayMetrics();
    allMetrics.push_back(metricsQS);
    cout << (isSorted(dataQS) ? "✓ QuickSort verificado\n" : "✗ QuickSort incorrecto\n") << endl;

    // HeapSort
    cout << "Ejecutando HeapSort..." << endl;
    vector<string> dataHS = dataset;
    HeapSortSolver hs;
    PerformanceMetrics metricsHS = hs.sort(dataHS);
    metricsHS.displayMetrics();
    allMetrics.push_back(metricsHS);
    cout << (isSorted(dataHS) ? "✓ HeapSort verificado\n" : "✗ HeapSort incorrecto\n") << endl;

    // AVL
    cout << "Ejecutando AVL Tree..." << endl;
    vector<string> dataAVL = dataset;
    AVLTree avl;
    PerformanceMetrics metricsAVL = avl.sort(dataAVL);
    metricsAVL.displayMetrics();
    allMetrics.push_back(metricsAVL);
    cout << (isSorted(dataAVL) ? "✓ AVL Tree verificado\n" : "✗ AVL Tree incorrecto\n") << endl;

    // Guardar salidas
    saveResults(dataQS, "sorted_quick.txt");
    saveResults(dataHS, "sorted_heap.txt");
    saveResults(dataAVL, "sorted_avl.txt");
    saveMetricsCSV(allMetrics, "performance_metrics.csv");
    

    cout << "Archivos guardados: sorted_quick.txt, sorted_heap.txt, sorted_avl.txt\n" << endl;

    // Comparar resultados
    cout << string(65, '=') << endl;
    cout << "COMPARACION DE RESULTADOS" << endl;
    cout << string(65, '=') << endl;
    printEqualityCheck(dataQS, dataHS, "QuickSort", "HeapSort");
    printEqualityCheck(dataQS, dataAVL, "QuickSort", "AVL");
    printEqualityCheck(dataHS, dataAVL, "HeapSort", "AVL");
    cout << endl;

    // Tabla comparativa
    cout << string(65, '=') << endl;
    cout << "COMPARATIVA DE RENDIMIENTO" << endl;
    cout << string(65, '=') << endl;

    cout << left << setw(25) << "Algoritmo"
         << setw(18) << "Tiempo (ms)"
         << setw(20) << "Memoria (MB)" << endl;
    cout << string(63, '-') << endl;

    for (const auto& m : allMetrics) {
        cout << left << setw(25) << m.algorithmName
             << setw(18) << fixed << setprecision(3) << m.executionTime
             << setw(20) << fixed << setprecision(3) << (m.memoryUsage / 1024.0 / 1024.0)
             << endl;
    }

    cout << "\n" << string(65, '=') << endl;
    cout << "OBSERVACION" << endl;
    cout << string(65, '=') << endl;
    cout << "La memoria reportada es una estimacion aproximada basada en sizeof,\n";
    cout << "capacidad de strings/vector y estructura de nodos, no memoria exacta del SO.\n";

    return 0;
}
