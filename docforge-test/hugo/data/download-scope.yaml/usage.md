---
title: Usage
---
This guide is about Gardener Logging, how it is organized and how to use the dashboard to view the log data of Kubernetes clusters.

# Cluster level logging
Log data is fundamental for the successful operation activities of Kubernetes landscapes. It is used for investigating problems and monitoring cluster activity.

Cluster level logging is the recommended way to collect and store log data for Kubernetes cluster components. With cluster level logging the log data is externalized
in a logging backend where the log lifecycle management is independent from the lifecycle management of the Kubernetes resources.

Cluster level logging is not available by default with [Kubernetes](https://kubernetes.io/docs/concepts/cluster-administration/logging/#cluster-level-logging-architectures) and consumers have to additionally implement it.
The Kubernetes project only provides basic logging capabilities via `kubectl logs` where the kubelet keeps one terminated container with its logs.
When a pod is evicted from the node, all corresponding containers are also evicted, along with their logs.
This is why the default log storage solution is considered short-lived and not sufficient when you want to properly operate a Kubernetes environment.

Gardener, as an advanced Kubernetes management solution, follows the general recommendations and offers a cluster level logging solution to ensure proper log storage for all managed Kubernetes resources.
The log management is setup when a new cluster is created.
Log collection is organized using [fluent-bit](https://fluentbit.io).
Log storage and search is organized using [Loki](https://grafana.com/oss/loki).
Log visualization is available using [Grafana](https://grafana.com/grafana) that is deployed with predefined dashboard and visualization for every shoot cluster.


Using Kubernetes operators can benefit from different capabilities like accessing the logs for
already terminated containers and performing fast and sophisticated search queries for investigating long-lasting or recurring problems based on logs from a long period of time.

In this guide, you will find out how to explore the log data for your clusters.

## Exploring logs

The sections below describe how access Grafana and use it to view the log data of your Kubernetes cluster.

### Accessing Grafana
1. On the Gardener dashboard, choose **CLUSTERS** > [YOUR-CLUSTER] > **OVERVIEW** > **Logging and Monitoring**.
![Navigate to Logging and Monitoring Tile](/__resources/14498783-85a3-465f-b476-0674f5edd2f1.png)

2. Use the link in the **Logging and Monitoring** tile to open the Grafana dashboard.
3. Enter the login credentials shown in the **Logging and Monitoring** tile to log in the Grafana dashboard.
The default values of the credentials for Grafana are:
- Username : `admin`
- Password : `admin`
![Login Screen](/__resources/38b7dbf0-be82-4f89-b120-0054c0581552.png)

Upon successful login you will be asked to changing the default password.
**Note:** These credentials are shared among all operators. Changing the default password will affect their access. You can safely skip this step.
![Button to Skip Password Change](/__resources/b5eb6536-75ee-458e-93b2-2727347fc75e.png)

### Using Grafana

There are two ways to explore log messages in Grafana.

#### Predefined Dashboards
The first one is to use the predefined dashboards.
1. Go to the **Home** tab.
2. Choose which dashboard to open.
The dashboards that contain log visualizations for the different Grafana deployments are:

  * Garden Grafana
    * Pod Logs
    * Extensions
    * Systemd Logs
  * User Grafana
    * Kubernetes Control Plane Status
  * Operator Grafana
    * Kubernetes Pods
    * Kubernetes Control Plane Status

    ![Dashboard Navigator](/__resources/a84f981c-13e8-4f64-ad00-da80d3e4dac7.png)

#### Explore tab
The second one is to use the **Explore** tab.

To enable this option you need to authenticate in front of the Grafana UI.
1. Choose the login button (bottom left corner).
![Login Button on Grafana Home Screen](/__resources/b4ad4393-eeed-4c0b-b882-027fd4a0fa12.png)

2. Log in following the steps described in the [Acccessing Grafana](#accessing-grafana) section.
3. Choose the ***Explore*** tab (upper left side of the screen).
![Grafana Explore Tab](/__resources/dcd12250-c87e-45bb-b19b-c603895fcc3e.png)
You can create a custom log filters based on the predefined labels used in `Loki`.
The following properties can be managed in the `Explore` tab:
- `Datasource` (top left corner) should be set on Loki
- `Timerange` (top right corner) is used to filter logs over a different period of time
- `Label Selector` (top left corner) is used to filter logs based on the `Loki`'s labels and their values.
For example:
`pod_name="kube-apiserver-1234-1234"` or you can use a regular expression (regex): `pod_name=~"kube-apiserver.+"`
- `Severity` (left side of the screen). This option is used to filter log messages with specific severity.

4. Click on **Run Query** (top right corner) and the log messages, which fulfil the list of selected properties above, will be displayed.
