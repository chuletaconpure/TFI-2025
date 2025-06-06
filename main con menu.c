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

//pila y sus funciones, pila de encuestas
struct pila {
	int Encuesta_id;
    char Denominacion[50];
    int Encuesta_Mes;
    int Anio;
    int Procesada;
	struct pila *sgte;
};
void apilar(struct pila **, struct pila **);
void desapilar(struct pila **, struct pila **);
int vaciaP(struct pila *);
int nuevoP(struct pila **nuevo);
void mostrarP(struct pila **tope);

//lista1 y sus funciones, lista de preguntas
struct lista1{
    int Encuesta_Id;
    int Pregunta_Id;
    char Pregunta[100];
    float Ponderacion;
    struct lista1 *sgte;
};
int nuevoL1(struct lista1 **nodo);
struct lista1 *insertarL1(struct lista1 **nodo, struct lista1 **L);
struct lista1 *mostrarListaL1(struct lista1 **L);

//lista2 y sus funciones, lista de respuestas
struct lista2{
    int Respuesta_Id;
    int Pregunta_Id;
    int Respuesta_Nro;
    char Respuesta [50];
    float Ponderacion;
    int Elegida;
    struct lista2 *sgte;
};
int nuevoL2(struct lista2 **nodo);
struct lista2 *insertarL2(struct lista2 **nodo, struct lista2 **L);
struct lista2 *mostrarListaL2(struct lista2 **L);

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

//Desarrollo de las consignas:
//usaremos una lista que contenga todos los datos de la encuesta a mostrar cumpliendo la consigna b,
//la lista contendra una lista de preguntas y una lista de respuestas que perteneceran a la encuesta
//durante el proceso de carga podremos calcular la ponderacion de todas las respuestas para calcular la ponderacion total de la encuesta
struct arbol{
    int Encuesta_Id;
    struct lista1 *L_Preguntas;
    struct lista2 *L_Respuestas;
    float Ponderacion_total;
    struct arbol *der;
    struct arbol *izq;
};



int main(){
    //menu principal

    //funcion system para permitir mas caracteres y caracteres especiales
    system("chcp 65001");
    colorMenu();
	int apagado=0;
	char w=1;
	int select=1,tam=2;
	while(!apagado){
		system("cls");
		printf("<<<<TP integrador>>>>\n");
        letraR();
        printf("---------------------\n");
        colorMenu();
        //menu visual para seleccionar una opcion
		if(select==1){
			printf(">> seleccionar una encuesta por id");
		}else
			printf("   seleccionar una encuesta por id");
		if(select==2){
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
				getch();
			}
			if(select==2)
				apagado++;
		}
	}
    return 0;
}


//funciones pila 1
void apilar (struct pila **n, struct pila **tp){
	(*n)->sgte = (*tp);
	(*tp) = (*n);
	(*n) = NULL;
}
void desapilar (struct pila **n, struct pila **tp){
	(*n) = (*tp);
	(*tp) = (*tp)->sgte;
	(*n)->sgte = NULL;
}
int vaciaP (struct pila *tp){
	int band = 0;
	if (tp == NULL){
		band = 1;
	}
	return band;
}
int nuevoP(struct pila **nuevo){
    *nuevo = (struct pila *) malloc(sizeof(struct pila));
    return (*nuevo == NULL)? 0 : 1;
}
void mostrarP(struct pila **tope){
    struct pila *nodo=NULL,*topeAux=NULL;
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

//funciones lista 1
int nuevoL1(struct lista1 **nodo){
    *nodo = (struct lista1 *) malloc(sizeof(struct lista1));
    return (*nodo == NULL)? 0 : 1;
}
struct lista1 *insertarL1(struct lista1 **nodo, struct lista1 **L){
    if((*L)!=NULL){
        if((*nodo)->Pregunta_Id < (*L)->Pregunta_Id){
            (*nodo)->sgte=(*L);
            (*L)=(*nodo);
        }else
            (*L)->sgte=insertarL1(nodo,&(*L)->sgte);
    }else{
        (*L)=(*nodo);
    }
    return (*L);
}
struct lista1 *mostrarListaL1(struct lista1 **L){
    if((*L)!=NULL){
        //contenido de la lista
        mostrarListaL1(&(*L)->sgte);
    }
    return (*L);
}

//funciones lista 2
int nuevoL2(struct lista2 **nodo){
    *nodo = (struct lista2 *) malloc(sizeof(struct lista2));
    return (*nodo == NULL)? 0 : 1;
}
struct lista2 *insertarL2(struct lista2 **nodo, struct lista2 **L){
    if((*L)!=NULL){
        if((*nodo)->Respuesta_Id < (*L)->Respuesta_Id){
            (*nodo)->sgte=(*L);
            (*L)=(*nodo);
        }else
            (*L)->sgte=insertarL2(nodo,&(*L)->sgte);
    }else{
        (*L)=(*nodo);
    }
    return (*L);
}
struct lista2 *mostrarListaL2(struct lista2 **L){
    if((*L)!=NULL){
        //contenido de la lista
        mostrarListaL2(&(*L)->sgte);
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
