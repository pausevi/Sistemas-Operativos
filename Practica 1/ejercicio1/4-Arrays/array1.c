#include <stdio.h>

#define N 5

void init_array(int array[], int size) ;
void init_array2(int array[N]);

int main(void)
{
	int i,list[N];
	printf("Address of list %p Address of list[0]: %p Address of list[1]: %p. Sizeof list %lu \n",
			list, &list[0], &list[1], sizeof(list));

	init_array(list, N);
	for (i = 0; i < N; i++)
		printf("next: %d ", list[i]);
	printf("\n-------------------------\n");

	init_array2(list);
	for (i = 0; i < N; i++)
		printf("next: %d ", list[i]);
	printf("\n-------------------------\n");
}

void init_array(int array[], int size)
{
	int i;
	printf("Address of the array: %p Sizeof array %lu \n", array, sizeof(array));
	for (i = 0; i < size; i++)
		array[i] = i;
	printf("Array initialized\n\n");
}

void init_array2(int array[N])
{
	int i;
	printf("Address of the array: %p Sizeof array %lu \n", array, sizeof(array));
	for (i = 0; i < N; i++)
		array[i] = i*2;
	printf("Array initialized\n\n");
}


/* 
- ¿Por qué no es necesario escribir "&list" para obtener la dirección del array list?
	Porque list ya es un puntero al primer elemento del array. Es decir list = &list[0]

– ¿Qué hay almacenado en la dirección de list?
	Se lmacena el primer elemento del array

– ¿Por qué es necesario pasar como argumento el tamaño del array en la función init_array?
	Porque realmente lo que se esta pasando es un puntero a la primera direccion del array, no el array en si. Por lo tanto es imposible 
	saber el tamaño del array sinque se le pase el tamaño.

– ¿Por qué el tamaño devuelto por sizeof() para el array de la función init_array no coincide con el declarado en main()?
	Porque devuelve el tamaño del puntero, ya que como hemos dicho antes, el array en si es un puntero a la primera direccion del arrayç

– ¿Por qué NO es necesario pasar como argumento el tamaño del array en la función init_array2?
	Porque usa la constante global como tamaño del array

– ¿Coincide el tamaño devuelto por sizeof() para el array de la función init_array2 con el declarado
en main()?
	No porque como hemos dicho antes, cuando un array se pasa por referencia, se maneja como un puntero
*/