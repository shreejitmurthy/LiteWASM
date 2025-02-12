//
// Created by Shreejit Murthy on 03/02/2025
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// NOTE: Using uint8_t *may* be insufficient in the future, if any problems arise from decoding, look here.
uint8_t decode_uleb128(uint8_t data) {
    uint8_t result = 0;
    unsigned int shift = 0;
    uint8_t byte;

    do {
        byte = data++;
        result |= (byte & 0x7F) << shift;
        shift += 7;
    } while (byte & 0x80);

    return result;
}



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
    if (argc < 2) {
        perror("No arguments supplied.\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file.\n");
        return 1;
    }

    // First check the header to ensure the .wasm file is valid
    uint8_t header[WASM_HEADER_SIZE];
    size_t bytesRead = fread(header, 1, WASM_HEADER_SIZE, file);

    if (bytesRead < WASM_HEADER_SIZE) {
        perror("File is too small to be valid WASM.\n");
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

    //  +----+------------+
    //  | Id |  Section   |
    //  +----+------------+
    //  |  0 | custom     |
    //  |  1 | type       |
    //  |  2 | import     |
    //  |  3 | function   |
    //  |  4 | table      |
    //  |  5 | memory     |
    //  |  6 | global     |
    //  |  7 | export     |
    //  |  8 | start      |
    //  |  9 | element    |
    //  | 10 | code       |
    //  | 11 | data       |
    //  | 12 | data count |
    //  +----+------------+
    
    size_t cursor = WASM_HEADER_SIZE;
    while (cursor < WASM_HEADER_SIZE + 1) {  // temp, normal: size
        uint8_t section_type = buffer[cursor++];
        uint8_t section_size;
        switch (section_type) {
            /* The typical structure of the type section is as such: 
             *     01 <section size> <function type count> <function type indicator> <param count> <param types> <return count> <return types>
             * For now, I have to assume this is always true. 
             * I actually *know* this is the case for my specific test case by cross verifying online.
             */
            // Type sections just for passing in arguments for the interpreter? In that case, allow multiple arguments instead of just 2.
            case 0x01:
                // Type section, read the next byte to determine section size, and thus how far we iterate through.
                section_size = decode_uleb128(buffer[cursor++]);
                break;

            default:
                break;
        }

        if (section_size > 0) {
            // Now we read through the section, for the length of the section size.
            printf("Section size: %d\n", section_size);
            uint8_t end_cursor = cursor + section_size;
            while (cursor < end_cursor && cursor < size) {
                printf("%02X ", buffer[cursor]);
                cursor++;
            }
            printf("\n");
        }

        // cursor++;
    }
    
    free(buffer);    

    return 0;
}