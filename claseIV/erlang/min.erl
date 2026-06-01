-module(min).
-export([min/1]).

min([Hd]) -> Hd; %% caso base

min([Hd|Tl]) ->
    Rest = min(Tl), %% defino resto como el minimo de la lista sin el primer elemento, separo con "," ya que pertenece a la misma clausula
    if
        Hd < Rest -> %% si el header de la lista es menor al resto 
            Hd; %% devuelvo el header y como termina una rama pongo ";"
    true -> %% else
        Rest %% devuelvo el resto si es menor y en el caso del else va sin nada
    end. %% fin de la función va con .
