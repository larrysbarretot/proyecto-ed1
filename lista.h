// lista.h
# define CADENA 80
typedef char TD[CADENA];

typedef struct Nodo {
   TD cadena;
   int numeroCampo;
   struct Nodo *siguiente;
}NODO;

void crearLista(NODO **inicio, NODO **fin);
NODO *nuevoNodo(TD cadena, int campoN);
void insertarFinal(NODO **inicio, NODO **fin, TD cadena, int campoN);
char *buscarCampo(int i, NODO *inicio);
void mostrarLista(NODO *inicio);
void eliminaLista(NODO **inicio, NODO **fin);
