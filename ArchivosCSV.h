// ArchivosCSV.h

#define NOMBRE 30
#define CLAVE 10
#define LINEA 500
#define DELIMITADOR ";"
#define ELIMINARDEL ';'

/************************************* Utilizadas en el Menu *************************************/
// Selecciona un archivo con la extensión adecuada(.csv).
// El archivo debe cumplir algunos *prerequisitos.
// - Los valores de la primera columna son únicos, será el CAMPO CLAVE.
// - Debe contener como valores del último campo(CAMPO ESTADO, creado por el usuario)
//   1: EXISTE ó 0: NO EXISTE que se utilizarán en operaciones posteriores.
int seleccionarArchivoCSV(char *nombre);

// Crea un archivo .csv con la cantidad de campos y nombres de estos pedidos al usuario.
int crearArchivoCSV(char *nombre);

// funciones auxiliares para seleccionarArchivoCSV() y crearArchivoCSV()
char *leerNombreDeArchivo(char *nombre);
int verificaNombreDeArchivoCSV(char *nombre); // verifica que sea un .csv
int verificaQueExistaArchivoCSV(char *nombre); // verifica que exista el archivo a usar
void guardarCabecerasDelArchivoCSV(char *nombre, NODO **inicio, NODO **fin, int *numeroDeCampos); // Guarda en una lista los nombres de los campos del archivo.
void quitaSaltoDeLinea(char *linea);
// ...talvez utilizar con un árbol-b de índices.
void guardaCamposClaveDelCSV(); // Guarda en un Árbol-B los valores del CAMPO CLAVE
                                // del archivo seleccionado para eficiencia en saber si existe.
// funciones auxiliares para crearArchivoCSV()

// funciones auxiliares para seleccionarArchivoCSV()

/************************************ Utilizadas en el Submenu ************************************/
// Operaciones a realizar con el archivo seleccionado

int adicionarRegistro(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz, long int *direccionUltima);
int editarRegistro(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz, long int *direccionUltima);
int buscarRegistro(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz);
int eliminarRegistroCSV(char *nombreArchivoCSV, NODO *inicio, int numeroDeCampos, Pagina **raiz);
void mostrarRegistros(char *nombreArchivoCSV, int numeroDeCampos);
int creaArchivoDeIndices(char *nombreArchivoCSV, char *nombreArchivoIndice, int campo, int numeroDeCampos, Pagina **raiz, int generar, long int *direccionUltima);

// funciones auxiliares para: - adicionarRegistro()
                           // - editarRegistro()
                           // - buscarRegistro()
                           // - eliminarRegistro()
void leerCampoClave(char *clave, char *nombreCampoClave, NODO *inicio, char *operacion);

// Busca en el último campo(ELIMINACIÓN LÓGICA) de la línea leída.
int registroExiste(char *linea, int numeroDeCampos); // 1: EXISTE 0: NO EXISTE
char *devuelveCampo(int i, char *deEstalinea);
int mostrarCabeceras(NODO *inicio, int numeroDeCampos); // Muestra el valor de cada campo para
                                                        // hacer el ingreso de un nuevo registro,
                                                        // buscar o editar un registro.
void cadenaEditada(int i, char *valor, NODO *inicioFuente, NODO **inicioDestino, NODO **finDestino);

// funciones auxiliares para adicionarRegistro()
void guardarCamposDeLinea(char *linea, NODO **inicio, NODO **fin); // Guarda en una lista auxiliar los valores leídos
char *concatenarCamposYDelimitador(char *linea, NODO *inicio); // Devuelve la cadena lista(con formato)
                                      // para adicionarla en el archivo.
// funciones auxiliares para eliminarRegistro()
int borraRegistro(char *nombreArchivoCSV, char *clave, Pagina *encontrado, int indiceDePagina, int numeroDeCampos, Pagina **raiz);
int longitudHastaCampo(int i, char *linea, int numeroDeCampos);
// funciones auxiliares para mostrarRegistros()
void imprimirLinea(char *linea, int numeroDeCampos);
