package main

import (
    "github.com/unseen2004/crudApi/internal/app"
    "net/http"
    "time"
    "fmt"
)

func main(){
    app, err := app.NewApplication()
    if err != nil {
        panic(err)
    }

    app.Logger.Println("running")
    
    http.HandleFunc("/health", HealthCheck)
    server := &http.Server{
        Addr:           ":8080",
        IdleTimeout:    time.Minute,
        ReadTimeout:    10 * time.Second,
        WriteTimeout:   30 * time.Second,
    }

    err = server.ListenAndServe()
    if err != nil {
        app.Logger.Fatal(err)
    }
}

func HealthCheck(w http.ResponseWriter, r *http.Request) {
    fmt.Fprint(w, "Status is available\n")
}




