-module(temp).
-export([init/0, wait/1, cronometroaux/3, cronometro/3]).


wait(Ms) ->
    receive 
        after 
            Ms -> ok
    end.
cronometroaux(_, 0, _) -> 
    ok;
cronometroaux(Fun, Hasta,Periodo) -> 
    Fun(),
    wait(Periodo),
    cronometroaux(Fun, Hasta - Periodo, Periodo).

cronometro(Fun, Hasta, Periodo) ->
    spawn(?MODULE,cronometroaux, [Fun, Hasta,Periodo]),
    ok.

init() ->
    cronometro(fun () -> io:fwrite("Tick~n") end, 5000, 1000).