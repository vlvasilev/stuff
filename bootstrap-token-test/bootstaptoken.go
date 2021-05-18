package main

import (
	"crypto/rand"
	"encoding/base64"
	"fmt"
	"math/big"
	"strings"
)

type writeFileCloudConfig struct {
}

func main() {
	cloudConfig := `
#!/bin/bash
somescript`
	alteredCloudConfig := addBootsrapKubeconfigToUserData([]byte(cloudConfig), "ca", "server", "token")
	alteredCloudConfigStr := string(alteredCloudConfig)
	fmt.Println(alteredCloudConfigStr)
}

func addBootsrapKubeconfigToUserData(userdata []byte, ca, server, token string) []byte {
	userDataStr := strings.TrimSpace(string(userdata))
	if strings.HasPrefix(userDataStr, "#cloud-config") {
		kubeconfigWriteFileContent := generateCloudConfigWriteFile(ca, server, token)
		return addWriteFileToCloudConfig(userDataStr, kubeconfigWriteFileContent)
	}

	kubeconfigWriteFileContent := generateBashScriptWriteFile(ca, server, token)
	return addWriteFileToScript(userDataStr, kubeconfigWriteFileContent)
}

// generateRandomStringFromCharset generates a cryptographically secure random string of the specified length <n>.
// The set of allowed characters can be specified. Returns error if there was a problem during the random generation.
func generateRandomStringFromCharset(n int, allowedCharacters string) (string, error) {
	output := make([]byte, n)
	max := new(big.Int).SetInt64(int64(len(allowedCharacters)))
	for i := range output {
		randomCharacter, err := rand.Int(rand.Reader, max)
		if err != nil {
			return "", err
		}
		output[i] = allowedCharacters[randomCharacter.Int64()]
	}
	return string(output), nil
}

const dirKubelet = "/var/lib/kubelet"
const kubeconfigBootstrap = dirKubelet + "/" + "kubeconfig-bootstrap"

func generateBootStrappKubeConfig(ca, server, token string) string {
	r := strings.NewReplacer("<ca_ph>", ca,
		"<server_ph>", server,
		"<token_ph>", token)
	return r.Replace(
		`---
apiVersion: v1
kind: Config
current-context: kubelet-bootstrap@default
clusters:
- cluster:
	certificate-authority-data: <ca_ph>
	server: <server_ph>
	name: default
contexts:
- context:
	cluster: default
	user: kubelet-bootstrap
	name: kubelet-bootstrap@default
users:
- name: kubelet-bootstrap
	user:
	as-user-extra: {}
	token: <token_ph>`)
}

func generateCloudConfigWriteFile(ca, server, token string) string {
	kubeconfig := generateBootStrappKubeConfig(ca, server, token)
	kubeconfig = base64.StdEncoding.EncodeToString([]byte(kubeconfig))

	r := strings.NewReplacer("<content>", kubeconfig,
		"<path>", kubeconfigBootstrap)
	return r.Replace(
		`- encoding: b64
  content: <content>
  path: <path>
  permissions: "0644"`)
}

func generateBashScriptWriteFile(ca, server, token string) string {
	kubeconfig := generateBootStrappKubeConfig(ca, server, token)
	kubeconfig = base64.StdEncoding.EncodeToString([]byte(kubeconfig))

	r := strings.NewReplacer("<content>", kubeconfig,
		"<path>", kubeconfigBootstrap)
	return r.Replace(
		`
cat << 'EOF' | base64 -d > <path>
<content>
EOF`)
}

func addWriteFileToCloudConfig(userdata, writeFile string) []byte {
	var sb strings.Builder
	var findWriteFile = false
	cloudConfigLines := strings.Split(userdata, "\n")
	for _, line := range cloudConfigLines {
		sb.WriteString(line)
		sb.WriteString("\n")
		if strings.HasPrefix(line, "write_file") {
			findWriteFile = true
			sb.WriteString(writeFile)
			sb.WriteString("\n")
		}
	}
	if !findWriteFile {
		sb.WriteString("write_file:\n")
		sb.WriteString(writeFile)
		sb.WriteString("\n")
	}

	return []byte(sb.String())
}

func addWriteFileToScript(userdata, writeFile string) []byte {
	return []byte(userdata + writeFile)
}
