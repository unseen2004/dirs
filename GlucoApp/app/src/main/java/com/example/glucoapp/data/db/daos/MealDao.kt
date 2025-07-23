package com.example.glucoapp.data.db.daos

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Update
import androidx.room.OnConflictStrategy
import com.example.glucoapp.data.db.models.Meal
import kotlinx.coroutines.flow.Flow
import androidx.room.Transaction


@Dao
interface MealDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(meal: Meal)

    @Update
    suspend fun update(meal: Meal)

    @Delete
    suspend fun delete(meal: Meal)

    @Query("SELECT * FROM Meals WHERE userId = :userId")
    fun getMealsByUserId(userId: Int): Flow<List<Meal>>

    @Query("SELECT * FROM Meals WHERE mealId = :mealId")
    fun getMealById(mealId: Int): Flow<Meal>

    @Transaction
    suspend fun insertAndUpdateMeal(meal: Meal) {
        insert(meal)
        update(meal)
    }
}