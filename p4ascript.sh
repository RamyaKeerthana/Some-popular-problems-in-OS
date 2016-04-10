#!/bin/bash 
echo "pid:"
read pid
timer=0
while [  $timer -lt 720 ];
do
perf stat -p $pid sleep 20 2> g
awk 'FNR==4 {print $5 >> "CPU-usage" }' g;
awk 'FNR==5 {print $4 >> "ContextSwitchPerSecond" }' g;
let timer=timer+1
echo "$(($timer * 20))" >> time
done
