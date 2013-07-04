#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "pagina.h"
#include "ArbolB.h"
#include "ArchivosCSV.h"
#include <time.h>

int seleccionarArchivoCSV(char *nombre)
{
   int crearlo = 0; // Si no existe el archivo, puede crearlo.
   nombre = leerNombreDeArchivo(nombre);
   if(verificaNombreDeArchivoCSV(nombre))
   {
      printf("El nombre tiene la extension correcta...\n");
      if(verificaQueExistaArchivoCSV(nombre))
         return 1;
      else
      {
         printf("%s, NO EXISTE!!!", nombre);
         puts("Desea crearlo?\n1)Si\n2)No");
         printf("-> ");
         scanf("%d", &crearlo);
         if(crearlo == 1)
         {
            if(crearArchivoCSV(nombre))
            {
               puts("Creado correctamente...");
               return 1;
            }
            else
            {
               puts("ERROR al crear el archivo!!!");
               return 0;
            }
         }
         return 0;
      }
   }
   else
   {
      puts("El archivo debe tener extension .csv");
      return 0;
   }
}

int crearArchivoCSV(char *nombre)
{
   FILE *nuevo;
   char campo[LINEA], *linea;
   int numeroDeCampos, i;
   NODO *cabeceraInicio, *cabeceraFin;
   crearLista(&cabeceraInicio, &cabeceraFin);
   if(nuevo = fopen(nombre, "w"))
   {
      do
      {
         printf("Cuantos campos desea ingresar? -> ");
         scanf("%d", &numeroDeCampos);
      }while(numeroDeCampos < 2); // debe tener un mínimo de dos campos.
                                  // CAMPO CLAVE y ESTADO
      for(i = 0; i < numeroDeCampos; i++)
      {
         fflush(stdin);
         printf("Ingrese el nombre del CAMPO %d\n", i+1);
         if(i == 0) // primer campo
         {
            printf("CAMPO %d (CAMPO CLAVE)-> ", i+1);
            gets(campo);
            insertarFinal(&cabeceraInicio, &cabeceraFin, campo, i+1);
         }
         else if(i == numeroDeCampos - 1) // Último campo
         {
            printf("Campo %d (CAMPO ELIMINACION LOGICA)-> ", i+1);
            gets(campo);
            insertarFinal(&cabeceraInicio, &cabeceraFin, campo, i+1);
         }
         else
         {
            printf("Campo %d -> ", i+1);
            gets(campo);
            insertarFinal(&cabeceraInicio, &cabeceraFin, campo, i+1);
         }
      }
      linea = concatenarCamposYDelimitador(campo, cabeceraInicio);
      fprintf(nuevo, "%s\n", linea);
      fclose(nuevo);
      return 1;
   }
   else
      return 0;
}

char *leerNombreDeArchivo(char *nombre)
{
   puts("Escriba el nombre del archivo.");
   printf("-> ");
   fflush(stdin);
   gets(nombre);
   return nombre;
}

int verificaNombreDeArchivoCSV(char *nombre)
{
   char copiaNombre[NOMBRE];
   strcpy(copiaNombre, nombre);
   if(strstr(copiaNombre, ".csv")) // nombre contiene a .csv
      return 1;
   else
      return 0;
}

int verificaQueExistaArchivoCSV(char *nombre)
{
   FILE *archivo;
   archivo = fopen(nombre, "r");
   if(archivo)
   {
      fclose(archivo);
      return 1;
   }
   else
      return 0;
}

void guardarCabecerasDelArchivoCSV(char *nombre, NODO **inicio, NODO **fin, int *numeroDeCampos)
{
   FILE *archivo;
   char leer[LINEA], *campo;
   int i;
   if(archivo = fopen(nombre, "r"))
   {
      fgets(leer,LINEA, archivo); // lee la primera línea. CABECERAS
      quitaSaltoDeLinea(leer); // Ya que si no, tomará como un campo más a \n
      i = 1;
      campo = strtok(leer, DELIMITADOR); // va obteniendo los campos, en este el primero.
      while(campo != NULL)
      {
         insertarFinal(inicio, fin, campo, i); // Se va insertando en la lista los nombres de los campos.
         campo = strtok(NULL, DELIMITADOR); // se obtiene los siguientes campos.
         i++;
      }
      *numeroDeCampos = i - 1;
      fclose(archivo);
   }
   else
      puts("Error al leer el archivo!!!");
}

void quitaSaltoDeLinea(char *linea)
{
   int i;
   for(i=0; linea[i] != '\0'; i++)
      if(linea[i] == '\n')
      {
         linea[i] = '\0';
         break;
      }
}

int adicionarRegistro(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz, long int *direccionUltima)
{
   FILE *archivo;
   tipoClave claveABuscar, claveAAdicionar; // para buscar e insertar en el árbol-b creado para ayudar en algunas operaciones.
   Pagina *encontrado; // guarda el resultado de la búsqueda en el árbol, si no está valdrá NULL.
   char nombreCampoClave[NOMBRE], clave[CLAVE], valor[NOMBRE], lineaAInsertar[LINEA], *ptrLineaAInsertar; // para legibilidad usamos para los demas campos la variable "valor".
   int j, indiceDePagina = 0;
   long int direccionLogica; // para guardar e insertar la direccionLogica de la nueva línea al árbol-b.
   NODO *valoresInicio, *valoresFin, *i; // Lista para almacenar los valores de los campos del CSV
   crearLista(&valoresInicio, &valoresFin);
   leerCampoClave(clave, nombreCampoClave, inicio, "insertar");
   strcpy(claveABuscar.campo, clave); // Se utiliza para buscar en el árbol-b
   encontrado = buscar(*raiz, claveABuscar, &indiceDePagina);
   if(!encontrado) // si no se encuentra se procede a insertar el nuevo registro tanto en el archivo como en el árbol-b
   {  // Usamos una lista para guardar los valores de los campos del CSV
      insertarFinal(&valoresInicio, &valoresFin, clave, 1); // Insertamos el primer campo sin preguntar,
                                                            // ya que el valor lo tenemos (clave)
      j = 2; // comenzamos a llenar los demás campos a partir del segundo.
      i = inicio->siguiente; // comenzamos con el segundo del Esta
      while(i) // Hasta acabar la lista de cabeceras
      {
         if (j == numeroDeCampos) // Si es el último campo se le inserta "1" sin preguntar.
         {
            insertarFinal(&valoresInicio, &valoresFin, "1", j); // CAMPO ESTADO por ser adicion EXISTE(1).
            break; // Salimos ya que no hay más campos.
         }
         printf("%s -> ", i->cadena); // Imprime el nombre del campo que va a recibir un valor.
         fflush(stdin);
         gets(valor); // leemos el valor del campo pedido.
         insertarFinal(&valoresInicio, &valoresFin, valor, j); // Se insertan los demás valores
         i = i->siguiente; // se avanza al siguiente campo
         j++; // campoi
      }
      ptrLineaAInsertar = concatenarCamposYDelimitador(lineaAInsertar, valoresInicio); // se obtine la cadena formato listo.
      if(archivo = fopen(nombreArchivoCSV, "a+")) // abrimos el archivo para agregar la línea anterior al archivo
      {
         fprintf(archivo, "%s\n", ptrLineaAInsertar);
         direccionLogica = *direccionUltima;
         claveAAdicionar.direccionLogica = direccionLogica; // copiamos la direccion obtenida al campo del tipoClave del árbol-b
         strcpy(claveAAdicionar.campo, clave); // copio clave al tipoClave
         *direccionUltima = *direccionUltima + strlen(ptrLineaAInsertar) + 2; // actualizo la dirección lógica de la próxima adición.
         insertar(raiz, claveAAdicionar); // agregar al árbol-b
         fclose(archivo);
         eliminaLista(&valoresInicio, &valoresFin); // Libero memoria al eliminar la lista.
         return 1;
      }
      else
      {
         puts("Error al abrir archivo!!!");
         return 0;
      }
   }
   else
   {
      printf("El %s (%s) ya se encuentra registrado!!!\n", nombreCampoClave, clave);
      return 0;
   }
}

void leerCampoClave(char *clave, char *nombreCampoClave, NODO *inicio, char *operacion)
{
   // Asumimos por cierto que la primera columna es del CAMPO CLAVE
   strcpy(nombreCampoClave, buscarCampo(1, inicio)); // Nombre del CAMPO CLAVE
   printf("Digite el CAMPO CLAVE a %s (%s)\n", operacion, nombreCampoClave);
   printf("%s -> ", nombreCampoClave);
   fflush(stdin);
   gets(clave); // CAMPO CLAVE a insertar
}

int editarRegistro(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz, long int *direccionUltima)
{
   FILE *archivo;
   char nombreCampoClave[NOMBRE], clave[CLAVE], linea[LINEA], actualizado[NOMBRE], *copiaLinea, *copiaLineaCabeceras;
   tipoClave claveABuscar, claveEditada; // Para el arbol-b
   int indiceDePagina = 0, campoAEditar;
   long int direccionLogica;
   Pagina *encontrado; // Acá se guarda la página(nodo) que contiene la clave buscada y su dirección Lógica ubicado por el índice de página.
   NODO *valoresInicio, *valoresFin, *editadoInicio, *editadoFin; // listas para almacenar los valores originales de la línea a editar.
   crearLista(&valoresInicio, &valoresFin); // lista fuente
   crearLista(&editadoInicio, &editadoFin); // lista editada. Copiada de la fuente, excepto el campo que se modifica.
   leerCampoClave(clave, nombreCampoClave, inicio, "editar");
   strcpy(claveABuscar.campo, clave); // copio en un tipo de dato adecuado para buscar en el árbol.
   encontrado = buscar(*raiz, claveABuscar, &indiceDePagina); // busco en el árbol-b la clave
   printf("Clave: %s\nDireccion Logica: %ld\n", encontrado->claves[indiceDePagina].campo, encontrado->claves[indiceDePagina].direccionLogica);
   if(encontrado) // Si existe, se edita
   {
      if(archivo = fopen(nombreArchivoCSV, "r"))
      {
         fgets(linea, LINEA, archivo); // guarda en linea los nombres de los campos.
         quitaSaltoDeLinea(linea);
         copiaLineaCabeceras = strdup(linea);
         imprimirLinea(linea, numeroDeCampos); // imprime los nombres de los campos
         fseek(archivo, encontrado->claves[indiceDePagina].direccionLogica, SEEK_SET);
         fgets(linea, LINEA, archivo); // guarda en linea los valores actuales de la línea a editar.
         quitaSaltoDeLinea(linea);
         copiaLinea = strdup(linea);
         imprimirLinea(copiaLinea, numeroDeCampos); // imprime los valores actuales de la línea a modificar
         guardarCamposDeLinea(linea, &valoresInicio, &valoresFin);
         fclose(archivo);
         printf("Que campo desea editar?\n");
         campoAEditar = mostrarCabeceras(inicio, numeroDeCampos);
         printf("Ingrese el nuevo valor de %s", devuelveCampo(campoAEditar, copiaLineaCabeceras));
         printf("-> ");
         fflush(stdin);
         gets(actualizado);
         cadenaEditada(campoAEditar, actualizado, valoresInicio, &editadoInicio, &editadoFin);
         copiaLinea = concatenarCamposYDelimitador(linea, editadoInicio);
      }
      else
         puts("Error al abrir archivo!!!");
      if(borraRegistro(nombreArchivoCSV, clave, encontrado, indiceDePagina, numeroDeCampos, raiz))
      {
         if(archivo = fopen(nombreArchivoCSV, "a")) // inserto
         {
            fprintf(archivo, "%s\n", linea); // se escribe la nueva línea al final del archivo
            fclose(archivo); // cerramos luego de operar.
            direccionLogica = *direccionUltima;
            strcpy(claveEditada.campo, clave); // copio la clave al campo adecuado de el tipo de datos que guarda el árbol-b
            claveEditada.direccionLogica = direccionLogica; // copio la dirección Lógica al campo adecuado de tipo de datos del árbol-b
            *direccionUltima = *direccionUltima + strlen(linea) + 2; // actualizo la dirección lógica de la próxima adición
            insertar(raiz, claveEditada); // inserta en el arbol-b
            return 1;
         }
         else
            return 0;
      }
      else
      {
         puts("Error al eliminar el registro!!!");
         return 0;
      }
   }
   else
   {
      printf("El %s (%s) NO EXISTE!!!\n", nombreCampoClave, clave);
      return 0;
   }
}

int buscarRegistro(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz)
{
   FILE *archivo;
   clock_t t_ini, t_fin;
   double secs;
   int campoi, numeroDeEncuentros = 0;
   char *campoElegido, campoABuscar[NOMBRE], linea[LINEA], *copiaLinea, *ptrToken, *copiaLinea2;
   printf("Por que campo buscar?\n");
   campoi = mostrarCabeceras(inicio, numeroDeCampos);
   campoElegido = buscarCampo(campoi, inicio);
   if(campoElegido)
   {
      printf("Ingrese que desea buscar en el campo %s\n", campoElegido);
      printf("-> ");
      fflush(stdin);
      gets(campoABuscar);
      if(archivo = fopen(nombreArchivoCSV, "r"))
      {
         t_ini = clock();
         while(1)
         {
            fgets(linea, 500, archivo);
            copiaLinea = strdup(linea);
            copiaLinea2 = strdup(linea);
            if(feof(archivo)) break;
            ptrToken = devuelveCampo(campoi, copiaLinea);
            if((strcmp(ptrToken, campoABuscar) == 0) && registroExiste(copiaLinea2, numeroDeCampos))
            {
               quitaSaltoDeLinea(linea); // quita el salto de lína puesto por fgets
               imprimirLinea(linea, numeroDeCampos);
               numeroDeEncuentros++;
            }
            else
               continue;
         }
         t_fin = clock();
      }
      else
      {
         puts("Error al abrir archivo!!!");
         return 0;
      }
      if(numeroDeEncuentros)
      {
         if(numeroDeEncuentros == 1)
         {
            printf("Encontrado %d vez\n", numeroDeEncuentros);
            secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
            printf("%.16g milisegundos\n", secs * 1000.0);
         }
         else
         {
            printf("Encontrado %d veces\n", numeroDeEncuentros);
         }
         return 1;
      }
      else
      {
         printf("El dato buscado NO SE ENCUENTRA en el archivo\n");
         return 0;
      }
   }
   else
   {
      printf("Eleccion Erronea!!!");
      return 0;
   }
}

int eliminarRegistroCSV(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz)
{
   char nombreCampoClave[NOMBRE], clave[CLAVE];
   tipoClave claveABuscar;
   int indiceDePagina = 0;
   Pagina *encontrado;
   leerCampoClave(clave, nombreCampoClave, inicio, "eliminar");
   strcpy(claveABuscar.campo, clave);
   encontrado = buscar(*raiz, claveABuscar, &indiceDePagina);
   if(encontrado) // verifica si el CAMPO CLAVE EXISTE
   {
      if(borraRegistro(nombreArchivoCSV, clave, encontrado, indiceDePagina, numeroDeCampos, raiz))
         return 1;
      else
         return 0;
   }
   else
   {
      printf("El %s (%s) NO EXISTE!!!\n", nombreCampoClave, clave);
      return 0;
   }
}

int borraRegistro(char *nombreArchivoCSV, char *clave, Pagina *encontrado, int indiceDePagina, int numeroDeCampos, Pagina **raiz)
{
   FILE *archivo;
   char lineaAEliminar[LINEA];
   long int direccionLogica;
   tipoClave claveAEliminar;
   if(archivo = fopen(nombreArchivoCSV, "r+"))
   {
      direccionLogica = encontrado->claves[indiceDePagina].direccionLogica;
      strcpy(claveAEliminar.campo, clave);
      fseek(archivo, direccionLogica, SEEK_SET);
      fgets(lineaAEliminar, LINEA, archivo);
      fseek(archivo, direccionLogica, SEEK_SET);
      fseek(archivo, (longitudHastaCampo(numeroDeCampos-1, lineaAEliminar, numeroDeCampos))*sizeof(char), SEEK_CUR);
      fprintf(archivo, "%c", '0');
      eliminar(raiz, claveAEliminar); // Elimina del árbol-b
      printf("eliminado...\n");
      fclose(archivo);
      return 1;
   }
   else
   {
      puts("Error al abrir archivo!!!");
      return 0;
   }
}

int longitudHastaCampo(int i, char *linea, int numeroDeCampos)
{
   int j = 0, campo = 0;
   while(linea[j] != '\0' && i <= numeroDeCampos && i > 0)
   {
      if(linea[j] == ELIMINARDEL)
      {
         campo++;
         if(campo == i) return j+1; // devuelve la longitud hasta después del ;
      }
      j++;
   }
   return j; // +1 ya que no cuenta la última coincidencia de ;
}

void mostrarRegistros(char *nombreArchivoCSV, int numeroDeCampos)
{
   FILE *archivo;
   char leer[LINEA], *copiaDeLeer;
   int filas = 0, continuar = 1;
   if(archivo = fopen(nombreArchivoCSV, "r"))
   {
      fgets(leer, LINEA, archivo); // Ignoramos esta línea pues son los nombres de los campos.
      quitaSaltoDeLinea(leer);
      imprimirLinea(leer, numeroDeCampos);
      while(1)
      {
         fgets(leer, LINEA, archivo); // Lee las líneas del archivo que contiene los valores de los campos.
         quitaSaltoDeLinea(leer); // le quitamos el \n (lo produce fgets()) para que no lo tome como un campo.
         copiaDeLeer = strdup(leer); // copia de leer para utilizarl la copia con strtok().
         if(feof(archivo)) break;
         if(registroExiste(leer, numeroDeCampos))
         {
            imprimirLinea(copiaDeLeer, numeroDeCampos);
            filas++;
            if(filas % 20 == 0) // pausa cada 20 líneas.
            {
               printf("Desea continuar?\n1)Si\n2)No\n-> ");
               scanf("%d", &continuar);
               if(continuar == 2) break;
               getchar();
            }
         }
         else
            continue;
      }
   }
   else
      puts("Error al abrir archivo!!!");
}

void imprimirLinea(char *linea, int numeroDeCampos)
{
   int i = 0;
   char *campo;
   campo = strtok(linea, DELIMITADOR); //Comienza la separación de cada línea en todos los campos
   while(campo != NULL)
   {
      if(i == numeroDeCampos-1) // para no imprimir el CAMPO ESTADO(Último campo).
      {
         printf("\n");
         return;
      }
      i++;
      printf("%-18s", campo); // %-1s para que se vea en dos líneas
      campo = strtok(NULL, DELIMITADOR);
   }
}

int creaArchivoDeIndices(char *nombreArchivoCSV, int campo, int numeroDeCampos, Pagina **raiz, int generar, long int *direccionUltima)
{
   FILE *archivo, *indices;
   char leer[LINEA], *copiaLinea, *copiaLinea2;
   long int direccionLinea;
   tipoClave claveAInsertar; // se utiliza para insertar en el Árbol-B
   if(archivo = fopen(nombreArchivoCSV, "r"))
   {
      fgets(leer,LINEA, archivo); // lee la primera línea. CABECERAS, omitimos los datos de esta línea
      while(1)
      {
         direccionLinea = ftell(archivo);
         fgets(leer, LINEA, archivo);
         copiaLinea = strdup(leer); // Ya que strtok modifica la línea que examina
         copiaLinea2 = strdup(leer);
         if(feof(archivo)) break; // Si se llega al fin de archivo, se sale.
         if(registroExiste(copiaLinea, numeroDeCampos))
         {
            if(generar != 1)
            {
               // Si existe agregar al arbol-B
               claveAInsertar.direccionLogica = direccionLinea;
               strcpy(claveAInsertar.campo, devuelveCampo(1, leer));
               insertar(raiz, claveAInsertar); // se inserta al árbol-b
               //printf("%s", leer);
            }
            if(generar == 1)
            {
               indices = fopen("codIndex.csv", "a");
               if(indices == NULL)
               {
                  indices = fopen("codIndex.csv", "w");
                  fclose(indices);
                  indices = fopen("codIndex.csv", "a");
               }
               fprintf(indices, "%ld;%s;\n", direccionLinea, devuelveCampo(1, copiaLinea2));
               fclose(indices);
            }
         }
      }
      *direccionUltima = direccionLinea;
      fclose(archivo);
      return 1;
   }
   else
      puts("Error al leer el archivo!!!");
   return 0;
}

int registroExiste(char *linea, int numeroDeCampos)
{
   if(!strcmp(devuelveCampo(numeroDeCampos, linea), "1")) // Si son iguales devuelve 0, por eso el !strcmp()
      return 1;
   else
      return 0;
}

char *devuelveCampo(int i, char *deEstalinea) // modifica la línea pasada
{
   char *campoi;
   int numeroDeCampo = 1;
   campoi = strtok(deEstalinea, DELIMITADOR);
   while((campoi != NULL) && (numeroDeCampo != i))
   {
      if(numeroDeCampo == i) break;
      campoi = strtok(NULL, DELIMITADOR);
      numeroDeCampo++;
   }
   return campoi;
}

int mostrarCabeceras(NODO *inicio, int numeroDeCampos) // Muestra los nombres de los campos para elegir por cual se desea operar.
{
   int opcion;
   NODO *i;
   i = inicio;
   while(i && (i->numeroCampo < numeroDeCampos))
   {
      printf("%d) %s\n", i->numeroCampo, i->cadena);
      i = i->siguiente;
   }
   printf("Digite la opcion(numero) elegida -> ");
   scanf("%d", &opcion);
   return opcion;
}

void cadenaEditada(int i, char *valor, NODO *inicioFuente, NODO **inicioDestino, NODO **finDestino)
{
   NODO *j;
   int numeroDeCampo = 1;
   j = inicioFuente;
   while(j)
   {
      if(j->numeroCampo == i)
      {
         insertarFinal(inicioDestino, finDestino, valor, i);
         numeroDeCampo++;
         j = j->siguiente;
      }
      insertarFinal(inicioDestino, finDestino, j->cadena, numeroDeCampo);
      numeroDeCampo++;
      j = j->siguiente;
   }
}

void guardarCamposDeLinea(char *linea, NODO **inicio, NODO **fin)
{
   int i;
   char *campo;
   i = 1;
   campo = strtok(linea, DELIMITADOR);
   while(campo != NULL)
   {
      insertarFinal(inicio, fin, campo, i); // llena las cabeceras a una lista
      campo = strtok(NULL, DELIMITADOR);
      i++;
   }
}

char *concatenarCamposYDelimitador(char *linea, NODO *inicio)
{
   char *concatenado;
   NODO *j;
   // limpia la cadena para concatenar los campos a insertar al archivo.
   memset(linea,'\0',strlen(linea));
   concatenado = linea;
   j = inicio;
   while(j != NULL)
   {
      concatenado = strcat(concatenado, j->cadena);
      linea = strcat(concatenado, DELIMITADOR);
      j = j->siguiente;
   }
   return concatenado;
}
