#include <stdio.h>
#include <stdlib.h>

//bitmap header 54 byte, color table size 1024 byte
#define BMP_HEADER_SIZE 54
#define BMP_COLOR_TABLE_SIZE 1024
#define CUSTOM_IMG_SIZE 1024*1024

void imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, 
                unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf);

void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable,
                unsigned char *buf, int bitDepth);


int main()
{
    //test: hold the image width, hole height, hold bit depth, 
    //create an array to hold image header, color table, pixel data (3 arrays total)
    
    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
    unsigned char imgBuffer[CUSTOM_IMG_SIZE];

    const char imgName[] = "images/man.bmp";
    const char newImgName[] = "images/man_copy.bmp";

    imageReader(imgName, &imgWidth, &imgHeight, &imgBitDepth, &imgHeader[0], &imgColorTable[0], &imgBuffer[0]);
    imageWriter(newImgName, imgHeader, imgColorTable, imgBuffer, imgBitDepth);
    
    printf("Succes!\n");
    return 0;
}

//image reader: name of the image, variable to store the height, width, 
//store bit depth, image header, color table, pixel data
//manipulate these variables and write to the image

void imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth,
                unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf)
                {
                    int i;
                    //create file 
                    FILE *streamIn;
                    streamIn = fopen(imgName, "rb");
                    
                    //check whether the file exist
                    if(streamIn == (FILE *)0)
                    {
                        printf("Cannot open\n");
                    }

                    //read img header and store it 
                    for(i=0; i<54; i++)
                    {
                        _header[i] = getc(streamIn);
                    }

                    *_width = *(int *)&_header[18];
                    *_height = *(int *)&_header[22];
                    *_bitDepth = *(int *)&_header[28];

                    //pixel info
                    if(*_bitDepth <= 8)
                    {
                        fread(_colorTable, sizeof(unsigned char), 1024, streamIn);
                    }

                    fread(_buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, streamIn);

                    fclose(streamIn);
                }

//1. wrtie the name of new image, 2. array/pointer to contain image header
//3. pointer to an array containing color table 4. pointer to an array pixel data
//5. bit depth of the image
void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable,
                unsigned char *buf, int bitDepth)
                {
                    FILE *fo = fopen(imgName, "wb");
                    fwrite(header, sizeof(unsigned char), 54, fo);

                    //check bit depth value to check whether we need to write to a colortable
                    if(bitDepth <= 8)
                    {
                        fwrite(colorTable, sizeof(unsigned char), 1024, fo);
                    }
                    fwrite(buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, fo);
                    fclose(fo);
                }