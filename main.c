#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pila.h"

#define archivoPersonas "personas.bin"
#define archivoBoletos "boletos.bin"
#define archivoGanador "boletoganador.bin"

///Estructuras
typedef struct
{
    int nro_carton;
    char num[5];

} Boleto;

typedef struct nodoDPersona
{
    Persona p;
    struct nodoDPersona* sig;
    struct nodoDPersona* ant;
} nodoDPersona;

typedef struct nodoDBoleto
{
    Boleto b;
    struct nodoDBoleto* sig;
    struct nodoDBoleto* ant;
} nodoDBoleto;


///Prototipados
nodoDPersona* crearNodoDoblePersona(Persona dato);
void insertarPersonasOrdenadas (nodoDPersona** listaDoble,Persona dato);
void insertarBoletosOrdenados (nodoDBoleto** listaDoble,Boleto dato);
void cargarDatosPersona(nodoDPersona** listaPersonas);
void cargarDatosBoleto(nodoDBoleto** listaBoletos);
void cargarBoletoGanador(Boleto* ganador);
void buscarPersonaGanadoraRec(nodoDPersona* listaPersonas,Boleto ganador);
void apilarOtrosPremiosYEliminar(nodoDPersona** listaPersonas, nodoDBoleto* listaBoletos,Pila* otrosPremios,Boleto ganador);
void mencionarGanadoresRec(Pila* otrosGanadores,Pila* aux);


///Main
int main()
{
    int opcion=0;
    char seguir;
    Pila otrosPremios;
    inicPila(&otrosPremios);
    Pila aux;
    inicPila(&aux);
    nodoDPersona* listaPersonas=NULL;
    nodoDBoleto* listaBoletos=NULL;
    Boleto ganador;

    do
    {
        printf("Ingrese el numero de operacion que desea realizar...\n\n");
        printf("1)Cargar datos de las personas y los boletos en las listas desde los archivos\n");
        printf("2)Mostrar a la persona con el billete ganador\n");
        printf("3)Eliminar a las personas de la lista y apilar en otros premios a los ganadores del 2do y 3er puesto\n");
        printf("4)Mencionar a los ganadores del resto de los premios de manera inversa a la pila\n");


        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:

            cargarDatosPersona(&listaPersonas);
            cargarDatosBoleto(&listaBoletos);
            cargarBoletoGanador(&ganador);
            mostrarListaPersonas(listaPersonas);
            mostrarListaBoletos(listaBoletos);
            printf("\nEl boleto ganador es:\n");
            mostrarBoletoGanador(ganador);

            break;

        case 2:

            buscarPersonaGanadoraRec(listaPersonas,ganador);

            break;

        case 3:

            apilarOtrosPremiosYEliminar(&listaPersonas,listaBoletos,&otrosPremios,ganador);

            break;

        case 4:

            mencionarGanadoresRec(&otrosPremios,&aux);

            break;

        case 5:

            break;

        case 0:

            printf("El programa se cerrara...\n");

            break;

        default:

            printf("Numero de operacion incorrecto, vuelva a intentar\n\n");

            break;
        }

        printf("Desea realizar otra operacion? [s/n]\n");
        fflush(stdin);
        scanf("%c",&seguir);

        system("pause");
        system("cls");

    }
    while(opcion!=0 && seguir=='s');

    return 0;
}

///Funciones auxiliares

nodoDPersona* crearNodoDoblePersona(Persona dato)
{
    nodoDPersona* nuevo=(nodoDPersona*)malloc(sizeof(nodoDPersona));
    nuevo->p=dato;
    nuevo->sig=NULL;
    nuevo->ant=NULL;

    return nuevo;
}

nodoDBoleto* crearNodoDobleBoleto(Boleto dato)
{
    nodoDBoleto* nuevo= (nodoDBoleto*)malloc(sizeof(nodoDBoleto));
    nuevo->b=dato;
    nuevo->sig=NULL;
    nuevo->ant=NULL;

    return nuevo;
}

/*1. Cargar los datos desde los archivos
a. Generar una lista doblemente enlazada de tipo “Persona” ordenada por
“nro_carton” ascendente.
b. Generar una lista doblemente enlazada de tipo “Boleto” ordenada por
“nro_carton” ascendente.
c. Leer el archivo “BoletoGanador.bin” y guardarlo en una variable de tipo Boleto.*/

void insertarPersonasOrdenadas (nodoDPersona** listaDoble,Persona dato)
{
    nodoDPersona* nuevo=crearNodoDoblePersona(dato);
    nodoDPersona* anterior;
    nodoDPersona* actual;

    if(*listaDoble==NULL)
    {
        *listaDoble=nuevo;
    }
    else
    {
        if(nuevo->p.nro_carton<(*listaDoble)->p.nro_carton)
        {
            nuevo->sig=*listaDoble;
            *listaDoble=nuevo;
        }
        else
        {
            anterior=*listaDoble;
            actual=(*listaDoble)->sig;

            while(actual != NULL && actual->p.nro_carton < nuevo->p.nro_carton)
            {
                anterior=actual;
                actual=actual->sig;
            }
            anterior->sig=nuevo;
            nuevo->ant=anterior;
            nuevo->sig=actual;

            if(actual != NULL)
            {
                actual->ant = nuevo;
            }
        }

    }
}

void insertarBoletosOrdenados (nodoDBoleto** listaDoble,Boleto dato)
{
    nodoDBoleto* nuevo=crearNodoDobleBoleto(dato);
    nodoDBoleto* anterior;
    nodoDBoleto* actual;

    if(*listaDoble==NULL)
    {
        *listaDoble=nuevo;
    }
    else
    {
        if(nuevo->b.nro_carton<(*listaDoble)->b.nro_carton)
        {
            nuevo->sig=*listaDoble;
            (*listaDoble)->ant=nuevo;
        }
        else
        {
            anterior=*listaDoble;
            actual=(*listaDoble)->sig;

            while(actual != NULL && actual->b.nro_carton < nuevo->b.nro_carton)
            {
                anterior=actual;
                actual=actual->sig;
            }
            anterior->sig=nuevo;
            nuevo->ant=anterior;
            nuevo->sig=actual;

            if(actual != NULL)
            {
                actual->ant = nuevo;
            }
        }

    }
}

void mostrarPersona(Persona p)
{
    printf("\nNombre: %s\n",p.nombre);
    printf("DNI: %s\n",p.DNI);
    printf("Nro Carton: %i\n",p.nro_carton);
}

void mostrarBoleto(Boleto b)
{
    printf("\nNumero de carton: %i\n",b.nro_carton);
    printf("Numero sorteado: %s\n",b.num);
}

void mostrarBoletoGanador(Boleto b)
{
    printf("\nNumero de carton: %i\n",b.nro_carton);
    printf("Numero sorteado: %s\n",b.num);
}


void cargarDatosPersona(nodoDPersona** listaPersonas)
{
    FILE* archiPersonas=fopen(archivoPersonas,"rb");
    Persona p;

    if(archiPersonas!=NULL)
    {
        while(fread(&p,sizeof(Persona),1,archiPersonas)>0)
        {
            insertarPersonasOrdenadas(listaPersonas,p);
        }
        fclose(archiPersonas);
    }
}

void cargarDatosBoleto(nodoDBoleto** listaBoletos)
{
    FILE* archiBoletos=fopen(archivoBoletos,"rb");
    Boleto b;

    if(archiBoletos!=NULL)
    {
        while(fread(&b,sizeof(Boleto),1,archiBoletos))
        {
            insertarBoletosOrdenados(listaBoletos,b);
        }
        fclose(archiBoletos);
    }
}


void cargarBoletoGanador(Boleto* ganador)
{
    FILE* archiGanador=fopen(archivoGanador,"rb");
    Boleto b;

    if(archiGanador!=NULL)
    {
        while(fread(&b,sizeof(Boleto),1,archiGanador)>0)
        {
            *ganador=b;
        }

        fclose(archiGanador);
    }
}

void mostrarListaPersonas(nodoDPersona* listaPersonas)
{
    while(listaPersonas!=NULL)
    {
        mostrarPersona(listaPersonas->p);
        listaPersonas=listaPersonas->sig;
    }
}

void mostrarListaBoletos(nodoDBoleto* listaBoletos)
{
    while(listaBoletos!=NULL)
    {
        mostrarBoleto(listaBoletos->b);
        listaBoletos=listaBoletos->sig;
    }
}



/*2. Hacer una función que busque la persona con el billete ganador e informar su
nombre. Resolver de manera recursiva.*/


void buscarPersonaGanadoraRec(nodoDPersona* listaPersonas,Boleto ganador)
{
    if(listaPersonas==NULL)
    {
        return 0;
    }
    else
    {
        if(listaPersonas->p.nro_carton==ganador.nro_carton)
        {
            printf("El ganador es: \n");
            mostrarPersona(listaPersonas->p);
        }
        buscarPersonaGanadoraRec(listaPersonas->sig,ganador);
    }
}



/*3. Hacer una función que elimine de la lista de personas y apile en una Pila
“otrosPremios”, aquellas personas ganadoras del 2do premio y 3er premio .*/


void eliminarNodoDoble(nodoDPersona** listaDoble,Persona p)
{
    nodoDPersona* laux;
    nodoDPersona* actual;
    nodoDPersona* anterior;

    if(*listaDoble!=NULL)
    {
        if(strcmp((*listaDoble)->p.DNI,p.DNI)==0)
        {
            laux=*listaDoble;
            *listaDoble=(*listaDoble)->sig;
            (*listaDoble)->ant=NULL;
            free(laux);
        }
        else
        {
            printf("Hola\n");
            actual=(*listaDoble)->sig;
            anterior=*listaDoble;

            while(actual!=NULL && strcmp(actual->p.DNI,p.DNI)!=0)
            {
                anterior=actual;
                actual=actual->sig;
            }
            if(actual != NULL && strcmp(actual->p.DNI,p.DNI)==0)
            {
                laux=actual;
                anterior->sig=actual->sig;

                if(actual->sig != NULL)
                {
                    actual->sig->ant=anterior;
                }
                free(laux);
            }
        }
    }
}

void apilarOtrosPremiosYEliminar(nodoDPersona** listaPersonas, nodoDBoleto* listaBoletos,Pila* otrosPremios,Boleto ganador)
{
    int aciertos=0;

    while(listaBoletos!=NULL)
    {
        for(int i=0; i<4; i++)
        {
            if(listaBoletos->b.num[i]==ganador.num[i])
            {
                aciertos++;
            }
        }
        if(aciertos>=2 && aciertos<=4)
        {
            apilar(otrosPremios,(*listaPersonas)->p);
            eliminarNodoDoble(listaPersonas,(*listaPersonas)->p);
        }
        listaBoletos=listaBoletos->sig;
        aciertos=0;
    }
}



/*4. Hacer una función genérica para mencionar los ganadores del resto de los premios
de manera inversa al orden de la pila. La pila recibida por parámetro no debe perder
sus valores. Resolver de manera recursiva*/

void mencionarGanadoresRec(Pila* otrosGanadores,Pila* aux)
{
    char nombre[20];

    if(pilaVacia(*otrosGanadores))
    {
        return 0;
    }
    else
    {
        strcpy(nombre,tope(*otrosGanadores).nombre);
        apilar(aux,desapilar(otrosGanadores));
        printf("%s\n",nombre);
        mencionarGanadoresRec(otrosGanadores,aux);
        apilar(otrosGanadores,desapilar(aux));
    }
}
