package com.example.glucoapp.navigation

sealed class Screen(val route: String) {
    object Login : Screen("login")
    object Main : Screen("main")
    object Notes : Screen("notes")
    object Meals : Screen("meals")
    object Settings : Screen("settings")
    object AddNote : Screen("add_note")
    object AddMeal : Screen("add_meal")
    object Register : Screen("register")
}