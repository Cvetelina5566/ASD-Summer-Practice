#include "./huffman.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_TREE_HT 256

// Implement Huffman tree and compression functions
typedef struct MinHeapNode {
    unsigned char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
} MinHeapNode;

typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
} MinHeap;

MinHeapNode* newNode(unsigned char data, unsigned freq) {
    MinHeapNode* temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
        smallest = left;
    }

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
        smallest = right;
    }

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

MinHeapNode* extractMin(MinHeap* minHeap) {
    MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i) {
        minHeapify(minHeap, i);
    }
}

int isLeaf(MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = newNode(data[i], freq[i]);
    }

    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size) {
    MinHeapNode *left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void printCodes(MinHeapNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root)) {
        printf("%c: ", root->data);
        for (int i = 0; i < top; ++i) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

void compressData(MinHeapNode* root, int arr[], int top, unsigned char *data, int data_size, FILE *output_file) {
    static int codes[256][MAX_TREE_HT];
    static int codeLengths[256];

    if (root->left) {
        arr[top] = 0;
        compressData(root->left, arr, top + 1, data, data_size, output_file);
    }

    if (root->right) {
        arr[top] = 1;
        compressData(root->right, arr, top + 1, data, data_size, output_file);
    }

    if (isLeaf(root)) {
        codeLengths[root->data] = top;
        for (int i = 0; i < top; ++i) {
            codes[root->data][i] = arr[i];
        }
    }

    // Write compressed data
    if (root->data == '$') { // Only execute once after building the full tree
        for (int i = 0; i < data_size; ++i) {
            int value = data[i];
            for (int j = 0; j < codeLengths[value]; ++j) {
                fputc(codes[value][j] + '0', output_file);
            }
        }
    }
}

void HuffmanCodes(unsigned char data[], int freq[], int size, unsigned char *input_data, int input_size, const char *output_file) {
    MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;

    FILE *output = fopen(output_file, "wb");
    if (!output) {
        perror("Unable to open output file");
        exit(1);
    }

    compressData(root, arr, top, input_data, input_size, output);
    fclose(output);
}

void calculateFrequency(unsigned char *data, int size, int freq[256]) {
    for (int i = 0; i < size; ++i) {
        freq[data[i]]++;
    }
}

