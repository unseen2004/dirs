package com.example.glucoapp.data.db.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "InsulinTypes")
data class InsulinType(
    @PrimaryKey(autoGenerate = true) val typeId: Int = 0,
    @ColumnInfo(name = "typeName") val typeName: String,
    @ColumnInfo(name = "insulinName") val insulinName: String,
    @ColumnInfo(name = "userId") val userId: Int // Add this field

)