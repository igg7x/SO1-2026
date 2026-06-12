-module(client_echo)
-export([client/0])
-define(PUERTO,1234).
-define(DIR , "localhost").
client()-> 
     case gen_tcp:connect(?DIR, ?PUERTO, [binary , {actibve, false }]) of   
            {ok,Socket} -> gen_tcp:send(Socket , "HOla"),
                            case gen_tcp:recv(Socket , 0) of 
                            {ok,Packet} ->  io:fwrite("Cliente  haciendo echo ~p ~n",[Packet]),    
                            gen_tcp:close();
                            {error, Reason} -> io:fwrite("Fallo con la razon socket ~p ~n"[Reason]);
            {error , Reason } -> io:fwrite("..");
    end.
