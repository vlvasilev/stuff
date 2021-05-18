package main

import (
	"bytes"
	"crypto/rand"
	"encoding/base64"
	"fmt"
	"math/big"
	"strings"
	"text/template"
)

func main() {
	userData := []byte(
		`#cloud-config
write_file:
- encoding: b64
  content: <content>
  path: <path>
  permissions: "0644"`)
	userData = addBootsrapKubeconfigToUserData(userData, "CA", "KUBE_API", "12345.67890")
	fmt.Println(string(userData))
}

const bootstraptemplate = `
{{- define "CLOUD_CONFIG_WRITE_FILE" -}}
- encoding: b64
  content: {{ index . "CONTENT" }}
  path: {{ index . "PATH" }}
  permissions: "0644"
{{- end -}}

{{- define "SCRIPT_WRITE_FILE" }}
cat << 'EOF' | base64 -d > {{ index . "PATH" }}
{{ index . "PATH" }}
EOF
{{- end -}}`
const dirKubelet = "/var/lib/kubelet"
const kubeconfigBootstrap = dirKubelet + "/" + "bootstrap-token"

func addBootsrapKubeconfigToUserData(userdata []byte, ca, server, token string) []byte {

	config := map[string]interface{}{
		"CA":      ca,
		"SERVER":  server,
		"TOKEN":   token,
		"PATH":    kubeconfigBootstrap,
		"CONTENT": "",
	}

	tmpl, err := template.New("bootstrapkubeconfig.tmpl").Parse(bootstraptemplate)
	if err != nil {
		//TODO: handle this error
	}
	buffer := bytes.NewBufferString("")

	//generate kubeconfig for the bootstrapping
	err = tmpl.ExecuteTemplate(buffer, "BOOTSTRAP_KUBECONFIG", config)
	if err != nil {
		//TODO: handle this error
	}
	config["CONTENT"] = base64.StdEncoding.EncodeToString(buffer.Bytes())
	buffer.Reset()

	userDataStr := strings.TrimSpace(string(userdata))
	if strings.HasPrefix(userDataStr, "#cloud-config") {
		err = tmpl.ExecuteTemplate(buffer, "CLOUD_CONFIG_WRITE_FILE", config)
		if err != nil {
			//TODO: handle this error
		}
		kubeconfigWriteFileContent := string(buffer.Bytes())
		return addWriteFileToCloudConfig(userDataStr, kubeconfigWriteFileContent)
	}
	err = tmpl.ExecuteTemplate(buffer, "SCRIPT_WRITE_FILE", config)
	if err != nil {
		//TODO: handle this error
	}
	kubeconfigWriteFileContent := string(buffer.Bytes())
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
	return []byte(userdata + "\n" + writeFile)
}
