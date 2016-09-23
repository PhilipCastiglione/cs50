/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 * 
 * Now resizes by a factor of n! Also available in strawberry flavour!
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

void adj_headers(BITMAPFILEHEADER *bf, BITMAPINFOHEADER *bi, int factor, int padding, int new_padding);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4 || atoi(argv[1]) < 1 || atoi(argv[1]) > 100)
    {
        printf("Usage: ./resize n infile outfile\n");
        printf("Note n must be 1<=n<=100\n");
        return 1;
    }

    // remember factor and filenames
    int factor = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // determine original padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine new padding for scanlines
    int new_padding = (4 - (bi.biWidth * factor * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // adjust the headers to take on the properties of the new resized file
    adj_headers(&bf, &bi, factor, padding, new_padding);

    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight) / factor; i < biHeight; i++)
    {
        // then, for each scanline, factor times
        for (int j = 0; j < factor; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0, biWidth = bi.biWidth / factor; k < biWidth; k++)
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
                fseek(inptr, -(bi.biWidth * 3 / factor + padding), SEEK_CUR);
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
 * Takes in header info for a bmp, a scaling factor and precalculated padding
 * amount, and modifies the headers for a new resized file.
 */
void adj_headers(BITMAPFILEHEADER *bf, BITMAPINFOHEADER *bi, int factor, int padding, int new_padding)
{
    // calculate the biWidth of the file for the BITMAPINFOHEADER
    bi->biWidth *= factor;
    
    // calculate the biHeight of the file for the BITMAPINFOHEADER
    bi->biHeight *= factor;
    
    // calculate the bfSize of the file for the BITMAPFILEHEADER
    bf->bfSize = 54 + (bi->biWidth * 3 + new_padding) * abs(bi->biHeight);
    
    // calculate the biSizeImage of the file for the BITMAPINFOHEADER
    bi->biSizeImage = bf->bfSize - 54;
}