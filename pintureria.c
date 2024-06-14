#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        printf("\n%s\n%s\n%.2f\n", temp->marca, temp->color, temp->tamanio);
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

void borrarProducto(lata **cabeza, char *marca, char *color, float tamanio){
    lata *temp = *cabeza;
    lata *prev = NULL;
    // Si la cabeza contiene el producto a eliminar
    if (temp != NULL && strcmp(temp->marca, marca) == 0 && strcmp(temp->color, color) == 0 && temp->tamanio == tamanio) {
        *cabeza = temp->pLata;
        free(temp);
        printf("\n[PRODUCTO BORRADO]\n");
        return;
    }

    // Buscar el producto en la lista
    while (temp != NULL && (strcmp(temp->marca, marca) != 0 || strcmp(temp->color, color) != 0 || temp->tamanio != tamanio)) {
        prev = temp;
        temp = temp->pLata;
    }

    // Si el producto no se encuentra en la lista
    if (temp == NULL) {
        printf("\n[PRODUCTO NO ENCONTRADO]\n");
        return;
    }

    // Desconectar el nodo de la lista
    prev->pLata = temp->pLata;
    free(temp);

}


void validacionInt(int opcion, int scanfopcion){
    if(scanfopcion!=1){      
            printf("\n\nERROR, el dato ingresado no es valido, debe ser un numero entero\n\n");
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

void convertirMayuscula(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}
int main(){
	
	lata *cabeza=NULL;
	leerArchivo("stock.txt",&cabeza);
	int opcion;
	int validarOpcion;

    printf("\n*****[MENU PRINCIPAL]*****\n\nIngrese 1 para agregar un producto\nIngrese 2 para eliminar un producto\nIngrese 3 para ver la disponivilidad de un color\n");
	
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
            convertirMayuscula(marca);

			printf("\nIngrese el color de la lata: ");
			scanf("%19s",color);
            convertirMayuscula(color);

            do {
                printf("\nIngrese el tamaño de la lata: ");
                validarOpcion = scanf("%f", &tamanio);
                validacionFloat(&tamanio, validarOpcion);
            } while (tamanio == -1);

			agregarProducto(&cabeza, marca, color, tamanio);
			imprimirLista(cabeza);
            printf("\n[PRODUCTO AGREGADO]\n");
			
		}else if(opcion==2){

            char marca[20];
			char color[20];
			float tamanio;

            printf("\nIngrese el nombre de la marca de la lata: ");
			scanf("%19s",marca);
            convertirMayuscula(marca);

			printf("\nIngrese el color de la lata: ");
			scanf("%19s",color);
            convertirMayuscula(color);

            do {
                printf("\nIngrese el tamaño de la lata: ");
                validarOpcion = scanf("%f", &tamanio);
                validacionFloat(&tamanio, validarOpcion);
            } while (tamanio == -1);

            imprimirLista(cabeza);
            borrarProducto(&cabeza, marca, color, tamanio);

        }else if(opcion==3){

            char color[20];
            char opcionC;

            printf("\nIngrese el color: ");
            scanf("%19s",color);

            //aca va la funcion que buscas todas las latas de ese color

            printf("\n¿Desea guardar esta informacion en un archivo de texto(s/n)?: ");
            scanf("%c",&opcionC);//por aca hay un error

            if (opcionC == 's' || opcionC == 'S') {
                //aca va la funcion para guardar
            } else if (opcionC == 'n' || opcionC == 'N') {
                printf("\nNo se guardará la información\n");
            } 

        }else{
            printf("\n\nERROR, ingrese una opcion valida\n\n");
            while(getchar()!='\n'){
                opcion=1;
                continue;
            }
        }
	}   

	liberarLista(cabeza);

	printf("\n\n[**********PROGRAMA FINALIZADO**********]\n\n\n");

    return 0;

}
