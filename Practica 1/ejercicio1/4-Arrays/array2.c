#include <stdio.h>

#define N 10

// function that prints on the standard output the contents of the array of v of size size
void imprimeArray(int v[],int size)
{
	int i;
	printf("-------------------\n");
	for (i=0;i<size;i++)
		printf("%d ",v[i]);
	printf("\n\n");
}

// function that copies the contents of a source array in a destination array
void copyArray(int src[],int dst[],int size)
{
 for(int i=0;i<size;i++){
	dst[i] = src[i];
 }
}


int main()
{
	int A[N] = {4,3,8,5,6,9,0,1,7,2};
	int B[N];

	copyArray(A,B,N);
	imprimeArray(B,N);
}

/*
– ¿La copia del array se realiza correctamente? ¿Por qué?
	No, porque de esa manera se asigna el puntero dst al src, pero lo hace de manera local y no afecta al array original

– Si no es correcto, escribe un código que sí realice la copia correctamente.
	Antes hacia esto (	dst = src;), ahora, lo que esta arriba 
*/