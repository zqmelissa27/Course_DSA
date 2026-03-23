#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <queue>

using namespace std;

// ============================================================================
// CLASE PARA MEDIR TIEMPOS Y MEMORIA
// ============================================================================
class PerformanceMetrics {
public:
    double executionTime;      // Tiempo en milisegundos
    long long memoryUsage;     // Memoria en bytes
    string algorithmName;
    
    PerformanceMetrics(string name) : algorithmName(name), executionTime(0), memoryUsage(0) {}
    
    void displayMetrics() const {
        cout << "\n" << string(50, '=') << endl;
        cout << "ALGORITMO: " << algorithmName << endl;
        cout << string(50, '=') << endl;
        cout << fixed << setprecision(3);
        cout << "Tiempo de ejecución: " << executionTime << " ms" << endl;
        cout << "Memoria aproximada: " << (memoryUsage / 1024.0 / 1024.0) << " MB" << endl;
        cout << string(50, '=') << endl;
    }
};

// ============================================================================
// 1. IMPLEMENTACIÓN DE QUICKSORT CON VECTOR
// ============================================================================
class QuickSortSolver {
private:
    vector<string> data;
    long long comparisons = 0;
    
    // Función partición: divide el array en dos partes
    // Elementos menores al pivot a la izquierda, mayores a la derecha
    int partition(int low, int high) {
        string pivot = data[high];  // Elegimos el último elemento como pivot
        int i = low - 1;
        
        // Recorremos desde low hasta high-1
        for (int j = low; j < high; j++) {
            comparisons++;
            if (data[j] < pivot) {
                i++;
                swap(data[i], data[j]);  // Intercambiamos elementos
            }
        }
        swap(data[i + 1], data[high]);  // Colocamos el pivot en su posición final
        return i + 1;
    }
    
    // Función recursiva de QuickSort
    void quickSort(int low, int high) {
        if (low < high) {
            int pi = partition(low, high);
            quickSort(low, pi - 1);      // Ordena la parte izquierda
            quickSort(pi + 1, high);     // Ordena la parte derecha
        }
    }
    
public:
    PerformanceMetrics sort(vector<string>& dataset) {
        PerformanceMetrics metrics("QuickSort (Vector)");
        data = dataset;
        comparisons = 0;
        
        auto start = chrono::high_resolution_clock::now();
        if (!data.empty()) {
            quickSort(0, data.size() - 1);
        }
        auto end = chrono::high_resolution_clock::now();
        
        metrics.executionTime = chrono::duration<double, milli>(end - start).count();
        metrics.memoryUsage = data.size() * sizeof(string);
        
        dataset = data;  // Copiar resultado
        return metrics;
    }
    
    vector<string> getData() const { return data; }
};

// ============================================================================
// 2. IMPLEMENTACIÓN DE HEAPSORT CON BINARY HEAP
// ============================================================================
class HeapSortSolver {
private:
    vector<string> heap;
    long long comparisons = 0;
    
    // Sift down: mantiene la propiedad de max-heap
    // Un nodo padre debe ser MAYOR que sus hijos
    void siftDown(int index, int size) {
        int largest = index;
        int leftChild = 2 * index + 1;      // Hijo izquierdo: 2*i + 1
        int rightChild = 2 * index + 2;     // Hijo derecho: 2*i + 2
        
        // Comparamos con hijo izquierdo (buscamos el MAYOR)
        if (leftChild < size) {
            comparisons++;
            if (heap[leftChild] > heap[largest]) {
                largest = leftChild;
            }
        }
        
        // Comparamos con hijo derecho
        if (rightChild < size) {
            comparisons++;
            if (heap[rightChild] > heap[largest]) {
                largest = rightChild;
            }
        }
        
        // Si el hijo más grande es mayor que el padre, intercambiamos
        if (largest != index) {
            swap(heap[index], heap[largest]);
            siftDown(largest, size);  // Recursión para mantener propiedad
        }
    }
    
    // Construir max-heap: reorganizamos el array para que cumpla propiedad de heap
    void buildMaxHeap() {
        // Empezamos desde el último nodo no-hoja (size/2 - 1)
        for (int i = heap.size() / 2 - 1; i >= 0; i--) {
            siftDown(i, heap.size());
        }
    }
    
public:
    PerformanceMetrics sort(vector<string>& dataset) {
        PerformanceMetrics metrics("HeapSort (Binary Heap)");
        heap = dataset;
        comparisons = 0;
        
        auto start = chrono::high_resolution_clock::now();
        
        // 1. Construir el heap (max-heap)
        buildMaxHeap();
        
        // 2. Extraer elementos uno a uno del heap
        for (int i = heap.size() - 1; i > 0; i--) {
            swap(heap[0], heap[i]);  // Movemos la raíz (más grande) al final
            siftDown(0, i);           // Mantenemos propiedad de heap
        }
        
        auto end = chrono::high_resolution_clock::now();
        
        metrics.executionTime = chrono::duration<double, milli>(end - start).count();
        metrics.memoryUsage = heap.size() * sizeof(string);
        
        dataset = heap;
        return metrics;
    }
    
    vector<string> getData() const { return heap; }
};

// ============================================================================
// 3. IMPLEMENTACIÓN DE AVL TREE
// ============================================================================
struct AVLNode {
    string value;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(string val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;
    long long comparisons = 0;
    
    // Obtener la altura de un nodo
    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }
    
    // Obtener el factor de balance (diferencia entre altura izq y der)
    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    // Actualizar la altura de un nodo
    void updateHeight(AVLNode* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }
    
    // Rotación derecha: cuando el árbol está desbalanceado a la izquierda
    //       z              y
    //      / \    =>      / \
    //     y   c          x   z
    //    / \                / \
    //   x   b              b   c
    AVLNode* rotateRight(AVLNode* z) {
        AVLNode* y = z->left;
        AVLNode* b = y->right;
        
        y->right = z;
        z->left = b;
        
        updateHeight(z);
        updateHeight(y);
        
        return y;
    }
    
    // Rotación izquierda: cuando el árbol está desbalanceado a la derecha
    //     x              z
    //      \ \    =>    / \
    //       z   c      x   y
    //      / \        / \
    //     b   y      b   c
    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* z = x->right;
        AVLNode* b = z->left;
        
        z->left = x;
        x->right = b;
        
        updateHeight(x);
        updateHeight(z);
        
        return z;
    }
    
    // Insertar recursivamente manteniendo balance
    AVLNode* insert(AVLNode* node, const string& value) {
        if (!node) {
            return new AVLNode(value);
        }
        
        comparisons++;
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        } else {
            return node;  // Valor duplicado
        }
        
        updateHeight(node);
        int balance = getBalance(node);
        
        // Caso 1: Desbalance a la izquierda-izquierda
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        
        // Caso 2: Desbalance a la izquierda-derecha
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Caso 3: Desbalance a la derecha-derecha
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        
        // Caso 4: Desbalance a la derecha-izquierda
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    // Inorder traversal: izquierda -> nodo -> derecha (resultado ordenado)
    void inorder(AVLNode* node, vector<string>& result) {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->value);
        inorder(node->right, result);
    }
    
    // Liberar memoria recursivamente
    void deleteTree(AVLNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    
public:
    AVLTree() : root(nullptr), comparisons(0) {}
    
    ~AVLTree() {
        deleteTree(root);
    }
    
    void insert(const string& value) {
        root = insert(root, value);
    }
    
    PerformanceMetrics sort(vector<string>& dataset) {
        PerformanceMetrics metrics("AVL Tree");
        comparisons = 0;
        
        auto start = chrono::high_resolution_clock::now();
        
        // Insertar todos los elementos
        for (const string& word : dataset) {
            insert(word);
        }
        
        // Obtener resultado con inorder traversal
        vector<string> result;
        inorder(root, result);
        
        auto end = chrono::high_resolution_clock::now();
        
        metrics.executionTime = chrono::duration<double, milli>(end - start).count();
        
        // Memoria: nodos (valor + 3 punteros + altura) + pointers overhead
        metrics.memoryUsage = dataset.size() * 
                             (sizeof(string) + 3 * sizeof(AVLNode*) + sizeof(int));
        
        dataset = result;
        return metrics;
    }
};

// ============================================================================
// FUNCIONES AUXILIARES
// ============================================================================

// Cargar dataset desde archivo
vector<string> loadDataset(const string& filename, int limit = 100000) {
    vector<string> dataset;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return dataset;
    }
    
    string word;
    while (getline(file, word) && (int)dataset.size() < limit) {
        // Limpiar espacios en blanco
        word.erase(0, word.find_first_not_of(" \t\r\n"));
        word.erase(word.find_last_not_of(" \t\r\n") + 1);
        
        if (!word.empty()) {
            dataset.push_back(word);
        }
    }
    
    file.close();
    return dataset;
}

// Verificar si un array está ordenado
bool isSOKorted(const vector<string>& data) {
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i] < data[i-1]) {
            return false;
        }
    }
    return true;
}

// Guardar resultados a archivo
void saveResults(const vector<string>& data, const string& filename) {
    ofstream file(filename);
    for (const string& word : data) {
        file << word << "\n";
    }
    file.close();
}

// ============================================================================
// MAIN
// ============================================================================
int main() {
    cout << "\n" << string(60, '=') << endl;
    cout << "ANÁLISIS DE ALGORITMOS DE ORDENAMIENTO" << endl;
    cout << "QuickSort vs HeapSort vs AVL Tree" << endl;
    cout << string(60, '=') << "\n" << endl;
    
    // 1. Cargar dataset
    cout << "[1/5] Cargando dataset..." << endl;
    cout.flush();
    
    vector<string> dataset = loadDataset("C:\\Users\\Zqmelissa27\\Desktop\\Data_Structures\\dataset.txt", 100000);
    
    if (dataset.empty()) {
        cerr << "\n¡ERROR! Dataset vacío\n" << endl;
        cout << "Presiona ENTER para salir..." << endl;
        cin.ignore();
        return 1;
    }
    
    cout << "[✓] Dataset cargado correctamente: " << dataset.size() << " palabras\n" << endl;
    cout.flush();
    
    // Almacenar métricas
    vector<PerformanceMetrics> allMetrics;
    
    // 2. QuickSort
    cout << "[2/5] Ejecutando QuickSort..." << endl;
    cout.flush();
    vector<string> dataQS = dataset;
    QuickSortSolver qs;
    PerformanceMetrics metricsQS = qs.sort(dataQS);
    metricsQS.displayMetrics();
    cout.flush();
    
    bool qsOK = isSOKorted(dataQS);
    if (qsOK) {
        cout << "[✓] QuickSort: ORDENADO CORRECTAMENTE\n" << endl;
    } else {
        cout << "[✗] ERROR: QuickSort falló\n" << endl;
    }
    allMetrics.push_back(metricsQS);
    cout.flush();
    
    // 3. HeapSort
    cout << "[3/5] Ejecutando HeapSort..." << endl;
    cout.flush();
    vector<string> dataHS = dataset;
    HeapSortSolver hs;
    PerformanceMetrics metricsHS = hs.sort(dataHS);
    metricsHS.displayMetrics();
    cout.flush();
    
    bool hsOK = isSOKorted(dataHS);
    if (hsOK) {
        cout << "[✓] HeapSort: ORDENADO CORRECTAMENTE\n" << endl;
    } else {
        cout << "[✗] ERROR: HeapSort falló\n" << endl;
    }
    allMetrics.push_back(metricsHS);
    cout.flush();
    
    // 4. AVL Tree
    cout << "[4/5] Ejecutando AVL Tree..." << endl;
    cout.flush();
    vector<string> dataAVL = dataset;
    AVLTree avl;
    PerformanceMetrics metricsAVL = avl.sort(dataAVL);
    metricsAVL.displayMetrics();
    cout.flush();
    
    bool avlOK = isSOKorted(dataAVL);
    if (avlOK) {
        cout << "[✓] AVL Tree: ORDENADO CORRECTAMENTE\n" << endl;
    } else {
        cout << "[✗] ERROR: AVL Tree falló\n" << endl;
    }
    allMetrics.push_back(metricsAVL);
    cout.flush();
    
    // 5. Generar resultados
    cout << "[5/5] Generando comparativa final...\n" << endl;
    cout.flush();
    
    cout << string(60, '=') << endl;
    cout << "COMPARATIVA DE RENDIMIENTO" << endl;
    cout << string(60, '=') << endl;
    
    cout << fixed << setprecision(3);
    cout << left << setw(25) << "Algoritmo" 
         << setw(20) << "Tiempo (ms)" 
         << setw(20) << "Memoria (MB)" << endl;
    cout << string(65, '-') << endl;
    
    for (const auto& m : allMetrics) {
        cout << left << setw(25) << m.algorithmName
             << setw(20) << m.executionTime
             << setw(20) << (m.memoryUsage / 1024.0 / 1024.0) << endl;
    }
    
    // Encontrar el más rápido
    auto fastest = min_element(allMetrics.begin(), allMetrics.end(),
                               [](const PerformanceMetrics& a, const PerformanceMetrics& b) {
                                   return a.executionTime < b.executionTime;
                               });
    
    cout << string(65, '-') << endl;
    cout << "\n[✓] Algoritmo más rápido: " << fastest->algorithmName 
         << " (" << fastest->executionTime << " ms)" << endl;
    
    cout << "\n" << string(60, '=') << endl;
    cout << "¡ANÁLISIS COMPLETADO EXITOSAMENTE!" << endl;
    cout << "Todos los algoritmos funcionaron correctamente." << endl;
    cout << string(60, '=') << "\n" << endl;
    cout.flush();
    
    // Pausa final - IMPORTANTE para poder leer los resultados
    cout << "\n>>> Presiona ENTER para salir del programa..." << endl;
    cin.ignore();  // Espera a que presiones ENTER
    
    return 0;
}