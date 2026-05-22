-module(func).
-export([fact/1]).
%% EL 1 ES LA CANT DE ARGUMENTOS de la funcion
%% variable siempre van en mayus , funciones en minusculas
%% tipo de dato : atomo , para decir errores y demas
%% la salida de una funcion no solo va depender de los argumentos de entrada
fact(0)-> 1;
fact(N)-> N * fact(N-1).


