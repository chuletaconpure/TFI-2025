#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estructura del Nodo renombrada ---
struct LES_encuestasCSV {
    int id_enc_respondida;
    int id_encuesta;
    int id_pregunta;
    int id_respuesta;
    char fecha_realizo_enc[9]; // YYYYMMDD + terminador nulo
    int encuestador_id;
    struct LES_encuestasCSV *siguiente; // Puntero al siguiente nodo del mismo tipo
};

// --- Funciones Auxiliares para la LES renombrada ---

// Crear un nuevo nodo
struct LES_encuestasCSV* crearNodoLES_encuestasCSV(int id_resp, int id_enc, int id_preg, int id_rpta, const char* fecha, int id_encdor) {
    struct LES_encuestasCSV* nuevoNodo = (struct LES_encuestasCSV*)malloc(sizeof(struct LES_encuestasCSV));
    if (nuevoNodo == NULL) {
        perror("Error al asignar memoria para el nodo LES_encuestasCSV");
        exit(EXIT_FAILURE);
    }
    nuevoNodo->id_enc_respondida = id_resp;
    nuevoNodo->id_encuesta = id_enc;
    nuevoNodo->id_pregunta = id_preg;
    nuevoNodo->id_respuesta = id_rpta;
    strncpy(nuevoNodo->fecha_realizo_enc, fecha, 8);
    nuevoNodo->fecha_realizo_enc[8] = '\0';
    nuevoNodo->encuestador_id = id_encdor;
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}

// Insertar un nodo al final de la LES
void insertarAlFinalLES_encuestasCSV(struct LES_encuestasCSV** cabezaRef, struct LES_encuestasCSV* nuevoNodo) {
    if (nuevoNodo == NULL) return;
    if (*cabezaRef == NULL) {
        *cabezaRef = nuevoNodo;
        return;
    }
    struct LES_encuestasCSV* ultimo = *cabezaRef;
    while (ultimo->siguiente != NULL) {
        ultimo = ultimo->siguiente;
    }
    ultimo->siguiente = nuevoNodo;
}

// Mostrar la LES (para verificación)
void mostrarLES_encuestasCSV(struct LES_encuestasCSV* cabeza) {
    struct LES_encuestasCSV* actual = cabeza;
    if (actual == NULL) {
        printf("La LES de encuestas CSV está vacía.\n");
        return;
    }
    printf("Contenido de la LES_encuestasCSV:\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| ID Enc Resp. | ID Encuesta | ID Pregunta | ID Respuesta | Fecha    | Encuestador |\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    while (actual != NULL) {
        printf("| %-12d | %-11d | %-11d | %-12d | %-8s | %-11d |\n",
               actual->id_enc_respondida,
               actual->id_encuesta,
               actual->id_pregunta,
               actual->id_respuesta,
               actual->fecha_realizo_enc,
               actual->encuestador_id);
        actual = actual->siguiente;
    }
    printf("--------------------------------------------------------------------------------------------------\n");
}

// Liberar la memoria de la LES
void liberarLES_encuestasCSV(struct LES_encuestasCSV** cabezaRef) {
    struct LES_encuestasCSV* actual = *cabezaRef;
    struct LES_encuestasCSV* siguiente;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    *cabezaRef = NULL;
}
// --- Fin de Funciones Auxiliares ---


// --- Función Principal para Cargar la LES usando sscanf ---
void cargar_LES_encuestasCSV_DesdeArchivo(const char* nombreArchivo, struct LES_encuestasCSV** cabezaRef) {
    FILE* archivo = fopen(nombreArchivo, "r");
    char linea[512]; // Buffer para leer cada línea

    if (archivo == NULL) {
        perror("Error al abrir el archivo CSV");
        return;
    }

    printf("Leyendo datos desde '%s' para LES_encuestasCSV (usando sscanf)...\n", nombreArchivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        linea[strcspn(linea, "\r")] = '\0';

        if (strlen(linea) == 0) {
            continue;
        }

        int id_encuesta_val, id_pregunta_val, id_respuesta_val, encuestador_id_val, id_enc_respondida_val;
        char fecha_realizo_enc_val[9];

        int itemsLeidos = sscanf(linea, "%d,%d,%d,%8[^,],%d,%d",
                                 &id_encuesta_val,
                                 &id_pregunta_val,
                                 &id_respuesta_val,
                                 fecha_realizo_enc_val,
                                 &encuestador_id_val,
                                 &id_enc_respondida_val);

        if (itemsLeidos == 6) {
            struct LES_encuestasCSV* nuevo = crearNodoLES_encuestasCSV(
                id_enc_respondida_val,
                id_encuesta_val,
                id_pregunta_val,
                id_respuesta_val,
                fecha_realizo_enc_val,
                encuestador_id_val
            );
            insertarAlFinalLES_encuestasCSV(cabezaRef, nuevo);
        } else {
            // printf("Línea mal formada o con datos insuficientes (items leídos: %d): %s\n", itemsLeidos, linea);
        }
    }

    fclose(archivo);
    printf("Carga de datos desde '%s' completada.\n", nombreArchivo);
}


// --- Función Principal (main) ---
int main() {
    struct LES_encuestasCSV* miListaEncuestasCSV = NULL; // Variable renombrada
    const char* nombreDelArchivoCSV = "datos.csv";

    printf("Creando archivo de ejemplo '%s'...\n", nombreDelArchivoCSV);
    FILE* archivoEjemplo = fopen(nombreDelArchivoCSV, "w");
    if (archivoEjemplo == NULL) {
        perror("No se pudo crear el archivo CSV de ejemplo");
        return EXIT_FAILURE;
    }
    fprintf(archivoEjemplo, "000010,00000015,000000000125,20230527,0002,000000000001\n");
    fprintf(archivoEjemplo, "000010,00000018,000000000222,20230528,0002,000000000002\n");
    fprintf(archivoEjemplo, "000011,00000015,000000000130,20230529,0003,000000000003\n");
    fprintf(archivoEjemplo, "12,1,5,20240115,99,4\n");
    fprintf(archivoEjemplo, "  \n");
    fprintf(archivoEjemplo, "000012,00000020,000000000300,20240201,0001,000000000005\n");
    fprintf(archivoEjemplo, "mal,formato,linea\n");
    fclose(archivoEjemplo);
    printf("Archivo de ejemplo '%s' creado.\n\n", nombreDelArchivoCSV);

    cargar_LES_encuestasCSV_DesdeArchivo(nombreDelArchivoCSV, &miListaEncuestasCSV);
    printf("\n");

    mostrarLES_encuestasCSV(miListaEncuestasCSV);
    printf("\n");

    liberarLES_encuestasCSV(&miListaEncuestasCSV);
    printf("Memoria de la LES_encuestasCSV liberada.\n");

    return 0;
}
