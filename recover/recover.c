#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//defining type
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //check arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //open file
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("%s cannot be opened for reading\n", argv[1]);
        return 1;
    }

    //temporary store
    BYTE buffer[512];

    //filename
    char *filename = malloc(8);

    int count = 0;

    //reading and writiing
    while (fread(buffer, 1, 512, fp) == 512)
    {

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff & (buffer[3] & 0xf0) == 0xe0)
        {
            //open new file
            sprintf(filename, "%03i.jpg", count);
            FILE *photo = fopen(filename, "w");

            //write into new file from buffer
            fwrite(buffer, 1, 512, photo);

            //copy rest of jpg into new file
            while (fread(buffer, 1, 512, fp) == 512)
            {
                if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff & (buffer[3] & 0xf0) == 0xe0)
                {
                    count++;
                    fseek(fp, -512, SEEK_CUR);
                    break;
                }
                else
                {
                    fwrite(buffer, 1, 512, photo);
                }
            }
            fclose(photo);
        }
    }
    fclose(fp);
    free(filename);
}