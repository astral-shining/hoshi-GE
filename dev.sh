#!/bin/bash

p=-1

function terminate() {
    kill $p
}

trap terminate EXIT
while :; do 
    make segf -j 
    kill $p &>/dev/null
    ./test & 
    p=$!
    inotifywait -r -e modify . &>/dev/null
done

kill $p