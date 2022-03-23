package main

import (
	"io/ioutil"
	"log"
	"net/http"
	"/Users/i330716/go/pkg/mod/sigs.k8s.io/controller-runtime@v0.10.2/pkg/healthz"
)

func main() {
	//Make the Handler
	handler := healthz.
	resp, err := http.Get("https://jsonplaceholder.typicode.com/posts")
	if err != nil {
		log.Fatalln(err)
	}
	//We Read the response body on the line below.
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}
}
