#!/bin/bash

KUBECONFIG=$1
TEST_NAME=$2

mkdir loki_bihavior

touch loki_bihavior/result

start=`date +%s`

while true
do

top_result=$(kubectl --kubeconfig=$KUBECONFIG -n garden top pod -l app=loki | awk 'NR>1')
num_chunks=$(kubectl --kubeconfig=$KUBECONFIG -n garden exec loki-0 -- ls /data/loki/chunks | wc -l)
current_time=`date +%s`
elapsed_time=$((current_time-start))

echo "${elapsed_time}   ${top_result}   ${num_chunks}" >> "loki_bihavior/$TEST_NAME"
echo "${elapsed_time}   ${top_result}   ${num_chunks}"
sleep 30
done