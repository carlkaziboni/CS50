#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//prototypes
bool only_letters(string key);
bool only_once(string key);
string cipher(string text, string key);

int main(int argc, string argv[])
{
    //checking no. of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //checking length of arguments
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 letters\n");
        return 1;
    }

    //checking letters only
    if (only_letters(argv[1]) == false)
    {
        printf("Key must only be letters\n");
        return 1;
    }

    //checking only one appearance
    if (only_once(argv[1]) == false)
    {
        printf("Each letter must only be used once\n");
        return 1;
    }

    //getting plaintext
    string plaintext = get_string("plaintext:  ");

    //getting ciphertext
    string ciphertext = cipher(plaintext, argv[1]);
    printf("ciphertext: %s\n", ciphertext);
}

//checking if only letters
bool only_letters(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        int temp = key[i];
        if ((temp >= 65 && temp < 91) || (temp >= 97 && temp < 123))
        {
            continue;
        }
        else
        {
            return false;
            break;
        }
    }
    return true;
}

//checking if letters appear once
bool only_once(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        key[i] = toupper(key[i]);
        for (int j = i + 1; j < (strlen(key) - i); j++)
        {
            key[j] = toupper(key[j]);
            if (key[i] == key[j])
            {
                return false;
                break;
            }
            else
            {
                continue;
            }
        }
    }
    return true;
}

//ciphering text
string cipher(string text, string key)
{
    for (int i = 0; i < strlen(text); i++)
    {
        int temp = text[i];
        //uppercase case
        if (temp >= 65 && temp < 91)
        {
            text[i] = toupper(key[temp - 65]);
        }
        //lowercase case
        else if (temp >= 97 && temp < 123)
        {
            text[i] = tolower(key[temp - 97]);
        }
        //non-alphabetic characters case
        else
        {
            continue;
        }
    }
    return text;
}