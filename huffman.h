#ifndef HUFFMAN_H
#define HUFFMAN_H

void calculateFrequency(unsigned char *data, int size, int freq[256]);
void HuffmanCodes(unsigned char data[], int freq[], int size, unsigned char *input_data, int input_size, const char* output_file);

#endif //HUFFMAN_H
