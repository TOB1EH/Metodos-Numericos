#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f (double x);

int main(int argc, char const *argv[])
{
    double x = 0.0;
    double y = 0.0;

    do
    {
        printf("\nIngrese el valor de x: ");
        scanf("%lf", &x);
        // y = f(x);
        printf("El valor de f(%.4f) es: %.4f\n", x, f(x));
    
    } while (1);
    
    
    return 0;
}

double f (double x)
{
    // return (sqrt(1 + pow(x, 2)));
    // return (x / (sqrt(1 + pow(x, 2))));
    // return (exp(sqrt(1 + x)) * log(1 + ( 2 * (x * x))));


    return (log(1.0 - (x / 6.5)));
}
