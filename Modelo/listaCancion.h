#ifndef listaCancion_h
#define listaCancion_h
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct cancion cancion;

struct cancion
{
  char* nombre; // Almacena el nombre de la canción
  int anyo; // Almacena el año de la canción
  List* genero; // Almacena el género de la canción
  char* artista; // Almacena el nombre del artista
  char* nombreLista; // Almacena el nombre de la lista a la que pertenece la canción
};

typedef struct listaCanciones listaCanciones;

struct listaCanciones
{
  int totalCanciones; // Almacena el total de canciones guardadas
  List* ListaReproduccion; // Lista que guarda todas las canciones
};

typedef struct datosLista datosLista;

struct datosLista
{
  char* nombreLista; //Almacena el nombre de la lista
  int cancionesLista; // Almacena el total de canciones que tiene cada lista
};

//Se encarga de inicializar la lista
listaCanciones* createPlaylist();

//Lee lo escritop por pantalla para poder almacenarlo en un arreglo dinamico
void leerChar (char** nombreArchivo);

//Reserva memoria para las cadena de caracteres
char* reservarMemoria (int largo);

//
void asignarGenero(List* genero, char* aux);

void imprimirGenero (List* genero);

//Comprobar si la canción esta presente en la lista de reproducción
int comprobarSiEsta(cancion* aux, List* listaReproduccion);

void insertarLista(datosLista* datos, List* lista);

//Función que guardara canciones en lista
void guardarCanciones(listaCanciones* listaCancion, char* nombreArchivo, List* datos, List* generosSinCortar);

void exportarCanciones(listaCanciones* listaCancion, char* nombreArchivo, List* listaGenero);

void agregarCancion(listaCanciones* listaCancion, cancion* cancionUsuario, List* datos);

void buscarCancionNombre(listaCanciones* listaCancion, char* nombre);

void buscarCancionGenero(listaCanciones* listaCancion, char* genero);

void buscarCancionArtista(listaCanciones* listaCancion, char* artista);

void disminuirDatosLista(datosLista* tmpDatos, List* datos);

void eliminarCancion(List* listaReproduccion, cancion* cancionUsuario, List* datos);

void mostrarNombresListas(List* datos);

void mostrarListaCompleta(listaCanciones* listaCancion, char* nombreLista);

void mostrarTodasLasCanciones(listaCanciones* listaCancion);

void imprimirOpciones();

void menu(listaCanciones* listaCancion);

#endif