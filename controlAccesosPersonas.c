#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<time.h>
#define tam 50

struct persona
{
    unsigned long int dni;
    char nombre[15];
    char apellido[20];
    int pin;
    char estado; //I = ingreso, E = egreso, B = bloqueado
    int primer_ingreso; //Ingresa = 0 AFUERA, = 1 ADENTRO, = 2 BLOQUEADO
};

//Declaracion de funciones
void cargarPersonas(struct persona * datos,int *contadorTotal); //C
void ingresoPersona(struct persona *datos, int );  //C
void ingresePin(struct persona *datos, int i);
void ingresePin_Exit(struct persona *datos,int i);
void salidaPersona(struct persona *datos, int );
void movimientoPersonas(struct persona *datos, int i); //C
void mostrarEstadisticas (struct persona *datos, int );
int mostrarRegistro(FILE *registro);                   //C
int buscardni (struct persona nueva[], int indice, int dni);
void crearBD(struct persona* myarray,int *);

int main()
{
    int opcion;
    int contadorTotal = -1;
    FILE *aFile;
    FILE *registro;

    struct persona *datos;
    struct persona *myarray;

    datos =(struct persona*)malloc(tam*sizeof(struct persona));
    myarray =(struct persona*)malloc(tam*sizeof(struct persona));


do{
    printf("\n     ======================================================================\n");
	printf("\t\t\t     Bienvenido.\n");
	printf("\t   Control de accesos y egresos de personas con autorizaci%cn.\n",162);
	printf("     ======================================================================\n");
	printf("\n\t\t\t\tMENU PRINCIPAL\n");
    printf("\n\t\t[1]. Cargar base de datos\n");
    printf("\t\t[2]. Ingresar persona\n");
    printf("\t\t[3]. Egresar persona\n");
    printf("\t\t[4]. Informar estadisticas del dia\n");
    printf("\t\t[5]. Mostrar registro diario\n");
    printf("\t\t[6]. Cargar persona en la base de datos.\n");
    printf("\t\t[7]. Salir\n");
    printf("\n\t\tIngrese su opci%cn: [ ]\b\b",162);
    fflush(stdin);
    scanf("%d", &opcion);

    switch(opcion){
        case 1:
        cargarPersonas(datos,&contadorTotal);
        printf("\n");
        printf("\n\t\t");
        system("pause");
        system("cls");
            break;
        case 2:
        ingresoPersona(datos,contadorTotal);
        printf("\n");
        printf("\n\t\t");
        system("pause");
        system("cls");
            break;
        case 3:
        salidaPersona(datos,contadorTotal);
        printf("\n");
        printf("\n\t\t");
        system("pause");
        system("cls");
            break;
        case 4:
        mostrarEstadisticas(datos,contadorTotal);
        printf("\n");
        printf("\n\t\t");
        system("pause");
        system("cls");
            break;
        case 5:
        mostrarRegistro(registro);
        printf("\n");
        printf("\n\t\t");
        system("pause");
        system("cls");
            break;
        case 6:
        crearBD(myarray,&contadorTotal);
        printf("\n");
        printf("\n\t\t");
        system("pause");
        system("cls");
            break;
    }

}
while(opcion != 7);
free(datos);
return 0;
}


void cargarPersonas(struct persona * datos,int *contadorTotal)
{

long int contador=0;
FILE *aFile;
aFile = fopen("bd.bin", "rb");    // Abrimos el archivo en modo lectura y binario

if(contadorTotal == 0)
 {
  printf("\n\t\tNo hay personas registradas aun...");
 }else{
  if(aFile == NULL)
    {  // En caso de no existir, le devolvemos el error
   printf("\n\t\tEl archivo no existe");

    } else {
      fseek(aFile, 0, SEEK_END);  //Fichero, desplazamiento, desde
      contador=ftell(aFile);
      //Si lo tuviera que leer deberia rebobinarlo con rewind() o con fseek en la posicion SEEK_SET
      rewind(aFile);
      fread(datos, sizeof(char), contador, aFile);

      printf("\n\t\tBase de datos cargada correctamente.\n");
    *contadorTotal = contador/sizeof(struct persona);
    for (int i = 0;i < *contadorTotal ; i++)
     {
       printf("\n\t\t%s - %s - %ld - %d",datos[i].nombre,datos[i].apellido,datos[i].dni,datos[i].pin);
     }
   }
 }
    fclose(aFile);
}


void ingresoPersona(struct persona *datos,int contadorTotal)
{
unsigned long int DNI;
int dni_encontrado = 0;

if (contadorTotal == 0)
{
    printf("\n\t\tNo se a cargado la base de datos...\n");
}else {
    printf("\n\t\tIngrese DNI: ");
    scanf("%ld", &DNI);

    for(int i=0; i<5;i++)
     {
      if((*(datos+i)).dni==DNI)
       {
        dni_encontrado++;
        printf("\n\t\t%s %s\n",(*(datos+i)).nombre,(*(datos+i)).apellido);
        ingresePin(datos, i);
       }

     }

     if(dni_encontrado == 0)
     {
         printf("\n\t\tNo se encontro el DNI...");
     }
   }
}


void ingresePin(struct persona *datos, int i)
{
int PIN;
int intento = 0;
int ingresa = 0;                          //Ingresa = 0 AFUERA, = 1 ADENTRO, = 2 BLOQUEADO

 if ((*(datos+i)).estado == 'B')
  {
    printf("\n\t\tUSUARIO BLOQUEADO");

  }

else if((*(datos+i)).primer_ingreso == 1 && (*(datos+i)).estado == 'I')
 {
    printf("\n\t\tYa se encuentra adentro...\n");
 }
  /***********EN CASO DE HABER INGRESADO ANTERIORMENTE***********/

  else if((*(datos+i)).primer_ingreso == 1 && (*(datos+i)).estado == 'E')

  {
  printf("\t\tIngrese pin: ");
  scanf("%d", &PIN);
  //PIN = '\0';

  //if(strcmp((*(datos+i)).pin,PIN)==0)
   if ((*(datos+i)).pin== PIN)
   {
    printf("\t\tClave correcta, bienvenido Nuevamente.");
    (*(datos+i)).estado = 'I';
   }else
    {
     while ((*(datos+i)).pin != PIN)  //solicita la clave hasta que la ingrese bien
      {
       printf("\n\t\tIngrese nuevamente el pin:");
       scanf("%d", &PIN);
       //PIN[4] = '\0';
      }
        printf("\n\t\tClave correcta, bienvenido Nuevamente.");
        (*(datos+i)).estado = 'I';
    }
  } else {

            /*********EN CASO DE INGRESAR POR PRIMERA VEZ*********/

    printf("\n\t\tTiene 3 intentos para ingresar su pin\n\t\tcaso contrario, sera bloqueado por el resto del dia\n");

    do{

    printf("\n\t\tIngrese pin: ");
    scanf("%d", &PIN);

    if((*(datos+i)).pin== PIN)
    {
    printf("\n\t\tClave correcta, bienvenido %s %s.",(*(datos+i)).nombre,(*(datos+i)).apellido);
    (*(datos+i)).estado = 'I';

    movimientoPersonas(datos, i);  /*REGISTRO*/

    ingresa = 1;
    (*(datos+i)).primer_ingreso = 1;

        }else
             {
              intento++;
              ingresa = 3;
             }
        }while(intento<3 && ingresa==3);
         if(intento == 3)
         {
          printf("\t\tAlcanzo el maximo de intentos, USUARIO BLOQUEADO");
           (*(datos+i)).estado = 'B';

           movimientoPersonas(datos, i);  /*REGISTRO*/
         }
     }
}


void ingresePin_Exit(struct persona *datos,int i)
{

int PIN;

     /********PARA LA REGISTRAR LA SALIDA********/

if((*(datos+i)).primer_ingreso == 1 && (*(datos+i)).estado == 'I' )
{
    printf("\t\tIngrese pin: ");
    scanf("%d", &PIN);

    if((*(datos+i)).pin== PIN)
    {
      printf("\t\tClave correcta,Hasta Luego.");
      (*(datos+i)).estado = 'E';

      movimientoPersonas(datos, i);   /*REGISTRO*/
    }else
    {
        while ((*(datos+i)).pin!= PIN)  //solicita la clave hasta que la ingrese bien
        {
         printf("\n\t\tIngrese nuevamente el pin:");
         scanf("%d", &PIN);
       }

       printf("\t\tClave correcta, Hasta Luego.");
      (*(datos+i)).estado = 'E';
      movimientoPersonas(datos, i);   /*REGISTRO*/
  }
 } else {
            printf("\n\t\tAun no ingreso...\n");
        }
}


void salidaPersona(struct persona *datos, int contadorTotal)
{
  unsigned long int dniSalida;
  int correcto = 0;

  if(contadorTotal == 0)
  {
      printf("\n\t\tAun no esta cargada la base de datos...\n");
  }else {

  printf("\n\t\tIngrese su DNI:");
  scanf("%ld",&dniSalida);

  for (int i = 0; i<tam; i++)
  {
    if((*(datos+i)).dni == dniSalida)
    {
     printf("\n\t\t%s %s\n",(*(datos+i)).nombre,(*(datos+i)).apellido);
     ingresePin_Exit(datos, i);
     correcto = 1;
    }
   }

   if(correcto ==0)
   {
       printf("\n\t\tDNI incorrecto, no se encuentra en la base de datos...\n");
   }

  }
 }

 /* Se requiere registrar un log con los ingresos y egresos del día de cada persona,
  el archivo de log (log.out), debe ser de texto y contener el siguiente formato por cada novedad a
  ingresar (una novedad por línea): Hora|legajo|estado
 La hora debe estar almacenada con el siguiente formato: hhmm y el estado debe ser un carácter I o E,
 para el ingreso o egreso respectivamente.
*/

void movimientoPersonas(struct persona *datos, int i){

    FILE *registro;

    int hora,minuto,horaMinuto;
    time_t tiempoAhora;
    time(&tiempoAhora);
    struct tm *miTiempo = localtime(&tiempoAhora);
    hora = miTiempo->tm_hour;
    minuto =  miTiempo->tm_min;
    registro = fopen("log.out", "a");
    fprintf(registro,"\t\t%d:%d | %ld | %c\n",hora,minuto,(*(datos+i)).dni,(*(datos+i)).estado);
    fclose(registro);
}


int mostrarRegistro(FILE *registro)
{
    registro = fopen("log.out", "r");

    if (registro == NULL){
        printf("\n\t\tEl archivo no existe");
        return 1;
    }
    fseek(registro, 0, SEEK_END);//Se desplaza desde el final hasta el comienzo
    int num_elementos = ftell(registro);
    rewind(registro);

    char * cadena = (char *) calloc(sizeof(char), num_elementos);

        if (cadena == NULL)

        {
            printf("\n\t\tError en la reserva de la memoria");
            return 2;
        }
        //Es necesaria la validacion para que salga en caso de no asignar memoria
    int num_elementos_leidos = fread(cadena, sizeof(char), num_elementos, registro);
    printf("\n\t\tHora | Legajo   | Estado\n");
    printf("\n%s\n", cadena); //imprimo el contenido
    free(cadena);
    fclose(registro);
    printf("\n\t\tSe ha leido el archivo correctamente.\n");
    return 0;
}



void mostrarEstadisticas (struct persona *datos, int contadorTotal)
{

if(contadorTotal == 0)
{
    printf("\n\t\tNo se a registrado personas autorizadas aun...\n");
    printf("\n\t\tCantidad de personas autorizadas a ingresar: %d",contadorTotal);
} else {
int contadorBloqueados = 0;
int contadorPresentes = 0;
int contadorAusentes = 0;



for (int i = 0; i < tam; i++)
 {
  if(datos[i].estado == 'B')
   {
    contadorBloqueados++;
    contadorAusentes--;
    contadorTotal--;
   }
  if(datos[i].primer_ingreso == 0)
   {
    contadorAusentes++;
   }
  if(datos[i].primer_ingreso == 1)
   {
    contadorPresentes++;
   }
 }


 printf("\n\t\tCantidad de personas autorizadas a ingresar: %d",contadorTotal);
 printf("\n\t\tCantidad de presentes: %d",contadorPresentes);
 printf("\n\t\tCantidad de ausentes: %d",contadorAusentes);
 printf("\n\t\tCantidad de bloqueados: %d",contadorBloqueados);

 }
}

void crearBD(struct persona * miarray,int * contadorTotal)
{
int respuesta;
int indice = 0;
int dni1,dni2,dni_encontrado;
FILE *aFile;

    if((*(contadorTotal)) < tam){

        do{
            printf("\n\t\tIngrese un nombre:  ");
            scanf("%s",(*(miarray+indice)).nombre);

            printf("\n\t\tIngrese un apellido:  ");
            scanf("%s",(*(miarray+indice)).apellido);

            printf("\n\t\tIngrese el DNI:  ");
            scanf("%d",&dni1);

            while(dni1<1000000 || dni1>100000000){

                printf("\n\t\tDNI invalido, ingrese un formato correcto:  ");
                scanf("%d",&dni1);
            }

        /**********************************************************************************/
            /***VALIDAR QUE EL  DNI INGRESADO, NO SE REPITA NUEVAMENTE***/
            dni_encontrado = buscardni(miarray,indice,dni1);

            if(dni_encontrado == -1)
            {
            miarray[indice].dni = dni1;
            printf("\n\t\tDNI correcto\n");
            }
            else if (dni_encontrado == 1)
            {
             do{
                printf("\n\t\tEl dni ya fue ingresado, ingrese otro nuevamente:  ");
                scanf("%d",&dni2);
            }while(dni2 == dni1);

              miarray[indice].dni = dni2;
            }

        /**********************************************************************************/
            /***VALIDAR QUE EL  PIN INGRESADO SEA CORRECTO***/

            printf("\n\t\tIngrese un PIN de seguridad de 4 numeros:  ");
            scanf("%d",&miarray[indice].pin);

            while(miarray[indice].pin<1000 || miarray[indice].pin>=9999)
            {
            printf("\n\t\tPin incorrecto, ingrese un pin valido:  ");
            scanf("%d",&miarray[indice].pin);
            }

        /**********************************************************************************/



            miarray[indice].primer_ingreso = 0;
            miarray[indice].estado = 'E';

            printf("\n\t\tQuiere ingresar a otra persona a la base de datos?");
            printf("\n\t\t[1] si ---- [2]no:  ");
            scanf("%d",&respuesta);

            indice++;
            (*(contadorTotal))++;
            system("cls");

        }while(respuesta != 2 && indice<tam);

        if((*(contadorTotal))==tam)
        {
            printf("\n\t\tEl maximo de personas registradas ya fue alcanzado");
        }

    aFile = fopen("bd.bin", "wb");

    for (int i=0;i<indice;i++)
     {
      fwrite(&(miarray[i]), sizeof(struct persona), 1, aFile);
     }
    free(miarray);
    fclose(aFile);
    printf("\n\t\tDatos cargados correctamente.\n");

    }else
    {
        printf("\t\tel maximo de registro ya fue alcanzado...\n");
    }
}


int buscardni (struct persona nueva[], int indice, int dni)
{
 int buscador = 0;

 while(buscador < indice && nueva[buscador].dni == dni)
    {
     buscador++;
    }

    if(buscador == 0)
    {
        return -1;

    }else{

        return 1;
    }

}
