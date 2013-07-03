// pagina.c
#include <stdio.h>
#include "pagina.h"

int nodoLLeno(Pagina* actual)
{
   return (actual->cuenta == m-1);
}

int nodoSemiVacio(Pagina* actual)
{
   return (actual->cuenta < m/2);
}

void escribeNodo(Pagina* actual)
{
   int k;
   printf("\n Nodo: ");
   for(k=1; k<=actual->cuenta; k++)
      //printf(" %d ", actual->claves[k]);
      printf(" %s ", actual->claves[k].campo);
   printf("\n");
}
