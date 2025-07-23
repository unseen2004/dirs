package com.example.glucoapp.data
import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.booleanPreferencesKey
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.intPreferencesKey
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import dagger.hilt.android.qualifiers.ApplicationContext
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map
import javax.inject.Inject

class UserPreferences @Inject constructor(
    @ApplicationContext private val context: Context
)  {

    // Keys for preferences
    private object PreferencesKeys {
        val THEME_KEY = stringPreferencesKey("theme")
        val LANGUAGE_KEY = stringPreferencesKey("language")
        val USER_ID_KEY = intPreferencesKey("user_id")
        val IS_DOCTOR_LOGGED_IN = booleanPreferencesKey("is_doctor_logged_in")

    }
    private val Context.dataStore: DataStore<Preferences> by preferencesDataStore(name = "user_preferences")

    val languageFlow: Flow<String> = context.dataStore.data
        .map { preferences ->
            preferences[PreferencesKeys.LANGUAGE_KEY] ?: "en"
        }

    suspend fun saveLanguage(language: String) {
        context.dataStore.edit { preferences ->
            preferences[PreferencesKeys.LANGUAGE_KEY] = language
        }
    }
    // DataStore instance (initialize once)

    // User ID operations
    val userId: Flow<Int?> = context.dataStore.data.map { preferences ->
        preferences[PreferencesKeys.USER_ID_KEY]
    }

//    suspend fun setUserId(userId: Int) {
//        context.dataStore.edit { preferences ->
//            preferences[PreferencesKeys.USER_ID_KEY] = userId
//        }
//    }

    suspend fun saveUserId(userId: Int) {
        context.dataStore.edit { preferences ->
            preferences[PreferencesKeys.USER_ID_KEY] = userId
        }
    }
    suspend fun clearUserId() {
        context.dataStore.edit { preferences ->
            preferences.remove(PreferencesKeys.USER_ID_KEY)
        }
    }

    // Doctor login status
    val isDoctorLoggedIn: Flow<Boolean> = context.dataStore.data.map { preferences ->
        preferences[PreferencesKeys.IS_DOCTOR_LOGGED_IN] ?: false
    }

    suspend fun setDoctorLoggedIn(isLoggedIn: Boolean) {
        context.dataStore.edit { preferences ->
            preferences[PreferencesKeys.IS_DOCTOR_LOGGED_IN] = isLoggedIn
        }
    }

    // Theme preferences
    suspend fun saveTheme(theme: String) {
        context.dataStore.edit { preferences ->
            preferences[PreferencesKeys.THEME_KEY] = theme
        }
    }

    val themeFlow: Flow<String> = context.dataStore.data
        .map { preferences ->
            preferences[PreferencesKeys.THEME_KEY] ?: "system"
        }


    // Clear all preferences
    suspend fun clearPreferences() {
        context.dataStore.edit { preferences ->
            preferences.clear()
        }
    }
}