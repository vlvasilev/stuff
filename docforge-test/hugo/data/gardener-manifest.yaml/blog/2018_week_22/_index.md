---
archivedate: 2018-07-11T00:00:00Z
authors:
    - avatar: https://avatars1.githubusercontent.com/u/1155039?v=4
      email: andreas.herz@sap.com
      name: Andreas Herz
publishdate: 2018-06-11T00:00:00Z
title: Anti Patterns
type: Blog
---

![](https://github.com/gardener/documentation/raw/master/website/blog/2018_week_22/blog-antipattern.png)

## Running as root user
Whenever possible, do not run containers as root users. One could be 
tempted to say that Kubernetes Pods and Node are well separated. The host and the container 
share the same kernel. If the container is compromised, a root user can damage the underlying 
node. Use `RUN groupadd -r anygroup && useradd -r -g anygroup myuser` to create a group 
and a user in it. Use the `USER` command to switch to this user. 

 
## Storing data or logs in containers
Containers are ideal for stateless applications 
and should be transient. This means that no data or logs should be stored in the 
container, as they are lost when the container is closed. If absolutely necessary, 
you can use persistence volumes instead to persist them outside the containers. 
However, an ELK stack is preferred for storing and processing log files. 

..read some more on [Common Kubernetes Antipattern](https://github.com/gardener/documentation/blob/master/website/documentation/guides/applications/antipattern/_index.md).
