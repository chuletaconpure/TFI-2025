#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

/*/ consignas 1: 
a. Calcular ponderación de una encuesta X: El sistema deberá pedir el ingreso de
    un id de encuesta por teclado y el sistema deberá calcular la ponderación total
    de la encuesta, así como su conformación, o sea la ponderación obtenida en
    cada pregunta.
b. Mostrar encuestas respondidas una a una. Encuesta completa (todas las
    preguntas, cada una con todas las respuestas y diferenciar de alguna manera
    la respondida. /*/

/*/consignas 2: 
El sistema deberá permitir la impresión de un listado de todas las encuestas 
respondidas de una determinada encuesta en particular, las mismas deberán estar ordenadas por id de encuesta respondida.
IMPORTANTE: Se deberá cargar un árbol AB (árbol binario) de forma ordenada y a posterior imprimir haciendo el recorrido correspondiente./*/

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
    struct arbol *isq;
};


int main(){

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
