Para poder compilar el archivo de manera correcta hay que abrir una nueva terminal puede ser Powershell o gitbash, luego hay que crear el archivo ejecutable de la siguiente manera *.c ./Modelo/*.c -o main.exe, de esta manera se
creara main.exe, y para poder ejecutarlo basta con introducir ./main.exe y correria correctamente.

Todas las funciones del programa funcionan de manera correcta, el unico pero seria la función get_csv_field que si el final del archivo no posee un salto de linea no lee el ultimo caracter.
