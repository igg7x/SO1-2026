-module(balancear).
-export([bal/1, balrsp/4, serv/0, init/0]).

balrsp(Proc, req, Arg, Pid) -> 
    Proc ! { req, Arg, self() },
        receive
            Reply -> Pid ! Reply,
            ok
        end.


bal(Servs) ->
    receive
        {req, Arg, Pid} ->
            Proc = lists:nth(rand:uniform(length(Servs)), Servs),
            spawn(balancear, balrsp, [Proc, req, Arg, Pid]) 
        end,
    bal(Servs).

serv()->
    receive
        {req, _ , Pid} ->
            Pid ! servidorRecibioReq
    end,
    serv().

init() ->
    S1 = spawn(balancear, serv, []),
    S2 = spawn(balancear, serv, []),
    S3 = spawn(balancear, serv, []),
    Servidores = [S1, S2, S3],
    BalPid = spawn(?MODULE, bal, [Servidores]),
    %% test.
    BalPid ! {req, arg, self()},
    receive 
        Msg -> io:fwrite("El proceso client recibio ~p ~n",[Msg])
    end,
    ok.

