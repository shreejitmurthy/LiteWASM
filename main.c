#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define WASM_HEADER_SIZE 8
/* 
 * Every .wasm file begins with the same 8 codes:
 * 00 61 73 6D 01 00 00 00 
 */
const uint8_t valid_header[WASM_HEADER_SIZE] = { 0x00, 0x61, 0x73, 0x6D, 0x01, 0x00, 0x00, 0x00 };

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("None or excess arguments supplied. One path argument in required\n");
        return 1;
    }

    FILE* file = fopen(argv[0], "rb");
    int valid_wasm = 0;
    // We'll check the header to ensure the .wasm file is valid
    uint8_t header[WASM_HEADER_SIZE];
    size_t bytesRead = fread(header, 1, WASM_HEADER_SIZE, file);

    if (bytesRead < WASM_HEADER_SIZE) {
        printf("File is too small to be valid WASM.");
        valid_wasm = 0;
        return 1;
    }

    if (memcmp(header, valid_header, WASM_HEADER_SIZE) == 0) {
        valid_wasm = 1;
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


    return 0;
}
