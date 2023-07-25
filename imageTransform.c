#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define DEFAULT_PROBABILITY 0.33333
/*
P3
width height
colour_max
*/
double probability = DEFAULT_PROBABILITY;
typedef struct colour {
    int red;
    int green;
    int blue;
}colour;
colour *makeColour(int r, int g, int b){
    colour* a = malloc(sizeof(colour));
    a->red = r;
    a -> blue = b;
    a -> green = g;
    return a;
}
colour makeColor(int r, int g, int b){
    colour a;
    a.red = r;
    a.green = g;
    a.blue = b;
    return a;
}
colour xorColour(colour a,colour b){
    return makeColor(a.red^b.red,a.green^b.green,a.blue^b.blue);
}
colour randXorColour(colour original,colour mask){
    double randval = (double)rand() /RAND_MAX;
    if(randval <= probability){
        return xorColour(original,mask);
    }
    return original;
}
colour *xorColor(colour *a,colour *b){
    return makeColour(a->red^b->red,a->green^b->green,a->blue^b->blue);
}
void printColour(colour *c){
    printf("%d,%d,%d ",c->red,c->green,c->blue);
}
void writeColor(colour *c,FILE *file){
    fprintf(file,"%d %d %d ",c->red,c->green,c->blue);
}

//used for debugging
void printColor(colour c){
    printf("%d,%d,%d ",c.red,c.green,c.blue);
}
void writeColour(colour c,FILE *file){
    fprintf(file,"%d %d %d ",c.red,c.green,c.blue);
}
void replaceWhitespace(char* str) {
    int i, j;
    int length = strlen(str);
    int prevCharWasWhitespace = 0;

    for (i = j = 0; i < length; i++) {
        if (isspace((unsigned char)str[i])) {
            if (!prevCharWasWhitespace) {
                str[j++] = ' '; // Replace whitespace with a single space
                prevCharWasWhitespace = 1;
            }
        } else {
            str[j++] = str[i];
            prevCharWasWhitespace = 0;
        }
    }
    str[j] = '\0'; // Null-terminate the modified string
}
int main(int argc, char *argv[]){
    if (argc <5){
        printf("Usage: %s input_file.ppm mask output_file.ppm probability\n", argv[0]);
        return 1;
    }
    srand(time(0));

    colour mask = makeColor(0, 0, 0); // Default mask (all zeros)
    char* mask_arg = argv[2];
    if (strncmp(mask_arg, "0x", 2) == 0) {
        sscanf(mask_arg, "0x%x", &mask.red);
        sscanf(mask_arg, "0x%*x %x", &mask.green);
        sscanf(mask_arg, "0x%*x %*x %x", &mask.blue);
    }
    // Check if the mask value is in binary format (starts with "0b")
    else if (strncmp(mask_arg, "0b", 2) == 0) {
        sscanf(mask_arg, "0b%d", &mask.red);
        sscanf(mask_arg, "0b%*d %d", &mask.green);
        sscanf(mask_arg, "0b%*d %*d %d", &mask.blue);
    }
    // The mask value is in decimal format
    else {
        sscanf(mask_arg, "%d", &mask.red);
        sscanf(mask_arg, "%*d %d", &mask.green);
        sscanf(mask_arg, "%*d %*d %d", &mask.blue);
    }

    FILE *file = fopen(argv[1],"r");
    FILE *file2 = fopen(argv[3],"w");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    probability = atof(argv[4]);

    char headerBuffer[1024*1024];

    fgets(headerBuffer,sizeof(headerBuffer),file);
    if(strcmp(headerBuffer,"P3\n")!= 0){
        printf("file must be P3 ppm");
        return 1;
    }

    fgets(headerBuffer,sizeof(headerBuffer),file);
    int width, height;
    sscanf(headerBuffer,"%d %d", &width,&height);
    int max_color;
    fgets(headerBuffer,sizeof(headerBuffer),file);
    sscanf(headerBuffer,"%d", &max_color);

    colour **pixelRow = malloc(sizeof(colour)*width);
    int pixelbuffer[3];
    int x;
    fprintf(file2,"P3\n%d %d\n%d\n",width,height,max_color);
    const int BYTES_PER_ROW = 3*width;  //should be the length of the colours
    int colourRow[BYTES_PER_ROW];
    int y = 0;
    while(fgets(headerBuffer,sizeof(headerBuffer),file)){
        x = 0;
        replaceWhitespace(headerBuffer);
        if (headerBuffer[strlen(headerBuffer)-1] == '\n'){
            headerBuffer[strlen(headerBuffer)-1] = '\0';
        }
        char* token = strtok(headerBuffer," ");
        int count = 0;

        while(token != NULL){
            colourRow[y] = atoi(token);
            ++y;
            token = strtok(NULL," ");
            if(y>= BYTES_PER_ROW) {
                y = 0;
                for(int i = 0;i<BYTES_PER_ROW;i+=3){
                    writeColour(randXorColour(makeColor(colourRow[i],colourRow[i+1],colourRow[i+2]),mask),file2);
                }
                fprintf(file2,"\n");

            }
        }

    }
    fclose(file);
    fclose(file2);
    free(pixelRow);
}  