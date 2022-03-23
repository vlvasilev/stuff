---
title: README
---
# Documentation Index

## Overview

* [General Architecture](https://github.com/gardener/gardener/blob/master/docs/concepts/architecture.md)
* [Gardener landing page `gardener.cloud`](https://gardener.cloud/)
* ["Gardener, the Kubernetes Botanist" blog on kubernetes.io](https://kubernetes.io/blog/2018/05/17/gardener/)
* ["Gardener Project Update" blog on kubernetes.io](https://kubernetes.io/blog/2019/12/02/gardener-project-update/)

## Concepts

* Components
  * [Gardener API server](https://github.com/gardener/gardener/blob/master/docs/concepts/apiserver.md)
    * [In-Tree admission plugins](https://github.com/gardener/gardener/blob/master/docs/concepts/apiserver_admission_plugins.md)
  * [Gardener Controller Manager](https://github.com/gardener/gardener/blob/master/docs/concepts/controller-manager.md)
  * [Gardener Scheduler](https://github.com/gardener/gardener/blob/master/docs/concepts/scheduler.md)
  * [Gardener Admission Controller](https://github.com/gardener/gardener/blob/master/docs/concepts/admission-controller.md)
  * [Gardener Seed Admission Controller](https://github.com/gardener/gardener/blob/master/docs/concepts/seed-admission-controller.md)
  * [Gardenlet](https://github.com/gardener/gardener/blob/master/docs/concepts/gardenlet.md)
* [Backup Restore](https://github.com/gardener/gardener/blob/master/docs/concepts/backup-restore.md)
* [Network Policies](https://github.com/gardener/gardener/blob/master/docs/concepts/network_policies.md)
* [Relation between Gardener API and Cluster API](https://github.com/gardener/gardener/blob/master/docs/concepts/cluster-api.md)

## Usage

* [Register Shoot as Seed](https://github.com/gardener/gardener/blob/master/docs/usage/managed_seed.md)
* [API Server Network Proxy Reverse Tunneling](https://github.com/gardener/gardener/blob/master/docs/usage/reverse-tunnel.md)
* [Audit a Kubernetes cluster](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_auditpolicy.md)
* [Auto-Scaling for shoot clusters](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_autoscaling.md)
* [Custom `CoreDNS` configuration](https://github.com/gardener/gardener/blob/master/docs/usage/custom-dns.md)
* [Gardener configuration and usage](https://github.com/gardener/gardener/blob/master/docs/usage/configuration.md)
* [`ManagedIstio` feature](https://github.com/gardener/gardener/blob/master/docs/usage/istio.md)
* [Network Policies in the Shoot Cluster](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_network_policies.md)
* [`NodeLocalDNS` feature](https://github.com/gardener/gardener/blob/master/docs/usage/node-local-dns.md)
* [OpenIDConnect presets](https://github.com/gardener/gardener/blob/master/docs/usage/openidconnect-presets.md)
* [Projects](https://github.com/gardener/gardener/blob/master/docs/usage/projects.md)
* [Seed Bootstrapping](https://github.com/gardener/gardener/blob/master/docs/usage/seed_bootstrapping.md)
* [Seed Settings](https://github.com/gardener/gardener/blob/master/docs/usage/seed_settings.md)
* [Shoot cluster purposes](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_purposes.md)
* [Shoot Kubernetes and Operating System Versioning](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_versions.md)
* [Shoot Networking](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_networking.md)
* [Shoot Maintenance](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_maintenance.md)
* [Shoot Status](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_status.md)
* [Shoot Updates and Upgrades](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_updates.md)
* [Supported Kubernetes versions](https://github.com/gardener/gardener/blob/master/docs/usage/supported_k8s_versions.md)
* [Tolerations](https://github.com/gardener/gardener/blob/master/docs/usage/tolerations.md)
* [Trigger shoot operations](https://github.com/gardener/gardener/blob/master/docs/usage/shoot_operations.md)
* [Troubleshooting guide](https://github.com/gardener/gardener/blob/master/docs/usage/trouble_shooting_guide.md)
* [Trusted TLS certificate for shoot control planes](https://github.com/gardener/gardener/blob/master/docs/usage/trusted-tls-for-control-planes.md)
* [APIServerSNI environment variable injection](https://github.com/gardener/gardener/blob/master/docs/usage/apiserver-sni-injection.md)
* [Reversed Cluster VPN](https://github.com/gardener/gardener/blob/master/docs/usage/reversed-vpn-tunnel.md)

## Proposals

* [GEP: Gardener Enhancement Proposal Description](https://github.com/gardener/gardener/blob/master/docs/proposals/README.md)
* [GEP: Template](https://github.com/gardener/gardener/blob/master/docs/proposals/00-template.md)
* [GEP-1: Gardener extensibility and extraction of cloud-specific/OS-specific knowledge](https://github.com/gardener/gardener/blob/master/docs/proposals/01-extensibility.md)
* [GEP-2: `BackupInfrastructure` CRD and Controller Redesign](https://github.com/gardener/gardener/blob/master/docs/proposals/02-backupinfra.md)
* [GEP-3: Network extensibility](https://github.com/gardener/gardener/blob/master/docs/proposals/03-networking-extensibility.md)
* [GEP-4: New `core.gardener.cloud/v1alpha1` APIs required to extract cloud-specific/OS-specific knowledge out of Gardener core](https://github.com/gardener/gardener/blob/master/docs/proposals/04-new-core-gardener-cloud-apis.md)
* [GEP-5: Gardener Versioning Policy](https://github.com/gardener/gardener/blob/master/docs/proposals/05-versioning-policy.md)
* [GEP-6: Integrating etcd-druid with Gardener](https://github.com/gardener/gardener/blob/master/docs/proposals/06-etcd-druid.md)
* [GEP-7: Shoot Control Plane Migration](https://github.com/gardener/gardener/blob/master/docs/proposals/07-shoot-control-plane-migration.md)
* [GEP-8: SNI Passthrough proxy for kube-apiservers](https://github.com/gardener/gardener/blob/master/docs/proposals/08-shoot-apiserver-via-sni.md)
* [GEP-9: Gardener integration test framework](https://github.com/gardener/gardener/blob/master/docs/proposals/09-test-framework.md)
* [GEP-10: Support additional container runtimes](https://github.com/gardener/gardener/blob/master/docs/proposals/10-shoot-additional-container-runtimes.md)
* [GEP-11: Utilize API Server Network Proxy to Invert Seed-to-Shoot Connectivity](https://github.com/gardener/gardener/blob/master/docs/proposals/11-apiserver-network-proxy.md)
* [GEP-12: OIDC Webhook Authenticator](https://github.com/gardener/gardener/blob/master/docs/proposals/12-oidc-webhook-authenticator.md)
* [GEP-13: Automated Seed Management](https://github.com/gardener/gardener/blob/master/docs/proposals/13-automated-seed-management.md)
* [GEP-14: Reversed Cluster VPN](https://github.com/gardener/gardener/blob/master/docs/proposals/14-reversed-cluster-vpn.md)
* [GEP-15: Manage Bastions and SSH Key Pair Rotation](https://github.com/gardener/gardener/blob/master/docs/proposals/15-manage-bastions-and-ssh-key-pair-rotation.md)
* [GEP-16: Dynamic kubeconfig generation for Shoot clusters](https://github.com/gardener/gardener/blob/master/docs/proposals/16-adminkubeconfig-subresource.md)

## Development

* [Setting up a local development environment](https://github.com/gardener/gardener/blob/master/docs/development/local_setup.md)
* [Unit Testing and Dependency Management](https://github.com/gardener/gardener/blob/master/docs/development/testing_and_dependencies.md)
* [Changing the API](https://github.com/gardener/gardener/blob/master/docs/development/changing-the-api.md)
* [Releases, Features, Hotfixes](https://github.com/gardener/gardener/blob/master/docs/development/process.md)
* [Adding New Cloud Providers](https://github.com/gardener/gardener/blob/master/docs/development/new-cloud-provider.md)
* [Extending the Monitoring Stack](https://github.com/gardener/gardener/blob/master/docs/development/monitoring-stack.md)
* [How to create log parser for container into fluent-bit](https://github.com/gardener/gardener/blob/master/docs/development/log_parsers.md)
* [Network Policies in the Seed Cluster](https://github.com/gardener/gardener/blob/master/docs/development/seed_network_policies.md)

## Extensions

* [Extensibility overview](https://github.com/gardener/gardener/blob/master/docs/extensions/overview.md)
* [Extension controller registration](https://github.com/gardener/gardener/blob/master/docs/extensions/controllerregistration.md)
* [`Cluster` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/cluster.md)
* Extension points
  * [General conventions](https://github.com/gardener/gardener/blob/master/docs/extensions/conventions.md)
  * [Trigger for reconcile operations](https://github.com/gardener/gardener/blob/master/docs/extensions/reconcile-trigger.md)
  * [Deploy resources into the shoot cluster](https://github.com/gardener/gardener/blob/master/docs/extensions/managedresources.md)
  * [Shoot resource customization webhooks](https://github.com/gardener/gardener/blob/master/docs/extensions/shoot-webhooks.md)
  * [Logging and monitoring for extensions](https://github.com/gardener/gardener/blob/master/docs/extensions/logging-and-monitoring.md)
  * [Contributing to shoot health status conditions](https://github.com/gardener/gardener/blob/master/docs/extensions/shoot-health-status-conditions.md)
    * [Health Check Library](https://github.com/gardener/gardener/blob/master/docs/extensions/healthcheck-library.md)
  * Blob storage providers
    * [`BackupBucket` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/backupbucket.md)
    * [`BackupEntry` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/backupentry.md)
  * DNS providers
    * [`DNSProvider` and `DNSEntry` resources](https://github.com/gardener/gardener/blob/master/docs/extensions/dns.md)
  * IaaS/Cloud providers
    * [Control plane customization webhooks](https://github.com/gardener/gardener/blob/master/docs/extensions/controlplane-webhooks.md)
    * [`Bastion` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/bastion.md)
    * [`ControlPlane` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/controlplane.md)
    * [`ControlPlane` exposure resource](https://github.com/gardener/gardener/blob/master/docs/extensions/controlplane-exposure.md)
    * [`Infrastructure` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/infrastructure.md)
    * [`Worker` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/worker.md)
  * Network plugin providers
    * [`Network` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/network.md)
  * Operating systems
    * [`OperatingSystemConfig` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/operatingsystemconfig.md)
  * Container runtimes
    * [`ContainerRuntime` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/containerruntime.md)
  * Generic (non-essential) extensions
    * [`Extension` resource](https://github.com/gardener/gardener/blob/master/docs/extensions/extension.md)
* [Extending project roles](https://github.com/gardener/gardener/blob/master/docs/extensions/project-roles.md)
* [Referenced resources](https://github.com/gardener/gardener/blob/master/docs/extensions/referenced-resources.md)
* [Control plane migration](https://github.com/gardener/gardener/blob/master/docs/extensions/migration.md)

## Testing

* [Integration Testing Manual](https://github.com/gardener/gardener/blob/master/docs/testing/integration_tests.md)

## Deployment

* [Setup Gardener on a Kubernetes cluster](https://github.com/gardener/gardener/blob/master/docs/deployment/setup_gardener.md)
* [Deploying Gardenlets](https://github.com/gardener/gardener/blob/master/docs/deployment/deploy_gardenlet.md)
    * [Automatic Deployment of Gardenlets](https://github.com/gardener/gardener/blob/master/docs/deployment/deploy_gardenlet_automatically.md)
    * [Deploy a Gardenlet Manually](https://github.com/gardener/gardener/blob/master/docs/deployment/deploy_gardenlet_manually.md)
    * [Scoped API Access for Gardenlets](https://github.com/gardener/gardener/blob/master/docs/deployment/gardenlet_api_access.md)
* [Deploying the Gardener and a Seed into an AKS cluster](https://github.com/gardener/gardener/blob/master/docs/deployment/aks.md)
* [Overwrite image vector](https://github.com/gardener/gardener/blob/master/docs/deployment/image_vector.md)
* [Migration from Gardener `v0` to `v1`](https://github.com/gardener/gardener/blob/master/docs/deployment/migration_v0_to_v1.md)
* [Feature Gates in Gardener](https://github.com/gardener/gardener/blob/master/docs/deployment/feature_gates.md)

## Monitoring

* [Alerting](https://github.com/gardener/gardener/blob/master/docs/monitoring/alerting.md)
* [User Alerts](https://github.com/gardener/gardener/blob/master/docs/monitoring/user_alerts.md)
* [Operator Alerts](https://github.com/gardener/gardener/blob/master/docs/monitoring/operator_alerts.md)
