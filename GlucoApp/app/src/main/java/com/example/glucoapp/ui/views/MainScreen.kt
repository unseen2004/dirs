package com.example.glucoapp.ui.views

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavController
import com.example.glucoapp.data.UserPreferences
import com.example.glucoapp.navigation.Screen
import com.example.glucoapp.ui.viewmodels.MainViewModel
import com.example.glucoapp.R
import androidx.compose.ui.res.stringResource
import com.example.glucoapp.ui.viewmodels.SettingsViewModel

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MainScreen(
    navController: NavController,
    viewModel: MainViewModel = hiltViewModel(),
    settingsViewModel: SettingsViewModel = hiltViewModel(), // Inject SettingsViewModel
    userPreferences: UserPreferences
) {
    val selectedScreen by viewModel.selectedScreen.collectAsState()
    val currentLanguage by settingsViewModel.currentLanguage.collectAsState()

    Scaffold(
        topBar = {
            TopAppBar(title = { Text(stringResource(R.string.glucose_tracker)) })
        },
        bottomBar = {
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(8.dp),
                horizontalArrangement = Arrangement.SpaceAround
            ) {
                Button(
                    onClick = { viewModel.onScreenSelected(Screen.Notes) },
                    modifier = Modifier.weight(1f),
                    content = {
                        Text(
                            stringResource(R.string.notes),
                            color = if (selectedScreen == Screen.Notes) Color.Red else Color.White
                        )
                    }
                )
                Spacer(modifier = Modifier.weight(0.2f))
                Button(
                    onClick = { viewModel.onScreenSelected(Screen.Meals) },
                    modifier = Modifier.weight(1f),
                    content = {
                        Text(
                            stringResource(R.string.meals),
                            color = if (selectedScreen == Screen.Meals) Color.Red else Color.White
                        )
                    }
                )
                Spacer(modifier = Modifier.weight(0.2f))
                Button(
                    onClick = { viewModel.onScreenSelected(Screen.Settings) },
                    modifier = Modifier.weight(1f),
                    content = {
                        Text(
                            stringResource(R.string.settings),
                            color = if (selectedScreen == Screen.Settings) Color.Red else Color.White
                        )
                    }
                )
            }
        }
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
        ) {
            when (selectedScreen) {
                Screen.Notes -> NotesScreen(navController, userPreferences = userPreferences)
                Screen.Meals -> MealsScreen(navController, isDoctor = userPreferences.isDoctorLoggedIn.collectAsState(initial = false).value)
                Screen.Settings -> SettingsScreen(
                    navController,
                    isDoctor = userPreferences.isDoctorLoggedIn.collectAsState(initial = false).value,
                    language = currentLanguage // Pass the language parameter
                )
                else -> {}
            }
        }
    }
}