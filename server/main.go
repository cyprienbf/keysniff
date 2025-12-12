package main

import (
	"fmt"
	"log"
	"net/http"
	"net/http/httputil"
	"os"
	"path/filepath"
	"time"
)

// Directory where logs will be stored
const LOG_DIR = "logs"
const MAX_UPLOAD_SIZE = 1024 * 100

func requestHandler(w http.ResponseWriter, r *http.Request) {
	// 1. Handle only POST requests (optional, but cleaner)
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST method is accepted", http.StatusMethodNotAllowed)
		return
	}

	r.Body = http.MaxBytesReader(w, r.Body, MAX_UPLOAD_SIZE)

	// 2. Capture the full request (Headers + Body) in RAW format
	requestDump, err := httputil.DumpRequest(r, true)
	if err != nil {
		http.Error(w, fmt.Sprint(err), http.StatusInternalServerError)
		return
	}

	// 3. Generate filename based on current date and time
	filename := fmt.Sprintf("%s_%s.txt", 
		time.Now().Format("2006-01-02"), 
		time.Now().Format("15-04-05.000000"), // Added microseconds to avoid collisions
	)
	filePath := filepath.Join(LOG_DIR, filename)

	// 4. Write to file
	err = os.WriteFile(filePath, requestDump, 0644)
	if err != nil {
		log.Printf("File write error: %v", err)
		http.Error(w, "Server error", http.StatusInternalServerError)
		return
	}

	// 5. Console log and response to client
	fmt.Printf("[+] Received POST -> Saved to %s\n", filePath)
	w.WriteHeader(http.StatusOK)
	w.Write([]byte("OK"))
}

func main() {
	// Create logs directory if it doesn't exist
	if _, err := os.Stat(LOG_DIR); os.IsNotExist(err) {
		err := os.Mkdir(LOG_DIR, 0755)
		if err != nil {
			log.Fatal("Could not create log directory:", err)
		}
	}

	port := ":8080"
	fmt.Printf("Server listening on port %s...\n", port)
	
	http.HandleFunc("/", requestHandler)
	
	// Start the server
	if err := http.ListenAndServe(port, nil); err != nil {
		log.Fatal(err)
	}
}