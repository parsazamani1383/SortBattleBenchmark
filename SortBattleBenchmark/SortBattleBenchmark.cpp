/*
 * Sorting Algorithms Benchmark
 * ----------------------------
 * This program benchmarks several sorting algorithms: 
 * Insertion Sort, Merge Sort, Quick Sort, and Radix Sort.
 * 
 * The program measures runtime for various input sizes
 * and stores the results in a CSV file for further analysis.
 * 
 * Author: Parsa zamani
 * GitHub: https://github.com/parsazamani1383
 */

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <iomanip>
#include <chrono>

// --------------------- Insertion Sort ---------------------
void InsertionSort(int* A, int n) {
    for (int i = 1; i < n; i++) {
        int key = A[i];
        int j = i - 1;
        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key;
    }
}

// --------------------- Merge Sort ---------------------
void merge(int* A, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = A[p + i];
    for (int j = 0; j < n2; j++) R[j] = A[q + j + 1];

    int i = 0, j = 0, k = p;
    while (i < n1 && j < n2)
        A[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) A[k++] = L[i++];
    while (j < n2) A[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int* A, int p, int r) {
    if (p < r) {
        int q = p + (r - p) / 2;
        mergeSort(A, p, q);
        mergeSort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

// --------------------- Quick Sort ---------------------
int partition(int* A, int p, int r) {
    int pivot = A[r];
    int i = p - 1;
    for (int j = p; j < r; j++) {
        if (A[j] <= pivot) {
            i++;
            std::swap(A[i], A[j]);
        }
    }
    std::swap(A[i + 1], A[r]);
    return i + 1;
}

void QuickSort(int* A, int p, int r) {
    if (p < r) {
        int q = partition(A, p, r);
        QuickSort(A, p, q - 1);
        QuickSort(A, q + 1, r);
    }
}

// --------------------- Radix Sort ---------------------
int getMax(const int* A, int n) {
    int max = A[0];
    for (int i = 1; i < n; i++)
        if (A[i] > max) max = A[i];
    return max;
}

void countingSort(int* A, int n, int exp) {
    int* output = new int[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(A[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        output[count[(A[i] / exp) % 10] - 1] = A[i];
        count[(A[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++)
        A[i] = output[i];

    delete[] output;
}

void RadixSort(int* A, int n) {
    int max = getMax(A, n);
    for (int exp = 1; max / exp > 0; exp *= 10)
        countingSort(A, n, exp);
}

// --------------------- Timing Functions ---------------------
void measureTime(void (*sortFunc)(int*, int), int* A, int n, char* result) {
    int* copy = new int[n];
    std::copy(A, A + n, copy);

    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(copy, n);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (duration > 300000)
        strcpy(result, "time limit");
    else
        sprintf(result, "%lld", duration);

    delete[] copy;
}

void measureTimeMergeSort(int* A, int n, char* result) {
    int* copy = new int[n];
    std::copy(A, A + n, copy);

    auto start = std::chrono::high_resolution_clock::now();
    mergeSort(copy, 0, n - 1);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (duration > 300000)
        strcpy(result, "time limit");
    else
        sprintf(result, "%lld", duration);

    delete[] copy;
}

void measureTimeQuickSort(int* A, int n, char* result) {
    int* copy = new int[n];
    std::copy(A, A + n, copy);

    auto start = std::chrono::high_resolution_clock::now();
    QuickSort(copy, 0, n - 1);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (duration > 300000)
        strcpy(result, "time limit");
    else
        sprintf(result, "%lld", duration);

    delete[] copy;
}

// --------------------- Array Generator ---------------------
int* generateRandomArray(int size) {
    int* arr = new int[size];
    for (int i = 0; i < size; i++)
        arr[i] = rand() % (INT_MAX / 2);
    return arr;
}

// --------------------- Main Function ---------------------
int main() {
    srand(time(0));

    int sizes[] = {
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
        200, 300, 400, 500, 600, 700, 800, 900, 1000,
        2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000,
        20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000
    };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    std::ofstream outFile("sorting_results.csv");
    if (!outFile) {
        std::cerr << "Failed to open sorting_results.csv" << std::endl;
        return 1;
    }

    outFile << "Array Size,Insertion Sort,Merge Sort,Quick Sort,Radix Sort\n";

    std::cout << std::left
              << std::setw(15) << "Array Size"
              << std::setw(20) << "Insertion Sort"
              << std::setw(15) << "Merge Sort"
              << std::setw(15) << "Quick Sort"
              << std::setw(15) << "Radix Sort" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        int* original = generateRandomArray(size);

        char time1[20], time2[20], time3[20], time4[20];
        measureTime(InsertionSort, original, size, time1);
        measureTimeMergeSort(original, size, time2);
        measureTimeQuickSort(original, size, time3);
        measureTime(RadixSort, original, size, time4);

        std::cout << std::left
                  << std::setw(15) << size
                  << std::setw(20) << time1
                  << std::setw(15) << time2
                  << std::setw(15) << time3
                  << std::setw(15) << time4 << std::endl;

        outFile << size << "," << time1 << "," << time2 << "," << time3 << "," << time4 << "\n";

        delete[] original;
    }

    outFile.close();
    std::cout << "\nResults saved to sorting_results.csv\n";
    return 0;
}
