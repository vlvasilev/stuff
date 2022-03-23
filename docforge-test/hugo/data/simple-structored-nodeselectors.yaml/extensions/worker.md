---
title: Worker
---
# Contract: `Worker` resource

While the control plane of a shoot cluster is living in the seed and deployed as native Kubernetes workload, the worker nodes of the shoot clusters are normal virtual machines (VMs) in the end-users infrastructure account.
The Gardener project features a sub-project called [machine-controller-manager](https://github.com/gardener/machine-controller-manager).
This controller is extending the Kubernetes API using custom resource definitions to represent actual VMs as `Machine` objects inside a Kubernetes system.
This approach unlocks the possibility to manage virtual machines in the Kubernetes style and benefit from all its design principles.

## What is the machine-controller-manager exactly doing?

Generally, there are provider-specific `MachineClass` objects (`AWSMachineClass`, `AzureMachineClass`, etc.; similar to `StorageClass`), and `MachineDeployment`, `MachineSet`, and `Machine` objects (similar to `Deployment`, `ReplicaSet`, and `Pod`).
A machine class describes **where** and **how** to create virtual machines (in which networks, region, availability zone, SSH key, user-data for bootstrapping, etc.) while a `Machine` results in an actual virtual machine.
You can read up [more information](https://github.com/gardener/machine-controller-manager) in the machine-controller-manager's [repository](https://github.com/gardener/machine-controller-manager).

Before the introduction of the `Worker` extension resource Gardener was deploying the machine-controller-manager, the machine classes, and the machine deployments itself.
Now, Gardener commissions an external, provider-specific controller to take over these tasks.

## What needs to be implemented to support a new worker provider?

As part of the shoot flow Gardener will create a special CRD in the seed cluster that needs to be reconciled by an extension controller, for example:

```yaml
---
apiVersion: extensions.gardener.cloud/v1alpha1
kind: Worker
metadata:
  name: bar
  namespace: shoot--foo--bar
spec:
  type: azure
  region: eu-west-1
  secretRef:
    name: cloudprovider
    namespace: shoot--foo--bar
  infrastructureProviderStatus:
    apiVersion: aws.provider.extensions.gardener.cloud/v1alpha1
    kind: InfrastructureStatus
    ec2:
      keyName: shoot--foo--bar-ssh-publickey
    iam:
      instanceProfiles:
      - name: shoot--foo--bar-nodes
        purpose: nodes
      roles:
      - arn: arn:aws:iam::0123456789:role/shoot--foo--bar-nodes
        purpose: nodes
    vpc:
      id: vpc-0123456789
      securityGroups:
      - id: sg-1234567890
        purpose: nodes
      subnets:
      - id: subnet-01234
        purpose: nodes
        zone: eu-west-1b
      - id: subnet-56789
        purpose: public
        zone: eu-west-1b
      - id: subnet-0123a
        purpose: nodes
        zone: eu-west-1c
      - id: subnet-5678a
        purpose: public
        zone: eu-west-1c
  pools:
  - name: cpu-worker
    minimum: 3
    maximum: 5
    maxSurge: 1
    maxUnavailable: 0
    machineType: m4.large
    machineImage:
      name: coreos
      version: 1967.5.0
    userData: c29tZSBkYXRhIHRvIGJvb3RzdHJhcCB0aGUgVk0K
    volume:
      size: 20Gi
      type: gp2
    zones:
    - eu-west-1b
    - eu-west-1c
    machineControllerManager:
      drainTimeout: 10m
      healthTimeout: 10m
      creationTimeout: 10m
      maxEvictRetries: 30
      nodeConditions:
      - ReadonlyFilesystem
      - DiskPressure
      - KernelDeadlock
```

The `.spec.secretRef` contains a reference to the provider secret pointing to the account that shall be used to create the needed virtual machines.
Also, as you can see, Gardener copies the output of the infrastructure creation (`.spec.infrastructureProviderStatus`), see [`Infrastructure` resource](/extensions/infrastructure), into the `.spec`.

In the `.spec.pools[]` field the desired worker pools are listed.
In the above example, one pool with machine type `m4.large` and `min=3`, `max=5` machines shall be spread over two availability zones (`eu-west-1b`, `eu-west-1c`).
This information together with the infrastructure status must be used to determine the proper configuration for the machine classes.

The `spec.pools[].machineControllerManager` field allows to configure the settings for machine-controller-manager component. Providers must populate these settings on worker-pool to the related [fields](https://github.com/gardener/machine-controller-manager/blob/master/kubernetes/machine_objects/machine-deployment.yaml#L30-L34) in MachineDeployment.

When seeing such a resource your controller must make sure that it deploys the machine-controller-manager next to the control plane in the seed cluster.
After that, it must compute the desired machine classes and the desired machine deployments.
Typically, one class maps to one deployment, and one class/deployment is created per availability zone.
Following this convention, the created resource would look like this:

```yaml
apiVersion: v1
kind: Secret
metadata:
  name: shoot--foo--bar-cpu-worker-z1-3db65
  namespace: shoot--foo--bar
  labels:
    gardener.cloud/purpose: machineclass
type: Opaque
data:
  providerAccessKeyId: eW91ci1hd3MtYWNjZXNzLWtleS1pZAo=
  providerSecretAccessKey: eW91ci1hd3Mtc2VjcmV0LWFjY2Vzcy1rZXkK
  userData: c29tZSBkYXRhIHRvIGJvb3RzdHJhcCB0aGUgVk0K
---
apiVersion: machine.sapcloud.io/v1alpha1
kind: AWSMachineClass
metadata:
  name: shoot--foo--bar-cpu-worker-z1-3db65
  namespace: shoot--foo--bar
spec:
  ami: ami-0123456789 # Your controller must map the stated version to the provider specific machine image information, in the AWS case the AMI.
  blockDevices:
  - ebs:
      volumeSize: 20
      volumeType: gp2
  iam:
    name: shoot--foo--bar-nodes
  keyName: shoot--foo--bar-ssh-publickey
  machineType: m4.large
  networkInterfaces:
  - securityGroupIDs:
    - sg-1234567890
    subnetID: subnet-01234
  region: eu-west-1
  secretRef:
    name: shoot--foo--bar-cpu-worker-z1-3db65
    namespace: shoot--foo--bar
  tags:
    kubernetes.io/cluster/shoot--foo--bar: "1"
    kubernetes.io/role/node: "1"
---
apiVersion: machine.sapcloud.io/v1alpha1
kind: MachineDeployment
metadata:
  name: shoot--foo--bar-cpu-worker-z1
  namespace: shoot--foo--bar
spec:
  replicas: 2
  selector:
    matchLabels:
      name: shoot--foo--bar-cpu-worker-z1
  strategy:
    type: RollingUpdate
    rollingUpdate:
      maxSurge: 1
      maxUnavailable: 0
  template:
    metadata:
      labels:
        name: shoot--foo--bar-cpu-worker-z1
    spec:
      class:
        kind: AWSMachineClass
        name: shoot--foo--bar-cpu-worker-z1-3db65
```

for the first availability zone `eu-west-1b`, and

```yaml
apiVersion: v1
kind: Secret
metadata:
  name: shoot--foo--bar-cpu-worker-z2-5z6as
  namespace: shoot--foo--bar
  labels:
    gardener.cloud/purpose: machineclass
type: Opaque
data:
  providerAccessKeyId: eW91ci1hd3MtYWNjZXNzLWtleS1pZAo=
  providerSecretAccessKey: eW91ci1hd3Mtc2VjcmV0LWFjY2Vzcy1rZXkK
  userData: c29tZSBkYXRhIHRvIGJvb3RzdHJhcCB0aGUgVk0K
---
apiVersion: machine.sapcloud.io/v1alpha1
kind: AWSMachineClass
metadata:
  name: shoot--foo--bar-cpu-worker-z2-5z6as
  namespace: shoot--foo--bar
spec:
  ami: ami-0123456789 # Your controller must map the stated version to the provider specific machine image information, in the AWS case the AMI.
  blockDevices:
  - ebs:
      volumeSize: 20
      volumeType: gp2
  iam:
    name: shoot--foo--bar-nodes
  keyName: shoot--foo--bar-ssh-publickey
  machineType: m4.large
  networkInterfaces:
  - securityGroupIDs:
    - sg-1234567890
    subnetID: subnet-0123a
  region: eu-west-1
  secretRef:
    name: shoot--foo--bar-cpu-worker-z2-5z6as
    namespace: shoot--foo--bar
  tags:
    kubernetes.io/cluster/shoot--foo--bar: "1"
    kubernetes.io/role/node: "1"
---
apiVersion: machine.sapcloud.io/v1alpha1
kind: MachineDeployment
metadata:
  name: shoot--foo--bar-cpu-worker-z1
  namespace: shoot--foo--bar
spec:
  replicas: 1
  selector:
    matchLabels:
      name: shoot--foo--bar-cpu-worker-z1
  strategy:
    type: RollingUpdate
    rollingUpdate:
      maxSurge: 1
      maxUnavailable: 0
  template:
    metadata:
      labels:
        name: shoot--foo--bar-cpu-worker-z1
    spec:
      class:
        kind: AWSMachineClass
        name: shoot--foo--bar-cpu-worker-z2-5z6as
```

for the second availability zone `eu-west-1c`.

Another convention is the 5-letter hash at the end of the machine class names.
Most controllers compute a checksum out of the specification of the machine class.
This helps to trigger a rolling update of the worker nodes if, for example, the machine image version changes.
In this case, a new checksum will be generated which results in the creation of a new machine class.
The `MachineDeployment`'s machine class reference (`.spec.template.spec.class.name`) is updated which triggers the rolling update process in the machine-controller-manager.
However, all of this is only a convention that eases writing the controller, but you can do it completely differently if you desire - as long as you make sure that the described behaviours are implemented correctly.

After the machine classes and machine deployments have been created the machine-controller-manager will start talking to the provider's IaaS API and create the virtual machines.
Gardener makes sure that the content of the `userData` field that is used to bootstrap the machines contain the required configuration for installation of the kubelet and registering the VM as worker node in the shoot cluster.
The `Worker` extension controller shall wait until all the created `MachineDeployment`s indicate healthiness/readiness before it ends the control loop.

## Does Gardener need some information that must be returned back?

Another important benefit of the machine-controller-manager's design principles (extending the Kubernetes API using CRDs) is that the [cluster-autoscaler](https://github.com/gardener/autoscaler) can be used **without** any provider-specific implementation.
We have forked the [upstream Kubernetes community's cluster-autoscaler](https://github.com/kubernetes/autoscaler/tree/master/cluster-autoscaler) and extended it so that it understands the machine API.
Definitely, we will merge it back into the community's versions once it has been adapted properly.

Our cluster-autoscaler only needs to know the minimum and maximum number of replicas **per** `MachineDeployment` and is ready to act without that it needs to talk to the provider APIs (it just modifies the `.spec.replicas` field in the `MachineDeployment` object).
Gardener deploys this autoscaler if there is at least one worker pool that specifies `max>min`.
In order to know how it needs to configure it, the provider-specific `Worker` extension controller must expose which `MachineDeployment`s it had created and how the `min`/`max` numbers should look like.

Consequently, your controller should write this information into the `Worker` resource's `.status.machineDeployments` field:

```yaml
---
apiVersion: extensions.gardener.cloud/v1alpha1
kind: Worker
metadata:
  name: worker
  namespace: shoot--foo--bar
spec:
  ...
status:
  lastOperation: ...
  machineDeployments:
  - name: shoot--foo--bar-cpu-worker-z1
    minimum: 2
    maximum: 3
  - name: shoot--foo--bar-cpu-worker-z2
    minimum: 1
    maximum: 2
```

In order to support a new worker provider you need to write a controller that watches all `Worker`s with `.spec.type=<my-provider-name>`.
You can take a look at the below referenced example implementation for the AWS provider.

## That sounds like a lot that needs to be done, can you help me?

All of the described behaviour is mostly the same for every provider.
The only difference is maybe the version/configuration of the machine-controller-manager, and the machine class specification itself.
You can take a look at our [extension library](https://github.com/gardener/gardener/blob/master/extensions), especially the [worker controller](https://github.com/gardener/gardener/tree/master/extensions/pkg/controller/worker) part where you will find a lot of utilities that you can use.
Also, using the library you only need to implement your provider specifics - all the things that can be handled generically can be taken for free and do not need to be re-implemented.
Take a look at the [AWS worker controller](https://github.com/gardener/gardener-extension-provider-aws/tree/master/pkg/controller/worker) for finding an example.

## Non-provider specific information required for worker creation

All the providers require further information that is not provider specific but already part of the shoot resource.
One example for such information is whether the shoot is hibernated or not.
In this case all the virtual machines should be deleted/terminated, and after that the machine controller-manager should be scaled down.
You can take a look at the [AWS worker controller](https://github.com/gardener/gardener-extension-provider-aws/tree/master/pkg/controller/worker) to see how it reads this information and how it is used.
As Gardener cannot know which information is required by providers it simply mirrors the `Shoot`, `Seed`, and `CloudProfile` resources into the seed.
They are part of the [`Cluster` extension resource](/extensions/cluster) and can be used to extract information that is not part of the `Worker` resource itself.

## References and additional resources

* [`Worker` API (Golang specification)](https://github.com/gardener/gardener/blob/master/pkg/apis/extensions/v1alpha1/types_worker.go)
* [Extension controller library](https://github.com/gardener/gardener/blob/master/extensions)
* [Generic worker controller](https://github.com/gardener/gardener/tree/master/extensions/pkg/controller/worker)
* [Exemplary implementation for the AWS provider](https://github.com/gardener/gardener-extension-provider-aws/tree/master/pkg/controller/worker)
