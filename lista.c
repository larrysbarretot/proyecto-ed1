#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

void crearLista(NODO **inicio, NODO **fin)
{
   *inicio = *fin = NULL;
}

NODO *nuevoNodo(TD cadena, int campoN)
{
   NODO *nuevo;
   nuevo = (NODO*)malloc(sizeof(NODO));
   strcpy(nuevo->cadena, cadena);
   nuevo->numeroCampo = campoN;
   nuevo->siguiente = NULL;
   return nuevo;
}

void insertarFinal(NODO **inicio, NODO **fin, TD cadena, int campoN)
{
   NODO *nuevo;

   if(*fin) {
      nuevo = nuevoNodo(cadena, campoN);
      (*fin)->siguiente = nuevo;
      *fin = nuevo;
   } else {
      // inserta el primer nodo
      nuevo = nuevoNodo(cadena, campoN);
      *inicio = nuevo;
      *fin = nuevo;
   }
}

char *buscarCampo(int i, NODO *inicio)
{
   NODO *temporal;
   temporal = inicio;
   while(temporal != NULL) {
      if(temporal->numeroCampo == i)
         return temporal->cadena;
      temporal = temporal->siguiente;
   }
   return NULL;
}

void mostrarLista(NODO *inicio)
{
   NODO *temporal;
   temporal = inicio;
   while(temporal != NULL) {
      printf("%s ", temporal->cadena);
      temporal = temporal->siguiente;
   }
}

void eliminaLista(NODO **inicio, NODO **fin)
{
   NODO *i, *j;
   i = *inicio;
   while(i != NULL)
   {
      j = i->siguiente;
      free(i);
      i = j;
   }
   *inicio = NULL;
   *fin = NULL;
}
