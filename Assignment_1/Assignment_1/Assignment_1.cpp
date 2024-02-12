/*
a. Determine the first (and only) 8 natural numbers (x1, x2,  …,  x8) greater than 2 with the following property:
    all the natural numbers smaller than xi and that are relatively prime with xi (except for the number 1) are prime, i =1,2, …, n.
b. Given a vector of numbers, find the longest contiguous subsequence such that any consecutive elements contain the same digits.
*/

#include <stdio.h>
#pragma warning(disable : 4996)

int prime(int x)
{
    // verifies if a number is prime
    if (x < 2)
        return 0;
    if (x % 2 == 0 && x != 2)
        return 0;
    for (int d = 3; d * d <= x; d = d + 2)
        if (x % d == 0)
            return 0;
    return 1;
}
void test_prim()
{
    //tests if a number is prime
    int a, ok;
    printf("Insert a number to verify if its prime > ");
    scanf("%d", &a);
    ok = prime(a);
    if (ok == 1)
        printf("Number is prime");
    else
        printf("Number is not prime");
}

int prime_between_themselves(int x, int y)
{
    // returns the greatest common divizor between 2 numbers
    int gdc = 0;
    for (int i = 1; i <= x && i <= y; i++)
    {
        if (x % i == 0 && y % i == 0)
            gdc = i;
    }
    if (gdc == 1)
        return 1;
    return gdc;
}


void test_prime()
{
    //tests if 2 numbers are prime between themselves
    int ok, a , b;
    printf("Insert 2 numbers to verify their greatest common divizor > ");
    scanf("%d", &a);
    scanf("%d", &b);

    ok = prime_between_themselves(a,b);
    printf("The greatest common divizor between %d and %d is : %d \n", a, b, ok);

}

void functionalitya()
{
    int v[8] = { 0 };
    int cnt = 0,ok ; 
    for (int i = 0; i <= 7; i++) 
    {
        for (int j = 3; j <= 100; j++)
        {
            ok = 1;
            for (int k = 2; k <= j; k++) 
            {
                
                if (prime_between_themselves(k, j) == 1)
                {
                    if (prime(k) == 0)
                    {
                        ok = 0;
                    }
                        
                }
            }
            if (ok == 1)
            {
                v[i] = j;
                cnt++;
                printf("%d \n", j);
                if (cnt == 8)
                    return;
            }
                

        }
    }
}
void functionalityaimproved(int v[])
{
    // function finds the first 8 numbers >=2 that have all the numbers below itself and prime relative to itself , also be prime
    //int v[8] = { 0 };
    int cnt = 0, ok;
    for (int j = 3; j <= 100; j++)
    {
        ok = 1;
        for (int k = 2; k <= j; k++) 
        {

            if (prime_between_themselves(k, j) == 1)
            {
                if (prime(k) == 0)
                {
                    ok = 0;
                }

            }
        }
        if (ok == 1)
        {
            v[cnt] = j;
            cnt++;
            /*printf("%d \n", j);*/
            if (cnt == 8)
                return;
        }


    }
}


int check_digits(int x, int y)
{
    //function return 1 if the numbers contain same digits , 0 otherwise
    int v[10] = {0}, w[10] = { 0 },cx,cy;
    while (x > 0)
    {
        cx = x % 10;
        v[cx] = 1;
        x = x / 10;
    }

    while (y > 0)
    {
        cy = y % 10;
        w[cy] = 1;
        y = y / 10;
    }
    for (int i = 0; i < 10; i++)
        if (v[i] != w[i])
            return 0;
    return 1;
}
void test_check()
{
    int a, b, ok;
    printf("Enter the numbers > ");
    scanf("%d", &a);
    scanf("%d", &b);
    ok = check_digits(a, b);
    if (ok == 1)
        printf("Numbers contain same digits");
    else
        printf("Numbers DONT contain same digits");
}
void functionalityb()
{
    //returns longest contigious subsequence where numbers contain same digits
    int v[12] = {0}, cnt=1 , maxcnt=0,end=0 ;
    for (int i = 0; i < 12; i++) 
    {
        printf("Enter number :");
        scanf("%d", &v[i]);
    }

    for (int i = 0; i < 11; i++)
    {
        if (check_digits(v[i], v[i + 1]) == 1)
        {
            cnt++;
            if (cnt > maxcnt)
            {
                maxcnt = cnt;
                end = i + 1;

            }

        }
        else
            cnt = 1;
    }
    printf("Longest contigious subsequence has lenght %d\n", maxcnt);
    printf("They are > \n");
    for (int j = end - maxcnt + 1; j <= end; j++)
        printf("%d \n", v[j]);
    


}

void functionalitybimproved(int v[], int* maxcnt, int* end)
{
    //returnes the lenght and end position of longest contigious subsequence with same elements
    
    int cnt = 1;
    int x=0, y=0;
    for (int i = 0; i < 11; i++)
    {
        if (check_digits(v[i], v[i + 1]) == 1)
        {
            cnt++;
            if (cnt > x)
            {
                x = cnt;
                y = i + 1;

            }

        }
        else
            cnt = 1;
    }

    *maxcnt = x;
    *end = y;
}

void print()
{
    //prints menu and enters choice opted
    int opt;
    do
    {
        printf("    MENU \n");
        printf("1. First functionality  \n");
        printf("2. Second functionality \n");
        printf("3. Exit program \n");

        printf("Insert option > ");
        scanf("%d", &opt);

        if (opt == 1)
        {
            int v[12], n;
            printf("Enter how many numbers to input");
            scanf("%d", &n);
            for (int i = 0; i < 8; i++)
                v[i] = 0;
            functionalityaimproved(v); 
            for (int i = 0; i < 8; i++)
            {
                printf("%d\n", v[i]);
            }
        }  
        else if (opt == 2)
        {
            int v[12], maxcnt , end = 0;
            for (int i = 0; i < 12; i++)
                v[i] = 0;
            for (int i = 0; i < 12; i++)
            {
                printf("Enter number :");
                scanf("%d", &v[i]);
            }
            
            functionalitybimproved(v,&maxcnt,&end);
            printf("Longest contigious subsequence has lenght %d\n", maxcnt);
            printf("They are > \n");

            for (int j = end - maxcnt + 1; j <= end; j++)
            {   
                printf("%d \n", *(v+j));
            }
                
                
        }
            

    }while (opt != 3);
}



int main()
{
    
    //test_prim();
    //test_prime();
    //test_check();
    
    print();
    return 0;
}
