---
archivedate: 2018-07-11T00:00:00Z
authors:
    - avatar: https://avatars1.githubusercontent.com/u/1155039?v=4
      email: andreas.herz@sap.com
      name: Andreas Herz
publishdate: 2018-06-11T00:00:00Z
title: Namespace Isolation
type: Blog
---

...or **DENY all traffic from other namespaces**


You can configure a **NetworkPolicy** to deny all traffic from other namespaces while allowing all traffic 
coming from the same namespace the pod is deployed to. There are many reasons why you may chose to configure Kubernetes 
network policies:
 - Isolate multi-tenant deployments
 - Regulatory compliance
 - Ensure containers assigned to different environments (e.g. dev/staging/prod) cannot interfere with each another                                                

![](https://github.com/gardener/documentation/raw/master/website/blog/2018_week_09/blog-namespaceisolation.png)


..read on [Namespace Isolation](https://github.com/gardener/documentation/blob/master/website/documentation/guides/applications/network-isolation/_index.md) how to configure it.