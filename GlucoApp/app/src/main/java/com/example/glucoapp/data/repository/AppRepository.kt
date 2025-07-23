package com.example.glucoapp.data.repository
import com.example.glucoapp.data.db.models.*
import kotlinx.coroutines.flow.Flow

interface AppRepository {
    // User operations
    fun getUserById(userId: Int): Flow<User?>
    suspend fun getUserByUsername(username: String): Flow<User?>
    suspend fun insertUser(user: User): Long
    suspend fun updateUser(user: User)
    suspend fun deleteUser(user: User)
    suspend fun registerUser(user: User): Long // Added function
    suspend fun updatePassword(userId: Int, newPasswordHash: String)
    suspend fun updateUsername(userId: Int, newUsername: String)
    suspend fun updateDoctorPassword(userId: Int, newDoctorPasswordHash: String)
    suspend fun deleteUserAndRelatedData(userId: Int)
    // Note operations
    suspend fun insertNote(note: Note)
    suspend fun updateNote(note: Note)
    suspend fun deleteNote(note: Note)
    fun getNotesByUserId(userId: Int): Flow<List<Note>>

    // Meal operations
    suspend fun insertMeal(meal: Meal)
    suspend fun updateMeal(meal: Meal)
    suspend fun deleteMeal(meal: Meal)
    fun getMealsByUserId(userId: Int): Flow<List<Meal>>
    fun getMealById(mealId: Int): Flow<Meal?>

    // Activity operations
    suspend fun insertActivity(activity: Activity): Long
    suspend fun updateActivity(activity: Activity)
    suspend fun deleteActivity(activity: Activity)
    fun getActivitiesByUserId(userId: Int): Flow<List<Activity>>
    fun getActivityById(activityId: Int): Flow<Activity?>

    // InsulinType operations
    suspend fun insertInsulinType(insulinType: InsulinType)
    suspend fun updateInsulinType(insulinType: InsulinType)
    suspend fun deleteInsulinType(insulinType: InsulinType)
    fun getAllInsulinTypes(): Flow<List<InsulinType>>
    fun getInsulinTypeById(typeId: Int): Flow<InsulinType?>
    fun getAllInsulinTypesByUserId(userId: Int): Flow<List<InsulinType>>

    // Ingredient operations
    suspend fun insertIngredient(ingredient: Ingredient)
    suspend fun updateIngredient(ingredient: Ingredient)
    suspend fun deleteIngredient(ingredient: Ingredient)
    fun getAllIngredients(): Flow<List<Ingredient>>
    fun getIngredientById(ingredientId: Int): Flow<Ingredient?>
    fun getAllIngredientsByUserId(userId: Int): Flow<List<Ingredient>>

    suspend fun logoutUser()

}