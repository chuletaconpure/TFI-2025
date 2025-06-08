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
void CrearCopiaNodoA(struct arbol_respondidas **A, int EncuestaId, struct arbol_respondidas **nodo);

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
void modificarEncuesta(struct pEncuesta **tp, int id);
void eliminarEncuesta(struct pEncuesta **tp, int id);
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
//funciones de carga en CSV
void cargar_respuestas_csv(struct lRespuesta *L);
void cargar_preguntas_csv(struct lPregunta *L);
void cargar_encuestas_csv(struct pEncuesta **tp);
// funciones de extraccion de datos desde CSV
void extraer_respuestas_csv(struct lRespuesta **L);
void extraer_preguntas_csv(struct lPregunta **L);
void extraer_encuestas_csv(struct pEncuesta **tp);
void extraer_respondidas_csv(struct arbol_respondidas **A);

//Desarrollo de las consignas:
void procesarEncuesta(struct pEncuesta **tp, int Encuesta_Id, struct lPregunta *LP, struct lRespuesta **L2, struct arbol_respondidas *A);
int buscarIdEncuestaArbol(struct arbol_respondidas *A, int Encuesta_Id);
int buscarIdPreguntaLP(struct lPregunta *LP, int Pregunta_Id);
int buscarIdRespuestaL2(struct lRespuesta *L2, int Respuesta_Id);

//el arbol representara la respuesta elegida de cada pregunta de la encuesta
struct arbol_respondidas{
    int Encuesta_Id;
    int Pregunta_Id;
    int Respuesta_Id; //representa el id de la respuesta elegida
    int Anio;
    int Encuesta_Mes;
    int dia;
    int encuestador_Id;
    int id_encuesta_respondida; //representa el id de la persona que respondio la encuesta
    struct arbol_respondidas *der;
    struct arbol_respondidas *izq;
};

int main(){
    //menu principal
    //variables
    struct pEncuesta *tp=NULL, *nodoP=NULL;
    struct lRespuesta *L2=NULL,*nodoL2=NULL;
    struct lPregunta *LP=NULL, *nodoLP=NULL;
    struct arbol_respondidas *A=NULL, *nodoA=NULL;
    //inicializacion de las estructuras
    extraer_respuestas_csv(&L2);
    extraer_preguntas_csv(&LP);
    extraer_encuestas_csv(&tp);
    int id;
    //funcion system para permitir mas caracteres y caracteres especiales
    system("chcp 65001");
    colorMenu();
	int apagado=0;
	char w=1;
	int select=1,tam=11;
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
		if(select==7) 
            printf("\n>> eliminar encuesta"); 
        else 
            printf("\n   eliminar encuesta");
        if(select==8)
            printf("\n>> crear encuesta"); 
        else 
            printf("\n   crear encuesta");
        if(select==9) 
            printf("\n>> mostrar encuestas"); 
        else 
            printf("\n   mostrar encuestas");
        if(select==10) 
            printf("\n>> modificar encuesta"); 
        else 
            printf("\n   modificar encuesta");
        if(select==11){
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
                system("cls");
                printf("Ingrese ID de encuesta a eliminar: ");
                scanf("%d", &id);
                eliminarEncuesta(&tp, id);
                getch();
            }
            if(select==8){
                system("cls");
                crearEncuesta(&tp);
                getch();
            }
            if(select==9){
                system("cls");
                mostrarEncuestas(&tp);
                getch();
            }
            if(select==10){
                system("cls");
                printf("Ingrese ID de encuesta a modificar: ");
                scanf("%d", &id);
                modificarEncuesta(&tp, id);
                getch();
            }
            if(select==11){
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
        if ((*A)->Encuesta_Id == (*nodo)->Encuesta_Id) {
            printf("Ya esta en el arbol_respondidas\n");
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
// Extrae (elimina y retorna) el nodo con Encuesta_Id dado, manteniendo el árbol ordenado.
// Devuelve NULL si no lo encuentra.
struct arbol_respondidas *extraerNodoA(struct arbol_respondidas **A, int Encuesta_Id) {//funcion experimental
    if (*A == NULL) {
        return NULL;
    }
    if (Encuesta_Id < (*A)->Encuesta_Id) {
        return extraerNodoA(&(*A)->izq, Encuesta_Id);
    } else if (Encuesta_Id > (*A)->Encuesta_Id) {
        return extraerNodoA(&(*A)->der, Encuesta_Id);
    } else {
        // Nodo encontrado
        struct arbol_respondidas *extraido = *A;
        // Caso 1: solo hijo derecho o ninguno
        if ((*A)->izq == NULL) {
            *A = (*A)->der;
        }
        // Caso 2: solo hijo izquierdo
        else if ((*A)->der == NULL) {
            *A = (*A)->izq;
        }
        // Caso 3: dos hijos
        else {
            // Buscar el mayor de los menores (máximo del subárbol izquierdo)
            struct arbol_respondidas **mayor = &(*A)->izq;
            while ((*mayor)->der != NULL) {
                mayor = &(*mayor)->der;
            }
            // Copiar datos del sucesor al nodo actual
            (*A)->Encuesta_Id = (*mayor)->Encuesta_Id;
            (*A)->Pregunta_Id = (*mayor)->Pregunta_Id;
            (*A)->Respuesta_Id = (*mayor)->Respuesta_Id;
            (*A)->Anio = (*mayor)->Anio;
            (*A)->Encuesta_Mes = (*mayor)->Encuesta_Mes;
            (*A)->dia = (*mayor)->dia;
            (*A)->encuestador_Id = (*mayor)->encuestador_Id;
            (*A)->id_encuesta_respondida = (*mayor)->id_encuesta_respondida;
            // Recursivamente eliminar el sucesor
            extraido = extraerNodoA(mayor, (*mayor)->Encuesta_Id);
        }
        // Desconectar el nodo extraído
        extraido->izq = NULL;
        extraido->der = NULL;
        return extraido;
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
void CrearCopiaNodoA(struct arbol_respondidas **A, int EncuestaId, struct arbol_respondidas **nodo){
    if(*A == NULL){
        printf("No se encontro.\n");
        return;
    }else{
        if((*A)->Encuesta_Id == EncuestaId){
            if(nuevoA(nodo)){
                (*nodo)->Encuesta_Id = (*A)->Encuesta_Id;
                (*nodo)->Pregunta_Id = (*A)->Pregunta_Id;
                (*nodo)->Respuesta_Id = (*A)->Respuesta_Id;
                (*nodo)->Anio = (*A)->Anio;
                (*nodo)->Encuesta_Mes = (*A)->Encuesta_Mes;
                (*nodo)->dia = (*A)->dia;
                (*nodo)->encuestador_Id = (*A)->encuestador_Id;
                (*nodo)->id_encuesta_respondida = (*A)->id_encuesta_respondida;
                (*nodo)->der = NULL;
                (*nodo)->izq = NULL;
            }
        }else if(EncuestaId < (*A)->Encuesta_Id){
            CrearCopiaNodoA(&(*A)->izq, EncuestaId, nodo);
        }else{
            CrearCopiaNodoA(&(*A)->der, EncuestaId, nodo);
        }
    }
}
//CRUD Encuestas
void crearEncuesta(struct pEncuesta **tp) {
    struct pEncuesta *nueva = NULL;
    if (nuevoP(&nueva)) {
        int c;
        
        // asignar ID automáticamente
        if(*tp == NULL){
            nueva->Encuesta_id = 1;
        }else
        nueva->Encuesta_id = buscarUltimoIdEncuesta(tp) + 1;    
        while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer
        // solicitar datos de la encuesta
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
void extraer_respondidas_csv(struct arbol_respondidas **A) {
    FILE *archivo = fopen("respondidas.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de respondidas.\n");
        return;
    }
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        struct arbol_respondidas *nueva = NULL;
        if (nuevoA(&nueva)) {
            sscanf(linea, "%d;%d;%d;%d;%d;%d",
                   &nueva->Encuesta_Id,
                   &nueva->Pregunta_Id,
                   &nueva->Respuesta_Id,
                   &nueva->Anio,
                   &nueva->Encuesta_Mes,
                   &nueva->dia);
            nueva->der = NULL;
            nueva->izq = NULL;
            insertarA(A, &nueva);
        }
    }
    fclose(archivo);
}

//desarrollo de las consignas
void procesarEncuesta(struct pEncuesta **tp, int Encuesta_Id, struct lPregunta *LP, struct lRespuesta **L2, struct arbol_respondidas *A) {
    struct pEncuesta *nodo = NULL, *aux = NULL;
    struct arbol_respondidas *nodoA = NULL, *auxA = NULL;
    auxA = A;
    // Verificar si la encuesta existe
    int encontrada = 0;

    // Buscar el ID de encuesta en la pila
    while (!vaciaP(*tp)) {
        desapilar(&nodo, tp);
        if (nodo->Encuesta_id == Encuesta_Id) {
            encontrada = 1;
        }
        apilar(&nodo, &aux);
    }
    // Restaurar la pila original
    while (!vaciaP(aux)) {
        desapilar(&nodo, &aux);
        apilar(&nodo, tp);
    }

    if (!encontrada) {
        printf("No existe una encuesta con el ID %d.\n", Encuesta_Id);
        return;
    } else {
        printf("Encuesta con ID %d existente.\n", Encuesta_Id);
        if(nodo->Procesada > 0){
            printf("La encuesta ya ha sido procesada.\n");
            return;
        }else{
            //una vez encontrado buscamos que los datos de ese nodo del arbol tengan datos coincidentes en preguntas y respuestas
            if(buscarIdEncuestaArbol(A, Encuesta_Id) && buscarIdPreguntaLP(LP, A->Pregunta_Id) && buscarIdRespuestaL2(*L2, A->Respuesta_Id)){
                //hasta aca llegue, no se si es funcional o no, pero la idea es que si se encuentra el id de encuesta en el arbol, y el id de pregunta en la lista de preguntas y el id de respuesta en la lista de respuestas, se pueda procesar la encuesta
            }
        }
    }
}
int buscarIdEncuestaArbol(struct arbol_respondidas *A, int Encuesta_Id) {
    if (A == NULL) {
        return 0; // No encontrado
    }
    if (A->Encuesta_Id == Encuesta_Id) {
        return 1; // Encontrado
    }
    if (Encuesta_Id < A->Encuesta_Id) {
        return buscarIdEncuestaArbol(A->izq, Encuesta_Id);
    } else {
        return buscarIdEncuestaArbol(A->der, Encuesta_Id);
    }
}
int buscarIdPreguntaLP(struct lPregunta *LP, int Pregunta_Id) {
    while (LP != NULL) {
        if (LP->Pregunta_Id == Pregunta_Id) {
            return 1; // Encontrado
        }
        LP = LP->sgte;
    }
    return 0; // No encontrado
}
int buscarIdRespuestaL2(struct lRespuesta *L2, int Respuesta_Id) {
    while (L2 != NULL) {
        if (L2->Respuesta_Id == Respuesta_Id) {
            return 1; // Encontrado
        }
        L2 = L2->sgte;
    }
    return 0; // No encontrado
}
