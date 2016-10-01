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
void populate_original_bmp(RGBTRIPLE original_bmp[], BIHEADER* bi_header, FILE* inptr, int padding);
void create_reduced(RGBTRIPLE original_bmp[], FILE* outptr, BIHEADER* bi_header, int new_padding, double factor);

int main(int argc, char* argv[])
{
    check_args(argc, argv);

    double factor = atof(argv[1]);
    FILE* inptr = fopen_safely(argv[2], "r");
    FILE* outptr = fopen_safely(argv[3], "w");

    BFHEADER bf_header;
    BIHEADER bi_header;
    fread(&bf_header, sizeof(BFHEADER), 1, inptr);
    fread(&bi_header, sizeof(BIHEADER), 1, inptr);

    confirm_infile_format(&bf_header, &bi_header);
    
    int padding = (4 - (bi_header.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    int new_width = (int) (floor(bi_header.biWidth * factor) || 1) * sizeof(RGBTRIPLE);
    int new_padding = (4 - new_width % 4) % 4;
    
    RGBTRIPLE original_bmp[bi_header.biSizeImage / 3];
    populate_original_bmp(original_bmp, &bi_header, inptr, padding);

    adj_headers(&bf_header, &bi_header, factor, padding, new_padding);
    
    fwrite(&bf_header, sizeof(BFHEADER), 1, outptr);
    fwrite(&bi_header, sizeof(BIHEADER), 1, outptr);

    if (factor < 1.0)
    {
        create_reduced(original_bmp, outptr, &bi_header, new_padding, factor);
    }
    else
    {
        //create_enlarged(original_bmp, outptr, &bi_header, new_padding);
    }

    fclose(inptr);
    fclose(outptr);

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
    bi_header->biWidth = (int) floor(bi_header->biWidth * factor) || 1;
    bi_header->biHeight = (int) floor(bi_header->biHeight * factor) || 1;
    bf_header->bfSize = 54 + (bi_header->biWidth * 3 + new_padding) * abs(bi_header->biHeight);
    bi_header->biSizeImage = bf_header->bfSize - 54;
}

/**
 * Populate the supplied array with the RGBTRIPLEs from the original image.
 */
void populate_original_bmp(RGBTRIPLE original_bmp[], BIHEADER* bi_header, FILE* inptr, int padding)
{
    for (int i = 0, biHeight = abs(bi_header->biHeight); i < biHeight; i++)
    {
        for (int j = 0, biWidth = bi_header->biWidth; j < biWidth; j++)
        {
            fread(&original_bmp[i * biWidth + j], sizeof(RGBTRIPLE), 1, inptr);
        }
        fseek(inptr, padding, SEEK_CUR);
    }
}

/**
 * Produce an outfile reduced by the factor.
 */
void create_reduced(RGBTRIPLE original_bmp[], FILE* outptr, BIHEADER* bi_header, int new_padding, double factor)
{
    int inverse_factor = (int) floor(1.0 / factor);
    for (int i = 0, biHeight = abs(bi_header->biHeight); i < biHeight; i++)
    {
        for (int j = 0; j < bi_header->biWidth; j++)
        {
            printf("new pixel position: %d, %d\n", i, j);
            printf("this pixel will be the average of:\n");
            for (int k = 0; k < inverse_factor; k++)
            {
                for (int l = 0; l < inverse_factor; l++)
                {
                    int row = i * inverse_factor + k;
                    int col = j * inverse_factor + l;
                    int pos = row * inverse_factor * bi_header->biWidth + col;
                    printf("old pixel position: %d, %d, %d", row, col, pos);
                    printf(" RGB %d ", original_bmp[pos].rgbtRed);
                    printf("%d ", original_bmp[pos].rgbtGreen);
                    printf("%d ", original_bmp[pos].rgbtBlue);
                    printf("\n");
                }
            }
        }
    }
}

// bug: when rounding up, we try to get too much information
