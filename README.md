# Indoor_Tactil
por que decidi elegir strings para guardar las cosas ugh algun dia lo voy a tener que cambiar

podria usar objetos para las fases

desde que agregue el rtc cada tanto se congela todo y se necesita reiniciar (quiza un delay al final del loop lo arregla)

las librerias actualizadas no funcionaban por pin_magic.h

todo:

agregar cantidad de ciclos

//deberia cambiar el sistema de cambio de fases para que cuando se inicia una fase nueva instantaneamente se vaya a una funcion que efectivamente cambie la fase y no solo la variable que dice que fase esta activa

//deberia cambiar todas las strings que guardan setpoints por ints de forma que al teclado le entren y salgan numeros y cuando necesito imprimirlos los cargo al buffer como al resto de numeros

agregar puntos medios de los setpoints para que el sistema se intente mantener en el punto medio siempre

prender el ventilador por x tiempo por la noche para refrigerar y cambiar el aire

agregar pantalla de startup

hard reset

sistema de alarmas para detectar si no esta funcionando un i/o

status bar

boton que lleve de vuelta al dashboard

guardar setpoints en eeprom

eliminar botones (los que no tienen texto o no lo tienen centrado) y variables que no necesite para ahorrar memoria

cuando termine no puedo olvidarme de eliminar todos los set de texto etc que no necesito volver a repetir

funciones opcionales:

vaporizador

heater

guardar programas personalizados