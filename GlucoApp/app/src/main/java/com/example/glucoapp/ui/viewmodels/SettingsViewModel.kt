package com.example.glucoapp.ui.viewmodels

import android.util.Log
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.glucoapp.data.UserPreferences
import com.example.glucoapp.data.repository.AppRepository
import com.example.glucoapp.data.db.models.User
import com.example.glucoapp.data.db.models.Ingredient
import com.example.glucoapp.data.db.models.InsulinType
import com.example.glucoapp.data.db.models.Activity
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.firstOrNull
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import java.nio.charset.StandardCharsets
import java.security.MessageDigest
import javax.inject.Inject

sealed class SettingsUiState {
    object Loading : SettingsUiState()
    data class Success(val user: User) : SettingsUiState()
    data class Error(val message: String) : SettingsUiState()
}

@HiltViewModel
class SettingsViewModel @Inject constructor(
    private val repository: AppRepository,
    private val userPreferences: UserPreferences
) : ViewModel() {
    private val _currentLanguage = MutableStateFlow("en")
    private val _uiState = MutableStateFlow<SettingsUiState>(SettingsUiState.Loading)
    val uiState: StateFlow<SettingsUiState> = _uiState.asStateFlow()

    private val _insulinTypes = MutableStateFlow<List<InsulinType>>(emptyList())
    val insulinTypes: StateFlow<List<InsulinType>> = _insulinTypes.asStateFlow()

    private val _ingredients = MutableStateFlow<List<Ingredient>>(emptyList())
    val ingredients: StateFlow<List<Ingredient>> = _ingredients.asStateFlow()

    private val _activities = MutableStateFlow<List<Activity>>(emptyList())
    val activities: StateFlow<List<Activity>> = _activities.asStateFlow()

    private val _userId = MutableStateFlow<Int?>(null)
    val userId: StateFlow<Int?> = _userId.asStateFlow()

    init {
        loadUserId()
        loadInsulinTypes()
        loadIngredients()
        loadActivities()
        loadCurrentLanguage()
    }
    private fun loadCurrentLanguage() {
        viewModelScope.launch {
            userPreferences.languageFlow.collect { language ->
                _currentLanguage.value = language
            }
        }
    }
    val currentLanguage = userPreferences.languageFlow.stateIn(
        scope = viewModelScope,
        started = SharingStarted.WhileSubscribed(5000),
        initialValue = "en"
    )
    fun changeLanguage(newLanguage: String) {
        viewModelScope.launch {
            userPreferences.saveLanguage(newLanguage) // Save language
            Log.d("SettingsViewModel", "changeLanguage called with: $newLanguage")
        }
    }
    private fun loadUserId() {
        viewModelScope.launch {
            userPreferences.userId.collect { id ->
                _userId.value = id
                id?.let {
                    loadCurrentUser(it)
                }
            }
        }
    }

    private fun loadCurrentUser(userId: Int) {
        viewModelScope.launch {
            try {
                repository.getUserById(userId).collect { user ->
                    user?.let {
                        _uiState.value = SettingsUiState.Success(it)
                    } ?: run {
                        _uiState.value = SettingsUiState.Error("User not found")
                    }
                }
            } catch (e: Exception) {
                _uiState.value = SettingsUiState.Error(e.localizedMessage ?: "An error occurred")
            }
        }
    }

    fun updateUser(user: User) {
        viewModelScope.launch {
            repository.updateUser(user)
            _uiState.value = SettingsUiState.Success(user)
        }
    }

    fun changePassword(newPassword: String) {
        viewModelScope.launch {
            _userId.value?.let { userId ->
                val user = repository.getUserById(userId).firstOrNull()
                user?.let {
                    val hashedPassword = hashPassword(newPassword)
                    val updatedUser = it.copy(passwordHash = hashedPassword)
                    repository.updateUser(updatedUser)
                    _uiState.value = SettingsUiState.Success(updatedUser)
                }
            }
        }
    }

    fun changeDoctorPassword(newPassword: String) {
        viewModelScope.launch {
            _userId.value?.let { userId ->
                val user = repository.getUserById(userId).firstOrNull()
                user?.let {
                    val hashedPassword = hashPassword(newPassword)
                    val updatedUser = it.copy(doctorPasswordHash = hashedPassword)
                    repository.updateUser(updatedUser)
                    _uiState.value = SettingsUiState.Success(updatedUser)
                }
            }
        }
    }

    private fun hashPassword(password: String): String {
        val digest = MessageDigest.getInstance("SHA-256")
        val hashBytes = digest.digest(password.toByteArray(StandardCharsets.UTF_8))
        return hashBytes.joinToString("") { "%02x".format(it) }
    }

    fun addInsulinType(insulinType: InsulinType) {
        viewModelScope.launch {
            repository.insertInsulinType(insulinType)
            loadInsulinTypes()
        }
    }

    fun deleteInsulinType(insulinType: InsulinType) {
        viewModelScope.launch {
            repository.deleteInsulinType(insulinType)
            loadInsulinTypes()
        }
    }

    fun addIngredient(ingredient: Ingredient) {
        viewModelScope.launch {
            repository.insertIngredient(ingredient)
            loadIngredients()
        }
    }

    fun deleteIngredient(ingredient: Ingredient) {
        viewModelScope.launch {
            repository.deleteIngredient(ingredient)
            loadIngredients()
        }
    }

    fun addActivity(activity: Activity) {
        viewModelScope.launch {
            repository.insertActivity(activity)
            loadActivities()
        }
    }

    fun deleteActivity(activity: Activity) {
        viewModelScope.launch {
            repository.deleteActivity(activity)
            loadActivities()
        }
    }

    private fun loadIngredients() {
        viewModelScope.launch {
            _userId.value?.let { userId ->
                repository.getAllIngredientsByUserId(userId).collect { ingredients ->
                    _ingredients.value = ingredients
                }
            }
        }
    }

    private fun loadInsulinTypes() {
        viewModelScope.launch {
            _userId.value?.let { userId ->
                repository.getAllInsulinTypesByUserId(userId).collect { insulinTypes ->
                    _insulinTypes.value = insulinTypes
                }
            }
        }
    }
    fun deleteUserAndLogout() {
        viewModelScope.launch {
            _userId.value?.let { userId ->
                repository.deleteUserAndRelatedData(userId)
                repository.logoutUser()
            }
        }
    }
    private fun loadActivities() {
        viewModelScope.launch {
            _userId.value?.let { userId ->
                repository.getActivitiesByUserId(userId).collect { activities ->
                    _activities.value = activities
                }
            }
        }
    }

    fun logout() {
        viewModelScope.launch {
            repository.logoutUser()
        }
    }
}