-module(contador).
-export([counter/1, increment/1, consul/1, start/0]).

counter(Value) ->
    receive 
            increment ->
                counter(Value + 1);
            {get, From} ->
                From ! {ok, Value},
                counter(Value)
        end.

increment(Pid) ->
    Pid ! increment.

consul(Pid) ->
    Pid ! {get, self()},
    receive 
        {ok, Value} ->
            Value
    end.

start() ->
    spawn(contador, counter, [0]).