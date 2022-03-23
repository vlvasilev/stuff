---
archivedate: 2019-07-11T00:00:00Z
authors:
    - avatar: https://avatars1.githubusercontent.com/u/1155039?v=4
      email: andreas.herz@sap.com
      name: Andreas Herz
publishdate: 2019-06-11T00:00:00Z
title: Organizing Access Using kubeconfig Files
type: Blog
---

The kubectl command-line tool uses `kubeconfig` files to find the information it needs to choose a cluster and 
communicate with the API server of a cluster.

![teaser](https://github.com/gardener/documentation/raw/master/website/blog/2019_week_02/teaser.svg)


> What happens if your kubeconfig file of your production cluster is leaked or published by accident?

Since there is no possibility to rotate or revoke the initial kubeconfig, there is only one 
way to protect your infrastructure or application if it is has leaked - **delete the cluster**.



..learn more on [Work with kubeconfig files](https://github.com/gardener/documentation/blob/master/website/documentation/guides/client_tools/working-with-kubeconfig/_index.md).

