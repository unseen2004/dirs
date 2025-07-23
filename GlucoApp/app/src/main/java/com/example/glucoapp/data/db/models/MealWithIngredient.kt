package com.example.glucoapp.data.db.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey

@Entity(
    tableName = "MealIngredients",
    foreignKeys = [
        ForeignKey(
            entity = Meal::class,
            parentColumns = ["mealId"],
            childColumns = ["mealId"],
            onDelete = ForeignKey.CASCADE
        ),
        ForeignKey(
            entity = Ingredient::class,
            parentColumns = ["ingredientId"],
            childColumns = ["ingredientId"],
            onDelete = ForeignKey.CASCADE
        )
    ]
)
data class MealWithIngredient(
    @PrimaryKey(autoGenerate = true) val mealIngredientId: Int = 0,
    @ColumnInfo(name = "mealId") val mealId: Int,
    @ColumnInfo(name = "ingredientId") val ingredientId: Int,
    @ColumnInfo(name = "quantity") val quantity: Double
)