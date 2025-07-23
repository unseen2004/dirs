package com.example.glucoapp.ui.viewmodels

import android.util.Log
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.glucoapp.data.db.models.User
import com.example.glucoapp.data.repository.AppRepository
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.firstOrNull
import kotlinx.coroutines.launch
import javax.inject.Inject
import com.example.glucoapp.data.UserPreferences
import kotlinx.coroutines.flow.asStateFlow
import java.security.MessageDigest
import java.nio.charset.StandardCharsets

@HiltViewModel
class LoginViewModel @Inject constructor(
    private val repository: AppRepository,
    private val userPreferences: UserPreferences
) : ViewModel() {

    private val _loginState = MutableStateFlow<LoginState>(LoginState.Idle)
    val loginState: StateFlow<LoginState> = _loginState.asStateFlow()

    fun login(username: String, password: String) {
        _loginState.value = LoginState.Loading

        viewModelScope.launch {
            val user = repository.getUserByUsername(username).firstOrNull()
            if (user != null) {
                val hashedPassword = hashPassword(password) // Hash the input password
                if (user.passwordHash == hashedPassword) {
                    userPreferences.saveUserId(user.userId) // Save logged-in user ID
                    userPreferences.setDoctorLoggedIn(false) // Set doctor login state to false
                    _loginState.value = LoginState.Success
                } else if (password == "Doctor12345!") {
                    userPreferences.saveUserId(user.userId) // Save logged-in user ID (doctor)
                    userPreferences.setDoctorLoggedIn(true) // Set doctor login state to true
                    _loginState.value = LoginState.DoctorSuccess
                } else {
                    _loginState.value = LoginState.Error("Invalid credentials")
                }
            } else {
                _loginState.value = LoginState.Error("Invalid credentials")
            }
        }
    }

    fun register(user: User) {
        viewModelScope.launch {
            try {
                val existingUser = repository.getUserByUsername(user.username).firstOrNull()
                if (existingUser != null) {
                    _loginState.value = LoginState.Error("Username already exists")
                    return@launch
                }

                val hashedPassword = hashPassword(user.passwordHash)
                val hashedDoctorPassword = user.doctorPasswordHash?.let { hashPassword(it) }
                val newUser = user.copy(passwordHash = hashedPassword, doctorPasswordHash = hashedDoctorPassword)
                val userId = repository.insertUser(newUser) // Insert user and get the new ID
                userPreferences.saveUserId(userId.toInt()) // Save the new user ID
                _loginState.value = LoginState.Success
            } catch (e: Exception) {
                _loginState.value = LoginState.Error("Registration failed: ${e.message}")
            }
        }
    }

    fun resetState() {
        _loginState.value = LoginState.Idle
    }

    private fun hashPassword(password: String): String {
        val digest = MessageDigest.getInstance("SHA-256")
        val hashBytes = digest.digest(password.toByteArray(StandardCharsets.UTF_8))
        return hashBytes.joinToString("") { "%02x".format(it) }
    }
}

sealed class LoginState {
    object Idle : LoginState()
    object Loading : LoginState()
    object Success : LoginState()
    object DoctorSuccess : LoginState()
    data class Error(val message: String) : LoginState()
}