#ifndef IMAGE_IO_H
#define IMAGE_IO_H

typedef struct {
    int width;
    int height;
    int max_color;
    unsigned char *data;
} PPM_Image;

typedef struct {
    int width;
    int height;
    int max_gray;
    unsigned char *data;
} PGM_Image;

typedef struct {
    int width;
    int height;
    unsigned char *data;
} PBM_Image;

PPM_Image* readPPM(const char* filename);
void writePPM(const char *filename, PPM_Image *image);
void freePPM(PPM_Image *image);

PGM_Image* readPGM(char* filename);
void writePGM(char *filename, PGM_Image *image);
void freePGM(PGM_Image *image);

PBM_Image* readPBM_P4(char *filename);
void writePBM_P4(char *filename, PBM_Image *image);
void freePBM_P4(PBM_Image *image);

#endif // IMAGE_IO_H
