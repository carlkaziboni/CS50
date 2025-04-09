#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

float sentences;

//functions
int count_letters(string words);
int count_words(string words);
int count_sentences(string words);

int main(void)
{
    //getting string
    string text = get_string("Text: ");

    //length of string
    float letters = count_letters(text);

    //sentences in string
    sentences = count_sentences(text);

    //words in string
    float words = count_words(text);


    //calculating index
    float l = (letters / words) * 100;
    float s = (sentences / words) * 100;
    int index = round(0.0588 * l - 0.296 * s - 15.8);

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}




//counting letters
int count_letters(string words)
{
    int length = strlen(words);
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        int temp = toupper(words[i]);
        if (temp >= 65 && temp <= 90)
        {
            count++;
        }
        else
        {
            continue;
        }
    }
    return count;
}

//counting words
int count_words(string words)
{
    int length = strlen(words);
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        int temp = words[i];
        //looking for full stops spaces question marks or exclamation marks
        if (temp == 32 || temp == 33 || temp == 46 || temp == 63)
        {
            count++;
        }
        else
        {
            continue;
        }
    }
    count = count - sentences + 1;

    return count;
}

//counting sentences
int count_sentences(string words)
{
    int length = strlen(words);
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        int temp = toupper(words[i]);
        //looking for questions marks periods and exclamation marks
        if (temp == 33 || temp == 46 || temp == 63)
        {
            count++;
        }
        else
        {
            continue;
        }
    }
    return count;
}
