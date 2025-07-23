package com.example.glucoapp.ui.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.glucoapp.data.UserPreferences
import com.example.glucoapp.data.db.models.Meal
import com.example.glucoapp.data.db.models.User
import com.example.glucoapp.data.db.models.Ingredient
import com.example.glucoapp.data.repository.AppRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

@HiltViewModel
class MealsViewModel @Inject constructor(
    private val repository: AppRepository,
    private val userPreferences: UserPreferences
) : ViewModel() {

    private val _meals = MutableStateFlow<List<Meal>>(emptyList())
    val meals: StateFlow<List<Meal>> = _meals.asStateFlow()

    private val _user = MutableStateFlow<User?>(null)
    val user: StateFlow<User?> = _user

    private val _ingredients = MutableStateFlow<List<Ingredient>>(emptyList())
    val ingredients: StateFlow<List<Ingredient>> = _ingredients.asStateFlow()

    private val _userId = MutableStateFlow<Int?>(null)
    val userId: StateFlow<Int?> = _userId.asStateFlow()

    init {
        loadUserId()
    }

    private fun loadUserId() {
        viewModelScope.launch {
            userPreferences.userId.collect { id ->
                _userId.value = id
            }
        }
    }

    fun loadMealsByUserId(userId: Int) {
        viewModelScope.launch {
            repository.getMealsByUserId(userId).collect { meals ->
                _meals.value = meals
            }
        }
    }

    fun insertMeal(meal: Meal) {
        viewModelScope.launch {
            repository.insertMeal(meal)
            _userId.value?.let { loadMealsByUserId(it) }
        }
    }

    fun deleteMeal(meal: Meal) {
        viewModelScope.launch {
            repository.deleteMeal(meal)
            _userId.value?.let { loadMealsByUserId(it) }
        }
    }

    fun updateMeal(meal: Meal) {
        viewModelScope.launch {
            repository.updateMeal(meal)
            _userId.value?.let { loadMealsByUserId(it) }
        }
    }

    fun loadIngredients() {
        viewModelScope.launch {
            repository.getAllIngredients().collect { ingredients ->
                _ingredients.value = ingredients
            }
        }
    }
}