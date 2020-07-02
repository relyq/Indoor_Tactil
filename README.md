# Indoor_Tactil
el dia de fin de fase solo se calcula al cambiar de fase, por lo tanto si pasa el tiempo mas alla del dia de fin de fase, no continua a la fase que deberia, solo comienza la fase que continuaba a la que termino

podria usar objetos para las fases - lo voy a hacer para una version futura // definitivamente deberia usar structs - por dios XD // esta hecho :D

desde que agregue el rtc cada tanto se congela todo y se necesita reiniciar (quiza un delay al final del loop lo arregla) // si alguno de los cables del rtc se mueve y se desconecta por un momento se freezea todo el sistema

las librerias actualizadas no funcionaban por pin_magic.h

no se que tan bien funciona el sistema de iluminacion aunque parece estar funcionando bien

deberia haber algo que baje la humedad? // la ventilación baja la humedad

podria usar memoria dinamica para guardar informacion que solo se necesita por momentos

hay algunas cosas en español y algunas en ingles deberia arreglarlo lmao

al terminar o cambiar de fase deberia llevar al dashboard????

quizá debería mostrar un mensaje antes de reiniciar el dispositivo de fabrica

si cargas un programa no cambia la configuracion activa hasta que cambie la fase activa

hay que probar si funciona la carga y guardado de todos los programas

todo:

no se puede cambiar la configuracion de fases porque cuando se
cambia la variable no se guarda en eeprom

se deberia poder cambiar la configuracion de fase mientras esta corriendo el programa

guardar programas en eeprom

prender el ventilador por x tiempo por la noche para refrigerar y cambiar el aire

una prueba de rutina para asegurar que la eeprom sigue funcionando (crc?)

tengo que cambiar todos los centrados de texto por getTextBounds()

eliminar botones (los que no tienen texto o no lo tienen centrado) y variables que no necesite para ahorrar memoria

cuando termine no puedo olvidarme de eliminar todos los settext etc que no necesito volver a repetir

sistema de alarmas para detectar si no esta funcionando un i/o

pantallas de errores (rtc/dht/etc desconectado, rtc perdio el tiempo)

sistema de fertilizante