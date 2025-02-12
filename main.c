//
// Created by Shreejit Murthy on 03/02/2025
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define WASM_HEADER_SIZE 8
/* 
 * Every .wasm file begins with the magic number and the version number:
 * Magic:   00 61 73 6D
 * Version: 01 00 00 00 
 * NOTE: The version of the WebAssembly binary format may increase in the future.
 * I'll just check for both numbers.
 */
const uint8_t valid_header[WASM_HEADER_SIZE] = { 0x00, 0x61, 0x73, 0x6D, 0x01, 0x00, 0x00, 0x00 };

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("None or excess arguments supplied. One path argument in required\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // First check the header to ensure the .wasm file is valid
    uint8_t header[WASM_HEADER_SIZE];
    size_t bytesRead = fread(header, 1, WASM_HEADER_SIZE, file);

    if (bytesRead < WASM_HEADER_SIZE) {
        printf("File is too small to be valid WASM.");
        return 1;
    }

    if (memcmp(header, valid_header, WASM_HEADER_SIZE) == 0) {

    } else {
        printf("Invalid .wasm file header!\nExpected:");
        for (int i = 0; i < WASM_HEADER_SIZE; i++)
            printf(" %02X", valid_header[i]);
        printf("\nBut found:");
        for (int i = 0; i < WASM_HEADER_SIZE; i++)
            printf(" %02X", header[i]);
        printf("\n");
        return 1;
    }

    // Now read entire file into a buffer
    fseek(file, 0, SEEK_END); 
    long size = ftell(file);
    fseek(file, 0, SEEK_SET); 
    
    uint8_t* buffer = (uint8_t*)malloc(size);
    if (!buffer) return 1;
    
    fread(buffer, 1, size, file);
    
    size_t cursor = 0;
    while (cursor < size) {
        // For now I'll just hexdump so I can cross verify with a WASM reader online.
        printf("%02X ", buffer[cursor]);
        if ((cursor + 1) % 16 == 0) printf("\n");
        cursor++;
    }
    printf("\n");
    
    free(buffer);    

    return 0;
}