// ArbolB.c
#include <stdio.h>
#include <stdlib.h>
#include "pagina.h"
#include "ArbolB.h"

void crearArbolB(Pagina **raiz)
{
   *raiz = NULL;
}

Pagina* buscar(Pagina* actual, tipoClave cl, int* indice)
{
   if (actual == NULL)
   {
      return NULL;
   }
   else
   {
      int esta;
      esta = buscarNodo(actual,cl,indice);
      if (esta)
         return actual;
      else
         return buscar(actual -> ramas[*indice],cl,indice);
   }
}

void insertar(Pagina**raiz, tipoClave cl)
{
   int subeArriba;
   tipoClave mediana;
   Pagina *p,*nd;

   empujar(*raiz, cl, &subeArriba, &mediana, &nd);

   if (subeArriba)
   {
      p = (Pagina*) malloc(sizeof(Pagina));
      p -> cuenta = 1;
      p -> claves[1] = mediana;
      p -> ramas[0] = *raiz;
      p -> ramas[1] = nd;
      *raiz = p;
   }
}

int buscarNodo(Pagina* actual, tipoClave cl, int* k)
{
   int encontrado;

   //if (cl < actual->claves[1])
   if(strcmp(cl.campo, actual->claves[1].campo) < 0)
   {
      encontrado = 0;
      *k = 0;
   }
   else
   {     /* examinan las claves del nodo en orden descendente */
      *k = actual->cuenta;
      //while ((cl < actual->claves[*k]) && (*k > 1))
      while ((strcmp(cl.campo, actual->claves[*k].campo) < 0) && (*k > 1))
         (*k)--;
      //encontrado = (cl == actual->claves[*k]);
      encontrado = (strcmp(cl.campo, actual->claves[*k].campo) == 0);
   }
   return encontrado;
}

void empujar(Pagina* actual, tipoClave cl, int* subeArriba, tipoClave* mediana, Pagina** nuevo)
{
   int k;

   if (actual == NULL)
   {
      *subeArriba = 1;
      *mediana = cl;
      *nuevo = NULL;
   }
   else
   {
      int esta;
      esta = buscarNodo(actual,cl,&k);

      if (esta)
      {
         puts("\nClave duplicada");
         *subeArriba = 0;
         return;
      }

      empujar(actual->ramas[k], cl, subeArriba, mediana, nuevo);

      /* devuelve control; vuelve por el camino de búsqueda */

      if (*subeArriba)
      {
         if (nodoLLeno(actual))
            dividirNodo(actual, *mediana, *nuevo, k, mediana, nuevo);
         else
         {
            *subeArriba = 0;
            meterHoja(actual, *mediana, *nuevo, k);
         }
      }
   }
}

void meterHoja(Pagina* actual, tipoClave cl, Pagina* rd, int k)
{
   int i;
   /* desplaza a la derecha los elementos para hacer un hueco */
   for (i = actual->cuenta; i >= k+1; i-- )
   {
      actual -> claves[i+1] = actual -> claves[i];
      actual -> ramas[i+1] = actual -> ramas[i];
   }
   actual -> claves[k+1] = cl;
   actual -> ramas[i+1] = rd;
   actual -> cuenta++ ;
}

void dividirNodo( Pagina* actual, tipoClave cl, Pagina* rd, int k, tipoClave* mediana, Pagina** nuevo)
{
   int i, posMdna;

   posMdna = (k <= m/2)? m/2: m/2+1; /* posición de clave mediana */

   (*nuevo) = (Pagina*) malloc(sizeof(Pagina)); /* nuevo nodo */
   for (i = posMdna+1; i < m; i++)
   {
      /* Es desplazada la mitad derecha al nuevo nodo, la clave mediana se queda en el nodo origen */

      (*nuevo)-> claves[i-posMdna] = actual -> claves[i];
      (*nuevo)-> ramas[i-posMdna] = actual -> ramas[i];
   }
   (*nuevo)-> cuenta = (m-1) - posMdna;/* claves en el nuevo nodo */
   actual -> cuenta = posMdna;         /* claves en el nodo origen */

   /* Es insertada la clave y rama en el nodo que le corresponde */
   if (k <= m/2)
      meterHoja(actual,cl,rd,k);      /* inserta en nodo origen */
   else
      meterHoja(*nuevo,cl,rd,k-posMdna);

   /* se extrae clave mediana del nodo origen */

   *mediana = actual->claves[actual->cuenta];
   /* Rama0 del nuevo nodo es la rama de la mediana */
   (*nuevo) -> ramas[0] = actual->ramas[actual->cuenta];
   actual->cuenta--  ;     /* disminuye ya que se quita la mediana */
}

void eliminar(Pagina**raiz, tipoClave cl)
{
   int encontrado;

   eliminarRegistro(*raiz, cl, &encontrado);
   if (encontrado)
   {
      //printf("Clave %d eliminada\n",cl);
      printf("Clave %s eliminada\n", cl.campo);
      if ((*raiz) -> cuenta == 0)
      {
         /* La raiz está vacía, se libera el nodo y se establece la nueva raíz */
         Pagina* p = *raiz;
         *raiz = (*raiz) -> ramas[0];
         free(p);
      }
   }
   else
      puts("La clave no se encuentra en el arbol\n");
}

void eliminarRegistro(Pagina* actual, tipoClave cl, int* encontrado)
{
   int k;

   if (actual != NULL)
   {
      *encontrado = buscarNodo(actual, cl, &k);
      if (*encontrado)
      {
         if (actual->ramas[k-1] == NULL)		/* es un nodo hoja */
            quitar(actual, k);
         else
         {
            sucesor(actual, k);
            /* se elimina la clave sucesora en su nodo */
            eliminarRegistro(actual->ramas[k], actual->claves[k], encontrado);
         }
      }
      else
      {
         eliminarRegistro(actual->ramas[k], cl, encontrado);
      }
      /* Las llamadas recursivas devuelven control a este punto.
         Se comprueba el número de claves del nodo descendiente,
         desde el nodo actual en la ruta de búsqueda seguida.
      */

      if (actual->ramas[k] != NULL)
      if (actual->ramas[k]->cuenta < m/2)
      restablecer(actual,k);
   }
   else
	  *encontrado = 0;
}

void quitar(Pagina* actual, int k)
{
   int j;

   for (j = k+1; j <= actual->cuenta; j++)
   {
      actual -> claves[j-1] = actual -> claves[j];
      actual -> ramas[j-1] = actual -> ramas[j];
   }
   actual->cuenta--;
}

void sucesor(Pagina* actual, int k)
{
   Pagina* q;

   q = actual -> ramas[k];
   while (q -> ramas[0] != NULL)
      q = q -> ramas[0];
   /* q referencia al nodo hoja */
   actual -> claves[k] = q -> claves[1];
}

void restablecer(Pagina* actual, int k)
{
   /* actual tiene la dirección del nodo antecedente de
      actual -> ramas[k] que tiene menos claves que el mínimo
   */

   if (k > 0)		/* Tiene "hermano" izquierdo */
      if (actual->ramas[k-1]->cuenta > m/2)
         moverDrcha(actual, k);
      else
         combina(actual, k);

   else				/* Sólo tiene "hermano" derecho */
      if (actual->ramas[1]->cuenta > m/2)
         moverIzqda(actual, 1);
      else
         combina(actual, 1);
}

void moverDrcha(Pagina* actual, int k)
{
   int j;
   Pagina* nodoProblema = actual->ramas[k];
   Pagina* nodoIzqdo = actual->ramas[k-1];

   for (j = nodoProblema->cuenta; j >= 1; j--) /* deja "hueco" */
   {
      nodoProblema ->	claves[j+1] = nodoProblema ->	claves[j];
      nodoProblema ->	ramas[j+1] = nodoProblema -> ramas[j];
   }
   nodoProblema -> cuenta++;
   nodoProblema -> ramas[1] = nodoProblema -> ramas[0];
   /*	"baja" la clave 	del nodo padre */
   nodoProblema -> claves[1]= actual -> claves[k];
   /* "sube" la clave desde el hermano izquierdo al nodo padre, para	reemplazar la que antes bajó */

   actual->claves[k] = nodoIzqdo->claves[nodoIzqdo->cuenta];
   nodoProblema->ramas[0] = nodoIzqdo->ramas[nodoIzqdo->cuenta];
   nodoIzqdo -> cuenta-- ;
}

void moverIzqda(Pagina* actual, int k)
{
   int j;
   Pagina* nodoProblema = actual->ramas[k-1];
   Pagina* nodoDrcho = actual->ramas[k];

   nodoProblema -> cuenta++;
   nodoProblema->claves[nodoProblema->cuenta] = actual->claves[k];
   nodoProblema->ramas[nodoProblema->cuenta] = nodoDrcho->ramas[0];

   /* "sube" la clave desde el hermano derecho al nodo padre, para 	reemplazar la que antes bajó */

   actual->claves[k] = nodoDrcho -> claves[1];
   nodoDrcho -> ramas[1] = nodoDrcho -> ramas[0];
   nodoDrcho -> cuenta-- ;

   for (j = 1; j <= nodoDrcho->cuenta; j++)
   {
      nodoDrcho -> claves[j] = nodoDrcho ->	claves[j+1];
      nodoDrcho -> ramas[j] = nodoDrcho ->	ramas[j+1];
   }
}

void combina(Pagina* actual, int k)
{
   int j;
   Pagina *nodoIzqdo, *q;

   q = actual -> ramas[k];
   nodoIzqdo = actual -> ramas[k-1];
   /* "baja" clave mediana desde el nodo padre */
   nodoIzqdo->cuenta++;
   nodoIzqdo->claves[nodoIzqdo->cuenta] = actual->claves[k];
   nodoIzqdo->ramas[nodoIzqdo->cuenta]= q->ramas[0];
   /* "mueve" claves del nodo derecho */
   for (j = 1; j <= q->cuenta; j++)
   {
      nodoIzqdo->cuenta++;
      nodoIzqdo->claves[nodoIzqdo->cuenta] = q->claves[j];
      nodoIzqdo->ramas[nodoIzqdo->cuenta] = q->ramas[j];
	}

   /* Se ajustan claves y ramas del nodo padre debido a que antes descendió la clave k */
   for (j = k; j <= actual->cuenta-1; j++)
   {
      actual -> claves[j] = actual -> claves[j+1];
      actual -> ramas[j] = actual -> ramas[j+1];
	}
	actual->cuenta-- ;

   free(q);
}

void dibujaArbol(Pagina* actual, int h)
{
   int i, j;

   if(actual)
   {
      dibujaArbol(actual->ramas[actual->cuenta], h+1);
      for(i=actual->cuenta; i>=1; i--)
      {
         for(j=1; j<=h; j++)
            printf("    ");
               // desplazamiento que se estima para el valor relativo h
         //printf("%4d\n", actual->claves[i]);
         printf("%8s\n", actual->claves[i].campo);
         dibujaArbol(actual->ramas[i-1], h+1);
      }
   }
}
