package com.example.glucoapp.data.db.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey

@Entity(
    tableName = "Notes",
    foreignKeys = [
        ForeignKey(
            entity = User::class,
            parentColumns = ["userId"],
            childColumns = ["userId"],
            onDelete = ForeignKey.CASCADE
        ),
        ForeignKey(
            entity = InsulinType::class,
            parentColumns = ["typeId"],
            childColumns = ["insulinTypeId"],
            onDelete = ForeignKey.SET_NULL
        ),
        ForeignKey(
            entity = Meal::class,
            parentColumns = ["mealId"],
            childColumns = ["mealId"],
            onDelete = ForeignKey.SET_NULL
        ),
        ForeignKey(
            entity = Activity::class,
            parentColumns = ["activityId"],
            childColumns = ["activityId"],
            onDelete = ForeignKey.SET_NULL
        )
    ]
)
data class Note(
    @PrimaryKey(autoGenerate = true) val noteId: Int = 0,
    @ColumnInfo(name = "userId") val userId: Int,
    @ColumnInfo(name = "timestamp") val timestamp: Long,
    @ColumnInfo(name = "glucoseLevel") val glucoseLevel: Int?,
    @ColumnInfo(name = "InsulinAmount") val InsulinAmount: Double?,
    @ColumnInfo(name = "insulinTypeId") val insulinTypeId: Int?,
    @ColumnInfo(name = "noteText") val noteText: String?,
    @ColumnInfo(name = "WW") val WW: Double?,
    @ColumnInfo(name = "WBT") val WBT: Double?,
    @ColumnInfo(name = "mealId") val mealId: Int?,
    @ColumnInfo(name = "activityId") val activityId: Int?
)