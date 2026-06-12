-module(turnos).
-export([server/0, wait_connect/3, get_request/2, contador/1]).

server() ->
    {ok, ListenSocket} = gen_tcp:listen(8000, [binary, {packet, line}, {reuseaddr, true},{active, false}]),
    ContadorPid = spawn(fun () -> contador(0) end),
    wait_connect(ListenSocket, 0, ContadorPid).

wait_connect(ListenSocket, N, ContadorPid) ->
    {ok, CSocket} = gen_tcp:accept(ListenSocket),
    spawn(fun () -> wait_connect (ListenSocket, N+1, ContadorPid) end),
    get_request(CSocket, ContadorPid).

get_request(Socket, CounterPid) ->
    case gen_tcp:recv(Socket,0) of
        {ok, <<"NUEVO\n">>} ->
            CounterPid ! {nuevo,self()},
            receive
                {turno,N} ->
                    gen_tcp:send(Socket,
                                 list_to_binary(integer_to_list(N) ++ "\n"))
            end,
            get_request(Socket, CounterPid);

        {ok, <<"CHAU\n">>} ->
            gen_tcp:close(Socket);

        {error, closed} ->
            ok
    end.

contador(N) ->
    receive 
        {nuevo, From} ->
            From ! {turno, N},
            contador(N + 1)
    end.