#include <stdio.h>
#include <stdlib.h>

int nelem;

int main(void)
{
	int *ptr;
	int * ptr2;

	nelem = 127;
	ptr = (int*) malloc(nelem*sizeof(int));
	*ptr = 5;
	ptr[0] = 7;
	ptr2 = ptr;

	printf("Address pointed by ptr %p. Memory content at that address: %d \n",
			ptr, *ptr);

	ptr[1] = 10;
	printf("Address of the element ptr[1] %p. Memory content at that address: %d \n",
			&ptr[1], ptr[1]);

	ptr2++;
	*ptr2 = 15;
	printf("Address of the element ptr[1] %p. Memory content at that address: %d \n",
			&ptr[1], ptr[1]);

	free(ptr);
	*ptr = 3;
	printf("Address pointed by ptr %p. Memory content at that address: %d \n",
			ptr, *ptr);
	return 0;
}

/*
– ¿Cuántos bytes se reservan en memoria con la llamada a malloc()?
	Se reservan --> ptr = (int*) malloc(nelem*sizeof(int)); Que son nelem x 4 (tamaño int), y nelem = 127. En total 506 bytes

– ¿Cuál es la dirección del primer y último byte de dicha zona reservada?
	La direccion del primer byte es ptr y el ultimo es ptr+nelem-1

– ¿Por qué el contenido de la dirección apuntada por ptr es 7 y no 5 en el primer printf()?
	Porque ptr y &ptr[0] es la misma direccion

– ¿Por qué se modfica el contenido de ptr[1] tras la sentencia *ptr2=15;?
	Porque se avanza una posicion de memoria el ptr2 (coincidiendo con ptr1[1]) y luego al asignarle 15 a ptr2 se almacena en ptr1[1]

– Indica dos modos diferentes de escribir el valor 13 en la dirección correspondiente a ptr[100].
	pr[100] = 13 o (ptr+100) = 13;

– Hay un error en el código. ¿Se manifiesta en compilación o en ejecución? Aunque no se manifieste, el error está. ¿Cuál es?
	Tras hacer un free de ptr, luego se accede  a el lo que puede provocar comportamientos indefinidos.
	Además se accede a una zona de memoria a la cual no se tiene acceso y podria provocar un error.
*/

