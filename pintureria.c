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
    if (temp!=NULL && strcmp(temp->marca, marca) == 0 && strcmp(temp->color, color) == 0 && temp->tamanio == tamanio) {
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

void disponibilidadColor(lata *cabeza, char* color, char* buffer){

    typedef struct {
        char marca[20];
        float tamanio;
    }suma;
    
    lata *temp = cabeza;
    int encontrado=0;
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
           sprintf(buffer + strlen(buffer), "\n%s, %.2f litros\n", sumas[i].marca, sumas[i].tamanio);
        }
    }else if(!encontrado){
        printf("\n[COLOR NO DISPONIBLE]\n");
        sprintf(buffer, "\n[COLOR NO DISPONIBLE]\n");
    }
}

void disponibilidadColor_Tamanio(lata *cabeza, char* color, float tamanio, char* buffer){

    typedef struct {
        char marca[20];
    }suma;
    
    lata *temp = cabeza;
    int encontrado=0;
    suma sumas[20];
    int cantSumas=0;
    int marcaEncotrada=0;

    while(temp!=NULL){
        if ((strcmp(temp->color,color) == 0) && temp->tamanio==tamanio){
            encontrado=1;
            
            for(int i;i<cantSumas;i++){
                if(strcmp(sumas[i].marca, temp->marca)==0){
                    marcaEncotrada=1;
                    break;
                }
            }

            if(!marcaEncotrada){
                strcpy(sumas[cantSumas].marca, temp->marca);
                cantSumas++;
            }
        }
        temp = temp->pLata;
    }
    if (encontrado) {
        printf("\nLas marcas de los productos con ese color y tamaño son:\n");
        sprintf(buffer, "\nLas marcas de los productos con ese color y tamaño son:\n");
        for (int i = 0; i < cantSumas; i++) {
            printf("\n%s\n", sumas[i].marca);
            sprintf(buffer + strlen(buffer), "\n%s", sumas[i].marca);
        }
    }else if(!encontrado){
        printf("\n[COLOR Y TAMAÑO NO DISPONIBLE]\n");
        sprintf(buffer, "\n[COLOR Y TAMAÑO NO DISPONIBLE]\n");
    }
}

void totalColor(lata *cabeza, char* color, char* buffer){

    typedef struct {
        char marca[20];
        float tamanio;
    }suma;
    
    lata *temp = cabeza;
    int encontrado=0;
    suma sumas[20];
    int cantSumas=0;
    int marcaEncotrada=0;
    float tamanioTotal=0;
    int cantLatas=0;

    while(temp!=NULL){
        if (strcmp(temp->color, color) == 0){
            encontrado=1;

            for(int i;i<cantSumas;i++){
                if (strcmp(sumas[i].marca, temp->marca) == 0) {
                    sumas[i].tamanio += temp->tamanio;
                    tamanioTotal += temp->tamanio;
                    cantLatas++;
                    marcaEncotrada = 1;
                    break;
                }
            }
            
            if(!marcaEncotrada){
                strcpy(sumas[cantSumas].marca, temp->marca);
                sumas[cantSumas].tamanio = temp->tamanio;
                tamanioTotal += temp->tamanio;
                cantSumas++;
                cantLatas++;
            }
        }
        temp = temp->pLata;
    }
    if (encontrado) {
        printf("\nLos productos con ese color son:\nMarcas: ");
        sprintf(buffer, "\nLos productos con ese color son:\nMarcas: ");
        for (int i = 0; i < cantSumas; i++) {
            printf("%s ", sumas[i].marca);
            sprintf(buffer + strlen(buffer), "%s", sumas[i].marca);
        }
        printf("\nTotal color: %.2f\n",tamanioTotal);
        printf("Cantidad de latas: %d",cantLatas);
        sprintf(buffer + strlen(buffer), "\nTotal color: %.2f\nCantidad de latas: %d", sumas->tamanio,cantLatas);
    
    }else if(!encontrado){
        printf("\n[COLOR NO DISPONIBLE]\n");
        sprintf(buffer, "\n[COLOR NO DISPONIBLE]\n");
    }
}

void totalMarca(lata *cabeza, char* marca, char* buffer){

    typedef struct {
        char color[20];
        float tamanio;
    }suma;
    
    lata *temp = cabeza;
    int encontrado=0;
    suma sumas[20];
    int cantSumas=0;
    int cantLatas=0;

    while(temp!=NULL){
        if (strcmp(temp->marca, marca) == 0) {
            encontrado = 1;
            int colorEncotrado = 0;

            for (int i = 0; i < cantSumas; i++) {
                if (strcmp(sumas[i].color, temp->color) == 0) {
                    colorEncotrado = 1;
                    break;
                }
            }

            if (!colorEncotrado) {
                strcpy(sumas[cantSumas].color, temp->color);
                cantSumas++;
            }
            cantLatas++;
        }
        temp = temp->pLata;
    }
    if (encontrado) {
        printf("\nLos productos con esa marca son:\nColores: ");
        sprintf(buffer, "\nLos productos con esa marca son:");
        for (int i = 0; i < cantSumas; i++) {
            printf("%s ", sumas[i].color);
            sprintf(buffer + strlen(buffer), "\nColores: %s", sumas[i].color);
        }

        printf("\nCantidad de latas: %d",cantLatas);
        sprintf(buffer + strlen(buffer), "\nCantidad de latas %d\n",cantLatas);

    }else if(!encontrado){
        printf("\n[MARCA NO DISPONIBLE]\n");
        sprintf(buffer, "\n[MARCA NO DISPONIBLE]\n");
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

void convertirMayuscula(char *str){
    for (int i=0;str[i]!='\0';i++){
        str[i]=toupper((unsigned char) str[i]);
    }
}


int main(){
	
	lata *cabeza=NULL;
	leerArchivo("stock.txt",&cabeza);
	int opcion;
	int validarOpcion;

    printf("\n*****[MENU PRINCIPAL]*****\n\nIngrese 1 para agregar un producto\nIngrese 2 para eliminar un producto\nIngrese 3 para ver la disponibilidad de un color\nIngrese 4 para ver la disponibilidad de un color y tamaño\nIngrese 5 para el total de pintura en litros, y la cantidad de latas de un color\nIngrese 6 para una lista de los colores disponibles y la cantidad de latas de una marca\n");
    
    imprimirLista(cabeza);
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

        }else if(opcion==3){

            char color[20];
            char opcionC[2];
            char nombre_archivo[20];
            char colorElegido[20];

            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color);
            convertirMayuscula(color);

            char buffer[1000] = ""; // Buffer para almacenar la salida
            disponibilidadColor(cabeza,color,buffer);
            
            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionC);
            convertirMayuscula(opcionC);

			if (strcmp(opcionC, "SI") == 0){
                printf("\nIngrese el nombre del archivo a guardar: ");
                scanf("%s",nombre_archivo);
        		//guardarEnArchivo(nombre_archivo,cabeza);
                strcat(nombre_archivo, ".txt");
                FILE *archivoSalida = fopen(nombre_archivo, "w"); 
                if (archivoSalida == NULL) {
                    printf("No se pudo abrir el archivo para escribir\n");
                    exit(1);
                }

                fprintf(archivoSalida, "Ingrese el Color: %s \n", colorElegido);
                fprintf(archivoSalida,"%s",buffer);
                fclose(archivoSalida);

                printf("Información guardada en el archivo: %s\n", nombre_archivo);

            }else if (strcmp(opcionC, "NO") == 0){
        		printf("\nNo se guardará el archivo\n");
            }

        }else if(opcion==4){

            char color[20];
            float tamanio;
            char opcionD[2];
            char nombre_archivo[20];
            char colorElegido[20];
            float tamanioElegido;

            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color);
            convertirMayuscula(color);

            do {
                printf("\nIngrese el tamaño de la lata: ");
                validarOpcion=scanf("%f", &tamanio);
                validacionFloat(&tamanio, validarOpcion);
                //(tamanioElegido,tamanio);
            } while(tamanio==-1);
            char buffer[1000] = ""; // Buffer para almacenar la salida

            disponibilidadColor_Tamanio(cabeza,color,tamanio,buffer);

            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionD);
            convertirMayuscula(opcionD);

			if (strcmp(opcionD, "SI") == 0) {
                printf("\nEn que archivo desea guardarlo?: ");
                scanf("%s",nombre_archivo);
        		strcat(nombre_archivo, ".txt");

                FILE *archivoSalida = fopen(nombre_archivo, "w"); 
                if (archivoSalida == NULL) {
                    printf("\nNo se pudo abrir el archivo para escribir\n");
                    exit(1);
                }

                fprintf(archivoSalida, "Ingrese el Color: %s \n", colorElegido);
                fprintf(archivoSalida,"Ingrese el tamaño de la lata: %.2f \n",tamanio);
                fprintf(archivoSalida,"%s\n",buffer);
                fclose(archivoSalida);

                printf("\nInformación guardada en el archivo: %s\n", nombre_archivo);

    		}else if (strcmp(opcionD, "NO") != 0) {
        		printf("\nNo se guardará el archivo\n");
            }
            
        }else if(opcion==5){

            char color[20];
            char opcionE[2];
            char nombre_archivo[20];
            char colorElegido[20];

            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color);
            convertirMayuscula(color);

            char buffer[1000] = ""; // Buffer para almacenar la salida
            totalColor(cabeza,color,buffer);
            
            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionE);
            convertirMayuscula(opcionE);

			if (strcmp(opcionE, "SI") == 0){
                printf("\nIngrese el nombre del archivo a guardar: ");
                scanf("%s",nombre_archivo);
        		//guardarEnArchivo(nombre_archivo,cabeza);
                strcat(nombre_archivo, ".txt");

                FILE *archivoSalida = fopen(nombre_archivo, "w"); 
                if (archivoSalida == NULL) {
                    printf("No se pudo abrir el archivo para escribir\n");
                    exit(1);
                }

                fprintf(archivoSalida, "Ingrese el Color: %s \n", colorElegido);
                fprintf(archivoSalida,"%s",buffer);
                fclose(archivoSalida);

                printf("Información guardada en el archivo: %s\n", nombre_archivo);

            }else if (strcmp(opcionE, "NO") == 0){
        		printf("\nNo se guardará el archivo\n");
            }
        }else if(opcion==6){

            char marca[20];
            char opcionF[2];
            char nombre_archivo[20];
            char marcaElegida[20];

            printf("\nIngrese la marca: ");
            scanf("%19s",marca);
            strcpy(marcaElegida,marca);
            convertirMayuscula(marca);

            char buffer[1000] = ""; // Buffer para almacenar la salida

            totalMarca(cabeza,marca,buffer);
            
            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionF);
            convertirMayuscula(opcionF);

			if (strcmp(opcionF, "SI") == 0){
                printf("\nIngrese el nombre del archivo a guardar: ");
                scanf("%s",nombre_archivo);
        		//guardarEnArchivo(nombre_archivo,cabeza);
                strcat(nombre_archivo, ".txt");

                FILE *archivoSalida = fopen(nombre_archivo, "w"); 
                if (archivoSalida == NULL) {
                    printf("No se pudo abrir el archivo para escribir\n");
                    exit(1);
                }

                fprintf(archivoSalida, "Ingrese el Color: %s \n", marcaElegida);
                fprintf(archivoSalida,"%s",buffer);
                fclose(archivoSalida);

                printf("Información guardada en el archivo: %s\n", nombre_archivo);

            }else if (strcmp(opcionF, "NO") == 0){
        		printf("\nNo se guardará el archivo\n");
            }
            
 
        }else if(opcion==7){

            char marca[20];
            char color[20];
            char opcionG[2];
            char nombre_archivo[20];
            char marcaElegida[20];
            char colorElegido[20];
            char buffer[1000]="";

            printf("\nIngrese la marca: ");
            scanf("%19s",marca);
            strcpy(marcaElegida,marca);
            convertirMayuscula(marca);

            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color);
            convertirMayuscula(color);

            //aca va la funcion

            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionG);
            convertirMayuscula(opcionG);

            if (strcmp(opcionG, "SI") == 0){
                printf("\nIngrese el nombre del archivo a guardar: ");
                scanf("%s",nombre_archivo);
        		//guardarEnArchivo(nombre_archivo,cabeza);
                strcat(nombre_archivo, ".txt");

                FILE *archivoSalida = fopen(nombre_archivo, "w"); 
                if (archivoSalida == NULL) {
                    printf("\nNo se pudo abrir el archivo para escribir\n");
                    exit(1);
                }

                fprintf(archivoSalida, "Ingrese el Color: %s \n", marcaElegida);
                fprintf(archivoSalida,"%s",buffer);
                fclose(archivoSalida);

                printf("Información guardada en el archivo: %s\n", nombre_archivo);

            }else if (strcmp(opcionG, "NO") == 0){
        		printf("\nNo se guardará el archivo\n");
            }

        }else if(opcion>8 || opcion<-1 && validarOpcion==1) {

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
