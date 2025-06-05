#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

struct arbol{
    int id;
    char nombre[50];
    struct arbol *der;
    struct arbol *izq;
};
struct arbol *insertarA(struct arbol **A, struct arbol **nodo);
void mostrarA(struct arbol **A);
int nuevoA(struct arbol **nodo);
void buscarA(int dato, struct arbol *r);
void busBorr(struct arbol **r, int dato);
void borrarNodo(struct arbol **nodo);
struct arbol *eliminarA(struct arbol **r);

int main(){
    struct arbol *A=NULL, *nodo=NULL;
    int id;
    char nombre[50];
    printf("ingrese un pene: ");scanf("%i",&id);
    while (id){
        printf("ingrese un nombre: ");scanf("%s",&nombre);
        if(!nuevoA(&nodo)){
            printf("ERROR de memoria");
            getch();
            return 1;
        }else{
            nodo->izq=nodo->der=NULL;
            strcpy(nodo->nombre,nombre);
            nodo->id=id;
            A = insertarA(&A, &nodo);
        }
        printf("ingrese un pene: ");scanf("%i",&id);
    }
    mostrarA(&A);
    printf("\n\ningrese un id a buscar: ");scanf("%i",&id);
    buscarA(id,A);
    system("cls");
    printf("\n\ningrese un id a borrar: ");scanf("%i",&id);
    busBorr(&A,id);
    mostrarA(&A);
    printf("\n\na continuacion el arbol se eliminara y se verificara la eliminacion\n-(presione una tecla)-\nLa lista se elimino?: ");
    eliminarA(&A);
    getch();
    if(A==NULL){
        printf("si xd");
    }else
        printf("no xd");
    mostrarA(&A);
    getch();
    return 0;
}
struct arbol *insertarA(struct arbol **A, struct arbol **nodo) {
    if ((*A) != NULL) {
        if ((*A)->id == (*nodo)->id) {
            printf("Ya esta en el arbol\n");
        } else if ((*nodo)->id < (*A)->id) {
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
        printf("\nid: %i", (*A)->id);
        mostrarA(&(*A)->der);
    }
}
int nuevoA(struct arbol **nodo){
    *nodo = (struct arbol *) malloc(sizeof(struct arbol));
    return (*nodo == NULL)? 0 : 1;
}
void buscarA(int dato, struct arbol *r) {
    if (r != NULL) {
        if (dato == r->id) {
            printf("Dato encontrado: ");puts(r->nombre);
        } else if (dato < r->id) {
            buscarA(dato,r->izq);
        } else {
            buscarA(dato,r->der);
        }
    } else {
        printf("El dato buscado no se encuentra en el árbol\n");
    }
}
void busBorr(struct arbol **r, int dato) {
    if (*r != NULL) {
        if (dato == (*r)->id) {
            borrarNodo(r);
        } else if (dato < (*r)->id) {
            busBorr(&(*r)->izq, dato);
        } else {
            busBorr(&(*r)->der, dato);
        }
    } else {
        printf("No se encontró\n");
    }
}
void borrarNodo(struct arbol **nodo) {
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
        (*nodo)->id = (*mayor)->id;
        borrarNodo(mayor);
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
