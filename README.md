# inifinito

El programa que hay que presentar constará de un único fichero fuente de nombre infinito.c. La correcta compilación de dicho programa, producirá un fichero ejecutable, cuyo nombre será obligatoriamente infinito. Respetad las mayúsculas/minúsculas de los nombres, si las hubiere.

La ejecución del programa creará una serie de procesos que se transmitirán señales entre ellos. En concreto, el árbol de procesos que hay que crear es el siguiente:


Para comprender mejor el funcionamiento de la práctica, cambiemos la posición de los procesos en el dibujo de forma que formen una figura semejante al símbolo de infinito:

Las relaciones paterno-filiales son las indicadas por las líneas sólidas.

Durante veinticinco segundos, deben estar circulando señales entre los procesos en el sentido que indican las flechas rojas de la figura, de modo que cuando un proceso recibe una señal, envía otra al siguiente proceso. El proceso padre enviará la primera señal para que se ponga en marcha el mecanismo. El tipo de señal o señales usadas se deja libre. El proceso padre es el que controlará estos veinticinco segundos (no uséis sleep).

Transcurridos los venticinco segundos, el proceso padre dejará de enviar señales, arbitrará un mecanismo para que los procesos mueran (o se maten), esperarán a que hayan muerto y escribirá por la salida estándar "La señal ha dado x vueltas.", donde x será el número de vueltas que ha dado la señal a los procesos del dibujo.

Los procesos, una vez creados, deben permanecer bloqueados, sin consumir CPU mientras esperan a recibir la señal y enviar esa u otra al siguiente proceso.

Si se requiere sincronizar alguna acción, se pueden usar señales, pero la espera se debe realizar sin consumo de CPU.


Restricciones
Se deberán usar llamadas al sistema siempre que sea posible, a no ser que se especifique lo contrario.
No está permitido usar la función de biblioteca system, salvo indicación explícita en el enunciado de la práctica.
Para generar el árbol de procesos, no se usarán llamadas al sistema de la familia exec.
No se puede suponer que los PIDs de los procesos de una ristra van a aparecer consecutivos. Puestos en plan exquisito, ni siquiera podemos suponer que estarán ordenados de menor a mayor (puede ocurrir que se agoten los PIDs y retome la cuenta partiendo de cero).
No está permitido el uso de ficheros, tuberías u otro mecanismo externo para transmitir información entre los procesos.
