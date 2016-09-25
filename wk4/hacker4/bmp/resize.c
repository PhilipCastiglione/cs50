/**
 * resize.c
 *
 * Resizes BMPs by a float between 0.0 and 100.0.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"
#define BFHEADER BITMAPFILEHEADER
#define BIHEADER BITMAPINFOHEADER

void check_args(int argc, char* argv[]);
FILE* fopen_safely(char* path, char* mode);
void confirm_infile_format(BFHEADER* bf_header, BIHEADER* bi_header);
void adj_headers(BFHEADER* bf_header, BIHEADER* bi_header, double factor, int padding, int new_padding);

int main(int argc, char* argv[])
{
    check_args(argc, argv);

    double factor = atof(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    BFHEADER bf_header;
    BIHEADER bi_header;

    FILE* inptr = fopen_safely(infile, "r");
    FILE* outptr = fopen_safely(outfile, "w");

    fread(&bf_header, sizeof(BFHEADER), 1, inptr);
    fread(&bi_header, sizeof(BIHEADER), 1, inptr);

    confirm_infile_format(&bf_header, &bi_header);
    
    int padding = (4 - (bi_header.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    int new_width = (int) floor(bi_header.biWidth * factor) * sizeof(RGBTRIPLE);
    int new_padding = (4 - new_width % 4) % 4;
    
    adj_headers(&bf_header, &bi_header, factor, padding, new_padding);
    
    fwrite(&bf_header, sizeof(BFHEADER), 1, outptr);
    fwrite(&bi_header, sizeof(BIHEADER), 1, outptr);

    // TODO: FROM HERE; WE NOW NEED FLOATYTHINGS ROUNDED LOL
    // READ THE ORIGINAL BMP CONTENT INTO AN ARRAY
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi_header.biHeight) / factor; i < biHeight; i++)
    {
        // then, for each scanline, factor times
        for (int j = 0; j < factor; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0, biWidth = bi_header.biWidth / factor; k < biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile, factor times
                for (int l = 0; l < factor; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
    
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
    
            // then add the new_padding required
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // seek back to the infiles start of the scanline, except on the
            // last run through for that line
            if (j != factor - 1)
            {
                fseek(inptr, -(bi_header.biWidth * 3 / factor + padding), SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

/**
 * Confirm input/usage.
 */
void check_args(int argc, char* argv[])
{
    if (argc != 4 || atof(argv[1]) < 0.0 || atof(argv[1]) > 100.0)
    {
        printf("Usage: ./resize n infile outfile\n");
        printf("Note n must be 0.0<=n<=100.0\n");
        exit(1);
    }
}

/**
 * ensure infile is (likely) a 24-bit uncompressed BMP 4.0
 */
void confirm_infile_format(BFHEADER* bf_header, BIHEADER* bi_header)
{
    if (bf_header->bfType != 0x4d42 ||
        bf_header->bfOffBits != 54 ||
        bi_header->biSize != 40 || 
        bi_header->biBitCount != 24 ||
        bi_header->biCompression != 0)
    {
        fprintf(stderr, "Unsupported file format.\n");
        exit(1);
    }
}

/**
 * Opens a file with error handling.
 * Yeah I know this doesn't do all the closes and frees it should
 * but that's not the point of this exercise and I can't be bothered
 * doing that yet sry.
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
 * Takes in header info for a bmp, a scaling factor and precalculated padding
 * amount, and modifies the headers for a new resized file.
 */
void adj_headers(BFHEADER* bf_header, BIHEADER* bi_header, double factor, int padding, int new_padding)
{
    // calculate the biWidth of the file for the BIHEADER
    bi_header->biWidth = (int) floor(bi_header->biWidth * factor);
    
    // calculate the biHeight of the file for the BIHEADER
    bi_header->biHeight = (int) floor(bi_header->biHeight * factor);
    
    // calculate the bfSize of the file for the BFHEADER
    bf_header->bfSize = 54 + (bi_header->biWidth * 3 + new_padding) * abs(bi_header->biHeight);
    
    // calculate the biSizeImage of the file for the BIHEADER
    bi_header->biSizeImage = bf_header->bfSize - 54;
}