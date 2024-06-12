#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lata {
	char marca[20];
	char color[20];
	float tamanio;
	struct lata *pLata;
} lata;

lata* crearProducto(char *marca, char *color, float tamanio) {
    lata *nuevolata = (lata*)malloc(sizeof(lata));
    if (nuevolata == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    strcpy(nuevolata->marca, marca);
    strcpy(nuevolata->color, color);
    nuevolata->tamanio = tamanio;
    nuevolata->pLata = NULL;
    return nuevolata;
}

void agregarProducto(lata **cabeza, char *marca, char *color, float cantidad) {
    lata *nuevolata = crearProducto(marca, color, cantidad);
    nuevolata->pLata = *cabeza;
    *cabeza = nuevolata;
}

void leerArchivo(const char *nombreArchivo, lata **cabeza) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        exit(1);
    }

    char marca[20];
    char color[20];
    float tamanio;
    while (fscanf(archivo, "%49s\n%49s\n%f\n", marca, color, &tamanio) == 3) {
        agregarProducto(cabeza, marca, color, tamanio);
    }

    fclose(archivo);
}

void imprimirLista(lata *cabeza) {
    lata *temp = cabeza;
    while (temp != NULL) {
        printf("Marca: %s, Color: %s, Cantidad: %.2f\n", temp->marca, temp->color, temp->tamanio);
        temp = temp->pLata;
    }
}

void liberarLista(lata *cabeza) {
    lata *temp;
    while (cabeza != NULL) {
        temp = cabeza;
        cabeza = cabeza->pLata;
        free(temp);
    }
}

void guardarEnArchivo(const char *nombreArchivo, lata *cabeza) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo para escribir.\n");
        exit(1);
    }

    lata *temp = cabeza;
    while (temp != NULL) {
        fprintf(archivo, "%s\n%s\n%.2f\n\n", temp->marca, temp->color, temp->tamanio);
        temp = temp->pLata;
    }

    fclose(archivo);
}

void validacionInt(int opcion, int scanfopcion){
    if(scanfopcion!=1){      
            printf("\n\nERROR, el dato ingresado no es valido\n\n");
            while(getchar()!='\n'){    
                opcion=1;
                continue;
            }
    }
}

void validacionFloat(float *opcion, int scanfopcion){
    if(scanfopcion!=1){      
            printf("\n\nERROR, el dato ingresado no es valido\n\n");
            while(getchar()!='\n');
            *opcion=-1;
    }
}

int main(){
	
	lata *cabeza=NULL;
	leerArchivo("stock.txt",&cabeza);
	int opcion;
	int validarOpcion;
	while (opcion!=-1){
		printf("\nIngrese una opcion: ");
		validarOpcion=scanf("%d",&opcion);
        validacionInt(opcion,validarOpcion);
		if(opcion==1){
			char marca[20];
			char color[20];
			float tamanio;
            int validarOpcion;
			printf("\nIngrese el nombre de la marca de la lata: ");
			scanf("%19s",marca);

			printf("\nIngrese el color de la lata: ");
			scanf("%19s",color);

            do {
                printf("\nIngrese el tamaño de la lata: ");
                validarOpcion = scanf("%f", &tamanio);
                validacionFloat(&tamanio, validarOpcion);
            } while (tamanio == -1);

			agregarProducto(&cabeza, marca, color, tamanio);
			//imprimirLista(cabeza);
            printf("\n[PRODUCTO AGREGADO]\n");
			
		}else if(opcion==2){
            char marca[20];
			char color[20];
			float tamanio;

            printf("\nIngrese el nombre de la marca de la lata: ");
			scanf("%s",marca);

			printf("\nIngrese el color de la lata: ");
			scanf("%s",color);

			printf("\nIngrese el tamaño de la lata: ");
			scanf("%f",&tamanio);


        }
	}
	

	liberarLista(cabeza);

	printf("\n\n[**********PROGRAMA FINALIZADO**********]\n\n\n");

    return 0;

}
