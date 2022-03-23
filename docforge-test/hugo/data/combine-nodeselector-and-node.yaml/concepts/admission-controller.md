---
title: Admission Controller
---
# Gardener Admission Controller

While the Gardener API server works with [admission plugins](/concepts/apiserver_admission_plugins) to validate and mutate resources belonging to Gardener related API groups, e.g. `core.gardener.cloud`, the same is needed for resources belonging to non-Gardener API groups as well, e.g. `Secret`s in the `core` API group.
Therefore, the Gardener Admission Controller runs a http(s) server with the following handlers which serve as validating/mutating endpoints for [admission webhooks](https://kubernetes.io/docs/reference/access-authn-authz/extensible-admission-controllers/).
It is also used to serve http(s) handlers for authorization webhooks.

## Admission Webhook Handlers

This section describes the admission webhook handlers that are currently served.

### Kubeconfig Secret Validator

[Malicious Kubeconfigs](https://github.com/kubernetes/kubectl/issues/697) applied by end users may cause a leakage of sensitive data.
This handler checks if the incoming request contains a Kubernetes secret with a `.data.kubeconfig` field and denies the request if the Kubeconfig structure violates Gardener's security standards.

### Namespace Validator

Namespaces are the backing entities of Gardener projects in which shoot clusters objects reside.
This validation handler protects active namespaces against premature deletion requests.
Therefore, it denies deletion requests if a namespace still contains shoot clusters or if it belongs to a non-deleting Gardener project (w/o `.metadata.deletionTimestamp`).

### Resource Size Validator

Since users directly apply Kubernetes native objects to the Garden cluster, it also involves the risk of being vulnerable to DoS attacks because these resources are read continuously watched and read by controllers.
One example is the creation of `Shoot` resources with large annotation values (up to 256 kB per value) which can cause severe out-of-memory issues for the Gardenlet component.
[Vertical autoscaling](https://github.com/kubernetes/autoscaler/tree/master/vertical-pod-autoscaler) can help to mitigate such situations, but we cannot expect to scale infinitely, and thus need means to block the attack itself.

The Resource Size Validator checks arbitrary incoming admission requests against a configured maximum size for the resource's group-version-kind combination and denies the request if the contained object exceeds the quota.

Example for Gardener Admission Controller configuration:
```yaml
server:
  resourceAdmissionConfiguration:
    limits:
    - apiGroups: ["core.gardener.cloud"]
      apiVersions: ["*"]
      resources: ["shoots", "plants"]
      size: 100k
    - apiGroups: [""]
      apiVersions: ["v1"]
      resources: ["secrets"]
      size: 100k
    unrestrictedSubjects:
    - kind: Group
      name: gardener.cloud:system:seeds
      apiGroup: rbac.authorization.k8s.io
 #  - kind: User
 #    name: admin
 #    apiGroup: rbac.authorization.k8s.io
 #  - kind: ServiceAccount
 #    name: "*"
 #    namespace: garden
 #    apiGroup: ""
    operationMode: block #log
```

With the configuration above, the Resource Size Validator denies requests for shoots and plants with Gardener's core API group which exceed a size of 100 kB. The same is done for Kubernetes secrets.

As this feature is meant to protect the system from malicious requests sent by users, it is recommended to exclude trusted groups, users or service accounts from the size restriction via `resourceAdmissionConfiguration.unrestrictedSubjects`.
For example, the backing user for the Gardenlet should always be capable of changing the shoot resource instead of being blocked due to size restrictions.
This is because the Gardenlet itself occasionally changes the shoot specification, labels or annotations, and might violate the quota if the existing resource is already close to the quota boundary.
Also, operators are supposed to be trusted users and subjecting them to a size limitation can inhibit important operational tasks.
Wildcard ("*") in subject `name` is supported.

Size limitations depend on the individual Gardener setup and choosing the wrong values can affect the availability of your Gardener service.
`resourceAdmissionConfiguration.operationMode` allows to control if a violating request is actually denied (default) or only logged.
It's recommended to start with `log`, check the logs for exceeding requests, adjust the limits if necessary and finally switch to `block`.

### SeedRestriction

Please refer to [this document](https://github.com/gardener/gardener/blob/master/docs/deployment/gardenlet_api_access.md) for more information.

## Authorization Webhook Handlers

This section describes the authorization webhook handlers that are currently served.

### SeedAuthorization

Please refer to [this document](https://github.com/gardener/gardener/blob/master/docs/deployment/gardenlet_api_access.md) for more information.
