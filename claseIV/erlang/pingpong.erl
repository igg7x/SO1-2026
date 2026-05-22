-module(pingpong).
-export([]0)


% Pong
                            

pong() ->
    recive 
    {0,Pid_ping} -> io:fwrite("Pong recive : ~p ~p ~n ,[N,Pid_pong]"), 
        Pid_ping ! {fin,self()}
    {N,Pid_ping} -> io:fwrite("Pong recive : ~p ~p ~n ,[N,Pid_pong]"),

ping() ->

play() ->   
    Pid_ping = spawn(pingpong,ping,[]),
    Pid_pong = spawn(pingpong,pong,[]),
    Pid_pong ! {10,Pid_ping},
    ok. %% cuando retorna la funcion play va a retornar el atomo final que le queda (ese que le puse ahi)


