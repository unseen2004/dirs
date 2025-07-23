package com.example.glucoapp.ui.viewmodels

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.glucoapp.data.db.models.User
import com.example.glucoapp.data.repository.AppRepository
import com.example.glucoapp.navigation.Screen
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject
import com.example.glucoapp.ui.viewmodels.LoginState

@HiltViewModel
class MainViewModel @Inject constructor(private val repository: AppRepository) : ViewModel() {

    private val _currentUser = MutableStateFlow<User?>(null)
    val currentUser: StateFlow<User?> = _currentUser

    private val _selectedScreen = MutableStateFlow<Screen>(Screen.Notes)
    val selectedScreen: StateFlow<Screen> = _selectedScreen

    private val _loginState = MutableStateFlow<LoginState>(LoginState.Idle)
    val loginState: StateFlow<LoginState> = _loginState.asStateFlow()

    fun checkCurrentUser() {
        // In a real app, you'd check for a stored user or token here
        viewModelScope.launch {
            // For now, assuming no user is logged in initially
        }
    }

    fun onScreenSelected(screen: Screen) {
        _selectedScreen.value = screen
    }

    fun logout() {
        // In a real app, you'd clear any stored user data or tokens here
        _currentUser.value = null
        _loginState.value = LoginState.Idle
    }
}