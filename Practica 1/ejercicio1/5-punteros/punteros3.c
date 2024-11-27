#include <stdio.h>
#include <stdlib.h>

int nelem;
int c;

int main(void)
{
	int *ptr;
	int i;

	c = 37;
	nelem = 127;
	ptr = (int*) malloc(nelem * sizeof(int));
	for (i=0; i < nelem; i++)
		ptr[i] = i;

	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	ptr = &c;
	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	free(ptr);

}

/*
– ¿Por qué cambia el valor de ptr[13] tras la asignación ptr = &c;?
	Porque el puntero pasa a apuntar a otra zona y ptr[13] ahora apunta a &c + 13

– El código tiene (al menos) un error. ¿Se manifiesta en compilación o en ejecución? ¿Por qué?
	Se manifiesta en ejecución ya que se libera la direccion de c apuntada por ptr, que ya no es la reservada dinamicamente por malloc

– ¿Qué ocurre con la zona reservada por malloc() tras a asignación ptr = &c;? ¿Cómo se puede acceder a ella? ¿Cómo se puede liberar dicha zona?
	Al no tener una referencia sobre ella se pierde su acceso y se nos hace imposible liberar esa zona. Se podria acceder a ella si se usara un
	puntero auxiliar para mantener la referencia y luego liberarla.
*/
