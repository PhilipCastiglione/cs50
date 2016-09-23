/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

FILE* fopen_safely(char* path, char* mode);
int new_jpg(uint8_t *stream_ptr);

int main(int argc, char* argv[])
{
    // open raw data file for reading
    FILE* raw = fopen_safely("card.raw", "r");
    
    // set up vars for reading and writing
    uint8_t *read_ptr = malloc(512);
    int jpg_num = 0;
    char *jpg_name = malloc(sizeof(char) * 7);
    
    while (1)
    {
        // read in the next block
        fread(read_ptr, 512, 1, raw);
        
        // if we just hit the end of the file, exit the loop
        if (feof(raw) != 0)
        {
            break;
        }
        
        // if the start of the block is a new jpeg
        if (new_jpg(read_ptr))
        {
            // set the current jpg name and increment counter
            sprintf(jpg_name, "%i%i%i.jpg",
                    jpg_num / 100, jpg_num % 100 / 10, jpg_num % 10);
            jpg_num++;
            
            // open, write the first block to the file and close
            FILE* out = fopen_safely(jpg_name, "w");
            fwrite(read_ptr, 512, 1, out);
            fclose(out);
        }
        // else if there is a current jpeg
        else if (jpg_num > 0)
        {
            // open, append to the current jpeg file and close
            FILE* out = fopen_safely(jpg_name, "a");
            fwrite(read_ptr, 512, 1, out);
            fclose(out);
        }
    }
    
    // close our input file and free memory
    fclose(raw);
    free(read_ptr);
    free(jpg_name);
    
    return 0;
}

/**
 * Opens a file with error handling.
 */
FILE* fopen_safely(char* path, char* mode)
{
    FILE* stream = fopen(path, mode);
    if (stream == NULL)
    {
        printf("Could not open %s.\n", path);
        exit(1);
    }
    return stream;
}

/**
 * Checks if a stream begins with the start of a jpg
 */
int new_jpg(uint8_t *stream_ptr)
{
    // in order, 0xff, 0xd8, 0xff, 0xe0 - 0xef
    return stream_ptr[0] == 255 && 
           stream_ptr[1] == 216 && 
           stream_ptr[2] == 255 && 
           stream_ptr[3] >= 224 && 
           stream_ptr[3] <= 239;
}