#include <stdio.h>
#include <stdlib.h>

typedef struct snodo{
	int valor;
	struct snodo *siguiente;
}tnodo;

typedef tnodo *tpuntero;

void insertarLista (tpuntero *cabeza, int e){
	tpuntero nuevo;
	nuevo=malloc(sizeof(tnodo));
	nuevo->valor=e;
	nuevo->siguiente=*cabeza;
	*cabeza=nuevo;
}



int main(){

    printf("Hola Mundo\n");
    return 0;

}
