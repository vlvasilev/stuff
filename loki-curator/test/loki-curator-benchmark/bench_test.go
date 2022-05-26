package main

import (
	"runtime"
	"testing"
<<<<<<< HEAD
	"time"
=======
>>>>>>> f21d90c (make revendor)

	"github.com/gardener/logging/pkg/loki/curator/utils"
)

var (
<<<<<<< HEAD
	filesNumber = 20000
=======
	filesNumber = 2000
>>>>>>> f21d90c (make revendor)
	filePath    = "./test-files"
)

func TestGetNOldestFiles(t *testing.T) {
	files, err := utils.GetNOldestFiles(filePath, filesNumber)
	if err != nil {
		t.Fatal(err)
	}
	if len(files) != filesNumber {
		t.Fatal("the returned files are less than 20000")
	}
}

func BenchmarkGetNOldestFiles(b *testing.B) {
<<<<<<< HEAD
	for i := 0; i < 10; i++ {
=======
	for i := 0; i < 1; i++ {
>>>>>>> f21d90c (make revendor)
		files, err := utils.GetNOldestFiles(filePath, filesNumber)
		if err != nil {
			b.Fatal(err)
		}
		if len(files) != filesNumber {
			b.Fatalf("the returned files are less than %d", filesNumber)
		}
		runtime.GC()
<<<<<<< HEAD
		time.Sleep(time.Minute * 5)
=======
		//time.Sleep(time.Minute * 5)
>>>>>>> f21d90c (make revendor)
	}
}
