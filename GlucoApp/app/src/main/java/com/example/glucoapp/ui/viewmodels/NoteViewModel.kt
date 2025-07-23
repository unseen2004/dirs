package com.example.glucoapp.ui.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.glucoapp.data.UserPreferences
import com.example.glucoapp.data.repository.AppRepository
import com.example.glucoapp.data.db.models.Note
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject
import com.example.glucoapp.data.db.models.User
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import com.example.glucoapp.data.db.models.Meal
import com.example.glucoapp.data.db.models.Ingredient
import com.example.glucoapp.data.db.models.Activity
import com.example.glucoapp.data.db.models.InsulinType

@HiltViewModel
class NoteViewModel @Inject constructor(
    private val repository: AppRepository,
    private val userPreferences: UserPreferences
) : ViewModel() {

    private val _notes = MutableStateFlow<List<Note>>(emptyList())
    val notes: StateFlow<List<Note>> = _notes.asStateFlow()

    private val _user = MutableStateFlow<User?>(null)
    val user: StateFlow<User?> = _user

    private val _meals = MutableStateFlow<List<Meal>>(emptyList())
    val meals: StateFlow<List<Meal>> = _meals.asStateFlow()

    private val _predefinedMeals = MutableStateFlow<List<Ingredient>>(emptyList())
    val predefinedMeals: StateFlow<List<Ingredient>> = _predefinedMeals.asStateFlow()

    private val _activities = MutableStateFlow<List<Activity>>(emptyList())
    val activities: StateFlow<List<Activity>> = _activities.asStateFlow()

    private val _selectedActivityId = MutableStateFlow<Int?>(null)
    val selectedActivityId: StateFlow<Int?> = _selectedActivityId.asStateFlow()

    private val _insulinTypes = MutableStateFlow<List<InsulinType>>(emptyList())
    val insulinTypes: StateFlow<List<InsulinType>> = _insulinTypes.asStateFlow()

    private val _userId = MutableStateFlow<Int?>(null)
    val userId: StateFlow<Int?> = _userId.asStateFlow()

    init {
        loadInsulinTypes()
        loadActivities()
        loadUserId()
    }

    private fun loadUserId() {
        viewModelScope.launch {
            userPreferences.userId.collect { id ->
                _userId.value = id
            }
        }
    }

    fun loadInsulinTypes() {
        viewModelScope.launch {
            repository.getAllInsulinTypes().collect { types ->
                _insulinTypes.value = types
            }
        }
    }

    fun loadUserById(userId: Int) {
        viewModelScope.launch {
            repository.getUserById(userId).collect { user ->
                _user.value = user
            }
        }
    }

    fun loadNotesByUserId(userId: Int) {
        viewModelScope.launch {
            repository.getNotesByUserId(userId).collect { notes ->
                _notes.value = notes
            }
        }
    }

    fun insertNote(note: Note) {
        viewModelScope.launch {
            repository.insertNote(note)
            _user.value?.userId?.let { loadNotesByUserId(it) }
        }
    }

    fun updateNote(note: Note) {
        viewModelScope.launch {
            repository.updateNote(note)
            _user.value?.userId?.let { loadNotesByUserId(it) }
        }
    }

    fun deleteNote(note: Note) {
        viewModelScope.launch {
            repository.deleteNote(note)
            _user.value?.userId?.let { loadNotesByUserId(it) }
        }
    }

    fun loadMealsByUserId(userId: Int) {
        viewModelScope.launch {
            repository.getMealsByUserId(userId).collect { meals ->
                _meals.value = meals
            }
        }
    }

    fun loadActivities() {
        viewModelScope.launch {
            repository.getActivitiesByUserId(1).collect { activities ->
                _activities.value = activities
            }
        }
    }

    fun insertActivity(activity: Activity, callback: (Int) -> Unit) {
        viewModelScope.launch {
            val newActivityId = repository.insertActivity(activity)
            callback(newActivityId.toInt())
        }
    }

    fun setSelectedActivityId(activityId: Int?) {
        _selectedActivityId.value = activityId
    }
}