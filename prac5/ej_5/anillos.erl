-module(anillos).
-export([make/2, init/0, msg/3]).
-define(N, 10).

make(?N - 1, OriginPid) ->
    io:fwrite("Soy proceso ~p y fui creado, enviando mensaje~n",[?N - 1]),
    OriginPid ! {msg, ?N},
    msg(?N - 1,OriginPid, OriginPid);
    
    
make(I, OriginPid) ->
    io:fwrite("Soy proceso ~p y fui creado~n",[I]),
    NextPid = spawn(fun() -> make(I + 1, OriginPid) end),
    
    msg(I,NextPid, OriginPid).

init() ->
    MyPid = self(),
    NextPid = spawn(fun () -> make(1, MyPid) end),
    msg(0, NextPid, self()),
    ok.
msg(I,NextPid, OriginPid) ->
    receive
        {msg, N} ->
            case N of
                0 ->
                    io:fwrite("Soy proceso ~p y recibi n = 0~n",[I]),
                    NextPid ! exit,
                    msg(I, NextPid, OriginPid);
                N ->
                    io:fwrite("Soy proceso ~p y recibi ~p~n",[I, N]),
                    NextPid ! {msg, N - 1},
                    msg(I,NextPid, OriginPid)
            end;
        exit ->
            io:fwrite("Soy proceso ~p y recibi exit, reenviando~n", [I]), 
            NextPid ! exit,
            io:fwrite("Soy proceso ~p y exiteo~n", [I]),
            ok
        
    end.