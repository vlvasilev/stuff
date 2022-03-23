---
archivedate: 2018-07-11T00:00:00Z
authors:
    - avatar: https://avatars1.githubusercontent.com/u/1155039?v=4
      email: andreas.herz@sap.com
      name: Andreas Herz
publishdate: 2018-06-11T00:00:00Z
title: Shared storage with S3 backend
type: Blog
---

The storage is definitely the most complex and important part of an application setup, once this part is completed, 
one of the most problematic parts could be solved.

Mounting a S3 bucket into a pod using [FUSE](https://github.com/libfuse/libfuse) allows to access data stored in S3 via 
the filesystem. The mount is a pointer to an S3 location, so the data is never synced locally. Once mounted, any pod 
can read or even write from that directory without the need for explicit keys.


![](https://github.com/gardener/documentation/raw/master/website/blog/2018_week_10/blog-s3-shared-storage.png)


However, it can be used to import and parse large amounts of data into a database.

..read on [Shared S3 Storage](https://github.com/freegroup/kube-s3/blob/master/README.md) how to configure it.