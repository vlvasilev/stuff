---
archivedate: 2018-07-11T00:00:00Z
authors:
    - avatar: https://avatars1.githubusercontent.com/u/1155039?v=4
      email: andreas.herz@sap.com
      name: Andreas Herz
publishdate: 2018-06-11T00:00:00Z
title: Frontend HTTPS
type: Blog
---

![](https://github.com/gardener/documentation/raw/master/website/blog/2018_week_17/blog-https.png)
 
For encrypted communication between the client to the load balancer, you need to specify a TLS private key and 
certificate to be used by the ingress controller.

Create a secret in the namespace of the ingress containing the TLS private key and certificate. Then configure the 
secret name in the TLS configuration section of the ingress specification.

..read on [HTTPS - Self Signed Certificates](https://github.com/gardener/documentation/blob/master/website/documentation/guides/applications/https/_index.md) how to configure it.
