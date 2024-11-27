#include <stdio.h>
#include <stdlib.h>

int c = 7;
int main(void)
{
	int *ptr;
	printf("Address of ptr %p. ptr points to %p. Address of c: %p Value of c %d\n",
			&ptr, ptr, &c, c);

	ptr = &c;
	printf("Address of ptr %p,. ptr points to %p. Address of c: %p Value of c %d\n",
			&ptr, ptr, &c, c);

	*ptr = 4;
	printf("ptr points to %p. Content in the address pointed by ptr: %d Address of c: %p Value of c %d\n",
			ptr, *ptr, &c, c);

	ptr =  (int*) 0x600a48;
	printf("Address of ptr %p. Value of c %d\n", ptr, c);

	*ptr = 13;
	printf("Address of ptr %p. Value of c %d\n", ptr, c);

    return 0;
}

/*
– ¿Qué operador utilizamos para declarar una variable como un puntero a otro tipo?
	Usamos el asterisco *. Por ejemplo --> int *ptr; (ptr es ahora un puntero)

– ¿Qué operador utilizamos para obtener la dirección de una variable?
	Usamos el ampersand &. Por ejemplo --> &c; (Obtenemos la direccion de la vble c)

– ¿Qué operador se utiliza para acceder al contenido de la dirección “a la que apunta” un puntero?
	Usamos el asterisco de nuevo *. Por ejemplo --> *ptr; (Devuelve lo que hay almacenado en la direccion ptr)
	
– Hay un error en el código. ¿Se produce en compilación o en ejecución? ¿Por qué se produce?
	El fallo es de ejecución y se produce en la linea 22 y 19. Se asigna una direccion aleatoria a ptr (Que puede no ser valida o accesible)
	y luego se le asigna un valor. Al no ser una direccion autorizada, produce el error.
*/
