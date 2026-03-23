# Course_DSA

This repository contains a comparative implementation of three sorting approaches in C++ for a dataset of 100,000 words:

- QuickSort
- HeapSort
- AVL Tree-based sorting

The project was developed for the **Data Structures and Algorithms** course and includes source code, performance measurements, graphs, and the final report.

---

##  Objective

The goal of this project is to compare different sorting strategies based on:

- implementation approach
- execution time
- memory usage
- theoretical algorithmic complexity
- practical suitability for the problem

The dataset consists of **100,000 words**, and each algorithm was implemented from scratch in C++.

---

## Implemented Algorithms

### 1. QuickSort
QuickSort was implemented recursively over a `vector<string>` using a partition-based strategy.  
It offers average-case complexity of **O(n log n)**, although its worst case is **O(n^2)**.

### 2. HeapSort
HeapSort was implemented using a binary max-heap.  
It guarantees **O(n log n)** complexity in the best, average, and worst cases.

### 3. AVL Tree-Based Sorting
This approach inserts all words into a self-balancing binary search tree and then retrieves them in sorted order through an inorder traversal.  
Its overall complexity is **O(n log n)**, but it requires more memory because of its node-based structure.

---

## Experimental Results

The measured results obtained in the project were:

| Algorithm | Time (ms) | Memory (MB) |
|----------|----------:|------------:|
| QuickSort | 128.604 | 4.395 |
| HeapSort | 189.136 | 4.395 |
| AVL Tree | 128.413 | 8.210 |



## Repository Structure

```text
Course_DSA/
│
├── sorting_algorithms.cpp          # Main C++ implementation
├── Demo.cpp                        # Additional/alternative test file
├── dataset.txt                     # Input dataset
├── graphs.py                       # Python script for generating plots
├── graphs/
│   ├── execution_time_comparison.png
│   ├── memory_usage_comparison.png
│   ├── time_vs_memory_tradeoff.png
│   └── theoretical_complexity_comparison.png
├── DATA_STRUCTURESI_ReportMZQ.pdf  # Final report
└── README.md
