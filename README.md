# Indoor_Tactil
el dia de fin de fase solo se calcula al cambiar de fase, por lo tanto si pasa el tiempo mas alla del dia de fin de fase, no continua a la fase que deberia, solo comienza la fase que continuaba a la que termino

podria usar objetos para las fases - lo voy a hacer para una version futura

desde que agregue el rtc cada tanto se congela todo y se necesita reiniciar (quiza un delay al final del loop lo arregla) // si alguno de los cables del rtc se mueve y se desconecta por un momento se freezea todo el sistema

las librerias actualizadas no funcionaban por pin_magic.h

no se que tan bien funciona el sistema de iluminacion aunque parece estar funcionando bien

deberia haber algo que baje la humedad? // la ventilación baja la humedad

podria usar memoria dinamica para guardar informacion que solo se necesita por momentos

todo:

prender el ventilador por x tiempo por la noche para refrigerar y cambiar el aire

sistema de alarmas para detectar si no esta funcionando un i/o

guardar setpoints en eeprom

hard reset

eliminar botones (los que no tienen texto o no lo tienen centrado) y variables que no necesite para ahorrar memoria

cuando termine no puedo olvidarme de eliminar todos los settext etc que no necesito volver a repetir

pantallas de errores (rtc/dht/etc desconectado, rtc perdio el tiempo)

sistema de fertilizante

funciones opcionales:

vaporizador

heater

guardar programas personalizados
