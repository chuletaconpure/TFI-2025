#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lista2 {
    int Respuesta_Id;
    int Pregunta_Id;
    int Respuesta_Nro;
    char Respuesta[50];
    float Ponderacion;
    int Elegida;
    struct lista2 *sgte;
};

int nuevoL2(struct lista2 **nodo);
void insertarL2(struct lista2 **nuevo, struct lista2 **L);
void crearRespuesta(struct lista2 **L);
void cargar_respuestas_csv(struct lista2 *L, const char *RespuestasCSV);

int nuevoL2(struct lista2 **nodo) {
    *nodo = (struct lista2 *)malloc(sizeof(struct lista2));
    return (*nodo != NULL);
}

void insertarL2(struct lista2 **nuevo, struct lista2 **L) {
    if (*L == NULL) {
        *L = *nuevo;
    } else {
        struct lista2 *aux = *L;
        while (aux->sgte != NULL) {
            aux = aux->sgte;
        }
        aux->sgte = *nuevo;
    }
}

void crearRespuesta(struct lista2 **L) {
    struct lista2 *nueva = NULL;
    if (nuevoL2(&nueva)) {
        printf("Ingrese ID de la respuesta: ");
        scanf("%d", &nueva->Respuesta_Id);
        printf("Ingrese ID de la pregunta: ");
        scanf("%d", &nueva->Pregunta_Id);
        printf("Ingrese número de la respuesta (orden): ");
        scanf("%d", &nueva->Respuesta_Nro);
        printf("Ingrese el texto de la respuesta: ");
        scanf("%s", &nueva->Respuesta);
        printf("Ingrese la ponderación de la respuesta: ");
        scanf("%f", &nueva->Ponderacion);
        while(nueva->Ponderacion <= 0 && nueva->Ponderacion > 1){
            printf("(numero invalido) Ingrese la ponderación de la respuesta: ");
            scanf("%f", &nueva->Ponderacion);
        }
        printf("¿Es la opción elegida? (1=Sí, 0=No): ");
        scanf("%d", &nueva->Elegida);
        nueva->sgte = NULL;

        insertarL2(&nueva, L);
        printf("Respuesta creada con éxito.\n");
    } else {
        printf("Error al reservar memoria para la respuesta.\n");
    }
}

void cargar_respuestas_csv(struct lista2 *L, const char *RespuestasCSV) {
    struct lista2 *aux = NULL;

    FILE *archivo = fopen(RespuestasCSV, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    } else {
        fprintf(archivo, "Respuesta_Id;Pregunta_Id;Respuesta_Nro;Respuesta;Ponderacion;Elegida\n");
        aux = L;
        while (aux != NULL) {
            fprintf(archivo, "%d;%d;%d;\"%s\";%.2f;%d\n",
	        aux->Respuesta_Id,
	        aux->Pregunta_Id,
	        aux->Respuesta_Nro,
	        aux->Respuesta,
	        aux->Ponderacion,
	        aux->Elegida);
            aux = aux->sgte;
        }
    }

    fclose(archivo);
    printf("Respuestas cargadas en el archivo %s\n", RespuestasCSV);
}

// Función main con menú
int main() {
    struct lista2 *listaRespuestas = NULL;
    int opcion;

    do {
        printf("\n---- MENÚ ----\n");
        printf("1. Cargar una respuesta\n");
        printf("2. Guardar respuestas en CSV\n");
        printf("3. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        fflush(stdin);

        switch(opcion) {
            case 1:
                crearRespuesta(&listaRespuestas);
                break;
            case 2:
                cargar_respuestas_csv(listaRespuestas, "respuestas.csv");
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 3);

    return 0;
}
