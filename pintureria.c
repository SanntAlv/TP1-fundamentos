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

void disponibilidadColor(lata *cabeza, char* color){

    typedef struct {
        char marca[20];
        float tamanio;
    }suma;
    
    lata *temp = cabeza;
    int encontrado;
    suma sumas[20];
    int cantSumas=0;
    int marcaEncotrada=0;

    while(temp!=NULL){
        if (strcmp(temp->color, color) == 0){
            encontrado=1;
            
            for(int i;i<cantSumas;i++){
                if(strcmp(sumas[i].marca, temp->marca)==0){
                    sumas[i].tamanio+=temp->tamanio;
                    marcaEncotrada=1;
                    break;
                }
            }

            if(!marcaEncotrada){
                strcpy(sumas[cantSumas].marca, temp->marca);
                sumas[cantSumas].tamanio = temp->tamanio;
                cantSumas++;
            }
        }
        temp = temp->pLata;
    }
    if (encontrado) {
        printf("\nLos productos con ese color son:\n");
        for (int i = 0; i < cantSumas; i++) {
           printf("\n%s, %.2f litros\n", sumas[i].marca, sumas[i].tamanio);
        }
    }else if(!encontrado){
        printf("Color no disponible\n");
    }
    
}

void validacionInt(int opcion, int scanfopcion){
    if(scanfopcion!=1){      
            printf("\n\nERROR, el dato ingresado no es valido, debe ser un numero entero\n\n");
            while(getchar()!='\n'){    
                opcion=0;
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

    printf("\n*****[MENU PRINCIPAL]*****\n\nIngrese 1 para agregar un producto\nIngrese 2 para eliminar un producto\nIngrese 3 para ver la disponibilidad de un color\nIngrese 4 para ver la disponibilidad de un color y tamaño\n");
	
    while (opcion!=-1){

		printf("\nIngrese una opcion: ");
		validarOpcion=scanf("%d",&opcion);
        if(validarOpcion!=1){      //si el scanf no puede leer la entrada, entonces imprime un mensaje de error.
            printf("\n\nERROR, el dato ingresado no es valido, recuerde que debe ingresar una opcion disponible\n\n");
            while(getchar()!='\n'){     
                opcion=0;
                continue;
            }


		}else if(opcion==1){

			char marca[20];
			char color[20];
			float tamanio;
            int validarOpcion;
            char* nombre_archivo={"stock.txt"};
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

            printf("\n[Producto agregado]\n");
			agregarProducto(&cabeza, marca, color, tamanio);
			imprimirLista(cabeza);
            //guardarEnArchivo(nombre_archivo,cabeza);            
		}else if(opcion==2){

            char marca[20];
			char color[20];
			float tamanio;
            char* nombre_archivo={"stock.txt"};
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

            borrarProducto(&cabeza, marca, color, tamanio);
            imprimirLista(cabeza);
            //guardarEnArchivo(nombre_archivo,cabeza);            

        }else if(opcion==3){

            char color[20];
            char opcionC[2];
            char nombre_archivo[20];

            printf("\nIngrese el color: ");
            scanf("%19s",color);
            convertirMayuscula(color);
            //printf("\nEn que archivo desea ver que productos hay con color %s? ",color);
            //scanf("%s",nombre_archivo);
            disponibilidadColor(cabeza,color);
            

            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionC);
			if (strcmp(opcionC, "SI") == 0){
                printf("\nEn que archivo desea guardarlo?: ");
                scanf("%s",nombre_archivo);
        		guardarEnArchivo(nombre_archivo,cabeza);
            } else if (strcmp(opcionC, "NO") != 0) {
        		printf("\nERROR, el dato ingresado no es correcto\n");
            }

        }else if(opcion==4){
            char color[20];
            float tamanio;
            char opcionD[2];
            char nombre_archivo[2];

            printf("\nIngrese el color: ");
            scanf("%19s",color);
            convertirMayuscula(color);

            do {
                printf("\nIngrese el tamaño de la lata: ");
                validarOpcion = scanf("%f", &tamanio);
                validacionFloat(&tamanio, validarOpcion);
            } while (tamanio == -1);

            //aca va la funcion

            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionD);
			if (strcmp(opcionD, "SI") == 0) {
                printf("\nEn que archivo desea guardarlo?: ");
                scanf("%s",nombre_archivo);
        		guardarEnArchivo(nombre_archivo,cabeza);
                printf("Producto agregado\n");
    		} else if (strcmp(opcionD, "NO") != 0) {
        		printf("\nERROR, el dato ingresado no es correcto\n");
            }
        }else if(opcion>5 || opcion<-1 && validarOpcion==1) {

            printf("\n\nERROR, ingrese una opcion valida\n\n");

            while(getchar()!='\n'){
                opcion=0;
                continue;
            }
        }
	}   

	liberarLista(cabeza);

	printf("\n\n[**********PROGRAMA FINALIZADO**********]\n\n\n");

    return 0;

}
