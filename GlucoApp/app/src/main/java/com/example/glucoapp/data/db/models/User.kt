package com.example.glucoapp.data.db.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey


@Entity(tableName = "Users")
data class User(
    @PrimaryKey(autoGenerate = true) val userId: Int = 0,
    @ColumnInfo(name = "username") val username: String,
    @ColumnInfo(name = "passwordHash") val passwordHash: String,
    @ColumnInfo(name = "themeMode") val themeMode: Int = 0, // 0 - System, 1 - Light, 2 - Dark, its Blue light or Blue dark
    @ColumnInfo(name = "language") val language: String = "en", // "en" or "ja"
    @ColumnInfo(name = "doctorPasswordHash") val doctorPasswordHash: String? = null,
    @ColumnInfo(name = "accessLevel") val accessLevel: Int = 0, // 0 - User, 1 - Doctor
    @ColumnInfo(name = "glucoseUnit") val glucoseUnit: Int = 0 // 0 - mg/dL, 1 - mmol/L
)