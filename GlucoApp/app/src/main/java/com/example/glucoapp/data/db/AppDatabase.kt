package com.example.glucoapp.data.db

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase
import com.example.glucoapp.data.db.daos.*
import com.example.glucoapp.data.db.models.*
@Database(
    entities = [User::class, Note::class, Meal::class, Activity::class, InsulinType::class, Ingredient::class, MealWithIngredient::class],
    version = 1,
    exportSchema = false
)
abstract class AppDatabase : RoomDatabase() {
    abstract fun userDao(): UserDao
    abstract fun noteDao(): NoteDao
    abstract fun mealDao(): MealDao
    abstract fun activityDao(): ActivityDao
    abstract fun insulinTypeDao(): InsulinTypeDao
    abstract fun ingredientDao(): IngredientDao
    abstract fun mealWithIngredientDao(): MealWithIngredientDao // Added this line

    companion object {
        @Volatile
        private var INSTANCE: AppDatabase? = null

        fun getDatabase(context: Context): AppDatabase {
            return INSTANCE ?: synchronized(this) {
                Room.databaseBuilder(
                    context,
                    AppDatabase::class.java,
                    "glucose_tracker_database"
                )
                    .fallbackToDestructiveMigration()
                    .build()
            }
        }
    }
}