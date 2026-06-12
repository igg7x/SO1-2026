%% IMPORTANTE: ERLANG PERMITE LA COMPARACIÓN DE DATOS DE DISTINTO TIPO.
%% EL ORDEN ES: ---- Número < Átomo < Referencia < Función < Puerto < PID < Tupla < Mapa < Lista < Binary ----.
%% LA COMPARACIÓN EN:
%% NÚMEROS : LA COMÚN.
%% ÁTOMOS : ORDEN LEXICOGRÁFICO, EJ: ABC < DEF (compara ascii).
%% TUPLAS: COMPARA PRIMERO TAMAÑO Y LUEGO LOS ELEMENTOS DE A PARES.
%% LISTAS : ORDEN LEXICOGRÁFICO, EJ: [1,2,3] < [1,3].
%% CUALQUIER STRING ES UNA LISTA DE ASCII Y COMPARA ESO.
-module(intro).
-export([init/0, match_test/0, nombre/1, apellido/1, tuple_test/2, string_test/0, filtrar_por_apellido/2]).

match_test() ->
    {A,B} = {5,4}, %% A = 5, B = 4.
    %%{C,C} = {5,4}, %% ASIGNACIÓN INVÁLIDA: RECORDEMOS QUE ERLANG ES DE ASIGNACIÓN ÚNICA.
    {B,A} = {4,5}, %% B = 4, A = 5 verifica que matcheen como == aunque falla con excepción.
    {D,D} = {5,5}. %% D = 5.

string_test () -> [
    helloworld == 'helloworld', %% true: ambos átomos y se ignoran las comillas.
    "helloworld" < 'helloworld', %% false ; recordemos átomo < lista. 
    helloworld == "helloworld", %% false : recordemso átomo < lista. 
    [$h,$e,$l,$l,$o,$w,$o,$r,$l,$d] == "helloworld", %% true: recordemos listas compara de a pares y ambos tienen mismo ascii.
    [104,101,108,108,111,119,111,114,108,100] < {104,101,108,108,111,119,111,114,108,100}, %% false: recordemos tupla < lista.
    [104,101,108,108,111,119,111,114,108,100] > 1, %% true: recordemos número < lista.
    [104,101,108,108,111,119,111,114,108,100] == "helloworld"]. %% true: es la lista de codigo ascii de "helloworld".

tuple_test (P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido (P) -> 
    {persona, {nombre, _}, {apellido, Apellido}} = P,
    Apellido.

nombre(P) ->
    {persona, {nombre, Nombre}, {apellido, _}} = P,
    Nombre.

filtrar_por_apellido(Personas, Apellido) -> 
    [Nombres || {persona, {nombre, Nombres}, {apellido, Apellidos}} <- Personas, Apellidos == Apellido].

init() ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    string_test(),
    Garcias = filtrar_por_apellido([P4, P3, P2, P1], "Garcia"),
    io:fwrite("Los Garcias son: ~p ~n", [Garcias]).