package app

import(
    "log"
    "os"
    "fmt"
    "net/http"
    "database/sql"

    "github.com/unseen2004/crudApi/internal/api"
    "github.com/unseen2004/crudApi/internal/store"
)

type Application struct {
    Logger          *log.Logger
    WorkoutHandler  *api.WorkoutHandler
    DB              *sql.DB
}

func NewApplication() (*Application, error){
    pgDB, err := store.Open()
    if err != nil {
        return nil, err
    }

    logger := log.New(os.Stdout, "", log.Ldate|log.Ltime)

    
    workoutHandler := api.NewWorkoutHandler()

    app := &Application{
        Logger:         logger,
        WorkoutHandler: workoutHandler,
        DB:             pgDB,
    }

    return app, nil
}


func (a *Application) HealthCheck(w http.ResponseWriter, r *http.Request) {
    fmt.Fprint(w, "Status is available\n")
}
