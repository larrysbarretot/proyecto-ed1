#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "pagina.h"
#include "ArbolB.h"
#include "ArchivosCSV.h"
#include <time.h>

int menu();
int submenu();
void operaciones(char *nombreArchivo, NODO *inicio, int opcion, int numeroDeCampos, Pagina **raiz, long int *direccionUltima);

int main()
{
   FILE *buscarArchivo;
   int eleccion, eleccionSubmenu, eleccionSubmenuArbolB, numeroDeCampos, *ptrNumeroDeCampos, indice=0, dosCampos; // el puntero sirve para calcular el número de campos.
   long int direccionUltima;
   char nombreArchivo[NOMBRE], nombreArchivoIndice[NOMBRE], *ptrArchivoIndice, campoClave[CLAVE], lineaEncontrada[LINEA], leer[LINEA], *ptrleer, *campo, fuente[NOMBRE], *ptrFuente;
   NODO *cabeceraInicio, *cabeceraFin; // Lista que guarda los nombres de los campos del CSV
   Pagina *raiz, *encontrado; // Árbol-B para realizar algunas verificaciones más rápido.
   tipoClave aBuscar;
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
               if(creaArchivoDeIndices(nombreArchivo, "", 1, numeroDeCampos, &raiz, 0, &direccionUltima)) // se genera un árbol-b para ayudar en las operaciones
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
         case 3:
            dosCampos = 1;
            tipoClave cl;
            ptrArchivoIndice = leerNombreDeArchivo(nombreArchivoIndice);
            if(verificaNombreDeArchivoCSV(ptrArchivoIndice))
            {
               if(verificaQueExistaArchivoCSV(ptrArchivoIndice))
               {
                  // Se debería crear una función para esto
                  buscarArchivo = fopen(ptrArchivoIndice, "r");
                  while(1)
                  {
                     fgets(leer, LINEA, buscarArchivo);
                     quitaSaltoDeLinea(leer);
                     ptrleer = strdup(leer);
                     if(feof(buscarArchivo)) break;
                     campo = strtok(ptrleer, DELIMITADOR);
                     while(campo)
                     {
                        if(dosCampos)
                        {
                           cl.direccionLogica = atol(campo);
                           printf("%s ", campo);
                           dosCampos = 0;
                        }
                        else
                        {
                           strcpy(cl.campo, campo);
                           dosCampos = 1;
                           printf("%s\n", campo);
                           insertar(&raiz, cl);
                        }
                        campo = strtok(NULL, DELIMITADOR);
                     }
                  }

                  // fin de la que debería ser una función
                  do
                  {
                     eleccionSubmenuArbolB = submenuArbolB();
                     if(eleccionSubmenuArbolB == 1)
                     {
                        puts("Ingrese el nombre del CAMPO CLAVE A BUSCAR.");
                        printf("-> ");
                        fflush(stdin);
                        gets(campoClave);
                        strcpy(aBuscar.campo, campoClave);
                        clock_t t_ini, t_fin;
                        double secs;
                        t_ini = clock(); // empieza a medir el tiempo
                        encontrado = buscar(raiz, aBuscar, &indice);
                        if(encontrado)
                        {
                           ptrFuente = leerNombreDeArchivo(fuente);
                           if(verificaNombreDeArchivoCSV(ptrFuente))
                           {
                              if(verificaQueExistaArchivoCSV(ptrFuente))
                              {
                                 if(buscarArchivo = fopen(ptrFuente, "r"))
                                 {
                                    fseek(buscarArchivo, encontrado->claves[indice].direccionLogica, SEEK_SET);
                                    fgets(lineaEncontrada, LINEA, buscarArchivo);
                                    printf("%s\n", lineaEncontrada);
                                    t_fin = clock(); // medicion del tiempo completa
                                    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
                                    printf("%.16g milisegundos\n", secs * 1000.0);
                                    puts("Registro encontrado...");
                                    fclose(buscarArchivo);
                                 }
                                 else
                                    puts("ERROR al leer archivo!!!");
                              }
                              else
                                 printf("%s, NO EXISTE!!!", ptrFuente);
                           }
                           else
                              puts("El archivo debe tener extension .csv");
                        }
                        else
                           puts("Registro NO ENCONTRADO!!!");
                     }
                  }while(eleccionSubmenuArbolB != 2);
               }
               else
                  printf("%s, NO EXISTE!!!", ptrArchivoIndice);

            }
            else
            {
               puts("El archivo debe tener extension .csv");
            }
            break;
      }
   }while(eleccion != 4);

   return 0;
}

int menu()
{
   int opcion;
   puts("\n     ARCHIVOS .CSV - APLICACION ARBOL-B     \n");
   puts("----------------------------------------------");
   puts("1) Seleccionar archivo CSV");
   puts("2) Crear archivo CSV");
   puts("----------------------------------------------");
   puts("3) Generar ARBOL-B a partir de un archivo .csv");
   puts("----------------------------------------------");
   puts("4) Salir");
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

int submenuArbolB()
{
   int opcion;
   puts("\nOPERACIONES CON EL ARBOL-B");
   puts("1) Buscar");
   puts("2) Salir");
   printf("Digite la opcion(numero) elegida -> ");
   scanf("%d", &opcion);
   return opcion;
}

void operaciones(char *nombreArchivo, NODO *inicio, int opcion, int numeroDeCampos, Pagina **raiz, long int *direccionUltima)
{
   int campoElegido;
   char nombreArchivoIndice[NOMBRE], *ptrNombreIndice;
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
         printf("\nPor que CAMPO desea crear el ARCHIVO DE INDICES?\n");
         campoElegido = mostrarCabeceras(inicio, numeroDeCampos);
         ptrNombreIndice = leerNombreDeArchivo(nombreArchivoIndice);
         if(verificaNombreDeArchivoCSV(ptrNombreIndice))
         {
            if(creaArchivoDeIndices(nombreArchivo, ptrNombreIndice, campoElegido, numeroDeCampos, raiz, 1, direccionUltima))
               printf("El archivo %s se ha creado correctamente...\n", nombreArchivoIndice);
            else
               puts("Se produjo algun ERROR en la creacion!!!");
         }
         else
            puts("El archivo debe tener extension .csv");
         break;
   }
}
