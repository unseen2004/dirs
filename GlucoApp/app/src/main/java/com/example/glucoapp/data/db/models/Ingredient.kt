package com.example.glucoapp.data.db.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "Ingredients")
data class Ingredient(
    @PrimaryKey(autoGenerate = true) val ingredientId: Int = 0,
    @ColumnInfo(name = "foodName") val foodName: String,
    @ColumnInfo(name = "protein") val protein: Float,
    @ColumnInfo(name = "carbs") val carbs: Float,
    @ColumnInfo(name = "fat") val fat: Float,
    @ColumnInfo(name = "kcal") val kcal: Float,
    @ColumnInfo(name = "userId") val userId: Int // Add this field

    )