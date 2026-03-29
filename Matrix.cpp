#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
int main() {
    ifstream fa("C:\\Users\\user\\Desktop\\учеба\\A.txt");
    ifstream fb("C:\\Users\\user\\Desktop\\учеба\\B.txt");
    ofstream fc("C:\\Users\\user\\Desktop\\учеба\\C.txt");

    int n,m;
    fa >> n;
    fb >> m;
    if (n != m) {
        cout << "Matrix sizes not equal" << endl;
        return 1;
    }

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
            fb >> B[i][j];

    auto start = chrono::high_resolution_clock::now();

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

    fc << n << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fc << C[i][j] << " ";
        }
        fc << endl;
    }

    cout << "Time: " << time << " sec" << endl;

    long long ops = (long long)n * n * n;
    cout << "Task size (operations): " << ops << endl;

    return 0;
}
