# Paralelo
Los resultados arrojan que el 1er bucles es más rápido.
Lo que ocurre es que el el pimer bucle va recorriendo el bloque 0 de cache hasta llegar al final recorriéndolo por completo, luego acaba y recién pasa al bloque 1.
Suponiendo que cada fila es capaz de llenar un bloque.
Lo que pasa en el 2do bloque es que no está recorriendo el mismo bloque de manera consecutiva, sino que en la 1ra iteración va al bloque 0, en la 2da iteración pasa al bloque 1, luego al bloque 2 y así hasta llegar al final, luego regresa al bloque 0 de nuevo hasta el bloque final, así hasta recorrer por completo la matriz.


Vemos la diferencia, es que el 1er bucle realiza menos cambios de líneas de cache (la cantidad de filas), mientras que el 2do bucle va cambiando las líneas de cache a cada iteración (MAX veces).
