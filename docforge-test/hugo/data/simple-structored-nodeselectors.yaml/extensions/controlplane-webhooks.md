---
title: Controlplane Webhooks
---
# Controlplane customization webhooks

Gardener creates the Shoot controlplane in several steps of the Shoot flow. At different point of this flow, it:

* deploys standard controlplane components such as kube-apiserver, kube-controller-manager, and kube-scheduler by creating the corresponding deployments, services, and other resources in the Shoot namespace.
* initiates the deployment of custom controlplane components by [ControlPlane controllers](/extensions/controlplane) by creating a `ControlPlane` resource in the Shoot namespace.

In order to apply any provider-specific changes to the configuration provided by Gardener for the standard controlplane components, cloud extension providers can install mutating admission webhooks for the resources created by Gardener in the Shoot namespace.

## What needs to be implemented to support a new cloud provider?

In order to support a new cloud provider you should install "controlplane" mutating webhooks for any of the following resources:

* Deployment with name `kube-apiserver`, `kube-controller-manager`, or `kube-scheduler`
* StatefulSet with name `etcd-main` or `etcd-events`
* Service with name `kube-apiserver`
* `OperatingSystemConfig` with any name and purpose `reconcile`

See [Contract Specification](#contract-specification) for more details on the contract that Gardener and webhooks should adhere to regarding the content of the above resources.

You can install 3 different kinds of controlplane webhooks:

* `Shoot`, or `controlplane` webhooks apply changes needed by the Shoot cloud provider, for example the `--cloud-provider` command line flag of `kube-apiserver` and `kube-controller-manager`. Such webhooks should only operate on Shoot namespaces labeled with `shoot.gardener.cloud/provider=<provider>`.
* `Seed`, or `controlplaneexposure` webhooks apply changes needed by the Seed cloud provider, for example annotations on the `kube-apiserver` service to ensure cloud-specific load balancers are correctly provisioned for a service of type `LoadBalancer`. Such webhooks should only operate on Shoot namespaces labeled with `seed.gardener.cloud/provider=<provider>`.

The labels `shoot.gardener.cloud/provider` and `shoot.gardener.cloud/provider` are added by Gardener when it creates the Shoot namespace.

## Contract Specification

This section specifies the contract that Gardener and webhooks should adhere to in order to ensure smooth interoperability. Note that this contract can't be specified formally and is therefore easy to violate, especially by Gardener. The Gardener team will nevertheless do its best to adhere to this contract in the future and to ensure via additional measures (tests, validations) that it's not unintentionally broken. If it needs to be changed intentionally, this can only happen after proper communication has taken place to ensure that the affected provider webhooks could be adapted to work with the new version of the contract.

**Note:** The contract described below may not necessarily be what Gardener does currently (as of May 2019). Rather, it reflects the target state after changes for [Gardener extensibility](/extensions/overview) have been introduced.

### kube-apiserver

To deploy kube-apiserver, Gardener **shall** create a deployment and a service both named `kube-apiserver` in the Shoot namespace. They can be mutated by webhooks to apply any provider-specific changes to the standard configuration provided by Gardener.

The pod template of the `kube-apiserver` deployment **shall** contain a container named `kube-apiserver`.

The `command` field of the `kube-apiserver` container **shall** contain the [kube-apiserver command line](https://kubernetes.io/docs/reference/command-line-tools-reference/kube-apiserver/). It **shall** contain a number of provider-independent flags that should be ignored by webhooks, such as:

* admission plugins (`--enable-admission-plugins`, `--disable-admission-plugins`)
* secure communications (`--etcd-cafile`, `--etcd-certfile`, `--etcd-keyfile`, ...)
* audit log (`--audit-log-*`)
* ports (`--insecure-port`, `--secure-port`)

The kube-apiserver command line **shall not** contain any provider-specific flags, such as:

* `--cloud-provider`
* `--cloud-config`

These flags can be added by webhooks if needed.

The `kube-apiserver` command line **may** contain a number of additional provider-independent flags. In general, webhooks should ignore these unless they are known to interfere with the desired kube-apiserver behavior for the specific provider. Among the flags to be considered are:

* `--endpoint-reconciler-type`
* `--advertise-address`
* `--feature-gates`

Gardener **may** use [SNI](https://github.com/gardener/gardener/blob/master/docs/proposals/08-shoot-apiserver-via-sni.md) to expose the apiserver (`APIServerSNI` feature gate). In this case, Gardener **shall** label the `kube-apiserver`'s `Deployment` with `core.gardener.cloud/apiserver-exposure: gardener-managed` label and expects that the `--endpoint-reconciler-type` and `--advertise-address` flags are not modified.

The `--enable-admission-plugins` flag **may** contain admission plugins that are not compatible with CSI plugins such as `PersistentVolumeLabel`. Webhooks should therefore ensure that such admission plugins are either explicitly enabled (if CSI plugins are not used) or disabled (otherwise).

The `env` field of the `kube-apiserver` container **shall not** contain any provider-specific environment variables (so it will be empty). If any provider-specific environment variables are needed, they should be added by webhooks.

The `volumes` field of the pod template of the `kube-apiserver` deployment, and respectively the `volumeMounts` field of the `kube-apiserver` container **shall not** contain any provider-specific `Secret` or `ConfigMap` resources. If such resources should be mounted as volumes, this should be done by webhooks.

The `kube-apiserver` `Service` **may** be of type `LoadBalancer`, but **shall not** contain any provider-specific annotations that may be needed to actually provision a load balancer resource in the Seed provider's cloud. If any such annotations are needed, they should be added by webhooks (typically `controlplaneexposure` webhooks).

The `kube-apiserver` `Service` **shall** be of type `ClusterIP`, if Gardener is using [SNI](https://github.com/gardener/gardener/blob/master/docs/proposals/08-shoot-apiserver-via-sni.md) to expose the apiserver (`APIServerSNI` feature gate). In this case, Gardener **shall** label this `Service` with `core.gardener.cloud/apiserver-exposure: gardener-managed` label and expects that no mutations happen.

### kube-controller-manager

To deploy kube-controller-manager, Gardener **shall** create a deployment named `kube-controller-manager` in the Shoot namespace. It can be mutated by webhooks to apply any provider-specific changes to the standard configuration provided by Gardener.

The pod template of the `kube-controller-manager` deployment **shall** contain a container named `kube-controller-manager`.

The `command` field of the `kube-controller-manager` container **shall** contain the [kube-controller-manager command line](https://kubernetes.io/docs/reference/command-line-tools-reference/kube-controller-manager/). It **shall** contain a number of provider-independent flags that should be ignored by webhooks, such as:

* `--kubeconfig`, `--authentication-kubeconfig`, `--authorization-kubeconfig`
* `--leader-elect`
* secure communications (`--tls-cert-file`, `--tls-private-key-file`, ...)
* cluster CIDR and identity (`--cluster-cidr`, `--cluster-name`)
* sync settings (`--concurrent-deployment-syncs`, `--concurrent-replicaset-syncs`)
* horizontal pod autoscaler (`--horizontal-pod-autoscaler-*`)
* ports (`--port`, `--secure-port`)

The kube-controller-manager command line **shall not** contain any provider-specific flags, such as:

* `--cloud-provider`
* `--cloud-config`
* `--configure-cloud-routes`
* `--external-cloud-volume-plugin`

These flags can be added by webhooks if needed.

The kube-controller-manager command line **may** contain a number of additional provider-independent flags. In general, webhooks should ignore these unless they are known to interfere with the desired kube-controller-manager behavior for the specific provider. Among the flags to be considered are:

* `--feature-gates`

The `env` field of the `kube-controller-manager` container **shall not** contain any provider-specific environment variables (so it will be empty). If any provider-specific environment variables are needed, they should be added by webhooks.

The `volumes` field of the pod template of the `kube-controller-manager` deployment, and respectively the `volumeMounts` field of the `kube-controller-manager` container **shall not** contain any provider-specific `Secret` or `ConfigMap` resources. If such resources should be mounted as volumes, this should be done by webhooks.

### kube-scheduler

To deploy kube-scheduler, Gardener **shall** create a deployment named `kube-scheduler` in the Shoot namespace. It can be mutated by webhooks to apply any provider-specific changes to the standard configuration provided by Gardener.

The pod template of the `kube-scheduler` deployment **shall** contain a container named `kube-scheduler`.

The `command` field of the `kube-scheduler` container **shall** contain the [kube-scheduler command line](https://kubernetes.io/docs/reference/command-line-tools-reference/kube-scheduler/). It **shall** contain a number of provider-independent flags that should be ignored by webhooks, such as:

* `--config`
* `--authentication-kubeconfig`, `--authorization-kubeconfig`
* secure communications (`--tls-cert-file`, `--tls-private-key-file`, ...)
* ports (`--port`, `--secure-port`)

The kube-scheduler command line **may** contain additional provider-independent flags. In general, webhooks should ignore these unless they are known to interfere with the desired kube-controller-manager behavior for the specific provider. Among the flags to be considered are:

* `--feature-gates`

The kube-scheduler command line can't contain provider-specific flags, and it makes no sense to specify provider-specific environment variables or mount provider-specific `Secret` or `ConfigMap` resources as volumes.

### etcd-main and etcd-events

To deploy etcd, Gardener **shall** create 2 StatefulSets named `etcd-main` and `etcd-events` in the Shoot namespace. They can be mutated by webhooks to apply any provider-specific changes to the standard configuration provided by Gardener.

The pod template of these 2 deployments **shall** contain a container named `etcd`. It **shall not** contain a sidecar container for etcd backups. If such a container is needed, it should be added by webhooks, together with any volumes it may need to mount.

The `command` field of the `etcd` container **shall** contain the etcd command line. It **shall** contain only provider-independent flags that should be ignored by webhooks. It can't contain provider-specific flags, and it makes no sense to specify provider-specific environment variables or mount provider-specific `Secret` or `ConfigMap` resources as volumes.

The `volumeClaimTemplates` section of these 2 StatefulSets **shall** contain a template named `etcd-main` or `etcd-events`. This template **shall** use the default storage class. The corresponding claim is mounted into the `etcd` container at `/var/etcd/data`. If it is desirable to use a non-default storage class, this should be done by webhooks.

### cloud-controller-manager

Gardener **shall not** deploy a cloud-controller-manager. If it is needed, it should be added by a [`ControlPlane` controller](/extensions/controlplane)

### CSI controllers

Gardener **shall not** deploy a CSI controller. If it is needed, it should be added by a [`ControlPlane` controller](/extensions/controlplane)

### kubelet

To specify the kubelet configuration, Gardener **shall** create a [`OperatingSystemConfig` resource](/extensions/operatingsystemconfig) with any name and purpose `reconcile` in the Shoot namespace. It can therefore also be mutated by webhooks to apply any provider-specific changes to the standard configuration provided by Gardener. Gardener **may** write multiple such resources with different `type` to the same Shoot namespaces if multiple OSs are used.

The OSC resource **shall** contain a unit named `kubelet.service`, containing the corresponding systemd unit configuration file. The `[Service]` section of this file **shall** contain a single `ExecStart` option having the [kubelet command line](https://kubernetes.io/docs/reference/command-line-tools-reference/kubelet/) as its value.

The OSC resource **shall** contain a file with path `/var/lib/kubelet/config/kubelet`, which contains a `KubeletConfiguration` resource in YAML format. Most of the flags that can be specified in the kubelet command line can alternatively be specified as options in this configuration as well.

The kubelet command line **shall** contain a number of provider-independent flags that should be ignored by webhooks, such as:

* `--config`
* `--bootstrap-kubeconfig`, `--kubeconfig`
* `--network-plugin` (and, if it equals `cni`, also `--cni-bin-dir` and `--cni-conf-dir`)
* `--node-labels`

The kubelet command line **shall not** contain any provider-specific flags, such as:

* `--cloud-provider`
* `--cloud-config`
* `--provider-id`

These flags can be added by webhooks if needed.

The kubelet command line / configuration **may** contain a number of additional provider-independent flags / options. In general, webhooks should ignore these unless they are known to interfere with the desired kubelet behavior for the specific provider. Among the flags / options to be considered are:

* `--enable-controller-attach-detach` (`enableControllerAttachDetach`) - should be set to `true` if CSI plugins are used, but in general can also be ignored since its default value is also `true`, and this should work both with and without CSI plugins.
* `--feature-gates` (`featureGates`) - should contain a list of specific feature gates if CSI plugins are used. If CSI plugins are not used, the corresponding feature gates can be ignored since enabling them should not harm in any way.
