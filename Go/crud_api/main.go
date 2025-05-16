package main

import (
    "net/http"
    "time"
    "fmt"
    "flag"

    "github.com/unseen2004/crudApi/internal/routes"
    "github.com/unseen2004/crudApi/internal/app"
)

func main(){
    var port int
    flag.IntVar(&port, "port", 8080, "go backend server port")

    app, err := app.NewApplication()
    if err != nil {
        panic(err)
    }

    defer app.DB.Close()
    
    r := routes.SetupRoutes(app)
    server := &http.Server{
        Addr:           fmt.Sprintf(":%d", port), 
        Handler:        r,
        IdleTimeout:    time.Minute,
        ReadTimeout:    10 * time.Second,
        WriteTimeout:   30 * time.Second,
    }

    app.Logger.Printf("we are running on port %d\n", port)

    err = server.ListenAndServe()
    if err != nil {
        app.Logger.Fatal(err)
    }
}






