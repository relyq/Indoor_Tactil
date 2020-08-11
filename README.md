# Indoor_Tactil
las librerias actualizadas no funcionaban por pin_magic.h // el shield tiene D0-D7 en los pines 2-9, pin_magic.h usa esos pines en arduino uno, y 22-29 en el mega2560
quizá J1-J4 en mi shield son IM0-IM3 en la breakout board de adafruit? si fuera así podría conectar J2, J3, J4 a 3.3v para usar el modo SPI

el dia de fin de fase solo se calcula al cambiar de fase, por lo tanto si pasa el tiempo mas alla del dia de fin de fase, no continua a la fase que deberia, solo comienza la fase que continuaba a la que termino

hay que probar si funciona la carga y guardado de todos los programas

podría hacer que los colores sean personalizables

habria que probar si la hora de iluminacion se termina bien al cambiarla y si el dia de fin de fase cambia la fase luego de cambiarlo

ahora la configuración activa cambia ni bien cambias la configuración de la fase que está activa. hay que hacer pruebas relacionadas a esto. hacer esto me hizo escribir cosas que parecen estar de más, en algun momento debería limpìar el código relacionado con esto.

me gustaría encontrar un mejor sistema que los Adafruit_GFX_Button

<br><br>
<h1>todo:</h1>

tengo que cambiar relojYYYY, relojMM, etc... a un objeto DateTime

cada tanto se freezea el dashboard y no actualiza hasta que lo tocas: cada 100 loops no se actualiza el dashboard, si no que se comparan los valores para ver si cambiaron, y solo si cambiaron se dibujan, si los valores cambian en un frame en el que no se actualiza la pantalla, y no cambian más, no se van a actualizar

problemas con la activación del ventilador-calefactor: calefactor prendido al minimo, se apaga a 26 - ventilador prendido al maximo, se apaga a 25

134: pin de dht hardcodeado

configurar los programas default

pantalla de configuración de programas

prender el ventilador por 'x' tiempo por la noche para refrigerar y cambiar el aire

una prueba de rutina para asegurar que la eeprom sigue funcionando (investigar: crc?)

sistema de alarmas para detectar si no esta funcionando un i/o

pantallas de errores (rtc/dht/etc desconectado, rtc perdio el tiempo)

sistema de fertilizante

<br><br>
<h1>LIMPIEZA:</h1>

tengo que cambiar el nombre del typedef struct fActiva, de la struct fActiva fActivaSP, y del parametro de void eeprom_cargarfActivaSP(fActiva* fActivaSP, uint8_t f) fActiva* fActivaSP

eliminar botones (los que no tienen texto o no lo tienen centrado) y variables que no necesite para ahorrar memoria

quizá debería comentar todos los serial.print de la version final - podría hacer que solo se compile con serial.print si una variable DEBUG esta activa

tengo que ponerle mejor nombres a las variables

hay algunas cosas en español y algunas en ingles deberia arreglarlo lmao
