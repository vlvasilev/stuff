---
title: Cluster
---
# `Cluster` resource

As part of the extensibility epic a lot of responsibility that was previously taken over by Gardener directly has now been shifted to extension controllers running in the seed clusters.
These extensions often serve a well-defined purpose, e.g. the management of [DNS records](/extensions/dns), [infrastructure](/extensions/infrastructure), etc.
We have introduced a couple of extension CRDs in the seeds whose specification is written by Gardener, and which are acted up by the extensions.

However, the extensions sometimes require more information that is not directly part of the specification.
One example of that is the GCP infrastructure controller which needs to know the shoot's pod and service network.
Another example is the Azure infrastructure controller which requires some information out of the `CloudProfile` resource.
The problem is that Gardener does not know which extension requires which information so that it can write it into their specific CRDs.

In order to deal with this problem we have introduced the `Cluster` extension resource.
This CRD is written into the seeds, however, it does not contain a `status`, so it is not expected that something acts upon it.
Instead, you can treat it like a `ConfigMap` which contains data that might be interesting for you.
In the context of Gardener, seeds and shoots, and extensibility the `Cluster` resource contains the `CloudProfile`, `Seed`, and `Shoot` manifest.
Extension controllers can take whatever information they want out of it that might help completing their individual tasks.

```yaml
---

apiVersion: extensions.gardener.cloud/v1alpha1
kind: Cluster
metadata:
  name: shoot--foo--bar
spec:
  cloudProfile:
    apiVersion: core.gardener.cloud/v1beta1
    kind: CloudProfile
    ...
  seed:
    apiVersion: core.gardener.cloud/v1beta1
    kind: Seed
    ...
  shoot:
    apiVersion: core.gardener.cloud/v1beta1
    kind: Shoot
    ...
```

The resource is written by Gardener before it starts the reconciliation flow of the shoot.

:warning: All Gardener components use the `core.gardener.cloud/v1beta1` version, i.e., the `Cluster` resource will contain the objects in this version.

## Important information that should be taken into account

There are some fields in the `Shoot` specification that might be interesting to take into account.

* `.spec.hibernation.enabled={true,false}`: Extension controllers might want to behave differently if the shoot is hibernated or not (probably they might want to scale down their control plane components, for example).
* `.status.lastOperation.state=Failed`: If Gardener sets the shoot's last operation state to `Failed` it means that Gardener won't automatically retry to finish the reconciliation/deletion flow because an error occurred that could not be resolved within the last `24h` (default). In this case end-users are expected to manually re-trigger the reconciliation flow in case they want Gardener to try again. Extension controllers are expected to follow the same principle. This means they have to read the shoot state out of the `Cluster` resource.

## References and additional resources

* [`Cluster` API (Golang specification)](https://github.com/gardener/gardener/blob/master/pkg/apis/extensions/v1alpha1/types_cluster.go)
