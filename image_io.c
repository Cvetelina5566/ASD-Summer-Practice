#include "./image_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PPM_Image *readPPM(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error: cannot open file");
    exit(1);
  }

  char format[3];
  if (fscanf(file, "%2s\n", format) != 1) {
    printf("Invalid PPM file");
    exit(1);
  }
  if (strcmp(format, "P6") != 0) {
    printf("Unsupprted PPM format");
    exit(1);
  }

  //
  char ch;
  while ((ch = fgetc(file)) == '#') {
    while (fgetc(file) != '\n')
      ;
  }
  ungetc(ch, file);
  //

  PPM_Image *image = (PPM_Image *)malloc(sizeof(PPM_Image));
  if (image == NULL) {
    printf("Error: cannot allocate memory");
    exit(1);
  }

  if (fscanf(file, "%d %d\n%d\n", &image->width, &image->height,
             &image->max_color) != 3) {
    printf("Invalid PPM header");
    exit(1);
  }

  // Allocate memory for pixel (image) data
  int image_size = image->width * image->height * 3;
  image->data = (unsigned char *)malloc(image_size);
  if (!image->data) {
    printf("Unable to allocate memory");
    exit(1);
  }

  // Read pixel data
  if (fread(image->data, 1, image_size, file) != image_size) {
    printf("Error reading pixel data");
    exit(1);
  }
  fclose(file);
  return image;
}

// Function to write PPM file
void writePPM(const char *filename, PPM_Image *image) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    printf("Unable to open file");
    exit(1);
  }

  // Write the PPM header
  fprintf(file, "P6\n%d %d\n%d\n", image->width, image->height,
          image->max_color);

  // Write pixel data
  int image_size = image->width * image->height * 3;
  if (fwrite(image->data, 1, image_size, file) != image_size) {
    printf("Error writing pixel data");
    exit(1);
  }
  fclose(file);
}

// function to free PPM image memory
void freePPM(PPM_Image *image) {
  free(image->data);
  free(image);
}

PGM_Image *readPGM(char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error: cannot open file");
    exit(1);
  }

  char format[3];
  if (fscanf(file, "%2s", format) != 1) {
    printf("Invalid PGM file");
    exit(1);
  }
  if (strcmp(format, "P5") != 0) {
    printf("Unsupprted PGM format");
    exit(1);
  }

  PGM_Image *image = (PGM_Image *)malloc(sizeof(PGM_Image));
  if (image == NULL) {
    printf("Error: cannot allocate memory");
    exit(1);
  }

  char ch;
  while ((ch = fgetc(file)) == '#') {
    while (fgetc(file) != '\n')
      ;
  }
  ungetc(ch, file);

  if (fscanf(file, "%d %d %d", &image->width, &image->height,
             &image->max_gray) != 3) {
    printf("Invalid PGM header");
    exit(1);
  }
  if (image->max_gray != 255) {
    printf("Unsupported PGM max gray value");
    exit(1);
  }
  fgetc(file);
  int data_size = image->width * image->height;
  image->data = (unsigned char *)malloc(data_size);
  if (!image->data) {
    printf("Unable to allocate memory");
    exit(1);
  }

  fclose(file);
  return image;

  if (image) {
    free(image->data);
    free(image);
  }
}

void writePGM(char *filename, PGM_Image *image) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    fprintf(stderr, "Error opening file for writing: %s\n", filename);
    return;
  }

  // Write the PGM header
  fprintf(file, "P5\n");
  fprintf(file, "%d %d\n", image->width, image->height);
  fprintf(file, "%d\n", image->max_gray);

  // Write the image data
  fwrite(image->data, sizeof(unsigned char), image->width * image->height,
         file);

  fclose(file);
}

void freePGM(PGM_Image *image){
  free(image->data);
  free(image);
}

PBM_Image* readPBM_P4(char *filename){
  FILE *file=fopen(filename,"rb");
  if(!file){
    printf("Unable to open file");
    exit(1);
  }

  //К'ъв е тоя header и тия проверки?
  char format[3];
  if(fscanf(file,"%2s\n",format)!=1){
    printf("Invalid PBM file");
    exit(1);
  }
  if(strcmp(format,"P4")!=0){
    printf("Unsupprted PBM format");
    exit(1);
  }

  //Skip comments
  char ch;
  while((ch=fgetc(file))=='#'){
    while(fgetc(file)!='\n');
  }
  ungetc(ch,file);

  PBM_Image *image=(PBM_Image*)malloc(sizeof(PBM_Image));
  if(!image){
    printf("Unable to allocate memory");
    exit(1);
  }

  //Read image size
  if(fscanf(file, "%d %d\n", &image->width, &image->height)!= 2){
    printf("Invalid PBM header");
  }
  //Calculate the size of the pixel data
  int row_bytes=(image->width + 7)/8;
  int image_size=row_bytes*image->height;

  //Allocate memory for pixel data
  image->data=(unsigned char*)malloc(image_size);
  if(!image->data){
    printf("Unable to allocate memory");
    exit(1);
  }

  //Read pixel data
  if(fread(image->data,1,image_size,file)!=image_size){
    printf("Error reading pixel data");
    exit(1);
  }

  fclose(file);
  return image;
}

void writePBM_P4(char *filename, PBM_Image *image){
    FILE *file=fopen(filename,"wb");
    if(!file){
      printf("Unable to open file");
      exit(1);
    }

    //Write the PBM header //What is header?
    fprintf(file,"P4\n%d %d\n",image->width,image->height);

    //Calculate the size of the pixel data
    int row_bytes=(image->width + 7)/8;
    int image_size=row_bytes*image->height;

    //Write pixel data
    if(fwrite(image->data,1,image_size,file)!=image_size){
      printf("Error writing pixel data");
      exit(1);
    }
    fclose(file);
  }

void freePBM_P4(PBM_Image *image){
  free(image->data);
  free(image);
}

PBM_Image* readPBM_P1(char *filename){
    FILE *file=fopen(filename,"rb");
    if(!file){
      printf("Unable to open file");
      exit(1);
    }

      //К'ъв е тоя header и тия проверки?
      char format[3];
      if(fscanf(file,"%2s\n",format)!=1){
        printf("Invalid PBM file");
        exit(1);
      }
      if(strcmp(format,"P1")!=0){
        printf("This is not PBM P1 format");
        exit(1);
      }

      //Skip comments
      char ch;
      while((ch=fgetc(file))=='#'){
        while(fgetc(file)!='\n');
      }
      ungetc(ch,file);

      PBM_Image *image=(PBM_Image*)malloc(sizeof(PBM_Image));
      if(!image){
        printf("Unable to allocate memory");
        exit(1);
      }

      //Read image size
      if(fscanf(file, "%d %d\n", &image->width, &image->height)!= 2){
        printf("Invalid PBM header");
      }

      //Allocate memory for pixel data
      int data_size = image->width * image->height;
      image->data = (unsigned char *)malloc(data_size);
      if (!image->data) {
          printf("Unable to allocate memory");
          exit(1);
      }
    // Read pixel data
    int index = 0;
    while (index < data_size && fscanf(file, "%hhu", &image->data[index]) == 1) {
        index++;
    }
     fclose(file);
      return image;
    }

void writePBM_P1(char *filename, PBM_Image *image){
    FILE *file=fopen(filename,"w");
    if(!file){
      printf("Unable to open file");
      exit(1);
    }

    //header (?)
    fprintf(file,"P1\n%d %d\n",image->width,image->height);

    //pixel data
    int data_size=image->width*image->height;
    for(int i=0;i<data_size;i++){
      fprintf(file, "%d ",image->data[i]);
      if((i+1)%image->width==0){
        fprintf(file, "\n");
      }
    }
    fclose(file);
}

void freePBM_P1(PBM_Image *image){
  free(image->data);
  free(image);
}