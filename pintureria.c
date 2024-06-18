#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lista.h"

/**Autores: Santiago Alvarez, padron: 111935
 * Santiago Seijas, padron: 112301
*/

//funcion que crea una lista enlazada, utilizando memoria dinamica y el struct definido en el archivo "lista.h"
lata* crearProducto(char *marca, char *color, float tamanio) {
    lata *nuevolata = (lata*)malloc(sizeof(lata));
    if (nuevolata == NULL) {                        //en caso de que no se puede asignar memoria, se finaliza el programa con exit(1)
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    strcpy(nuevolata->marca, marca);    //se copian los datos del struct a la lista enlazada
    strcpy(nuevolata->color, color);
    nuevolata->tamanio = tamanio;
    nuevolata->pLata = NULL;
    return nuevolata;
}
//funcion que agrega un "producto" al inicio de la lista enlazada
void agregarProducto(lata **cabeza, char *marca, char *color, float cantidad) {
    lata *nuevolata = crearProducto(marca, color, cantidad);
    nuevolata->pLata = *cabeza;
    *cabeza = nuevolata;

}
//funcion que dado un nombre de archivo, lo lee y agrega los elementos como nuevos nodos
void leerArchivo(const char *nombreArchivo, lata **cabeza) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {                          //en caso de que el archivo no existe, hay mensaje de error
        printf("\nNo se pudo abrir el archivo\n");
        exit(1);
    }

    char marca[20];
    char color[20];
    float tamanio;
    while (fscanf(archivo, "%20s\n%20s\n%f\n", marca, color, &tamanio) == 3) {  //lee los primero 20 caracteres de la marca y el color porque asi lo especifica el enunciado
        agregarProducto(cabeza, marca, color, tamanio);
    }

    fclose(archivo);
}
//funcion que imprime todos los elementos de la lista enlazada
void imprimirLista(lata *cabeza) {
    lata *temp = cabeza;
    while (temp != NULL){       //intera hasta que el puntero es NULL, osea que la lista enlaza finaliza
        printf("\n%s\n%s\n%.2f\n", temp->marca, temp->color, temp->tamanio);
        temp = temp->pLata;     //la variable puntero ahora apunta al siguiente nodo, en caso de no halla mas nodos su valor sera NULL
    }
}
//funcion que libera todos los nodos de la lista, ya que se uso memoria dinamica para asignarlos
void liberarLista(lata *cabeza) {
    lata *temp;
    while (cabeza != NULL){         //intera hasta que el puntero sea NULL
        temp = cabeza;              
        cabeza = cabeza->pLata;
        free(temp);                 //funcion para liberar la memoria pedida a malloc
    }
}
//funcion que dado el nombre de un archivo, guarda la todos los elementos de la lista enlazada en un archivo de texto
void guardarEnArchivo(const char *nombreArchivo, lata *cabeza) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("\nNo se pudo abrir el archivo para escribir\n"); //en caso de que no se pueda acceder al archivo hay mensaje de error
        exit(1);
    }

    lata *temp = cabeza;
    while (temp != NULL) {
        fprintf(archivo, "%s\n%s\n%.2f\n\n", temp->marca, temp->color, temp->tamanio);
        temp = temp->pLata;
    }

    fclose(archivo);
}
//funcion que dado todos los elementos que componen al nodo (maraca, color y tamaño) lo elimina de la lista enlazada
void borrarProducto(lata **cabeza, char *marca, char *color, float tamanio){
    lata *temp = *cabeza;
    lata *prev = NULL;
    // Si la cabeza contiene el producto a eliminar
    if (temp!=NULL && strcmp(temp->marca, marca) == 0 && strcmp(temp->color, color) == 0 && temp->tamanio == tamanio) {
        *cabeza = temp->pLata;
        free(temp);             //libera la memoria pedida a malloc para ese nodo en concreto
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
//funcion que dado un color, muestra una lista de marcas y tamaños de lata de ese color o un mensaje “Color no disponible”
void disponibilidadColor(lata *cabeza, char* color, char* buffer){      //decidimos utilizar un paramentro mas (buffer) para facilitar la funcionalidad de guardar en un archivo de texto

    typedef struct {
        char marca[20];
        float tamanio;
    }suma;
    
    lata *temp = cabeza;
    int encontrado=0;
    suma sumas[20]; //arreglo para almacenar las marcas y tamaños unicos (puede hacerce con un vector dinamico)
    int cantSumas=0;
    int marcaEncotrada=0;

    while(temp!=NULL){      //recorre la lista enlazada
        if (strcmp(temp->color, color) == 0){       //si el color coincide con el ingresado
            encontrado=1;
            
            for(int i;i<cantSumas;i++){         //verifica si la marca ya esta en el vector
                if(strcmp(sumas[i].marca, temp->marca)==0){     //si ya esta en el vector, se suma el tamaño
                    sumas[i].tamanio+=temp->tamanio;
                    marcaEncotrada=1;
                    break;
                }
            }

            if(!marcaEncotrada){    //si la marca no esta entonces se agrwga
                strcpy(sumas[cantSumas].marca, temp->marca);
                sumas[cantSumas].tamanio = temp->tamanio;
                cantSumas++;
            }
        }
        temp = temp->pLata;     //avanza al siguiente nodo
    }
    if (encontrado){    //si se encontro un producto o mas 
        printf("\nLos productos con ese color son:\n");
        for (int i = 0; i < cantSumas; i++) {
           printf("\n%s, %.2f litros\n", sumas[i].marca, sumas[i].tamanio);
           sprintf(buffer + strlen(buffer), "\n%s, %.2f litros\n", sumas[i].marca, sumas[i].tamanio);   // se guarda el string lo mismo que muestra el programa como salida
        }

    }else if(!encontrado){
        printf("\n[COLOR NO DISPONIBLE]\n");
        sprintf(buffer, "\n[COLOR NO DISPONIBLE]\n");
    }
}
//funcion que dado un color y un tamaño de lata, semuestra una lista de marcas de ese color y cantidad o un mensaje de “Stock no disponible”
void disponibilidadColor_Tamanio(lata *cabeza, char* color, float tamanio, char* buffer){   //en todas las siguientes funciones se utiliza la logica del parametro buffer, explicado mas arriba

    typedef struct {
        char marca[20];
    }suma;
    
    lata *temp = cabeza;
    int encontrado=0;
    suma sumas[20];     
    int cantSumas=0;
    int marcaEncotrada=0;

    while(temp!=NULL){      // recorre la lista enlazada
        if ((strcmp(temp->color,color) == 0) && temp->tamanio==tamanio){    //si el color y el tamaño coincide 
            encontrado=1;
            
            for(int i;i<cantSumas;i++){
                if(strcmp(sumas[i].marca, temp->marca)==0){ //verifica si la marca ya se encontro
                    marcaEncotrada=1;
                    break;
                }
            }

            if(!marcaEncotrada){        //en caso de que no este anteriormente, entonces se agrega la marca
                strcpy(sumas[cantSumas].marca, temp->marca);
                cantSumas++;
            }
        }
        temp = temp->pLata;
    }
    if (encontrado) {
        printf("\nLas marcas de los productos con ese color y tamaño son:\n");
        sprintf(buffer, "\nLas marcas de los productos con ese color y tamaño son:\n"); //se guarda todo la variable buffer
        for (int i = 0; i < cantSumas; i++) {
            printf("\n%s\n", sumas[i].marca);
            sprintf(buffer + strlen(buffer), "\n%s", sumas[i].marca);
        }
    }else if(!encontrado){
        printf("\n[COLOR Y TAMAÑO NO DISPONIBLE]\n");
        sprintf(buffer, "\n[COLOR Y TAMAÑO NO DISPONIBLE]\n");
    }
}
//funcion que muestra una lista de marcas, el total de pintura en litros, y la cantidad de latas de un color ingresado
void totalColor(lata *cabeza, char* color, char* buffer){

    typedef struct {
        char marca[20];
        float tamanio;
    }suma;
    
    lata *temp = cabeza;
    int encontrado=0;
    suma sumas[20]; //vector para almacenar las marcas y tamaños unicos
    int cantSumas=0;
    int marcaEncotrada=0;
    float tamanioTotal=0;   //variable que va a servir como un acumulador 
    int cantLatas=0;    //contador de latas 

    while(temp!=NULL){      //recorre la lista enlazada 
        if (strcmp(temp->color, color) == 0){   //si el color coincide
            encontrado=1;

            for(int i;i<cantSumas;i++){ //verifica si la marca ya esta en el vector
                if (strcmp(sumas[i].marca, temp->marca) == 0){      //en caso de que este solo se suma el tamaño
                    sumas[i].tamanio += temp->tamanio;
                    tamanioTotal += temp->tamanio;
                    cantLatas++;
                    marcaEncotrada = 1;
                    break;
                }
            }
            
            if(!marcaEncotrada){    //si no esta se agrega al vector
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
// funcion que muestra una lista de los colores disponibles y la cantidad de latas de una marca ingresada por el usuario
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
            sprintf(buffer + strlen(buffer), "%s ", sumas[i].color);
        }

        printf("\nCantidad de latas: %d",cantLatas);
        sprintf(buffer + strlen(buffer), "\nCantidad de latas %d\n",cantLatas);

    }else if(!encontrado){
        printf("\n[MARCA NO DISPONIBLE]\n");
        sprintf(buffer, "\n[MARCA NO DISPONIBLE]\n");
    }
}
//funcion que muestra la cantidad total de pintura en litros y la cantidad total de latas de una Marca y Color ingresado por el usuario
void totalMarcaColor(lata *cabeza, char* color, char* marca, char* buffer){

    lata *temp = cabeza;
    int encontrado = 0;
    float tamanioTotal = 0;
    int cantLatas = 0;

    while (temp != NULL) {  
        if (strcmp(temp->marca, marca) == 0 && strcmp(temp->color, color) == 0) {
            encontrado = 1;
            tamanioTotal += temp->tamanio;
            cantLatas++;
        }
        temp = temp->pLata;
    }

    if (encontrado) {
        printf("\nTotal color: %.2f litros\n", tamanioTotal);
        printf("Cantidad de latas: %d", cantLatas);
        sprintf(buffer + strlen(buffer), "\nTotal color: %.2f litros\nCantidad de latas: %d", tamanioTotal,cantLatas);
    
    }else if(!encontrado){
        printf("\n[MARCA Y COLOR NO DISPONIBLE]\n");
        sprintf(buffer, "\n[MARCA Y COLOR NO DISPONIBLE]\n");
    }
}
//funcion que valida si la informacion ingresada es un numero entero
void validacionInt(int opcion, int scanfopcion){
    if(scanfopcion!=1){      
        printf("\n\nERROR, el dato ingresado no es valido, debe ser un numero entero\n\n");
        while(getchar()!='\n'){    
            opcion=0;
            continue;
        }
    }
}
//funcion que valida si la informacion ingresada es un numero real o de tipo float
void validacionFloat(float *opcion, int scanfopcion){
    if(scanfopcion!=1){      //recordemos que se utiliza el scanf, devuelve un resultado, 1 en caso de que pudo leer el dato ingresado
            printf("\n\nERROR, el dato ingresado no es valido\n\n");    //este resultado puede guardarce en una varible int y de esta forma verifica si se ingreso el dato deceado
            while(getchar()!='\n');
            *opcion=-1;
    }
}
//funcion que dado un char[], transforma todos sus caracteres a mayuscula
void convertirMayuscula(char *str){
    for (int i=0;str[i]!='\0';i++){        //se recirre todo el string
        str[i]=toupper((unsigned char) str[i]);//la funcion touppper de la librerua ctype trasforma el carcter a mayuscula, esto nos ayuda en las comparaciones y ademas el enunciado lo pide
    }
}


int main(){
	
	lata *cabeza=NULL;      //puntero que apunta al primer nodo de la lista elazada, este es fundamental para:
                            //usar como punto de partida cuando se recorre la lista, cuando se agrega un nuevo nodo, cabeza se actualiza para apuntar al nuevo nodo y se usa para liberar la memoria al final del programa
	leerArchivo("stock.txt",&cabeza);
	int opcion;
	int validarOpcion;  //nos va a permitir validar si se ingreso un entero

    printf("\n*****[MENU PRINCIPAL]*****\n\nIngrese 1 para agregar un producto\nIngrese 2 para eliminar un producto\nIngrese 3 para ver la disponibilidad de un color\nIngrese 4 para ver la disponibilidad de un color y tamaño\nIngrese 5 para el total de pintura en litros, y la cantidad de latas de un color\nIngrese 6 para una lista de los colores disponibles y la cantidad de latas de una marca\nIngrese 7 para la cantidad total de pintura en litros y la cantidad total de latas de una marca y color\n");
    printf("\nIngrese -1 para finalizar el programa\n");
    imprimirLista(cabeza);  //se imprime la lista, que en este momento son los datos que existen en el archivo "stock.txt"

    while (opcion!=-1){ //con opcion == -1 el programa se finaliza

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
            //se piden los datos necesarios para agregar un nodo a la lista
			printf("\nIngrese el nombre de la marca de la lata: ");
			scanf("%19s",marca);
            convertirMayuscula(marca);

			printf("\nIngrese el color de la lata: ");
			scanf("%19s",color);
            convertirMayuscula(color);

            do {        // se verifica que la entrada es de tipo float (numeros reales)
                printf("\nIngrese el tamaño de la lata: ");
                validarOpcion = scanf("%f", &tamanio);
                validacionFloat(&tamanio, validarOpcion);
            } while (tamanio == -1);

            printf("\nPRODUCTO AGREGADO\n");
			agregarProducto(&cabeza, marca, color, tamanio);    //funcion para agregar un producto
			imprimirLista(cabeza);  //se imprime la lista para que el usario verifique que se inserto el producto correctamente
         
		}else if(opcion==2){

            char marca[20];
			char color[20];
			float tamanio;
            //se piden los datos necesarios para eliminar un nodo a la lista
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
            //se piden los datos necesarios para llamar a la funcion
            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color); //guarda en otra variable el color antes de transformarlo a mayuscula
            convertirMayuscula(color);

            char buffer[1000] = ""; // Buffer para almacenar la salida, aca van todos los caracteres que queremos que esten en el archivo de texto
            disponibilidadColor(cabeza,color,buffer);
            //se pregunta si quiere guardar la informacion en un archivo de texto distinto
            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionC);
            convertirMayuscula(opcionC); 

			if (strcmp(opcionC, "SI") == 0){
                printf("\nIngrese el nombre del archivo a guardar: ");
                scanf("%s",nombre_archivo);    
                strcat(nombre_archivo, ".txt");    //se concatena el nombre elegido con la extencion .txt
                FILE *archivoSalida = fopen(nombre_archivo, "w");   // se abre el archivo en modo escritura
                if (archivoSalida == NULL) {
                    printf("No se pudo abrir el archivo para escribir\n");
                    exit(1);
                }

                fprintf(archivoSalida, "Ingrese el Color: %s \n", colorElegido);    // se imprime en el archivo lo mismo que en el programa
                fprintf(archivoSalida,"%s",buffer);     //en buffer es donde guardamos la informacion recopilada de la funcion disponibilidadColor(cabeza,color,buffer);
                fclose(archivoSalida);

                printf("Información guardada en el archivo: %s\n", nombre_archivo);

            }else if (strcmp(opcionC, "NO") == 0){
        		printf("\nNo se guardará el archivo\n");
            
            }else{
                printf("\nERROR AL INGRESAR LA RESPUESTA\n");
            }

        }else if(opcion==4){

            char color[20];
            float tamanio;
            char opcionD[2];
            char nombre_archivo[20];
            char colorElegido[20];
            float tamanioElegido;
            char buffer[1000] = ""; // Buffer para almacenar la salida
            //se piden los datos necesarios para llamar a la funcion
            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color);
            convertirMayuscula(color);

            do {
                printf("\nIngrese el tamaño de la lata: ");
                validarOpcion=scanf("%f", &tamanio);
                validacionFloat(&tamanio, validarOpcion);
            } while(tamanio==-1);

            disponibilidadColor_Tamanio(cabeza,color,tamanio,buffer);

            printf("\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionD);
            convertirMayuscula(opcionD);

			if (strcmp(opcionD, "SI") == 0){    //mismo funcionamiento 
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

    		}else if (strcmp(opcionD, "NO") == 0) {
        		printf("\nNo se guardará el archivo\n");
            
            }else{      //en caso de que la respuesta este mal ingresada
                printf("\nERROR AL INGRESAR LA RESPUESTA\n");   
            }
            
        }else if(opcion==5){

            char color[20];
            char opcionE[2];
            char nombre_archivo[20];
            char colorElegido[20];
            char buffer[1000] = ""; // Buffer para almacenar la salida
            //se piden los datos necesarios para llamar a la funcion
            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color);
            convertirMayuscula(color);

            totalColor(cabeza,color,buffer);
            
            printf("\n\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionE);
            convertirMayuscula(opcionE);

			if (strcmp(opcionE, "SI") == 0){
                printf("\nIngrese el nombre del archivo a guardar: ");
                scanf("%s",nombre_archivo);
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

            }else{
                printf("\nERROR AL INGRESAR LA RESPUESTA\n");
            }

        }else if(opcion==6){

            char marca[20];
            char opcionF[2];
            char nombre_archivo[20];
            char marcaElegida[20];
            char buffer[1000]=""; // Buffer para almacenar la salida
            //se piden los datos necesarios para llamar a la funcion
            printf("\nIngrese la marca: ");
            scanf("%19s",marca);
            strcpy(marcaElegida,marca);
            convertirMayuscula(marca);

            totalMarca(cabeza,marca,buffer);
            
            printf("\n\n¿¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
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

            }else{
                printf("\nERROR AL INGRESAR LA RESPUESTA\n");
            }
            
 
        }else if(opcion==7){

            char marca[20];
            char color[20];
            char opcionG[2];
            char nombre_archivo[20];
            char marcaElegida[20];
            char colorElegido[20];
            char buffer[1000]="";
            //se piden los datos necesarios para llamar a la funcion
            printf("\nIngrese la marca: ");
            scanf("%19s",marca);
            strcpy(marcaElegida,marca);
            convertirMayuscula(marca);

            printf("\nIngrese el color: ");
            scanf("%19s",color);
            strcpy(colorElegido,color);
            convertirMayuscula(color);

            totalMarcaColor(cabeza, color, marca, buffer);

            printf("\n\n¿Desea guardar esta informacion en un archivo de texto(SI/NO)?: ");
            scanf("%s",opcionG);
            convertirMayuscula(opcionG);

            if (strcmp(opcionG, "SI") == 0){
                printf("\nIngrese el nombre del archivo a guardar: ");
                scanf("%s",nombre_archivo);
                strcat(nombre_archivo, ".txt");

                FILE *archivoSalida = fopen(nombre_archivo, "w"); 
                if (archivoSalida == NULL) {
                    printf("\nNo se pudo abrir el archivo para escribir\n");
                    exit(1);
                }

                fprintf(archivoSalida, "Ingrese la marca: %s \n", marcaElegida);
                fprintf(archivoSalida, "\n¿Ingrese el color: %s \n", colorElegido);
                fprintf(archivoSalida,"%s",buffer);
                fclose(archivoSalida);

                printf("Información guardada en el archivo: %s\n", nombre_archivo);

            }else if (strcmp(opcionG, "NO") == 0){
        		printf("\nNo se guardará el archivo\n");

            }else{
                printf("\nERROR AL INGRESAR LA RESPUESTA\n");
            }

        }else if(opcion>8 || opcion<-1 && validarOpcion==1) {   //si opcion es un entero, pero no es parte de las posibles opciones entonces se imprime un mensaje de error

            printf("\n\nERROR, ingrese una opcion valida\n\n");

            while(getchar()!='\n'){
                opcion=0;
                continue;
            }
        }
	}   

    guardarEnArchivo("stock.txt",cabeza);   //decidimos que tenia mas sentido que cuando el programa terminara, se sobreescriba el archivo "stock.txt" con todos lo que tenga la lista enlazada
    //esto es porque no tendria sentido que el usario elemine datos o los agrege para que despues no se guarden y se eliminen completamente cuando se libera la memoria dinamica (esto no lo pide el enunciado)

	liberarLista(cabeza);   //se libera toda la memoria dinamica utilizada para la lista enlazada

	printf("\n\n[**********PROGRAMA FINALIZADO**********]\n\n\n");

    return 0;

}