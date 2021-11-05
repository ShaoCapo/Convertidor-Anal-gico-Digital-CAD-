# Convertidor_Analogico_Digital_CAD
Programa de prueba con el CAD

Enunciado

Debe inicializarse el convertidor analógico digital (CAD) para respetar el TAD mínimo (1.6 us), teniendo en cuenta que el cristal del microcontrolador es de 20 MHz.

La señal a convertir varía en tensión en el mismo margen que la alimentación del circuito (0V, 5V) y es accesible por la patilla RA0.

El programa a construir debe realizar una conversión cada 500 ms y debe volcar el byte bajo, resultado de la conversión, por el puerto B. Del mismo modo, debe enviar el resultado completo de la conversión (10 bits) por la USART (9600 baudios).

Microchip PIC16F886
