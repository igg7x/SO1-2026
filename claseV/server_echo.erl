-module(server_echo).
-export(receptor/1 , start/0,echoResp/1).
-define(PUERTO ,1234).

start() -> 
    {ok, Socket} =   gen_tcp:listen(?PUERTO , [binary ,{active ,false}]),
    spawn(?MODULE , receptor , [Socket]).



receptor(Socket) -> 
    case gen_tcp:accept(Socket)  of 
        {ok ,CSocket} ->   spawn(?MODULE , echoResp , [CSocket]);
        {error , closed} -> io:fwrite("Se cerro el socket  ~ n",[]);
    end,
    receptor(Socket),

echoResp(CSocket) -> 
    case gen_tcp:recv(CSocket , 0) of 
        {ok,Packet} ->  
        io:fwrite("Servidor haciendo echo ~p ~n",[Packet]),    
        gen_tcp:send(CSocket , Packet);
        {error, Reason} -> io:fwrite("Fallo con la razon socket ~p ~n"[Reason])
    end,