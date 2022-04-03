#include <stdio.h>
#include "Modelo/listaCancion.h"

int main ()
{
  listaCanciones* listaCancion = createPlaylist();
  menu(listaCancion);
  return (0);
}