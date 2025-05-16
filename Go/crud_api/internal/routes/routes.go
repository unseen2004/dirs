package routes

import (
    "github.com/go-chi/chi/v5"
    "github.com/unseen2004/crudApi/internal/app"
)

func SetupRoutes(app *app.Application) *chi.Mux {
    r := chi.NewRouter()

    r.Get("/health", app.HealthCheck)
    r.Get("/workouts/{id}", app.WorkoutHandler.HandleGetWorkoutByID)

    r.Post("/workouts", app.WorkoutHandler.HandleCreateWorkout)

    return r
}
