#include "huffman.h"
#include "image_io.h"
#include <stdio.h>
#include <stdlib.h>

// Main function with menu
int main() {
  int choice;
  char input_file[256], output_file[256];

  printf("Select image format:\n");
  printf("1. PPM (P6)\n");
  printf("2. PBM (P4)\n");
  printf("3. PBM (P1)\n");
  printf("4. PGM (P5)\n");
  printf("Enter your choice: ");
  scanf("%d", &choice);

  printf("Enter input file name: ");
  scanf("%s", input_file);

  printf("Enter output compressed file name: ");
  scanf("%s", output_file);

  if (choice == 1) {
    PPM_Image *image = readPPM(input_file);
    if (image == NULL) {
      printf("Error reading PPM file.\n");
      return 1;
    }
    int data_size = image->width * image->height * 3;
    int freq[256] = {0};

    calculateFrequency(image->data, data_size, freq);

    unsigned char data[256];
    int size = 0;
    for (int i = 0; i < 256; ++i) {
      if (freq[i] > 0) {
        data[size++] = (unsigned char)i;
      }
    }

    HuffmanCodes(data, freq, size, image->data, data_size, output_file);
    freePPM(image);
  } else if (choice == 2) {
    PBM_Image *image = readPBM_P4(input_file);
    if (image == NULL) {
      printf("Error reading PBM file.\n");
      return 1;
    }
    int row_bytes = (image->width + 7) / 8;
    int data_size = row_bytes * image->height;
    int freq[256] = {0};

    calculateFrequency(image->data, data_size, freq);

    unsigned char data[256];
    int size = 0;
    for (int i = 0; i < 256; ++i) {
      if (freq[i] > 0) {
        data[size++] = (unsigned char)i;
      }
    }

    HuffmanCodes(data, freq, size, image->data, data_size, output_file);
    freePBM_P4(image);
  } else if (choice == 3) {
    PBM_Image *image = readPBM_P1(input_file);
    if (image == NULL) {
      printf("Error reading PBM file.\n");
      return 1;
    }
    int data_size = image->width * image->height;
    int freq[2] = {0};

    for (int i = 0; i < data_size; ++i) {
      freq[image->data[i]]++;
    }

    unsigned char data[2] = {0, 1};

    HuffmanCodes(data, freq, 2, image->data, data_size, output_file);
    freePBM_P1(image);
  } else if (choice == 4) {
    PGM_Image *image = readPGM(input_file);
    if (image == NULL) {
      printf("Error reading PGM file.\n");
      return 1;
    }
    int data_size = image->width * image->height;
    int freq[256] = {0};

    calculateFrequency(image->data, data_size, freq);

    unsigned char data[256];
    int size = 0;
    for (int i = 0; i < 256; ++i) {
      if (freq[i] > 0) {
        data[size++] = (unsigned char)i;
      }
    }

    HuffmanCodes(data, freq, size, image->data, data_size, output_file);
    freePGM(image);
  } else {
    printf("Invalid choice!\n");
    return 1;
  }

  printf("Compression successful!\n");
  return 0;
}
