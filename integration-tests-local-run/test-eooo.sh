#!/bin/bash

shoot_name=$1
kubeconfig=$2
rm -fr $shoot_name
mkdir $shoot_name
i=1
while [ $i -le 100 ]
do
  echo "$shoot_name/$i"
  mkdir "$shoot_name/test-$i"

  echo "Delete leftovers from other tests"
  kubectl --kubeconfig=$kubeconfig delete cluster --all
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig -n $ns delete deploy --all) &) ; done
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig -n $ns delete pod --all) &) ; done
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig -n $ns delete svc --all) &) ; done
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig delete namespace $ns) &) ; done
  

  kubectl --kubeconfig=$kubeconfig -n garden delete sts --all
  kubectl --kubeconfig=$kubeconfig -n garden delete ds --all
  kubectl --kubeconfig=$kubeconfig -n garden delete svc --all
  kubectl --kubeconfig=$kubeconfig -n garden delete pod --all
  kubectl --kubeconfig=$kubeconfig -n garden delete serviceaccount fluent-bit
  kubectl --kubeconfig=$kubeconfig -n garden delete clusterrolebinding fluent-bit-read
  kubectl --kubeconfig=$kubeconfig -n garden delete clusterrole fluent-bit-read
  kubectl --kubeconfig=$kubeconfig -n garden delete clusterrolebinding fluent-bit-psp
  kubectl --kubeconfig=$kubeconfig -n garden delete psp fluent-bit
  kubectl --kubeconfig=$kubeconfig delete ns garden
  

  echo "start test"
  go test -timeout=0 /home/hisshadow/go/src/github.com/gardener/gardener/test/suites/shoot --v -ginkgo.v -ginkgo.progress -ginkgo.noColor --report-file=/tmp/report.json --disable-dump=false -kubecfg="/home/hisshadow/.garden/dev/garden-gke/kubeconfig" -shoot-name=$shoot_name -project-namespace=garden-i330716 -ginkgo.focus="should not produce Entry Out Of Order error" > "$shoot_name/test-$i/test-logs"
  
  # grep "only " "$shoot_name/test-$i/test-logs" -IR
  # if [[ "$?" == "0" ]]
  # then
  #   echo "Test $1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!"

  #   echo "Get log count for each logger"
  #   pods=$(kubectl --kubeconfig=$kubeconfig -n garden get pod -l app=logger --all-namespaces | grep Running | awk '{print $2}')
  #   for pod in $pods;
  #   do
  #     result=$(kubectl --kubeconfig=$kubeconfig -n garden exec loki-0 wget -- 'http://localhost:3100/loki/api/v1/query' -O- --post-data="query=count_over_time({pod_name=\"$pod\"}[2h])")
  #     number_of_logs=$(echo -n $result | jq '[.data.result[].value[-1] | tonumber] | add')
  #     echo POD:$pod ":" $number_of_logs >> "$shoot_name/test-$i/pods-logs-summary"
  #   done;

  #   echo "Save loki-0 logs in $shoot_name/test-$i/loki-logs"
  #   kubectl --kubeconfig=$kubeconfig -n garden logs loki-0 > "$shoot_name/test-$i/loki-logs"

  #   echo "Geting logging test componens status and node info"
  #   kubectl --kubeconfig=$kubeconfig -n garden get pod -l role=logging -o wide > "$shoot_name/test-$i/garden-logging-pods"
  #   kubectl --kubeconfig=$kubeconfig -n garden get pod -l app=logger --all-namespaces -o wide > "$shoot_name/test-$i/logger-pods"

  #   echo "Get fluent-bit information"
  #   fluent_bit_pods=$(kubectl --kubeconfig=$kubeconfig -n garden get pod -l app=fluent-bit |  awk 'NR>1 {print $1}')
  #   for flb_pod in $fluent_bit_pods;
  #   do
  #     mkdir -p "$shoot_name/test-$i/$flb_pod/file_out-$flb_pod"
  #     echo "Store file output in $shoot_name/test-$i/$flb_pod/file_out-$flb_pod"
  #     kubectl --kubeconfig=$kubeconfig -n garden cp garden/$flb_pod:log "$shoot_name/test-$i/$flb_pod/file_out-$flb_pod"
  #     echo "Store fluent-bit DB in $shoot_name/test-$i/$flb_pod/{${flb_pod}_flb_kube.db, ${flb_pod}_flb_kube.db-shm, ${flb_pod}_flb_kube.db-wal}"
  #     kubectl --kubeconfig=$kubeconfig -n garden cp garden/$flb_pod:var/log/flb_kube.db "$shoot_name/test-$i/$flb_pod/${flb_pod}_flb_kube.db"
  #     kubectl --kubeconfig=$kubeconfig -n garden cp garden/$flb_pod:var/log/flb_kube.db-shm "$shoot_name/test-$i/$flb_pod/${flb_pod}_flb_kube.db-shm"
  #     kubectl --kubeconfig=$kubeconfig -n garden cp garden/$flb_pod:var/log/flb_kube.db-wal "$shoot_name/test-$i/$flb_pod/${flb_pod}_flb_kube.db-wal"
  #     echo "Store fluent-bit var.log.containers under $shoot_name/test-$i/$flb_pod/var-log-containers"
  #     kubectl --kubeconfig=$kubeconfig -n garden exec $flb_pod mkdir -- /var_log_containers
  #     kubectl --kubeconfig=$kubeconfig -n garden exec $flb_pod cp -- -rL /var/log/containers /var_log_containers
  #     mkdir "$shoot_name/test-$i/$flb_pod/var-log-containers"
  #     kubectl --kubeconfig=$kubeconfig -n garden cp garden/$flb_pod:var_log_containers $shoot_name/test-$i/$flb_pod/var-log-containers

  #     echo "kill $flb_pod"
  #     echo "Store fluent-bit logs to $shoot_name/test-$i/$flb_pod/${flb_pod}_logs"
  #     $(kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -f > "$shoot_name/test-$i/$flb_pod/${flb_pod}_logs") & 
  #     kubectl --kubeconfig=$kubeconfig -n garden exec $flb_pod kill 1
  #     echo "Wait 30 seconds"
  #     sleep 30
  #     echo "Geting fluent-bit-to-loki metrics"
  #     kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -p | grep "incomingStream" > "$shoot_name/test-$i/$flb_pod/$flb_pod-incoming-stream"
  #     kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -p | grep "incomingBufferStream" > "$shoot_name/test-$i/$flb_pod/$flb_pod-incoming-buffer-stream"
  #     kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -p | grep "incomingFinalClientStream" > "$shoot_name/test-$i/$flb_pod/$flb_pod-incoming-final-client-stream"
  #     kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -p | grep "outgoingToPromtailStream" > "$shoot_name/test-$i/$flb_pod/$flb_pod-outgoing-to-promtail-stream"
  #     kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -p | grep "frontIncomingLogsCounter" > "$shoot_name/test-$i/$flb_pod/$flb_pod-front-incoming-log-counter"
  #     kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -p | grep "(total = [0-9]+)" > "$shoot_name/test-$i/$flb_pod/$flb_pod-output-log-counter"
  #   done
  # fi
  # echo "Wait 120 seconds"
  # sleep 120

  # echo "Get log count for each logger after restarting the fluent-bit containers"
  # for pod in $pods;
  # do
  #   result=$(kubectl --kubeconfig=$kubeconfig -n garden exec loki-0 wget -- 'http://localhost:3100/loki/api/v1/query' -O- --post-data="query=count_over_time({pod_name=\"$pod\"}[2h])")
  #   number_of_logs=$(echo -n $result | jq '[.data.result[].value[-1] | tonumber] | add')
  #   echo POD:$pod ":" $number_of_logs >> "$shoot_name/test-$i/pods-logs-summary-second-edition"
  # done;

  # echo "Deleting the fluent bit pods"
  # for flb_pod in $fluent_bit_pods;
  # do
  #   kubectl --kubeconfig=$kubeconfig -n garden delete pod $flb_pod
  # done

  # echo "Wait 120 seconds"
  # sleep 120

  # echo "Get log count for each logger after deleting the fluent-bit pods"
  # for pod in $pods;
  # do
  #   result=$(kubectl --kubeconfig=$kubeconfig -n garden exec loki-0 wget -- 'http://localhost:3100/loki/api/v1/query' -O- --post-data="query=count_over_time({pod_name=\"$pod\"}[2h])")
  #   number_of_logs=$(echo -n $result | jq '[.data.result[].value[-1] | tonumber] | add')
  #   echo POD:$pod ":" $number_of_logs >> "$shoot_name/test-$i/pods-logs-summary-third-edition"
  # done;

  # echo "Get logs of the new fluent-bits pod in $shoot_name/test-$i/second_fluent_bits"
  # mkdir "$shoot_name/test-$i/second_fluent_bits"
  # new_fluent_bit_pods=$(kubectl --kubeconfig=$kubeconfig -n garden get pod -l app=fluent-bit |  awk 'NR>1 {print $1}')
  # for flb_pod in $new_fluent_bit_pods;
  # do
  #   kubectl --kubeconfig=$kubeconfig -n garden logs $flb_pod -f > "$shoot_name/test-$i/second_fluent_bits/$second_flb_pod-logs"
  # done

  echo "Tear Down"
  privileged_pods=$(kubectl --kubeconfig=$kubeconfig -n garden get pod -l app=privileged |  awk 'NR>1 {print $1}')
  for pod in $privileged_pods;
  do
    kubectl --kubeconfig=$kubeconfig -n garden exec $pod rm -- -f /var/log/flb_kube.db /var/log/flb_kube.db-wal /var/log/flb_kube.db-shm
  done

  kubectl --kubeconfig=$kubeconfig delete cluster --all
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig -n $ns delete deploy --all) &) ; done
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig -n $ns delete pod --all) &) ; done
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig -n $ns delete svc --all) &) ; done
  for ns in $(kubectl --kubeconfig=$kubeconfig -n garden get ns | grep shoot- | awk '{print $1}'); do ((kubectl --kubeconfig=$kubeconfig delete namespace $ns) &) ; done
  

  kubectl --kubeconfig=$kubeconfig -n garden delete sts --all
  kubectl --kubeconfig=$kubeconfig -n garden delete ds --all
  kubectl --kubeconfig=$kubeconfig -n garden delete svc --all
  kubectl --kubeconfig=$kubeconfig -n garden delete pod --all
  kubectl --kubeconfig=$kubeconfig -n garden delete serviceaccount fluent-bit
  kubectl --kubeconfig=$kubeconfig -n garden delete clusterrolebinding fluent-bit-read
  kubectl --kubeconfig=$kubeconfig -n garden delete clusterrole fluent-bit-read
  kubectl --kubeconfig=$kubeconfig -n garden delete clusterrolebinding fluent-bit-psp
  kubectl --kubeconfig=$kubeconfig -n garden delete psp fluent-bit
  kubectl --kubeconfig=$kubeconfig delete ns garden

  ((i++))
  sleep 300
done


