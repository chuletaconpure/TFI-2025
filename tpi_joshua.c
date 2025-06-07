#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <time.h>

/* consignas 1: 
a. Calcular ponderación de una encuesta X: El sistema deberá pedir el ingreso de
    un id de encuesta por teclado y el sistema deberá calcular la ponderación total
    de la encuesta, así como su conformación, o sea la ponderación obtenida en
    cada pregunta.
b. Mostrar encuestas respondidas una a una. Encuesta completa (todas las
    preguntas, cada una con todas las respuestas y diferenciar de alguna manera
    la respondida. 


consignas 2: 
El sistema deberá permitir la impresión de un listado de todas las encuestas 
respondidas de una determinada encuesta en particular, las mismas deberán estar ordenadas por id de encuesta respondida.
IMPORTANTE: Se deberá cargar un árbol AB (árbol binario) de forma ordenada y a posterior imprimir haciendo el recorrido correspondiente.

audios de quique:
la suma de la ponderacion de las preguntas debe ser 1, al menos 1 respuesta debe ser 1
al no cumplirse esta condicion se da de baja la pregunta, la baja de de una pregunta da de baja todas las preguntas y respuestas
de la encuesta, es decir, la encuesta queda en blanco. luego se manda al usuario a cargar toda la encuesta de nuevo a mano.
si decide borrar una respuestase borran todas las respuestas de esa pregunta y se lo manda a cargar respuestas.
tambien hay una opcion de marcar una encuesta de alguna forma como invalida en lugar de borrar todos sus datos (no me gusta esa forma tt: joshua)
*/


//valores hexagecimales que representan colores usados por windows.h
#define FOREGROUND_BLACK       0x0000
#define FOREGROUND_BLUE        0x0001
#define FOREGROUND_GREEN       0x0002
#define FOREGROUND_RED         0x0004
#define FOREGROUND_WHITE       0x0007
#define BACKGROUND_BLACK       0x0000
#define BACKGROUND_BLUE        0x0010
#define BACKGROUND_GREEN       0x0020
#define BACKGROUND_RED         0x0040
#define BACKGROUND_WHITE       0x0070

//archivos
FILE *arch_respuestas;

//pila y sus funciones, pila de encuestas
struct pEncuesta{
	int Encuesta_id;
    char Denominacion[50];
    int Encuesta_Mes;
    int Anio;
    int Procesada;
	struct pEncuesta *sgte;
};

void apilar(struct pEncuesta **, struct pEncuesta **);
void desapilar(struct pEncuesta **, struct pEncuesta **);
int vaciaP(struct pEncuesta *);
int nuevoP(struct pEncuesta **nuevo);
void mostrarP(struct pEncuesta **tope);

//lista1 y sus funciones, lista de preguntas
struct lPregunta{
    int Encuesta_Id;
    int Pregunta_Id;
    char Pregunta[100];
    float Ponderacion;
    struct lPregunta *sgte;
};

int nuevoLPregunta(struct lPregunta **nodo);
struct lPregunta *insertarLPregunta(struct lPregunta **nodo, struct lPregunta **L);
struct lPregunta *mostrarListaPregunta(struct lPregunta **L);


//lista2 y sus funciones, lista de respuestas
struct lRespuesta{
    int Respuesta_Id;
    int Pregunta_Id;
    int Respuesta_Nro;
    char Respuesta [50];
    float Ponderacion;
    int Elegida;
    struct lRespuesta *sgte;
};
int nuevoLRespuesta(struct lRespuesta **nodo);
struct lRespuesta *insertarLRespuesta(struct lRespuesta **nodo, struct lRespuesta **L);
struct lRespuesta *mostrarListaRespuesta(struct lRespuesta **L);

//funciones arbol
struct arbol *insertarA(struct arbol **A, struct arbol **nodo);
void mostrarA(struct arbol **A);
int nuevoA(struct arbol **nodo);
void buscarA(int dato, struct arbol *r);
void busBorrA(struct arbol **r, int dato);
void borrarNodoA(struct arbol **nodo);
struct arbol *eliminarA(struct arbol **r);

//funciones esteticas
void colorMenu(){ 
    //esta funcion y similares utiliza comandos de consola de windows para modificar colores, en este caso especifico las letras se mantienen negras
    //mientras que mesclando verde, azul y blanco se forma el color del fondo
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLACK | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
}
void letraR(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
}
void letraB(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
}
void letraA(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
}

//CRUD Preguntas
void crearPregunta(struct lPregunta **L);
void modificarPregunta(struct lPregunta *L, int preguntaId);
int buscarUltimoIdPregunta(struct lPregunta *L);
//CRUD respuestas
void crearRespuesta(struct lRespuesta **L);
void mostrarRespuestasPorPregunta(struct lRespuesta *respuestas, struct lPregunta *preguntas, int preguntaId);
void modificarRespuesta(struct lRespuesta *L, int respuestaId);
int buscarUltimoIdRespuesta(struct lRespuesta *L);
int buscarUltimoNumeroRespuesta(struct lRespuesta *L, int preguntaId);
void eliminarRespuestasDePregunta(struct lRespuesta **L, int preguntaId);
void cargar_respuestas_csv(struct lRespuesta *L);

//Desarrollo de las consignas:
//usaremos una lista que contenga todos los datos de la encuesta a mostrar cumpliendo la consigna b,
//la lista contendra una lista de preguntas y una lista de respuestas que perteneceran a la encuesta
//durante el proceso de carga podremos calcular la ponderacion de todas las respuestas para calcular la ponderacion total de la encuesta
struct arbol{
    int Encuesta_Id;
    struct lPregunta *L_Preguntas;
    struct lRespuesta *L_Respuestas;
    float Ponderacion_total;
    struct arbol *der;
    struct arbol *izq;
};

int main(){
    //menu principal
    //variables
    struct pEncuesta *tp=NULL, *nodoP=NULL;
    struct lRespuesta *L2=NULL,*nodoL2=NULL;
    struct lPregunta *LP=NULL, *nodoLP=NULL;
    int id;
    //funcion system para permitir mas caracteres y caracteres especiales
    system("chcp 65001");
    colorMenu();
	int apagado=0;
	char w=1;
	int select=1,tam=7;
	while(!apagado){
		system("cls");
		printf("<<<<TP integrador>>>>\n");
        letraR();
        printf("---------------------\n");
        colorMenu();
        //menu visual para seleccionar una opcion
		if(select==1){
			printf("\n>> crear respuesta");
		}else
			printf("\n   crear respuesta");
        if(select==2){
			printf("\n>> mostrar respuestas por pregunta");
		}else
			printf("\n   mostrar respuestas por pregunta");
        if(select==3){
			printf("\n>> modificar respuesta");
		}else
			printf("\n   modificar respuesta");
        if(select==4){
			printf("\n>> eliminar respuestas de una pregunta");
		}else
			printf("\n   eliminar respuestas de una pregunta");
		if(select==5){
			printf("\n>> crear pregunta");
		}else
			printf("\n   crear pregunta");
        if(select==6){
			printf("\n>> modificar pregunta");
		}else
			printf("\n   modificar pregunta");
		if(select==7){
			printf("\n>> salir");
		}else
			printf("\n   salir");
		w=getch();
		if(w=='w' || w==72 )
			select--;
		if(w=='s' || w==80)
			select++;
        //al presionar la tecla escape se cierra el programa
		if(w==27)
			apagado++;
		if(select>tam)
			select=1;
		if(select<1)
			select=tam;
        //seleccion de las opciones tras presionar enter
		if(w==13){
			if(select==1){
                system("cls");
				crearRespuesta(&L2);
			}
			if(select==2){
                system("cls");
                printf("ingrese un id de preguntas para mostrar sus respuesta: ");
                scanf("%i",&id);
				mostrarRespuestasPorPregunta(L2,LP,id);
                getch();
            }
            if(select==3){
                system("cls");
                printf("ingrese un id de respuestas para modificar una respuesta: ");
                scanf("%i",&id);
				modificarRespuesta(L2,id);
                getch();
			}
            if(select==4){
                system("cls");
                printf("ingrese un id de preguntasa para eliminar sus respuesta: ");
                scanf("%i",&id);
				eliminarRespuestasDePregunta(&L2,id);
                getch();
			}
			if(select==5){
                system("cls");
                crearPregunta(&LP);
            }
            if(select==6){
                system("cls");
                printf("ingrese un id de pregunta para modificar una pregunta: ");
                scanf("%i",&id);
                modificarPregunta(LP,id);
                getch();
            }
			if(select==7){
				apagado++;
            }
		}
	}
    //carga las respuestas en el arch_respuestas
    cargar_respuestas_csv(L2);
    //al finalizar se liberan todas las estructuras
    while(!vaciaP(tp)){
        desapilar(&nodoP,&tp);
        free(nodoP);
        nodoP=NULL;
    }
    return 0;
}

//funciones pila de encuestas
void apilar (struct pEncuesta **n, struct pEncuesta **tp){
    (*n)->sgte = (*tp);
	(*tp) = (*n);
	(*n) = NULL;
}
void desapilar (struct pEncuesta **n, struct pEncuesta **tp){
	(*n) = (*tp);
	(*tp) = (*tp)->sgte;
	(*n)->sgte = NULL;
}
int vaciaP (struct pEncuesta *tp){
	int band = 0;
	if (tp == NULL){
		band = 1;
	}
	return band;
}
int nuevoP(struct pEncuesta **nuevo){
    *nuevo = (struct pEncuesta *) malloc(sizeof(struct pEncuesta));
    return (*nuevo == NULL)? 0 : 1;
}
void mostrarP(struct pEncuesta **tope){
    struct pEncuesta *nodo=NULL,*topeAux=NULL;
    if(!vaciaP(*tope)){
        while (!vaciaP(*tope)){
            desapilar (&nodo, tope);
            //informacion a mostrar aqui con printf
            apilar(&nodo,&topeAux);
        }
        while (!vaciaP(topeAux)){
            desapilar (&nodo, &topeAux);
            apilar(&nodo,tope);
        }
    }else
        printf("ERROR: la pila ingresada para Mostrar esta vacia\n");
}

//funciones lista de preguntas
int nuevoLPregunta(struct lPregunta **nodo){
    *nodo = (struct lPregunta *) malloc(sizeof(struct lPregunta));
    return (*nodo == NULL)? 0 : 1;
}

struct lPregunta *insertarLPregunta(struct lPregunta **nodo, struct lPregunta **L){
    if((*L)!=NULL){
        if((*nodo)->Pregunta_Id < (*L)->Pregunta_Id){
            (*nodo)->sgte=(*L);
            (*L)=(*nodo);
        }else
        (*L)->sgte = insertarLPregunta(nodo,&(*L)->sgte);
    }else{
        (*L)=(*nodo);
    }
    return (*L);
}
struct lPregunta *mostrarListaPregunta(struct lPregunta **L){
    if((*L)!=NULL){
        //contenido de la lista de preguntas
        mostrarListaPregunta(&(*L)->sgte);
    }
    return (*L);
}

//funciones lista de respuestas
int nuevoLRespuesta(struct lRespuesta **nodo){
    *nodo = (struct lRespuesta *) malloc(sizeof(struct lRespuesta));
    return (*nodo == NULL)? 0 : 1;
}
struct lRespuesta *insertarLRespuesta(struct lRespuesta **nodo, struct lRespuesta **L){
    if((*L)!=NULL){
        if((*nodo)->Respuesta_Id < (*L)->Respuesta_Id){
            (*nodo)->sgte=(*L);
            (*L)=(*nodo);
        }else
        (*L)->sgte=insertarLRespuesta(nodo,&(*L)->sgte);
    }else{
        (*L)=(*nodo);
    }
    return (*L);
}
struct lRespuesta *mostrarListaRespuesta(struct lRespuesta **L){
    if((*L)!=NULL){
        //contenido de la lista de respuestas
        mostrarListaRespuesta(&(*L)->sgte);
    }
    return (*L);
}

//funciones arbol
struct arbol *insertarA(struct arbol **A, struct arbol **nodo) {
    if ((*A) != NULL) {
        if ((*A)->Encuesta_Id == (*nodo)->Encuesta_Id) {
            printf("Ya esta en el arbol\n");
        } else if ((*nodo)->Encuesta_Id < (*A)->Encuesta_Id) {
            (*A)->izq = insertarA(&(*A)->izq, nodo);
        } else {
            (*A)->der = insertarA(&(*A)->der, nodo);
        }
    } else {
        (*A) = (*nodo);
    }
    return (*A);
}
void mostrarA(struct arbol **A){
    if((*A)!=NULL){
        mostrarA(&(*A)->izq);
        printf("\nid: %i", (*A)->Encuesta_Id);
        mostrarA(&(*A)->der);
    }
}
int nuevoA(struct arbol **nodo){
    *nodo = (struct arbol *) malloc(sizeof(struct arbol));
    return (*nodo == NULL)? 0 : 1;
}
void buscarA(int dato, struct arbol *r) {
    if (r != NULL) {
        if (dato == r->Encuesta_Id) {
            printf("Dato encontrado: ");
        } else if (dato < r->Encuesta_Id) {
            buscarA(dato,r->izq);
        } else {
            buscarA(dato,r->der);
        }
    } else {
        printf("El dato buscado no se encuentra en el árbol\n");
    }
}
void busBorrA(struct arbol **r, int dato) {
    if (*r != NULL) {
        if (dato == (*r)->Encuesta_Id) {
            borrarNodoA(r);
        } else if (dato < (*r)->Encuesta_Id) {
            busBorrA(&(*r)->izq, dato);
        } else {
            busBorrA(&(*r)->der, dato);
        }
    } else {
        printf("No se encontró\n");
    }
}
void borrarNodoA(struct arbol **nodo) {
    struct arbol *aux = *nodo;
    if (aux->izq == NULL && aux->der == NULL) {
        free(aux);
        *nodo = NULL;
    }
    else if (aux->izq == NULL) {
        *nodo = aux->der;
        free(aux);
    }
    else if (aux->der == NULL) {
        *nodo = aux->izq;
        free(aux);
    }
    else {
        struct arbol **mayor = &(*nodo)->izq;
        while ((*mayor)->der != NULL) {
            mayor = &(*mayor)->der;
        }
        (*nodo)->Encuesta_Id = (*mayor)->Encuesta_Id;
        borrarNodoA(mayor);
    }
}
struct arbol *eliminarA(struct arbol **r){
    if ((*r) != NULL) {
        (*r)->izq = eliminarA(&(*r)->izq);
        (*r)->der = eliminarA(&(*r)->der);
        free(*r);
        (*r) = NULL;
    }
    return NULL;
}
//CRUD Preguntas
void crearPregunta(struct lPregunta **L) {
    struct lPregunta *nueva = NULL;
    if (nuevoLPregunta(&nueva)) {
        if(*L == NULL){
            nueva->Pregunta_Id = 1;
        } else {
            nueva->Pregunta_Id = buscarUltimoIdPregunta(*L) + 1;
        }

        printf("Ingrese el Encuesta_id de la pregunta: ");
        scanf("%i", &nueva->Encuesta_Id);

        // LIMPIAR EL BUFFER antes de fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Ingrese el texto de la pregunta: ");
        fgets(nueva->Pregunta, sizeof(nueva->Pregunta), stdin);
        nueva->Pregunta[strcspn(nueva->Pregunta, "\n")] = 0;

        char buffer[256];
        float ponderacion;
        do {
            printf("Ingrese la ponderación de la pregunta (0 < x ≤ 1): ");
            fgets(buffer, sizeof(buffer), stdin);
        } while (sscanf(buffer, "%f", &ponderacion) != 1 || ponderacion <= 0 || ponderacion > 1);

        nueva->Ponderacion = ponderacion;
        nueva->sgte = NULL;
        insertarLPregunta(&nueva, L);
        printf("Pregunta creada con éxito.\n");
    } else {
        printf("Error al reservar memoria para la pregunta.\n");
    }
}



void modificarPregunta(struct lPregunta *L, int preguntaId) {
    while (L != NULL) {
        if (L->Pregunta_Id == preguntaId) {
            printf("Pregunta actual: %s\nPonderación actual: %.2f\n", L->Pregunta, L->Ponderacion);

            //Limpiar buffer antes de usar fgets
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Ingrese nuevo texto: ");
            fgets(L->Pregunta, sizeof(L->Pregunta), stdin);
            L->Pregunta[strcspn(L->Pregunta, "\n")] = 0;

            //Leer y validar ponderación
            char buffer[256];
            float ponderacion;
            do {
                printf("Ingrese nueva ponderación (0 < x ≤ 1): ");
                fgets(buffer, sizeof(buffer), stdin);
            } while (sscanf(buffer, "%f", &ponderacion) != 1 || ponderacion <= 0 || ponderacion > 1);
            L->Ponderacion = ponderacion;

            printf("Pregunta modificada.\n");
            return;
        }
        L = L->sgte;
    }
    printf("Pregunta con ID %d no encontrada.\n", preguntaId);
}

int buscarUltimoIdPregunta(struct lPregunta *L) {
    if (L->sgte == NULL) {
        return L->Pregunta_Id;
    }
    return buscarUltimoIdPregunta(L->sgte);
}

//CRUD respuestas
void crearRespuesta(struct lRespuesta **L) {
    struct lRespuesta *nueva = NULL;
    if (nuevoLRespuesta(&nueva)) {
        if(*L == NULL){
            nueva->Respuesta_Id = 1;
        } else {
            nueva->Respuesta_Id = buscarUltimoIdRespuesta(*L) + 1;
        }
        printf("Ingrese ID de la pregunta: ");
        scanf("%d", &nueva->Pregunta_Id);
        nueva->Respuesta_Nro=buscarUltimoNumeroRespuesta(*L,nueva->Pregunta_Id)+1;
        char buffer[256];
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Ingrese el texto de la respuesta: ");
        fgets(nueva->Respuesta, sizeof(nueva->Respuesta), stdin);
        nueva->Respuesta[strcspn(nueva->Respuesta, "\n")] = 0;

        float ponderacion;
        do {
            printf("Ingrese la ponderación de la pregunta (0 < x ≤ 1): ");
            fgets(buffer, sizeof(buffer), stdin);
        } while (sscanf(buffer, "%f", &ponderacion) != 1 || ponderacion <= 0 || ponderacion > 1);
        nueva->Ponderacion = ponderacion;
        printf("¿Es la opción elegida? (1=Sí, 0=No): ");
        scanf("%d", &nueva->Elegida);
        nueva->sgte = NULL;
        insertarLRespuesta(&nueva, L);
        printf("Respuesta creada con éxito.\n");
    } else {
        printf("Error al reservar memoria para la respuesta.\n");
    }
}
void mostrarRespuestasPorPregunta(struct lRespuesta *respuestas, struct lPregunta *preguntas, int preguntaId) {
    // Buscar y mostrar la pregunta asociada
    struct lPregunta *p = preguntas;
    int encontrada = 0;

    while (p != NULL) {
        if (p->Pregunta_Id == preguntaId) {
            printf("=== Pregunta ID %d ===\n", p->Pregunta_Id);
            printf("Texto: %s\n", p->Pregunta);
            printf("Ponderación: %.2f\n", p->Ponderacion);
            encontrada = 1;
            break;
        }
        p = p->sgte;
    }

    if (!encontrada) {
        printf("No se encontró una pregunta con ID %d.\n", preguntaId);
        return;
    }

    // Mostrar respuestas asociadas
    printf("\n--- Respuestas asociadas ---\n");
    int hayRespuestas = 0;
    while (respuestas != NULL) {
        if (respuestas->Pregunta_Id == preguntaId) {
            printf("ID: %d | Nro: %d | Texto: %s | Ponderación: %.2f | Elegida: %s\n",
                   respuestas->Respuesta_Id,
                   respuestas->Respuesta_Nro,
                   respuestas->Respuesta,
                   respuestas->Ponderacion,
                   respuestas->Elegida ? "Sí" : "No");
            hayRespuestas = 1;
        }
        respuestas = respuestas->sgte;
    }

    if (!hayRespuestas) {
        printf("No hay respuestas registradas para esta pregunta.\n");
    }
}

void modificarRespuesta(struct lRespuesta *L, int respuestaId) {
    while (L != NULL) {
        if (L->Respuesta_Id == respuestaId) {
            printf("Respuesta actual: %s\n", L->Respuesta);
            printf("Ingrese nuevo texto: ");
            fflush(stdin);
            scanf("%s", &L->Respuesta);
            printf("Ingrese nueva ponderación: ");
            scanf("%f", &L->Ponderacion);
            while(L->Ponderacion <= 0 && L->Ponderacion > 1){
                printf("(numero invalido) Ingrese nueva ponderación: ");
                scanf("%f", &L->Ponderacion);
            }
            printf("¿Es la opción elegida? (1=Sí, 0=No): ");
            scanf("%d", &L->Elegida);
            printf("Respuesta modificada.\n");
            return;
        }
        L = L->sgte;
    }
    printf("Respuesta con ID %d no encontrada.\n", respuestaId);
}
int buscarUltimoIdRespuesta(struct lRespuesta *L) {
    if (L->sgte == NULL) {
        return L->Respuesta_Id;
    }
    return buscarUltimoIdRespuesta(L->sgte);
}
int buscarUltimoNumeroRespuesta(struct lRespuesta *L, int preguntaId) {
    int maxNro = 0;
    while (L != NULL) {
        if (L->Pregunta_Id == preguntaId && L->Respuesta_Nro > maxNro) {
            maxNro = L->Respuesta_Nro;
        }
        L = L->sgte;
    }
    return maxNro;
}

void eliminarRespuestasDePregunta(struct lRespuesta **L, int preguntaId) {
    struct lRespuesta *actual = *L;
    struct lRespuesta *anterior = NULL;

    while (actual != NULL) {
        if (actual->Pregunta_Id == preguntaId) {
            struct lRespuesta *aBorrar = actual;
            if (anterior == NULL) {
                *L = actual->sgte;
                actual = *L;
            } else {
                anterior->sgte = actual->sgte;
                actual = anterior->sgte;
            }
            free(aBorrar);
        } else {
            anterior = actual;
            actual = actual->sgte;
        }
    }
    printf("Respuestas de la pregunta %d eliminadas.\n", preguntaId);
}
void cargar_respuestas_csv(struct lRespuesta *L) {
    struct lRespuesta *aux = NULL;
    const char *RespuestasCSV="respuestas.csv";
    FILE *archivo = fopen(RespuestasCSV, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    } else {
        //esta es la forma en la que se guardan los datos en el archivo
        //fprintf(archivo, "Respuesta_Id;Pregunta_Id;Respuesta_Nro;Respuesta;Ponderacion;Elegida\n");
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
    fclose(arch_respuestas);
}
