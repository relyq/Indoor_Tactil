# Indoor_Tactil
el dia de fin de fase solo se calcula al cambiar de fase, por lo tanto si pasa el tiempo mas alla del dia de fin de fase, no continua a la fase que deberia, solo comienza la fase que continuaba a la que termino

podria usar objetos para las fases - lo voy a hacer para una version futura // definitivamente deberia usar structs - por dios XD // esta hecho :D

desde que agregue el rtc cada tanto se congela todo y se necesita reiniciar (quiza un delay al final del loop lo arregla) // si alguno de los cables del rtc se mueve y se desconecta por un momento se freezea todo el sistema

las librerias actualizadas no funcionaban por pin_magic.h

no se que tan bien funciona el sistema de iluminacion aunque parece estar funcionando bien

hay algunas cosas en español y algunas en ingles deberia arreglarlo lmao

al terminar o cambiar de fase deberia llevar al dashboard????

hay que probar si funciona la carga y guardado de todos los programas

debería hacer que el programa cambie las variables cuando se toca el boton que cambia la variable, en vez de ver si la variable cambió - ver: if (z1fActivalast != z1fActiva), switch (z1fActiva)

podría hacer que los colores sean personalizables

si se cambia la configuración de una fase o se cambia el programa mientras está el sistema funcionando no cambia la configuración activa. esto pasa porque la configuración activa está en fActivaSP y no lo cambio al cambiar la configuración



todo:

configurar los programas default

prender el ventilador por 'x' tiempo por la noche para refrigerar y cambiar el aire

una prueba de rutina para asegurar que la eeprom sigue funcionando (investigar: crc?)

sistema de alarmas para detectar si no esta funcionando un i/o

pantallas de errores (rtc/dht/etc desconectado, rtc perdio el tiempo)

sistema de fertilizante



LIMPIEZA:

tengo que cambiar todos los centrados de texto por getTextBounds() // getTextBounds() no funciona bien?

eliminar botones (los que no tienen texto o no lo tienen centrado) y variables que no necesite para ahorrar memoria

cuando termine no puedo olvidarme de eliminar todos los settext etc que no necesito volver a repetir

debería limpiar _defines.ino de cosas que no se usan

quizá debería comentar todos los serial.print de la version final - podría hacer que solo se compile con serial.print si una variable DEBUG esta activa

tengo que ponerle mejor nombres a las variables