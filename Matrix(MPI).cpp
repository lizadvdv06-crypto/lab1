#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    string fileName;
    int n;

    int** A = nullptr;
    int** B = nullptr;
    int** C = nullptr;

    int* partA;
    int* partC;

    if (rank == 0) {

        cout << "Enter file name: ";
        cin >> fileName;

        string path = "C:\\Users\\user\\Desktop\\учеба\\" + fileName;

        ifstream fa(path);

        if (!fa.is_open()) {
            cout << "File not found!" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fa >> n;

        A = new int* [n];
        B = new int* [n];
        C = new int* [n];

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
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows = n / size;

    partA = new int[rows * n];
    partC = new int[rows * n];

    int* fullB = new int[n * n];

    if (rank == 0) {
        int* tempA = new int[n * n];
        int* tempB = new int[n * n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                tempA[i * n + j] = A[i][j];
                tempB[i * n + j] = B[i][j];
            }
        }

        MPI_Scatter(tempA, rows * n, MPI_INT,
            partA, rows * n, MPI_INT,
            0, MPI_COMM_WORLD);

        for (int i = 0; i < n * n; i++)
            fullB[i] = tempB[i];

        delete[] tempA;
        delete[] tempB;
    }
    else {
        MPI_Scatter(nullptr, rows * n, MPI_INT,
            partA, rows * n, MPI_INT,
            0, MPI_COMM_WORLD);
    }

    MPI_Bcast(fullB, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < n; j++) {

            partC[i * n + j] = 0;

            for (int k = 0; k < n; k++) {
                partC[i * n + j] +=
                    partA[i * n + k] * fullB[k * n + j];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();

    double time = chrono::duration<double>(end - start).count();

    if (rank == 0) {

        int* tempC = new int[n * n];

        MPI_Gather(partC, rows * n, MPI_INT,
            tempC, rows * n, MPI_INT,
            0, MPI_COMM_WORLD);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                C[i][j] = tempC[i * n + j];

        cout << "-----------------------------------" << endl;
        cout << "Matrix size: " << n << "x" << n << endl;
        cout << "Processes used: " << size << endl;
        cout << "Time: " << time << " seconds" << endl;
        cout << "-----------------------------------" << endl;

        delete[] tempC;

        for (int i = 0; i < n; i++) {
            delete[] A[i];
            delete[] B[i];
            delete[] C[i];
        }

        delete[] A;
        delete[] B;
        delete[] C;
    }
    else {

        MPI_Gather(partC, rows * n, MPI_INT,
            nullptr, rows * n, MPI_INT,
            0, MPI_COMM_WORLD);
    }

    delete[] partA;
    delete[] partC;
    delete[] fullB;

    MPI_Finalize();

    return 0;
}
