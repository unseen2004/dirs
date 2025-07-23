package com.example.glucoapp.data.db.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey

@Entity(
    tableName = "Meals",
    foreignKeys = [
        ForeignKey(
            entity = User::class,
            parentColumns = ["userId"],
            childColumns = ["userId"],
            onDelete = ForeignKey.CASCADE
        )
    ]
)
//To Do: Meal connect with ingredient
data class Meal(
    @PrimaryKey(autoGenerate = true) val mealId: Int = 0,
    @ColumnInfo(name = "userId") val userId: Int,
    @ColumnInfo(name = "foodName") val foodName: String?, // Now allows null
    @ColumnInfo(name = "protein") val protein: Double?, // Allow null for custom values
    @ColumnInfo(name = "carbs") val carbs: Double?, // Allow null for custom values
    @ColumnInfo(name = "fat") val fat: Double?, // Allow null for custom values
    @ColumnInfo(name = "kcal") val kcal: Double?, // Allow null for custom values
)