-module(pingpong).
-export([ping/2, pong/0, start/0]).
pong() ->
    receive 
        {ping, PingPid} ->
            io:format("Pong recibió ping ~n"),
            PingPid ! pong,
            pong();
        stop ->
            io:format("Pong terminó ~n")
    end.

ping(0, PongPid) ->
    PongPid ! stop,
    io:format("Ping terminó~n");
ping(N, PongPid) ->
    PongPid ! {ping, self()},
    receive
            pong ->
                io:format("Ping recibió pong~n")
            end,
    ping(N - 1, PongPid).

start() ->
    PongPid = spawn(pingpong, pong, []),
    spawn(pingpong, ping, [5, PongPid]).
