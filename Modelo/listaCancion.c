#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csvread.h"
#include "listaCancion.h"


//Se encarga de inicializar la lista
listaCanciones* createPlaylist()
{
  listaCanciones* output = (listaCanciones*) calloc (1, sizeof(listaCanciones));
  output->ListaReproduccion = createList();
  output->totalCanciones =0;
  return(output);
}

//Lee lo escrito por pantalla para poder almacenarlo en un arreglo dinamico
void leerChar (char** nombreArchivo)
{
  char buffer[200];
  int largo;
  scanf("%[^\n]s", buffer);
  getchar();
  largo = strlen(buffer) + 1;
  (*nombreArchivo) = (char*) calloc (largo, sizeof(char));
  strcpy((*nombreArchivo), buffer);
}

//Reserva memoria para las cadena de caracteres
char* reservarMemoria (int largo)
{
  char* tmp = (char*) calloc (largo, sizeof(char));
  return tmp;
}

//Se separan los generos para asi poder asignarlos a la lista.
void asignarGenero(List* genero, char* aux)
{
  char delimitar[] = ", "; //Se separan cuando se encuentre una coma
  char* tmp = strtok(aux, delimitar);
  while (tmp != NULL)
  {
    pushBack(genero, tmp);
    tmp = strtok(NULL, delimitar);
  }  
}

//Funcion que recorre la lista de los generos y los imprime
void imprimirGenero (List* genero)
{
  char* tmpGenero = (char*)firstList(genero);
    //Se recorre la lista genero
    while (tmpGenero != NULL)
    {
      printf("%s, ", tmpGenero);
      tmpGenero = (char*)nextList(genero);
    } 
}

//Comprobar si la canción esta presente en la lista de reproducción
int comprobarSiEsta(cancion* aux, List* listaReproduccion)
{
  cancion* tmp = (cancion*)firstList(listaReproduccion);
  while (tmp != NULL)
  {
    if (strcmp(tmp->nombre, aux->nombre) == 0)
    {
      return (1); //Si esta retorna 1
    }
    tmp = (cancion*)nextList(listaReproduccion);
  }
  return (0); // SI no esta retorna 0
}

//Aumenta/Disminuya la cantidad de canciones que tiene cada lista
void insertarLista(datosLista* datos, List* lista)
{
  datosLista* tmp = (datosLista*)firstList(lista);
  while (tmp != NULL)
  {
    //Si la lista existe se aumenta la cantidad de canciones.
    if (strcmp(datos->nombreLista, tmp->nombreLista) == 0)
    {
      tmp->cancionesLista++;
      return;
    }
    tmp = (datosLista*)nextList(lista);
  }
  //Si la lista de reproduccion se grega a la lista y el total de canciones se inicializa en 1
  datos->cancionesLista = 1;
  pushBack(lista, datos);
  
}

//Función que guardara canciones del archivo en lista
void guardarCanciones(listaCanciones* listaCancion, char* nombreArchivo, List* datos, List* generosSinCortar)
{
  FILE* canciones = fopen (nombreArchivo, "r");
  //Si no existe el archivo o hubo un error al abrirlo imprime un mensaje
  if (canciones == NULL)
  {
    printf("El archivo que ingreso no existe o hubo un error al abrirlo\n");
    return;
  }
  char linea[1024];
  int largo;

  //Se lee el archivo csv linea por linea.
  while (fgets (linea, 1023, canciones) != NULL) 
  {
    cancion* tmpCancion = (cancion*) calloc (1, sizeof(cancion));
    datosLista* tmpDatos = (datosLista*) calloc (1, sizeof(datosLista));
    char* genero; // Almacenara los generos sin separaciones.
    for (int i = 0 ; i < 5 ; i = i + 1) //Itera 5 veces por linea, ya que una canción solo tiene 5 caracteristicas.
    {
      char* aux = get_csv_field(linea, i); // Se guardan los datos en la variable auxiliar
      largo = strlen(aux) + 1;
      switch (i)
      {
        case 0: //Se almacena el nombre
        {
          tmpCancion->nombre = reservarMemoria(largo);
          strcpy(tmpCancion->nombre, aux);
          break;
        }
        case 1: //Se almacena el artista
        {
          tmpCancion->artista = reservarMemoria(largo);
          strcpy(tmpCancion->artista, aux);
          break;
        }
        case 2: //Se almacena el genero
        {
          tmpCancion->genero = createList(); //Se crea la lista de generos
          genero = reservarMemoria(largo); 
          strcpy(genero, aux);
          asignarGenero(tmpCancion->genero, aux);//Se separan los diferentes generos que tiene una sola cancion y se agregan a la lista.
          break;
        }
        case 3: //Se almacena el año de la canción
        {
          tmpCancion->anyo = atoi(aux); //Se transforma el char a int
          break;
        }
        case 4: //Se almacena a la lista que pertenece
        {
          tmpCancion->nombreLista = reservarMemoria(largo);
          tmpDatos->nombreLista = reservarMemoria(largo); 
          strcpy(tmpDatos->nombreLista, aux);
          strcpy(tmpCancion->nombreLista, aux);
          break;
        }
      }
    }
    //Se verifica que no esten en la lista, si no estan se almacena en la lista.
    if (comprobarSiEsta(tmpCancion, listaCancion->ListaReproduccion) == 0)
    {
      pushBack(listaCancion->ListaReproduccion, tmpCancion);
      pushBack(generosSinCortar, genero);
      listaCancion->totalCanciones = listaCancion->totalCanciones + 1;
      insertarLista(tmpDatos, datos);
    }
    //Si se encuentra en la lista se libera la memoria
    else
    {
      free(tmpCancion->nombre);
      cleanList(tmpCancion->genero);
      free(tmpCancion->artista);
      free(tmpCancion->nombreLista);
      free(tmpCancion);
      free(genero);
    }
  }
  fclose(canciones);
  printf("Sus canciones se importaron correctamente\n");
}

void exportarCanciones(listaCanciones* listaCancion, char* nombreArchivo, List* listaGenero)
{
  FILE* archivo = fopen (nombreArchivo, "a");//Se abre en modo "append", el cual escribe al final del archivo si existe o crea uno si no existe.
  cancion* tmpCancion = (cancion*)firstList(listaCancion->ListaReproduccion);
  char* genero = (char*)firstList(listaGenero);
  //Se recorre la lista de canciones hasta que sea nula.
  while (tmpCancion != NULL)  
  {
    fprintf(archivo, "%s, %s, \"%s\", %d, %s\n", tmpCancion->nombre, tmpCancion->artista, genero, tmpCancion->anyo, tmpCancion->nombreLista);//Se escriben los datos en el archivo.
    tmpCancion = (cancion*)nextList(listaCancion->ListaReproduccion);
    genero = (char*)nextList(listaGenero);
  }
  fclose(archivo);
}

void agregarCancion(listaCanciones* listaCancion, cancion* cancionUsuario, List* datos)
{
  datosLista* tmpDatos = (datosLista*)calloc(1, sizeof(char));
  if(comprobarSiEsta(cancionUsuario, listaCancion->ListaReproduccion) == 0)
  {
    int largo = strlen(cancionUsuario->nombreLista);
    tmpDatos->nombreLista = reservarMemoria(largo);
    strcpy(tmpDatos->nombreLista, cancionUsuario->nombreLista);
    pushBack(listaCancion->ListaReproduccion, cancionUsuario);
    listaCancion->totalCanciones = listaCancion->totalCanciones + 1;
    insertarLista(tmpDatos, datos);
  }
  //Si se encuentra en la lista se libera la memoria
  else
  {
    free(cancionUsuario->nombre);
    cleanList(cancionUsuario->genero);
    free(cancionUsuario->artista);
    free(cancionUsuario->nombreLista);
    free(cancionUsuario);
  }
}

void imprimirDatosCanciones (cancion* tmp)
{
  printf("%s, ", tmp->nombre);
  printf("%s, ", tmp->artista);
  imprimirGenero(tmp->genero);
  printf("%d, ", tmp->anyo);
  printf("%s\n", tmp->nombreLista);
}

void buscarCancionNombre(listaCanciones* listaCancion, char* nombre)
{
  int cont = 0;
  cancion* tmp = firstList(listaCancion->ListaReproduccion);
  while(tmp != NULL)
  {
    if(strcmp(nombre, tmp->nombre) == 0)
    {
      imprimirDatosCanciones(tmp);
      cont = cont + 1;
      return;
    }
    tmp = nextList(listaCancion->ListaReproduccion);
  }
  if (cont == 0)
  {
    printf("No existe ninguna cancion con este nombre en su lista de reproduccion.\n");
  }
}

void buscarCancionGenero(listaCanciones* listaCancion, char* genero)
{
  int cont = 0;
  cancion* tmp = (cancion*)firstList(listaCancion->ListaReproduccion);
  char* tmpGenero;
  while (tmp != NULL)
  {
    tmpGenero = (char*)firstList(tmp->genero);
    //Se recorre la lista genero
    while (tmpGenero != NULL)
    {
      if (strcmp(genero, tmpGenero) == 0) // Se compara si el genero ingresado esta en la lista
      {
        imprimirDatosCanciones(tmp);
        cont = cont + 1;
      }
      tmpGenero = (char*)nextList(tmp->genero);
    } 
    tmp = (cancion*)nextList(listaCancion->ListaReproduccion);
  }
  if (cont == 0)
  {
    printf("No existe ninguna cancion de este género en su lista de reproduccion.\n");
  }
}

void buscarCancionArtista(listaCanciones* listaCancion, char* artista)
{
  int cont = 0;
  cancion* tmp = (cancion*)firstList(listaCancion->ListaReproduccion);
  while (tmp != NULL)
  {
    if (strcmp(artista, tmp->artista) == 0)
    {
      imprimirDatosCanciones(tmp);
      cont = cont + 1;
    }
    tmp = (cancion*)nextList(listaCancion->ListaReproduccion);
  }
  if (cont == 0)
  {
    printf("No existe ninguna cancion de este artista en su lista de reproduccion.\n");
  }
}
void disminuirDatosLista(datosLista* tmpDatos, List* datos)
{
  datosLista* tmp = (datosLista*)firstList(datos);
  while (tmp != NULL)
  {
    if (strcmp(tmpDatos->nombreLista, tmp->nombreLista) == 0)
    {
      tmp->cancionesLista--;
      if (tmp->cancionesLista == 0)
      {
        popCurrent(datos);
      }
      return;
    }
    tmp = (datosLista*)nextList(datos);
  }
}
void eliminarCancion(List* listaReproduccion, cancion* cancionUsuario, List* datos)
{
  cancion* tmp = (cancion*)firstList(listaReproduccion);
  datosLista* tmpDatos = (datosLista*)calloc(1, sizeof(char));
  while (tmp != NULL)
  {
    if ((strcmp(tmp->nombre, cancionUsuario->nombre) == 0) && (strcmp(tmp->artista, cancionUsuario->artista) == 0) && (tmp->anyo == cancionUsuario->anyo))
    {
      int largo = strlen(tmp->nombreLista);
      tmpDatos->nombreLista = reservarMemoria(largo);
      strcpy(tmpDatos->nombreLista, tmp->nombreLista);
      disminuirDatosLista(tmpDatos, datos);
      popCurrent(listaReproduccion);
      return;
    }
    tmp = (cancion*)nextList(listaReproduccion);
  }
  printf("No existe la cancion que desea eliminar\n");
}

void mostrarNombresListas(List* datos)
{
  datosLista* tmpDatos = (datosLista*)firstList(datos);
  if (tmpDatos == NULL)
  {
    printf("No existe ninguna lista de reproduccion\n");
  }
  while(tmpDatos != NULL)
  {
    printf("%s contiene %d cancion/es\n", tmpDatos->nombreLista, tmpDatos->cancionesLista);
    tmpDatos = (datosLista*)nextList(datos);
  }
}

void mostrarListaCompleta(listaCanciones* listaCancion, char* nombreLista)
{
  int cont = 0;
  cancion* tmp = (cancion*)firstList(listaCancion->ListaReproduccion);
  while (tmp != NULL)
  {
    if (strcmp(nombreLista, tmp->nombreLista) == 0)
    {
      imprimirDatosCanciones(tmp);
      cont = cont + 1;
    }
    tmp = (cancion*)nextList(listaCancion->ListaReproduccion);
  }
  if (cont == 0)
  {
    printf("No existe ninguna Lista de reproducción con este nombre\n");
  }
}

void mostrarTodasLasCanciones(listaCanciones* listaCancion)
{
  cancion* tmp = (cancion*)firstList(listaCancion->ListaReproduccion);
  if (tmp == NULL)
  {
    printf("No se ha agregado ninguna cancion\n");
  }
  while (tmp != NULL)
  {
    imprimirDatosCanciones(tmp);
    tmp = (cancion*)nextList(listaCancion->ListaReproduccion);
  }
}

void imprimirOpcionesMenu()
{
  printf("1.- Importar canciones.\n");
  printf("2.- Exportar canciones.\n");
  printf("3.- Agregar cancion.\n");
  printf("4.- Buscar cancion por nombre.\n");
  printf("5.- Buscar cancion por artista.\n");
  printf("6.- Buscar cancion por genero.\n");
  printf("7.- Eliminar una cancion.\n");
  printf("8.- Mostrar nombres de listas de reproduccion.\n");
  printf("9.- Mostrar lista de reproduccion.\n");
  printf("10.- Mostrar todas las canciones.\n");
  printf("11.- Salir\n");
  printf("Seleccione una opcion.\n");
}

void menu(listaCanciones* listaCancion)
{
  int opcion;
  List* datos = createList(); //Almacena los datos de las listas, la cantidad de cada una.
  List* generosSinCortar= createList(); //Almacena los generos sin ser separados.
  while(1)
  {
    do
    {
      imprimirOpcionesMenu();
      scanf("%d", &opcion);
      getchar();
    } while (opcion< 1 || opcion > 11);

    switch (opcion)
    {
      case 1:
      {
        printf("Ingrese el archivo a importar:\n");
        char* nombreArchivo = NULL;
        leerChar(&nombreArchivo);
        guardarCanciones(listaCancion, nombreArchivo, datos, generosSinCortar);
        break;
      }
      case 2:
      {
        printf("Ingrese el nombre del archivo al que desea importar las cacniones\n");
        char* archivo;
        leerChar(&archivo);
        exportarCanciones(listaCancion, archivo, generosSinCortar);
        break;
      }
      case 3:
      {
        cancion* cancionUsuario = (cancion*) calloc (1, sizeof(cancion));
        cancionUsuario->genero = createList();
        char* genero;
        printf("Ingrese el nombre de la cancion:\n");
        leerChar(&cancionUsuario->nombre);
        printf("Ingrese el artista de la cancion:\n");
        leerChar(&cancionUsuario->artista);
        printf("Ingrese el genero de la cancion:\n");
        leerChar(&genero);
        asignarGenero(cancionUsuario->genero, genero);
        printf("Ingrese el año de la cancion:\n");
        scanf("%d", &cancionUsuario->anyo);
        getchar();
        printf("Ingrese a la lista que pertenece la cancion:\n");
        leerChar(&cancionUsuario->nombreLista);
        agregarCancion(listaCancion, cancionUsuario, datos);
        break;
      }
      case 4:
      {
        char* nombreCancion;
        printf("Ingrese el nombre de la cancion:\n");
        leerChar(&nombreCancion);
        buscarCancionNombre(listaCancion, nombreCancion);
        break;
      }
      case 5:
      {
        char* artista;
        printf("Ingrese el nombre del artista:\n");
        leerChar(&artista);
        buscarCancionArtista(listaCancion, artista);
        break;
      }
      case 6:
      {
        char* genero;
        printf("Ingrese el género de la cancion:\n");
        leerChar(&genero);
        buscarCancionGenero(listaCancion, genero);
        break;
      }
      case 7:
      {
        cancion* cancionUsuario = (cancion*) calloc (1, sizeof(cancion));
        printf("Ingrese el nombre de la cancion a eliminar:\n");
        leerChar(&cancionUsuario->nombre);
        printf("Ingrese el artista de la cancion a eliminar:\n");
        leerChar(&cancionUsuario->artista);
        printf("Ingrese el año de la cancion a eliminar:\n");
        scanf("%d", &cancionUsuario->anyo);
        getchar();
        eliminarCancion(listaCancion->ListaReproduccion, cancionUsuario, datos);
        break;
      }
      case 8:
      {
        mostrarNombresListas(datos);
        break;
      }
      case 9:
      {
        char* nombreLista;
        printf("Ingrese a la lista que pertenece la cancion:\n");
        leerChar(&nombreLista);
        mostrarListaCompleta(listaCancion, nombreLista);
        break;
      }
      case 10:
      {
        mostrarTodasLasCanciones(listaCancion);
        break;
      }
      case 11:
      {
        printf("Gracias por utilizar el programa\n");
        return;
      }
    }
    do
    {
      printf("¿Desea continuar?\n");
      printf("1- Si. / 2- NO.\n");
      scanf("%d", &opcion);
      getchar();
    } while (opcion < 1 || opcion > 2);
      if (opcion == 2)
      {
        printf("Gracias por utilizar el programa\n");
        break;
      }
  }
}