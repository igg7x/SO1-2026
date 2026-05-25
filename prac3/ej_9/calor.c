#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "barrier_c.h"

#define N 10
#define W 4
#define ITERS 10
Barrera *b;

float arr1[N];
float arr2[N];

static inline int min(int x, int y) { return x < y ? x : y; }

void calor(float *in, int lo, int hi, float *out)
{
	int i;
	for (i = lo; i < hi; i++)
	{
		int m = in[i];
		int l = i > 0 ? in[i - 1] : m;
		int r = i < N - 1 ? in[i + 1] : m;
		out[i] = m + (l - m) / 1000.0 + (r - m) / 1000.0;
		// printf("i: %d, m: %d, l: %d, r: %d, out[i]: %f\n", i, m, l, r, out[i]);
	}
}

/* Dado un array de [n], devuelve el punto de corte [i] de los [m] totales. */
// la funcion cut es una forma de dividir el trabajo entre los hilos de manera equitativa, incluso cuando n no es divisible por m.
static inline int cut(int n, int i, int m)
{
	return i * (n / m) + min(i, n % m);
}

void *thr(void *arg)
{
	int id = arg - (void *)0;

	// ejemplo: cut(1000, 0, 4) = 0, cut(1000, 1, 4) = 250, cut(1000, 2, 4) = 500, cut(1000, 3, 4) = 750

	// repaticion del laburo
	// thread1 - (0, 250)
	// thread2 - (250, 500)
	// thread3 - (500, 750)
	// thread4 - (750, 1000)
	int lo = cut(N, id, W);
	int hi = cut(N, id + 1, W);
	int i;
	// printf("Hilo : %d\n", id);
	for (i = 0; i < ITERS; i++)
	{
		// cada hilo calcula la temperatura de su parte del array, y luego se copia el resultado al otro array para que el siguiente hilo pueda usarlo en la siguiente iteracion
		calor(arr1, lo, hi, arr2);
		barrier_wait(b);
		// se llama calor con arr2 como entrada y arr1 como salida, para que el siguiente hilo pueda usar los resultados de la iteracion anterior
		calor(arr2, lo, hi, arr1);
		barrier_wait(b);
	}
}

int main()
{

	pthread_t w[W];
	float f;
	int i;
	b = malloc(sizeof(Barrera));
	barrier_init(b, W);

	/*
	 * No cambiamos la semilla, por lo que este programa
	 * debería ser determinista
	 */
	for (i = 0; i < N; i++)
		arr1[i] = rand() / 10000.0;

	for (i = 0; i < W; i++)
		pthread_create(&w[i], NULL, thr, i + (void *)0);

	for (i = 0; i < W; i++)
		pthread_join(w[i], NULL);

	f = 0;
	for (i = 0; i < N; i++)
		f += arr1[i];
	f /= N;

	/* Imprimimos temperatura promedio, siempre debería dar igual */
	printf("Temp promedio: %f\n", f);
	return 0;
}
