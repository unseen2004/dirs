package com.example.glucoapp.data.db.daos

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Transaction
import androidx.room.Update
import com.example.glucoapp.data.db.models.Ingredient
import kotlinx.coroutines.flow.Flow

@Dao
interface IngredientDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(ingredient: Ingredient)

    @Update
    suspend fun update(ingredient: Ingredient)

    @Delete
    suspend fun delete(ingredient: Ingredient)

    @Query("SELECT * FROM Ingredients")
    fun getAllIngredients(): Flow<List<Ingredient>>


    @Transaction
    suspend fun insertAndUpdateIngredient(ingredient: Ingredient) {
        insert(ingredient)
        update(ingredient)
    }

    @Query("SELECT * FROM Ingredients WHERE userId = :userId")
    fun getAllIngredientsByUserId(userId: Int): Flow<List<Ingredient>>

    @Query("SELECT * FROM Ingredients WHERE ingredientId = :ingredientId")
    fun getIngredientById(ingredientId: Int): Flow<Ingredient?>

}