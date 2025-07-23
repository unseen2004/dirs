package com.example.glucoapp.data.db.daos

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Update
import androidx.room.OnConflictStrategy
import com.example.glucoapp.data.db.models.Activity
import kotlinx.coroutines.flow.Flow

@Dao
interface ActivityDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(activity: Activity): Long

    @Delete
    suspend fun delete(activity: Activity)

    @Update
    suspend fun update(activity: Activity)

    @Query("SELECT * FROM Activities WHERE userId = :userId")
    fun getActivitiesByUserId(userId: Int): Flow<List<Activity>>

    @Query("SELECT * FROM Activities WHERE activityId = :activityId")
    fun getActivityById(activityId: Int): Flow<Activity?>
}