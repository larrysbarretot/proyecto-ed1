#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "pagina.h"
#include "ArbolB.h"
#include "ArchivosCSV.h"

int menu();
int submenu();
void operaciones(char *nombreArchivo, NODO *inicio, int opcion, int numeroDeCampos, Pagina **raiz, long int *direccionUltima);

int main()
{
   int eleccion, eleccionSubmenu, numeroDeCampos, *ptrNumeroDeCampos; // el puntero sirve para calcular el número de campos.
   long int direccionUltima;
   char nombreArchivo[NOMBRE];
   NODO *cabeceraInicio, *cabeceraFin; // Lista que guarda los nombres de los campos del CSV
   Pagina *raiz; // Árbol-B para realizar algunas verificaciones más rápido.
   ptrNumeroDeCampos = &numeroDeCampos;
   crearLista(&cabeceraInicio, &cabeceraFin);
   crearArbolB(&raiz);
   do
   {
      eleccion = menu();

      switch(eleccion)
      {
         case 1:
            if(seleccionarArchivoCSV(nombreArchivo))
            {
               printf("Archivo correcto...\n");
               guardarCabecerasDelArchivoCSV(nombreArchivo, &cabeceraInicio, &cabeceraFin, ptrNumeroDeCampos);
               if(creaArchivoDeIndices(nombreArchivo, 1, numeroDeCampos, &raiz, 0, &direccionUltima)) // se genera un árbol-b para ayudar en las operaciones
               {
                  printf("Creado...\n");
                  //dibujaArbol(raiz, 0);
                  do
                  {
                     eleccionSubmenu = submenu();
                     operaciones(nombreArchivo, cabeceraInicio, eleccionSubmenu, numeroDeCampos, &raiz, &direccionUltima);
                  }while(eleccionSubmenu != 7);
               }
               else
               {
                  printf("Se produjo algun error al generar el archivo!!!\n");
               }
               eliminaLista(&cabeceraInicio, &cabeceraFin); // Se limpia la lista para una nueva selección.
            }
            break;
         case 2:
            strcpy(nombreArchivo,leerNombreDeArchivo(nombreArchivo));
            if(verificaNombreDeArchivoCSV(nombreArchivo))
            {
               if(crearArchivoCSV(nombreArchivo))
               {
                  puts("Creado correctamente...");
               }
               else
               {
                  puts("ERROR al crear el archivo!!!");
               }
            }
            else
            {
               puts("El archivo debe tener extension .csv");
            }
            break;
      }
   }while(eleccion != 3);

   return 0;
}

int menu()
{
   int opcion;
   puts("\nARCHIVOS .CSV\n");
   puts("1) Seleccionar archivo CSV");
   puts("2) Crear archivo CSV");
   puts("3) Salir");
   printf("Digite la opcion(numero) elegida -> ");
   scanf("%d", &opcion);
   return opcion;
}

int submenu()
{
   int opcion;
   puts("\nOPERACIONES CON EL ARCHIVO\n");
   puts("1) Adicionar");
   puts("2) Editar");
   puts("3) Buscar");
   puts("4) Eliminar");
   puts("5) Mostrar");
   puts("6) Crear archivo de Indices");
   puts("7) Volver a Menu");
   printf("Digite la opcion(numero) elegida -> ");
   scanf("%d", &opcion);
   return opcion;
}

void operaciones(char *nombreArchivo, NODO *inicio, int opcion, int numeroDeCampos, Pagina **raiz, long int *direccionUltima)
{
   switch(opcion)
   {
      case 1: // Adicionar
         if(adicionarRegistro(nombreArchivo, inicio, numeroDeCampos, raiz, direccionUltima))
         {
            puts("Registro Adicionado correctamente...");
            //dibujaArbol(*raiz, 0);
         }
         else
            puts("Error al adicionar el nuevo registro!!!");
         break;
      case 2: // Editar
         if(editarRegistro(nombreArchivo, inicio, numeroDeCampos, raiz, direccionUltima))
         {
            puts("Registro Editado correctamente...");
            //dibujaArbol(*raiz, 0);
         }
         else
            puts("Error al editar el registro!!!");
         break;
      case 3: // Buscar
         if(buscarRegistro(nombreArchivo, inicio, numeroDeCampos, raiz))
         {
            puts("Registro encontrado...");
         }
         else
            puts("Registro NO ENCONTRADO!!!");
         break;
      case 4: // Eliminar
         if(eliminarRegistroCSV(nombreArchivo, inicio, numeroDeCampos, raiz))
         {
            puts("Registro Eliminado correctamente...");
            //dibujaArbol(*raiz, 0);
         }
         else
            puts("Error al eliminar el registro!!!");
         break;
      case 5: // Mostrar
         mostrarRegistros(nombreArchivo, numeroDeCampos);
         break;
      case 6: // Crear archivo de índices
         creaArchivoDeIndices(nombreArchivo, 1, numeroDeCampos, raiz, 1, direccionUltima);
         break;
   }
}
