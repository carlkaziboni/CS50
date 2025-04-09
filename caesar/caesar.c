#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

//functions
bool only_digits(string key);
char rotate(char ogchar, int keynum);


int main(int argc, string argv[])
{
    //checking no. of arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //checking if only digits
    if (only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //converting input from string to int
    int cypher = atoi(argv[1]);

    //get plaintext
    string plaintext = get_string("plaintext: ");

    //rotating string
    for (int i = 0; i < strlen(plaintext); i++)
    {
        plaintext[i] = rotate(plaintext[i], cypher);
    }

    //printing final string
    printf("ciphertext: %s\n", plaintext);

}


//check if only digits
bool only_digits(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        if (isdigit(key[i]) == 0)
        {
            return false;
            break;
        }
    }
    return true;
}

//rotate values
char rotate(char ogchar, int keynum)
{
    int temp = ogchar;
    if ((temp < 91 && temp > 64) || (temp > 96 && temp < 123))
    {
        if (temp < 91)
        {
            temp = temp - 65;
            temp = ((temp + keynum) % 26) + 65;
        }
        else
        {
            temp = temp - 97;
            temp = ((temp + keynum) % 26) + 97;
        }
    }
    char temp2 = temp;
    return temp2;
}