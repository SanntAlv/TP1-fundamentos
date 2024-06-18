#ifndef LISTA_H
#define LISTA_H

//defino el struct lata
typedef struct lata {
	char marca[20];
	char color[20];
	float tamanio;
	struct lata *pLata;
} lata;

// Función para crear una lista
lata* crearProducto(char *marca, char *color, float tamanio);

// Función para crear un nodo
void agregarProducto(lata **cabeza, char *marca, char *color, float cantidad);

//Función para eleminar un nodo
void borrarProducto(lata **cabeza, char *marca, char *color, float tamanio);


#endif