#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>   // Para tolower()
#include "aditamentos_ui.h"

void opcionMenu(char *opcion)
{
    printf("Ingrese una opción: ");
    scanf(" %c", opcion);
    *opcion = tolower(*opcion); // Convierte el caracter a minúscula
    while (getchar() != '\n'); // Limpia el buffer
}

void pausa()
{
    printf("\nPresione ENTER para continuar...");
    getchar();
}