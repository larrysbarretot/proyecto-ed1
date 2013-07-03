// ArbolB.h

void crearArbolB(Pagina **raiz);
Pagina *buscar(Pagina *actual, tipoClave cl, int *k);
void insertar(Pagina **raiz, tipoClave cl);
void eliminar(Pagina **raiz, tipoClave cl);

/*
 * funciones auxiliares
 */
int buscarNodo(Pagina* actual, tipoClave cl, int* k);
void empujar(Pagina* actual, tipoClave cl, int* subeArriba, tipoClave* mediana, Pagina** nuevo);
void meterHoja(Pagina* actual, tipoClave cl, Pagina* rd, int k);
void dividirNodo(Pagina* actual, tipoClave cl, Pagina* rd, int k, tipoClave* mediana, Pagina** nuevo);
void eliminarRegistro(Pagina* actual, tipoClave cl, int* encontrado);
void quitar(Pagina* actual, int k);
void sucesor(Pagina* actual, int k);
void restablecer(Pagina* actual, int k);
void moverDrcha(Pagina* actual, int k);
void moverIzqda(Pagina* actual, int k);
void combina(Pagina* actual, int k);

void dibujaArbol(Pagina* actual, int h);
