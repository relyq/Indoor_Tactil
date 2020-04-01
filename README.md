# Indoor_Tactil
las librerias actualizadas no funcionaban por pin_magic.h

todo:

sistema de control de fases, de forma que al inicar una fase los setpoints de esa fase se conviertan en los setpoints activos del programa

arreglar los valores maximos de los setpoints, ahora mismo es posible configurar mas de 24h luz por dia

ademas, los setpoints permiten configurar valores mayores a 255, de forma que sobrecargarian las variables de setpoints. tengo que ponerle limite a los valores de setpoint que pueden sobrecargar sus int o cambiar los int de 8 bit a int de 16 bit (esto es particularmente un problema con la cantidad de dias de la fase, ya que es el unico setpoint que permite 3 digitos)

guardar setpoints en eeprom

podria cambiar todos los digitalWrite() y digitalRead() por manipulacion manual de puertos

sistema de alarmas para detectar si no esta funcionando un i/o

RTC - configuracion de fecha y hora

hard reset

funciones opcionales:

vaporizador

heater

guardar programas personalizados
