---
archivedate: 2018-07-11T00:00:00Z
authors:
    - avatar: https://avatars1.githubusercontent.com/u/1155039?v=4
      email: andreas.herz@sap.com
      name: Andreas Herz
publishdate: 2018-06-11T00:00:00Z
title: Auditing Kubernetes for Secure Setup
type: Blog
---

In summer 2018, the [Gardener project team](https://github.com/gardener/gardener) asked [Kinvolk](https://kinvolk.io/) to execute several penetration tests in its role as third-party contractor. The goal of this ongoing work is to increase the security of
all Gardener stakeholders in the open source community. Following the Gardener architecture, the control plane of a
Gardener managed shoot cluster resides in the corresponding seed cluster. This is a
[Control-Plane-as-a-Service](https://kubernetes.io/blog/2018/05/17/gardener/#kubernetes-control-plane) with
a [network air gap](https://kubernetes.io/blog/2018/05/17/gardener/#network-air-gap).

![teaser](https://github.com/gardener/documentation/raw/master/website/blog/2018_week_46/teaser.svg)

Along the way we found various kinds of security issues, for example, due to misconfiguration or missing isolation,
as well as two special problems with upstream Kubernetes and its Control-Plane-as-a-Service
architecture.


..read some more on [Auditing Kubernetes for Secure Setup](https://github.com/gardener/documentation/blob/master/website/documentation/guides/applications/insecure-configuration/_index.md).
