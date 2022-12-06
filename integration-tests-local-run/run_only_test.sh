#!/bin/bash

VIRTUAL_GARDENER_KUBECONFIG=${1:-/Users/i330716/.garden/dev/garden-gke/kubeconfig}
SHOOT_NAME=${2:-gcp-it}
TEST_NAME=${3:-should get container logs from loki for all namespaces}
PROJECT_NAMESPACE=${4:-garden-i330716}
#should get container logs from loki by tenant
#should get container logs from loki for all namespaces
#should get events from loki
#Test the stability of the fluent-bit components without GO plugin
echo $VIRTUAL_GARDENER_KUBECONFIG $SHOOT_NAME $TEST_NAME $PROJECT_NAMESPACE
GO111MODULE=on go test -timeout=0 "${HOME}/go/src/github.com/gardener/gardener/test/testmachinery/suites/shoot" --v -ginkgo.v -ginkgo.progress -ginkgo.noColor --report-file=/tmp/report.json --disable-dump=false -kubecfg="$VIRTUAL_GARDENER_KUBECONFIG" -shoot-name="$SHOOT_NAME" -project-namespace="$PROJECT_NAMESPACE" -ginkgo.focus="$TEST_NAME"