#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void test(int a[], int n);
int random_generator(int arr[], int size);
void exchage_sort(int arr[], int size);
void merge(int arr[], int left, int mid, int right);
void merge_sort(int arr[], int left, int right);
void quick_sort(int data[], int low, int high);
void heap_sort(int data[], int size);
int getMax(int arr[], int n);
void countingSort(int arr[], int size, int place);
void radix_sort(int arr[], int size);

int main(int argc, char* argv[]) {
    int c1[100], c2[500], c3[1000], c4[5000], c5[10000];
    int a = 100, b = 500, c = 1000, d = 5000, e = 10000;
    srand(time(NULL));

    test(c1, a);
    test(c2, b);
    test(c3, c);
    test(c4, d);
    test(c5, e);

    return 0;
}

void test(int a[], int n) {
    float result = 0;
    int repetition = 5;
    clock_t start, end;

    printf("\nN = %d\n", n);

    // exchange sort
    for (int i = 0; i < repetition; i++) {
        random_generator(a, n);
        start = clock();
        exchage_sort(a, n);
        end = clock();
        result += (float)(end - start) / CLOCKS_PER_SEC;
    }
    printf("exchange sort: %f\n", result / repetition);
    result = 0;

    // merge sort
    for (int i = 0; i < repetition; i++) {
        random_generator(a, n);
        start = clock();
        merge_sort(a, 0, n - 1);
        end = clock();
        result += (float)(end - start) / CLOCKS_PER_SEC;
    }
    printf("merge sort: %f\n", result / repetition);
    result = 0;

    // quick sort
    for (int i = 0; i < repetition; i++) {
        random_generator(a, n);
        start = clock();
        quick_sort(a, 0, n - 1);
        end = clock();
        result += (float)(end - start) / CLOCKS_PER_SEC;
    }
    printf("quick sort: %f\n", result / repetition);
    result = 0;

    // heap sort
    for (int i = 0; i < repetition; i++) {
        random_generator(a, n);
        start = clock();
        heap_sort(a, n);
        end = clock();
        result += (float)(end - start) / CLOCKS_PER_SEC;
    }
    printf("heap sort: %f\n", result / repetition);
    result = 0;

    // radix sort
    for (int i = 0; i < repetition; i++) {
        random_generator(a, n);
        start = clock();
        radix_sort(a, n);
        end = clock();
        result += (float)(end - start) / CLOCKS_PER_SEC;
    }
    printf("radix sort: %f\n", result / repetition);
    result = 0;
}

int random_generator(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }
}

void exchage_sort(int arr[], int size) {
    int temp = 0;
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size - 1; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void merge(int arr[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    int* sortArr = (int*)malloc(sizeof(int) * (right + 1));

    while (i <= mid && j <= right) {
        if (arr[i] < arr[j]) {
            sortArr[k] = arr[i];
            i++;
        }
        else {
            sortArr[k] = arr[j];
            j++;
        }
        k++;
    }
    if (i > mid) {
        for (int l = j; l <= right; l++) {
            sortArr[k] = arr[l];
            k++;
        }
    }
    else {
        for (int l = i; l <= mid; l++) {
            sortArr[k] = arr[l];
            k++;
        }
    }
    for (int l = left; l <= right; l++) {
        arr[l] = sortArr[l];
    }
    free(sortArr);
}

void merge_sort(int arr[], int left, int right) {
    int mid = 0;

    if (left < right) {
        mid = (left + right) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void quick_sort(int arr[], int left, int right) {
    int temp, pivot;
    if (left < right) {
        pivot = left;
        int i = left;
        int j = right;
        while (i < j) {
            while (arr[i] <= arr[pivot] && i < right)
                i++;
            while (arr[j] > arr[pivot])
                j--;
            if (i < j) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        temp = arr[pivot];
        arr[pivot] = arr[j];
        arr[j] = temp;
        quick_sort(arr, left, j - 1);
        quick_sort(arr, j + 1, right);
    }
}

void MakeHeap(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int child = i;
        do {
            int root = (child - 1) / 2;
            if (arr[root] < arr[child]) {
                int temp = arr[root];
                arr[root] = arr[child];
                arr[child] = temp;
            }
            child = root;
        } while (child != 0);
    }
}

void heap_sort(int arr[], int size) {
    MakeHeap(arr, size);
    for (int i = size - 1; i >= 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        int root = 0;
        int child = 1;
        do {
            child = 2 * root + 1;
            if (child < i - 1 && arr[child] < arr[child + 1]) {
                child++;
            }
            if (child < i && arr[root] < arr[child]) {
                temp = arr[root];
                arr[root] = arr[child];
                arr[child] = arr;
            }
            root = child;
        } while (child < i);
    }
}

int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

void countingSort(int arr[], int size, int place) {
    int max = (arr[0] / place) % 10; 
    int count[10]={0.};
    int output[10000]={0.};

    for (int i = 1; i < size; i++) {
        if (((arr[i] / place) % 10) > max)
            max = (arr[i] / place) % 10;
    }
    for (int i = 0; i < max; ++i)  
        count[i] = 0;

    for (int i = 0; i < size; i++)
        count[(arr[i] / place) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = size - 1; i >= 0; i--) {
        output[count[(arr[i] / place) % 10] - 1] = arr[i];
        count[(arr[i] / place) % 10]--;
    }

    for (int i = 0; i < size; i++)
        arr[i] = output[i];
}

void radix_sort(int arr[], int size) {
    int max = getMax(arr, size);
    for (int place = 1; max / place > 0; place *= 10)
        countingSort(arr, size, place);
}



