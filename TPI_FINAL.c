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

//funciones arbol_respondidas
struct arbol_respondidas *insertarA(struct arbol_respondidas **A, struct arbol_respondidas **nodo);
void mostrarA(struct arbol_respondidas **A);
int nuevoA(struct arbol_respondidas **nodo);
void buscarA(int dato, struct arbol_respondidas *r);
void busBorrA(struct arbol_respondidas **r, int dato);
void borrarNodoA(struct arbol_respondidas **nodo);
struct arbol_respondidas *eliminarA(struct arbol_respondidas **r);
void imprimirArbol(struct arbol_respondidas *r);
void MostrarEncuestasRespondidas(struct arbol_respondidas *A, struct lPregunta *preguntas, struct lRespuesta *respuestas, int encuestaRespondidaId, int *encontrado);

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
//CRUD Encuestas
void crearEncuesta(struct pEncuesta **tp);
void mostrarEncuestas(struct pEncuesta **tp);
int buscarUltimoIdEncuesta(struct pEncuesta **tp);
int buscarMaxIdEncuestaCSV(const char *archivo);
void modificarEncuesta(struct pEncuesta **tp, int id);
void eliminarEncuesta(struct pEncuesta **tp, int id);
//CRUD Preguntas
void crearPregunta(struct lPregunta **L);
void modificarPregunta(struct lPregunta *L, int preguntaId);
int buscarUltimoIdPregunta(struct lPregunta *L);
void eliminarpreguntasDeEncuesta(struct lPregunta **L,struct lRespuesta **L2, int Encuesta_Id);
void imprimirPreguntas(struct lPregunta *L);
//CRUD respuestas
void crearRespuesta(struct lRespuesta **L);
void mostrarRespuestasPorPregunta(struct lRespuesta *respuestas, struct lPregunta *preguntas, int preguntaId);
void modificarRespuesta(struct lRespuesta *L, int respuestaId);
int buscarUltimoIdRespuesta(struct lRespuesta *L);
int buscarUltimoNumeroRespuesta(struct lRespuesta *L, int preguntaId);
void eliminarRespuestasDePregunta(struct lRespuesta **L, int preguntaId);
//funciones de carga en CSV
void cargar_respuestas_csv(struct lRespuesta *L);
void cargar_preguntas_csv(struct lPregunta *L);
void cargar_encuestas_csv(struct pEncuesta **tp);
// funciones de extraccion de datos desde CSV
void extraer_respuestas_csv(struct lRespuesta **L);
void extraer_preguntas_csv(struct lPregunta **L);
void extraer_encuestas_csv(struct pEncuesta **tp);
void extraer_arbol_csv(struct arbol_respondidas **A, const char *Encuestas_Respondidas);


//Desarrollo de las consignas:
//usaremos una lista que contenga todos los datos de la encuesta a mostrar cumpliendo la consigna b,
//la lista contendra una lista de preguntas y una lista de respuestas que perteneceran a la encuesta
//durante el proceso de carga podremos calcular la ponderacion de todas las respuestas para calcular la ponderacion total de la encuesta

//el arbol representara la respuesta elegida de cada pregunta de la encuesta
struct arbol_respondidas{
    int Encuesta_Id;
    int Pregunta_Id;
    int Respuesta_Id;
    int Anio;
    int Encuesta_Mes;
    int Encuestador_id;
    int EncuestaRespondida_Id;
    int dia;
    struct arbol_respondidas *der;
    struct arbol_respondidas *izq;
};
struct pilaRespondida{
    int Encuesta_Id;
    int Pregunta_Id;
    int Respuesta_Id;
    int Anio;
    int Encuesta_Mes;
    int Encuestador_id;
    int EncuestaRespondida_Id;
    int dia;
    struct pilaRespondida *sgte;
};
struct lPponderaciones {
    int Encuesta_Id;
    int Pregunta_Id;
    float Ponderacion_Total;
    struct lPponderaciones *sgte;
};
//funciones de filtro
void verificarPonderacionPreguntas(struct lPregunta **L,struct lRespuesta **L2);
void filtrarRespondidas(struct arbol_respondidas **A, int Encuesta_Id, struct pilaRespondida **tope);
void mostrarPilaRespondida(struct pilaRespondida **tope);
int nuevoLPreguntaPonderacion(struct lPponderaciones **nodo);
struct lPponderaciones *insertarLPreguntaPonderacion(struct lPponderaciones **nodo, struct lPponderaciones **L);
void procesarEncuesta(struct pilaRespondida **tpRespondida,struct lPponderaciones **LPponder,struct pEncuesta **tp, int Encuesta_Id, struct lPregunta **LP, struct lRespuesta **L2, struct arbol_respondidas **A);
void apilarRespondida(struct pilaRespondida **nodo, struct pilaRespondida **tope);
int nuevoNodoPilaRespondida(struct pilaRespondida **nodo);
void desapilarRespondida(struct pilaRespondida **tope, struct pilaRespondida **nodo);
int vaciaPilaRespondida(struct pilaRespondida *tp);
void mostrarPonderaciones(struct lPponderaciones **L);
void sumarPonderacionesEncuesta(int Encuesta_Id, struct lPponderaciones **L);
void acumularListaPonderaciones(struct lPponderaciones **L, struct lPponderaciones **ListaFinal);
void apilarLP(struct lPponderaciones **nodo, struct lPponderaciones **L);
void desapilarLP(struct lPponderaciones **nodo, struct lPponderaciones **L);
int vaciaLP(struct lPponderaciones *L);

int main(){
    srand(time(NULL)); // inicializa la semilla para generar numeros aleatorios
    //menu principal
    //variables
    struct pEncuesta *tp=NULL, *nodoP=NULL;
    struct lRespuesta *L2=NULL,*nodoL2=NULL;
    struct lPregunta *LP=NULL, *nodoLP=NULL;
    struct arbol_respondidas *A=NULL, *nodoA=NULL;
    struct pilaRespondida *tpRespondida = NULL, *nodoPila = NULL;
    struct lPponderaciones *LPponder = NULL, *nodoLPonder = NULL;
    struct lPponderaciones *ListaFinal = NULL, *nodoListaFinal = NULL;
    //inicializacion de las estructuras
    extraer_respuestas_csv(&L2);
    extraer_preguntas_csv(&LP);
    extraer_encuestas_csv(&tp);
    extraer_arbol_csv(&A, "Encuestas_Respondidas.csv");

    int id;
    //funcion system para permitir mas caracteres y caracteres especiales
    system("chcp 65001");
    colorMenu();
	int apagado=0;
	char w=1;
    int select=1,tam=15;
    printf("verificando ponderacion de preguntas...\n");
    verificarPonderacionPreguntas(&LP,&L2);
    getch();
	while(!apagado){
		system("cls");
		printf("<<<<TP integrador>>>>\n");
        letraR();
        printf("---------------------\n");
        colorMenu();
        //verifica que las preguntas tengan una ponderacion correcta, si no la tienen se eliminan
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
			printf("\n>> mostrar preguntas");
		}else
			printf("\n   mostrar preguntas");
        if(select==7){
			printf("\n>> modificar pregunta");
		}else
			printf("\n   modificar pregunta");
		if(select==8) 
            printf("\n>> eliminar encuesta"); 
        else 
            printf("\n   eliminar encuesta");
        if(select==9)
            printf("\n>> crear encuesta"); 
        else 
            printf("\n   crear encuesta");
        if(select==10) 
            printf("\n>> mostrar encuestas"); 
        else 
            printf("\n   mostrar encuestas");
        if(select==11) 
            printf("\n>> modificar encuesta"); 
        else 
            printf("\n   modificar encuesta");
        if(select==12) 
            printf("\n>> procesar encuesta"); 
        else 
            printf("\n   procesar encuesta");
        if(select==13){
			printf("\n>> mostrar encuestas respondidas(arbol)");
		}else
			printf("\n   mostrar encuestas respondidas(arbol)");
        if(select==14) 
            printf("\n>> buscar encuesta respondida por id"); 
        else 
            printf("\n   buscar encuesta respondida por id");
        if(select==15){
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
                imprimirPreguntas(LP);
                getch();
            }
            if(select==7){
                system("cls");
                printf("ingrese un id de pregunta para modificar una pregunta: ");
                scanf("%i",&id);
                modificarPregunta(LP,id);
                getch();
            }
            if(select==8){
                system("cls");
                printf("Ingrese ID de encuesta a eliminar: ");
                scanf("%d", &id);
                eliminarEncuesta(&tp, id);
                getch();
            }
            if(select==9){
                system("cls");
                crearEncuesta(&tp);
                getch();
            }
            if(select==10){
                system("cls");
                mostrarEncuestas(&tp);
                getch();
            }
            if(select==11){
                system("cls");
                printf("Ingrese ID de encuesta a modificar: ");
                scanf("%d", &id);
                modificarEncuesta(&tp, id);
                getch();
            }
            if(select==12){
                system("cls");
                //procesar encuesta
                printf("Ingrese ID de encuesta a procesar: ");
                scanf("%d", &id);
                procesarEncuesta(&tpRespondida, &LPponder, &tp, id, &LP, &L2, &A);
                getch();
            }
            if(select==13){
                system("cls");
                imprimirArbol(A);
                getch();
            }
             if(select==14){
                system("cls");
    		    printf("Ingrese el ID de la encuesta respondida a mostrar: ");
    		    scanf("%d", &id);
    		    int encontrado = 0;
    		    MostrarEncuestasRespondidas(A, LP, L2, id, &encontrado);
    		    if (!encontrado) {
        		printf("\nNo se encontró ninguna encuesta respondida con ese ID.\n");
    		}
    		getch();
            }
            if(select==15){
                apagado++;
            }
        }
	}
    //carga las respuestas en el arch_respuestas
    cargar_respuestas_csv(L2);
    cargar_preguntas_csv(LP);
    cargar_encuestas_csv(&tp);
        
    //al finalizar se liberan todas las estructuras
    while(!vaciaP(tp)){
        desapilar(&nodoP,&tp);
        free(nodoP);
        nodoP=NULL;
    }
    while(L2 != NULL){
        nodoL2 = L2;
        L2 = L2->sgte;
        free(nodoL2);
    }
    while(LP != NULL){
        nodoLP = LP;
        LP = LP->sgte;
        free(nodoLP);
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
//funciones arbol_respondidas
struct arbol_respondidas *insertarA(struct arbol_respondidas **A, struct arbol_respondidas **nodo) {
    if ((*A) != NULL) {
        // Elegir aleatoriamente izquierda o derecha
        if (rand() % 2 == 0) {
            (*A)->izq = insertarA(&(*A)->izq, nodo);
        } else {
            (*A)->der = insertarA(&(*A)->der, nodo);
        }
    } else {
        (*A) = (*nodo);
    }
    return (*A);
}
void mostrarA(struct arbol_respondidas **A){
    if((*A)!=NULL){
        mostrarA(&(*A)->izq);
        printf("\nid: %i", (*A)->Encuesta_Id);
        mostrarA(&(*A)->der);
    }
}
int nuevoA(struct arbol_respondidas **nodo){
    *nodo = (struct arbol_respondidas *) malloc(sizeof(struct arbol_respondidas));
    return (*nodo == NULL)? 0 : 1;
}
void buscarA(int dato, struct arbol_respondidas *r) {
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
void busBorrA(struct arbol_respondidas **r, int dato) {
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
void borrarNodoA(struct arbol_respondidas **nodo) {
    struct arbol_respondidas *aux = *nodo;
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
        struct arbol_respondidas **mayor = &(*nodo)->izq;
        while ((*mayor)->der != NULL) {
            mayor = &(*mayor)->der;
        }
        (*nodo)->Encuesta_Id = (*mayor)->Encuesta_Id;
        borrarNodoA(mayor);
    }
}
struct arbol_respondidas *eliminarA(struct arbol_respondidas **r){
    if ((*r) != NULL) {
        (*r)->izq = eliminarA(&(*r)->izq);
        (*r)->der = eliminarA(&(*r)->der);
        free(*r);
        (*r) = NULL;
    }
    return NULL;
}
//CRUD Encuestas
void crearEncuesta(struct pEncuesta **tp) {
    struct pEncuesta *nueva = NULL;
    if (nuevoP(&nueva)) {
        int c;
        int maxIdMem = 0, maxIdCSV = 0;
        // Buscar el máximo ID en memoria
        if(*tp != NULL){
            maxIdMem = buscarUltimoIdEncuesta(tp);
        }
        // Buscar el máximo ID en el archivo CSV
        maxIdCSV = buscarMaxIdEncuestaCSV("encuestas.csv");
        // Asignar el ID mayor + 1
        int nuevoId = (maxIdMem > maxIdCSV ? maxIdMem : maxIdCSV) + 1;
        nueva->Encuesta_id = nuevoId;

        while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer
        printf("Ingrese denominación de la encuesta: ");
        fgets(nueva->Denominacion, sizeof(nueva->Denominacion), stdin);
        nueva->Denominacion[strcspn(nueva->Denominacion, "\n")] = 0;

        printf("Ingrese mes (1-12): ");
        scanf("%d", &nueva->Encuesta_Mes);
        printf("Ingrese año: ");
        scanf("%d", &nueva->Anio);

        nueva->Procesada = 0; // por defecto
        nueva->sgte = NULL;

        apilar(&nueva, tp);
        printf("Encuesta creada con éxito.\n");
    } else {
        printf("Error al reservar memoria para la encuesta.\n");
    }
}
void mostrarEncuestas(struct pEncuesta **tp) {
    struct pEncuesta *nodo = NULL, *aux = NULL;
    printf("=== Lista de encuestas ===\n");
    if (*tp == NULL) {
        printf("No hay encuestas cargadas.\n");
        return;
    }

    while (!vaciaP(*tp)) {
        desapilar(&nodo,tp);
        printf("ID: %d | Denominación: %s | Fecha: %02d/%d | Procesada: %s\n",
                nodo->Encuesta_id,
                nodo->Denominacion,
                nodo->Encuesta_Mes,
                nodo->Anio,
                nodo->Procesada ? "Sí" : "No");
        apilar(&nodo,&aux);
    }
    //devolvemos la pila usando un auxiliar para mantener el orden original
    while (!vaciaP(aux)){
        desapilar(&nodo,&aux);
        apilar(&nodo,tp);
    }
}
int buscarUltimoIdEncuesta(struct pEncuesta **tp) {
    int id;
    struct pEncuesta *nodo=NULL;
    //al ser una pila el ultimo id está en el tope de la pila
    if (vaciaP(*tp)) {
        printf("No hay encuestas cargadas.\n");
        return 0; // no hay encuestas
    }
    desapilar(&nodo,tp);
    id = nodo->Encuesta_id;
    apilar(&nodo,tp);
    return id;
}
int buscarMaxIdEncuestaCSV(const char *Encuestas) {
    FILE *archivo = fopen(Encuestas, "r");
    if (!archivo) return 0;
    int maxId = 0, id, mes, anio, procesada;
    char denominacion[100];
    while (fscanf(archivo, "%d;%[^;];%d;%d;%d\n", &id, denominacion, &mes, &anio, &procesada) == 5) {
        if (id > maxId) maxId = id;
    }
    fclose(archivo);
    return maxId;
}
void modificarEncuesta(struct pEncuesta **tp, int id) {
    struct pEncuesta *nodo = NULL, *aux = NULL;
    int encontrado=0;
    while (!vaciaP(*tp)) {
        desapilar(&nodo,tp);
        if (nodo->Encuesta_id == id) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer

            printf("Denominación actual: %s\n", nodo->Denominacion);
            printf("Ingrese nueva denominación: ");
            fgets(nodo->Denominacion, sizeof(nodo->Denominacion), stdin);
            nodo->Denominacion[strcspn(nodo->Denominacion, "\n")] = 0;

            printf("Mes actual: %d\n", nodo->Encuesta_Mes);
            printf("Ingrese nuevo mes: ");
            scanf("%d", &nodo->Encuesta_Mes);

            printf("Año actual: %d\n", nodo->Anio);
            printf("Ingrese nuevo año: ");
            scanf("%d", &nodo->Anio);

            printf("Encuesta modificada.\n");
            encontrado++;
        }
        apilar(&nodo,&aux);
    }
    while (!vaciaP(aux)){
        desapilar(&nodo,&aux);
        apilar(&nodo,tp);
    }
    printf("Encuesta con ID %d no encontrada.\n", id);
}
void eliminarEncuesta(struct pEncuesta **tp, int id) {
    struct pEncuesta *actual = *tp;
    struct pEncuesta *anterior = NULL;
    if (actual == NULL) {
        printf("No hay encuestas para eliminar.\n");
        return;
    }
    while (actual != NULL) {
        if (actual->Encuesta_id == id) {
            if (anterior == NULL) {
                *tp = actual->sgte;
            } else {
                anterior->sgte = actual->sgte;
            }
            free(actual);
            printf("Encuesta eliminada.\n");
            return;
        }
        anterior = actual;
        actual = actual->sgte;
    }

    printf("Encuesta con ID %d no encontrada.\n", id);
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
void eliminarpreguntasDeEncuesta(struct lPregunta **L, struct lRespuesta **L2, int Encuesta_Id){
    struct lPregunta *actual = *L;
    struct lPregunta *anterior = NULL;

    while (actual != NULL) {
        if (actual->Encuesta_Id == Encuesta_Id) {
            struct lPregunta *aBorrar = actual;
            int idPreguntaABorrar = aBorrar->Pregunta_Id; // <-- Guardar antes de modificar actual
            if (anterior == NULL) {
                *L = actual->sgte;
                actual = *L;
            } else {
                anterior->sgte = actual->sgte;
                actual = anterior->sgte;
            }
            printf("Pregunta eliminada: %s\n", aBorrar->Pregunta);
            eliminarRespuestasDePregunta(L2, idPreguntaABorrar); // Usar el id guardado
            free(aBorrar);
        } else {
            anterior = actual;
            actual = actual->sgte;
        }
    }
    printf("Preguntas de la encuesta %d eliminadas.\n", Encuesta_Id);
}
void imprimirPreguntas(struct lPregunta *L) {
    if (L == NULL) {
        printf("No hay preguntas registradas.\n");
        return;
    }
    printf("=== Lista de preguntas ===\n");
    while (L != NULL) {
        printf("Encuesta ID: %d | Pregunta ID: %d | Texto: %s | Ponderación: %.2f\n",
               L->Encuesta_Id, L->Pregunta_Id, L->Pregunta, L->Ponderacion);
        L = L->sgte;
    }
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
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            fgets(L->Respuesta, sizeof(L->Respuesta), stdin);
            L->Respuesta[strcspn(L->Respuesta, "\n")] = 0;
            printf("Ingrese nueva ponderación: ");
            scanf("%f", &L->Ponderacion);
            while(L->Ponderacion <= 0 || L->Ponderacion > 1){
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
    if (L == NULL) {
        printf("No hay respuestas registradas.\n");
        return 0; // No hay respuestas
    }
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
            printf("Respuesta eliminada: %s\n", aBorrar->Respuesta);
            free(aBorrar);
        } else {
            anterior = actual;
            actual = actual->sgte;
        }
    }
    printf("Respuestas de la pregunta %d eliminadas.\n", preguntaId);
}
//funciones de carga en CSV
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
    fclose(archivo);
}
void cargar_preguntas_csv(struct lPregunta *L) {
    struct lPregunta *aux = NULL;
    const char *PreguntasCSV="preguntas.csv";
    FILE *archivo = fopen(PreguntasCSV, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    } else {
        //esta es la forma en la que se guardan los datos en el archivo
        //fprintf(archivo, "Encuesta_Id;Pregunta_Id;Pregunta;Ponderacion\n");
        aux = L;
        while (aux != NULL) {
            fprintf(archivo, "%d;%d;\"%s\";%.2f\n",
            aux->Encuesta_Id,
            aux->Pregunta_Id,
            aux->Pregunta,
            aux->Ponderacion);
            aux = aux->sgte;
        }
    }
    fclose(archivo);
}
void cargar_encuestas_csv(struct pEncuesta **tp) {
    struct pEncuesta *nodo = NULL, *aux = NULL;
    const char *EncuestasCSV = "encuestas.csv";
    FILE *archivo = fopen(EncuestasCSV, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    } else {
        //fprintf(archivo, "Encuesta_id;Denominacion;Encuesta_Mes;Anio;Procesada\n");
        while (!vaciaP(*tp)) {
            desapilar(&nodo, tp);
            fprintf(archivo, "%d;\"%s\";%d;%d;%d\n",
                nodo->Encuesta_id,
                nodo->Denominacion,
                nodo->Encuesta_Mes,
                nodo->Anio,
                nodo->Procesada);
            apilar(&nodo, &aux);
        }
        // Restaurar la pila original
        while (!vaciaP(aux)) {
            desapilar(&nodo, &aux);
            apilar(&nodo, tp);
        }
    }
    fclose(archivo);
}
// funciones de extraccion de datos desde CSV
void extraer_respuestas_csv(struct lRespuesta **L) {
    FILE *archivo = fopen("respuestas.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de respuestas.\n");
        return;
    }
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        struct lRespuesta *nueva = NULL;
        if (nuevoLRespuesta(&nueva)) {
            sscanf(linea, "%d;%d;%d;%[^;];%f;%d",
                   &nueva->Respuesta_Id,
                   &nueva->Pregunta_Id,
                   &nueva->Respuesta_Nro,
                   nueva->Respuesta,
                   &nueva->Ponderacion,
                   &nueva->Elegida);
            nueva->sgte = NULL;
            insertarLRespuesta(&nueva, L);
        }
    }
    fclose(archivo);
}
void extraer_preguntas_csv(struct lPregunta **L) {
    FILE *archivo = fopen("preguntas.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de preguntas.\n");
        return;
    }
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        struct lPregunta *nueva = NULL;
        if (nuevoLPregunta(&nueva)) {
            sscanf(linea, "%d;%d;%[^;];%f",
                   &nueva->Encuesta_Id,
                   &nueva->Pregunta_Id,
                   nueva->Pregunta,
                   &nueva->Ponderacion);
            nueva->sgte = NULL;
            insertarLPregunta(&nueva, L);
        }
    }
    fclose(archivo);
}
void extraer_encuestas_csv(struct pEncuesta **tp) {
    FILE *archivo = fopen("encuestas.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de encuestas.\n");
        return;
    }
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        struct pEncuesta *nueva = NULL;
        if (nuevoP(&nueva)) {
            sscanf(linea, "%d;%[^;];%d;%d;%d",
                   &nueva->Encuesta_id,
                   nueva->Denominacion,
                   &nueva->Encuesta_Mes,
                   &nueva->Anio,
                   &nueva->Procesada);
            nueva->sgte = NULL;
            apilar(&nueva, tp);
        }
    }
    fclose(archivo);
}
void extraer_arbol_csv(struct arbol_respondidas **A, const char *Encuestas_Respondidas) {
	struct arbol_respondidas *nuevo = NULL;
    FILE *archivo = fopen(Encuestas_Respondidas, "r");
    char linea[300];
    
    if (archivo == NULL) {
        printf("Error en %s.\n", Encuestas_Respondidas);
        return;
    }   
    while (fgets(linea, sizeof(linea), archivo)) {
        nuevo = (struct arbol_respondidas *) malloc(sizeof(struct arbol_respondidas));
        if (nuevo == NULL) {
            printf("error de memoria.\n");
            continue;
        }

        int fecha_num;
        sscanf(linea, "%d;%d;%d;%d;%d;%d",
            &nuevo->Encuesta_Id,
            &nuevo->Pregunta_Id,
            &nuevo->Respuesta_Id,
            &fecha_num,
            &nuevo->Encuestador_id,
            &nuevo->EncuestaRespondida_Id
        );

        nuevo->Anio = fecha_num / 10000;
        nuevo->Encuesta_Mes = (fecha_num / 100) % 100;
        nuevo->dia = fecha_num % 100;

        nuevo->izq = NULL;
        nuevo->der = NULL;

        insertarA(A, &nuevo);
    }

    fclose(archivo);
}

void verificarPonderacionPreguntas(struct lPregunta **L,struct lRespuesta **L2) {
	struct lPregunta *r=NULL; //puntero para recorrer la lista
    int id_aux = 0; //para controlar que 
    float suma = 0;
    
    r = *L;
    while (r != NULL) {
    	//aca cambia de id de encuesta, o sea que evalua la ponderacion
        if (r->Encuesta_Id != id_aux) {
            if (id_aux != 0) {
                printf("la encuesta %d tiene %.2f de ponderacion\n ", id_aux, suma);
                if (suma == 1) {
                    printf("ponderacion correcta\n");
                }else{
                    printf("la preguntas de la encuesta %d no tienen una ponderacion correcta, se eliminan\n", id_aux);
                    eliminarpreguntasDeEncuesta(L,L2, id_aux);
                }
            }
            // actualizo todo
            id_aux = r->Encuesta_Id;
            suma = 0;
        }
        suma += r->Ponderacion;
        r = r->sgte;
    }
    // si  no hago esto no se evalua la ultima encuesta
    if (id_aux != -1) {
        printf("la encuesta %d tiene %.2f de ponderacion\n ", id_aux, suma);
        if (suma == 1) {
            printf("ponderacion correcta\n");
        }else{
            printf("la preguntas de la encuesta %d no tienen una ponderacion correcta, se eliminan\n", id_aux);
            eliminarpreguntasDeEncuesta(L,L2, id_aux);
        }
    }
}

void imprimirArbol(struct arbol_respondidas *raiz) {
    if (raiz != NULL) {
        imprimirArbol(raiz->izq);
        printf("Encuesta ID: %d | Pregunta ID: %d | Respuesta ID: %d | Fecha: %02d/%02d/%04d\n - Encuestador ID: %d | Encuesta Respondida ID: %d\n\n",
               raiz->Encuesta_Id,
               raiz->Pregunta_Id,
               raiz->Respuesta_Id,
               raiz->dia,
               raiz->Encuesta_Mes,
               raiz->Anio,
               raiz->Encuestador_id,
               raiz->EncuestaRespondida_Id);
        imprimirArbol(raiz->der);
    }
}
void MostrarEncuestasRespondidas(struct arbol_respondidas *A, struct lPregunta *preguntas, struct lRespuesta *respuestas, int encuestaRespondidaId, int *encontrado) {
	if (A == NULL) {
    		return;	
	}
    //recorro el arbol a la izquierda
    MostrarEncuestasRespondidas(A->izq, preguntas, respuestas, encuestaRespondidaId, encontrado);
    //evaluo los ids 
    if (A->EncuestaRespondida_Id == encuestaRespondidaId) {
    	*encontrado = 1;
        //recorro la lista de preguntas
        struct lPregunta *p = preguntas;
        while (p != NULL) {
            if (p->Pregunta_Id == A->Pregunta_Id) {
                printf("\nPregunta: %s (Pond: %.2f)\n", p->Pregunta, p->Ponderacion);
                //si encuentra una pregunta, sale del bucle para mostrar las respuestas y despues vuelve
                break;
            }
            p = p->sgte;
        }
        //muestro las respuestas asociadas a la pregunta y marco con 1 la elegida del arbol
        struct lRespuesta *r = respuestas;
        while (r != NULL) {
            if (r->Pregunta_Id == A->Pregunta_Id) {
                if (r->Respuesta_Id == A->Respuesta_Id) {
                    printf(" [1] ID %d: %s (%.2f)\n", r->Respuesta_Id, r->Respuesta, r->Ponderacion);
                } else {
                    printf(" [0] ID %d: %s (%.2f)\n", r->Respuesta_Id, r->Respuesta, r->Ponderacion);
                }
            }
            r = r->sgte;
        }
    }
    //recorro por la derecha
    MostrarEncuestasRespondidas(A->der, preguntas, respuestas, encuestaRespondidaId, encontrado);
}
//preocesar encuestas

int nuevoLPreguntaPonderacion(struct lPponderaciones **nodo) {
    *nodo = (struct lPponderaciones *) malloc(sizeof(struct lPponderaciones));
    return (*nodo == NULL) ? 0 : 1;
}
struct lPponderaciones *insertarLPreguntaPonderacion(struct lPponderaciones **nodo, struct lPponderaciones **L) {
    if((*L)!=NULL){
        if((*nodo)->Pregunta_Id < (*L)->Pregunta_Id){
            (*nodo)->sgte=(*L);
            (*L)=(*nodo);
        }else
        (*L)->sgte = insertarLPreguntaPonderacion(nodo,&(*L)->sgte);
    }else{
        (*L)=(*nodo);
    }
    return (*L);
}
void buscarLPreguntaPonderacion(struct lPponderaciones **L, int Pregunta_Id, struct lPponderaciones **nodo) {
    if ((*L) != NULL) {
        if ((*L)->Pregunta_Id == Pregunta_Id) {
            (*nodo) = (*L);
        } else {
            buscarLPreguntaPonderacion(&(*L)->sgte, Pregunta_Id, nodo);
        }
    } else {
        (*nodo) = NULL;
    }
}
void procesarEncuesta(struct pilaRespondida **tpRespondida,struct lPponderaciones **LPponder,struct pEncuesta **tp, int Encuesta_Id, struct lPregunta **LP, struct lRespuesta **L2, struct arbol_respondidas **A){
    struct pEncuesta *nodoP = NULL, *auxP = NULL;
    int existeEncuesta = 0, procesada = 0;;

    //recorrer la pila de encuestas buscando la encuesta por id
    while(!vaciaP(*tp)){
        desapilar(&nodoP,tp);
        if(nodoP->Encuesta_id == Encuesta_Id){
            existeEncuesta = 1;
            if(nodoP->Procesada == 1){
                //si la encuesta ya fue procesada
                printf("La encuesta %d ya ha sido procesada.\n", Encuesta_Id);
                procesada = 1;
            }else{
                //mostrar datos de la encuesta
                printf("Procesando encuesta con ID %d...\n", Encuesta_Id);
                printf("Denominación: %s\n", nodoP->Denominacion);
                printf("Fecha: %02d/%d\n", nodoP->Encuesta_Mes, nodoP->Anio);
            }
        }
        apilar(&nodoP,&auxP);
    }
    while(!vaciaP(auxP)){
        desapilar(&nodoP,&auxP);
        apilar(&nodoP,tp);
    }
    if(existeEncuesta == 0 || procesada == 1){
        printf("Encuesta con ID %d no encontrada o ya preocesada.\n", Encuesta_Id);
        getch();
        return;
    }
    //carga de los nodos de encuesta en la pila
    filtrarRespondidas(A, Encuesta_Id, tpRespondida);
    //tenemos solo los datos de las respuestas respondidas de la encuesta

    struct pilaRespondida *nodoR = NULL, *tpRespondidaAux = NULL;
    while(!vaciaPilaRespondida(*tpRespondida)){
        desapilarRespondida(tpRespondida, &nodoR);
        //procesar la encuesta
        struct lPregunta *preguntaActual = *LP;
        while (preguntaActual != NULL) {
            if (preguntaActual->Encuesta_Id == Encuesta_Id && 
                preguntaActual->Pregunta_Id == nodoR->Pregunta_Id) {
                //mostrar datos de la pregunta
                printf("\n=== Pregunta ===\n");
                printf("Pregunta ID: %d\n", preguntaActual->Pregunta_Id);
                printf("Texto: %s\n", preguntaActual->Pregunta);
                printf("Ponderación: %.2f\n", preguntaActual->Ponderacion);

                //recorrer la lista de respuestas
                struct lRespuesta *respuestaActual = *L2;
                while (respuestaActual != NULL) {
                    if (respuestaActual->Pregunta_Id == preguntaActual->Pregunta_Id &&
                        respuestaActual->Respuesta_Id == nodoR->Respuesta_Id) {
                        //mostrar datos de las respuesta
                        printf("Respuesta ID: %d | Nro: %d | Texto: %s | Ponderación: %.2f | Elegida: %s\n",
                               respuestaActual->Respuesta_Id,
                               respuestaActual->Respuesta_Nro,
                               respuestaActual->Respuesta,
                               respuestaActual->Ponderacion,
                               respuestaActual->Elegida ? "Sí" : "No");

                        respuestaActual->Elegida = 1; // ya que fue elegida
                        
                        //acumulamos la ponderacion de la pregunta
                        struct lPponderaciones *nodoPonderacion = NULL;
                        if (nuevoLPreguntaPonderacion(&nodoPonderacion)) {
                            nodoPonderacion->Pregunta_Id = preguntaActual->Pregunta_Id;
                            //los datos se cargan siguiendo el indice de ponderacion de la pregunta
                            nodoPonderacion->Ponderacion_Total = preguntaActual->Ponderacion * respuestaActual->Ponderacion;
                            nodoPonderacion->sgte = NULL;

                            //insertar en la lista de ponderaciones
                            insertarLPreguntaPonderacion(&nodoPonderacion, LPponder);
                        } else {
                            printf("Error al reservar memoria para la ponderación de la pregunta.\n");
                        }
                    }
                    //recorrer la lista de respuestas
                    respuestaActual = respuestaActual->sgte;
                }
            }
            //avanzar a la siguiente pregunta
            preguntaActual = preguntaActual->sgte;
        }

        apilarRespondida(&nodoR, &tpRespondidaAux);   
    }
    //devolver la pila a su estado original
    while(!vaciaPilaRespondida(tpRespondidaAux)){
        desapilarRespondida(&tpRespondidaAux, &nodoR);
        apilarRespondida(&nodoR, tpRespondida);
    }
    //mostrar las ponderaciones de las preguntas y luego sumarlas
    struct lPponderaciones *ListaFinal = NULL;
    acumularListaPonderaciones(LPponder, &ListaFinal);
    printf("\n=== Ponderaciones de las preguntas procesadas ===\n");
    mostrarPonderaciones(&ListaFinal);
    //marcar la encuesta como procesada
    while(!vaciaP(*tp)){
        desapilar(&nodoP,tp);
        if(nodoP->Encuesta_id == Encuesta_Id){
            nodoP->Procesada = 1; //marcar como procesada
            printf("Encuesta con ID %d ha sido procesada.\n", Encuesta_Id);
            sumarPonderacionesEncuesta(Encuesta_Id, &ListaFinal);
        }
        apilar(&nodoP,&auxP);
    }
    while(!vaciaP(auxP)){
        desapilar(&nodoP,&auxP);
        apilar(&nodoP,tp);
    }

}
void apilarRespondida(struct pilaRespondida **nodo, struct pilaRespondida **tope) {
    if (*tope == NULL) {
        *tope = *nodo;
    } else {
        (*nodo)->sgte = *tope;
        *tope = *nodo;
    }
}
int nuevoNodoPilaRespondida(struct pilaRespondida **nodo) {
    *nodo = (struct pilaRespondida *) malloc(sizeof(struct pilaRespondida));
    return (*nodo == NULL) ? 0 : 1;
}
void desapilarRespondida(struct pilaRespondida **tope, struct pilaRespondida **nodo) {
    if (*tope != NULL) {
        *nodo = *tope;
        *tope = (*tope)->sgte;
        (*nodo)->sgte = NULL; // Desconectar el nodo desapilado
    } else {
        *nodo = NULL; // Si la pila está vacía, asignar NULL al nodo
    }
}
int vaciaPilaRespondida(struct pilaRespondida *tp) {
    int band = 0;
	if (tp == NULL){
		band = 1;
	}
	return band;
}
void mostrarPilaRespondida(struct pilaRespondida **tope) {
    struct pilaRespondida *aux=NULL, *nodo=NULL;
    while (!vaciaPilaRespondida(*tope)) {
        desapilarRespondida(tope, &nodo);
        printf("Encuesta_Id: %d, Pregunta_Id: %d, Respuesta_Id: %d, Anio: %d, Mes: %d, Encuestador_id: %d, Respondida_Id: %d, Dia: %d\n",
               nodo->Encuesta_Id,
               nodo->Pregunta_Id,
               nodo->Respuesta_Id,
               nodo->Anio,
               nodo->Encuesta_Mes,
               nodo->Encuestador_id,
               nodo->EncuestaRespondida_Id,
               nodo->dia);
        apilarRespondida(&nodo, &aux); // Guardar el nodo en un auxiliar para restaurar la pila
    }
    // Restaurar la pila original
    while (!vaciaPilaRespondida(aux)) {
        desapilarRespondida(&aux, &nodo);
        apilarRespondida(&nodo, tope);
    }
}
void filtrarRespondidas(struct arbol_respondidas **A, int Encuesta_Id, struct pilaRespondida **tope){

    if((*A) != NULL){
        
        if((*A)->Encuesta_Id == Encuesta_Id){
            struct pilaRespondida *nodoPilaRespondida;
            if (nuevoNodoPilaRespondida(&nodoPilaRespondida)){    
                nodoPilaRespondida->Encuesta_Id = (*A)->Encuesta_Id;
                nodoPilaRespondida->Pregunta_Id = (*A)->Pregunta_Id;
                nodoPilaRespondida->Respuesta_Id = (*A)->Respuesta_Id;
                nodoPilaRespondida->Anio = (*A)->Anio;
                nodoPilaRespondida->Encuesta_Mes = (*A)->Encuesta_Mes;
                nodoPilaRespondida->Encuestador_id = (*A)->Encuestador_id;
                nodoPilaRespondida->EncuestaRespondida_Id = (*A)->EncuestaRespondida_Id;
                nodoPilaRespondida->dia = (*A)->dia;
                nodoPilaRespondida->sgte = NULL;      
                apilarRespondida(&nodoPilaRespondida, tope);
            }
        }
    
        filtrarRespondidas(&(*A)->izq, Encuesta_Id, tope);
        filtrarRespondidas(&(*A)->der, Encuesta_Id, tope);
    }
}
int vaciaLP(struct lPponderaciones *L) {
    return (L == NULL);
}
void desapilarLP(struct lPponderaciones **nodo, struct lPponderaciones **L) {
    if (*L != NULL) {
        *nodo = *L;
        *L = (*L)->sgte;
        (*nodo)->sgte = NULL; // Desconectar el nodo desapilado
    } else {
        *nodo = NULL; // Si la lista está vacía, asignar NULL al nodo
    }
}
void apilarLP(struct lPponderaciones **nodo, struct lPponderaciones **L) {
    if (*L == NULL) {
        *L = *nodo;
    } else {
        (*nodo)->sgte = *L;
        *L = *nodo;
    }
}
void acumularListaPonderaciones(struct lPponderaciones **L, struct lPponderaciones **ListaFinal) {
    struct lPponderaciones *nodo = NULL, *encontrado = NULL;
    while (!vaciaLP(*L)) {
        desapilarLP(&nodo, L);
        buscarLPreguntaPonderacion(ListaFinal, nodo->Pregunta_Id, &encontrado);
        if (encontrado == NULL) {
            // Crear nuevo nodo y agregarlo a la lista final
            struct lPponderaciones *nuevo = NULL;
            if (nuevoLPreguntaPonderacion(&nuevo)) {
                nuevo->Encuesta_Id = nodo->Encuesta_Id;
                nuevo->Pregunta_Id = nodo->Pregunta_Id;
                nuevo->Ponderacion_Total = nodo->Ponderacion_Total;
                nuevo->sgte = NULL;
                insertarLPreguntaPonderacion(&nuevo, ListaFinal);
            } else {
                printf("Error al reservar memoria para la ponderación de la pregunta.\n");
                free(nodo);
                return;
            }
        } else {
            // Acumular ponderación
            encontrado->Ponderacion_Total += nodo->Ponderacion_Total;
        }
        free(nodo);
    }
}
void sumarPonderacionesEncuesta(int Encuesta_Id, struct lPponderaciones **L) {
    struct lPponderaciones *nodo = NULL, *aux = NULL;
    float suma = 0.0;
    while (!vaciaLP(*L)) {
        desapilarLP(&nodo, L);
        suma += nodo->Ponderacion_Total;
        apilarLP(&nodo, &aux);
    }
    // Restaurar la lista original
    while (!vaciaLP(aux)) {
        desapilarLP(&nodo, &aux);
        apilarLP(&nodo, L);
    }
    printf("La suma de las ponderaciones para la encuesta %d es: %.2f\n", Encuesta_Id, suma);
}
void mostrarPonderaciones(struct lPponderaciones **L) {
    struct lPponderaciones *nodo = NULL, *aux = NULL;
    while (!vaciaLP(*L)) {
        desapilarLP(&nodo, L);
        printf("Pregunta ID: %d, Ponderación Total de la pregunta: %.2f\n",
               nodo->Pregunta_Id,
               nodo->Ponderacion_Total);
        apilarLP(&nodo, &aux);
    }
    // Restaurar la lista original
    while (!vaciaLP(aux)) {
        desapilarLP(&nodo, &aux);
        apilarLP(&nodo, L);
    }
}
