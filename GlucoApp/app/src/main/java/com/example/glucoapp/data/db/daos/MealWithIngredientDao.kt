package com.example.glucoapp.data.db.daos

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Update
import androidx.room.OnConflictStrategy
import com.example.glucoapp.data.db.models.MealWithIngredient
import kotlinx.coroutines.flow.Flow

@Dao
interface MealWithIngredientDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(mealWithIngredient: MealWithIngredient)

    @Update
    suspend fun update(mealWithIngredient: MealWithIngredient)

    @Delete
    suspend fun delete(mealWithIngredient: MealWithIngredient)

    @Query("SELECT * FROM MealIngredients WHERE mealId = :mealId")
    fun getMealIngredientsByMealId(mealId: Int): Flow<List<MealWithIngredient>>

    @Query("SELECT * FROM MealIngredients WHERE ingredientId = :ingredientId")
    fun getMealIngredientsByIngredientId(ingredientId: Int): Flow<List<MealWithIngredient>>
}