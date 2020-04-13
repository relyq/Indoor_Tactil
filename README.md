# Indoor_Tactil
deberia cambiar currentScreen y prevScreen por arrays de bytes

podria usar objetos para las fases - lo voy a hacer para una version futura

desde que agregue el rtc cada tanto se congela todo y se necesita reiniciar (quiza un delay al final del loop lo arregla) // si alguno de los cables del rtc se mueve y se desconecta por un momento se freezea todo el sistema

las librerias actualizadas no funcionaban por pin_magic.h

todo:

agregar cantidad de ciclos

//deberia cambiar el sistema de cambio de fases para que cuando se inicia una fase nueva instantaneamente se vaya a una funcion que efectivamente cambie la fase y no solo la variable que dice que fase esta activa

agregar puntos medios de los setpoints para que el sistema se intente mantener en el punto medio siempre

prender el ventilador por x tiempo por la noche para refrigerar y cambiar el aire

agregar pantalla de startup

hard reset

sistema de alarmas para detectar si no esta funcionando un i/o

status bar

guardar setpoints en eeprom

eliminar botones (los que no tienen texto o no lo tienen centrado) y variables que no necesite para ahorrar memoria

cuando termine no puedo olvidarme de eliminar todos los set de texto etc que no necesito volver a repetir

funciones opcionales:

vaporizador

heater

guardar programas personalizados