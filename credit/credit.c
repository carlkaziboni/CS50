#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //get the number
    long number = get_long("Number: ");

    //get the number of digits
    int numdig = 0;
    long cntnum = number;
    while (cntnum >= 1)
    {
        cntnum = cntnum / 10;
        numdig++;
    }

    //checksum part 1
    long evenchecknum = number;
    int evencheck;
    for (int i = 2; i <= numdig; i = i + 2)
    {
        long modop = pow(10, i);
        long modop2 = pow(10, i - 1);
        int temp = ((evenchecknum % modop) / modop2) * 2;
        //Digit greater than or equal to 10 case
        if (temp >= 10)
        {
            int temp1 = temp % 10;
            int temp2 = temp / 10;
            evencheck = evencheck + temp1 + temp2;
        }
        else
        {
            evencheck = evencheck + temp;
        }
    }

    //checksum part 2
    long oddchecknum = number;
    int oddcheck;
    for (int i = 1; i <= numdig; i = i + 2)
    {
        long modop = pow(10, i);
        long modop2 = pow(10, i - 1);
        int temp = ((oddchecknum % modop) / modop2);
        //Digit greater than or equal to 10 case
        if (temp >= 10)
        {
            int temp1 = temp % 10;
            int temp2 = temp / 10;
            oddcheck = oddcheck + temp1 + temp2;
        }
        else
        {
            oddcheck = oddcheck + temp;
        }
    }
    //checksum final
    int checksum = oddcheck + evencheck;
    printf("%d\n", checksum);

    //checking type of card
    //Amex
    if (checksum % 10 == 0 && numdig == 15)
    {
        if (number / 10000000000000 == 34 || number / 10000000000000 == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    //Visa
    else if (checksum % 10 == 0 && numdig == 13)
    {
        if (number / 1000000000000 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    //MASTERCARD
    else if (checksum % 10 == 0 && numdig == 16)
    {
        if (number / 100000000000000 == 51 || number / 100000000000000 == 52 || number / 100000000000000 == 53
            || number / 100000000000000 == 54 || number / 100000000000000 == 55)
        {
            printf("MASTERCARD\n");
        }
        //VISA
        else if (number / 1000000000000000 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}