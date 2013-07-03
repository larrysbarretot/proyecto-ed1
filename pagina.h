// pagina.h
#define CAMPO 80
#define m 5

typedef struct csv
{
   long int direccionLogica;
   char campo[CAMPO];
}CSV;

//typedef int tipoClave;
typedef CSV tipoClave;

typedef struct pagina
{
   tipoClave claves[m];
   struct pagina *ramas[m]; //Vector de apuntadores
   int cuenta;
} Pagina;

int nodoLLeno(Pagina* actual);
int nodoSemiVacio(Pagina* actual);
void escribeNodo(Pagina* actual);
