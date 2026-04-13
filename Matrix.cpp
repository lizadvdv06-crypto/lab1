#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <omp.h>

using namespace std;

int main() {
    string fileName;
    int threads;

    cout << "Enter file name (e.g., A.txt, B.txt...): ";
    cin >> fileName;
    cout << "Enter number of threads: ";
    cin >> threads;

    string path = "C:\\Users\\user\\Desktop\\учеба\\" + fileName;
    ifstream fa(path);

    if (!fa.is_open()) {
        cout << "File not found!" << endl;
        return 1;
    }

    int n;
    fa >> n; 
    int** A = new int* [n];
    int** B = new int* [n];
    int** C = new int* [n];

    for (int i = 0; i < n; i++) {
        A[i] = new int[n];
        B[i] = new int[n];
        C[i] = new int[n];
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fa >> A[i][j];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            B[i][j] = A[i][j];

    omp_set_num_threads(threads);

    auto start = chrono::high_resolution_clock::now();

#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double time = chrono::duration<double>(end - start).count();

    cout << "-----------------------------------" << endl;
    cout << "File: " << fileName << " (Size: " << n << "x" << n << ")" << endl;
    cout << "Threads used: " << threads << endl;
    cout << "Time: " << time << " seconds" << endl;
    cout << "-----------------------------------" << endl;

    for (int i = 0; i < n; i++) {
        delete[] A[i]; delete[] B[i]; delete[] C[i];
    }
    delete[] A; delete[] B; delete[] C;

    return 0;
}
