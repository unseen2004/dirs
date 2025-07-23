package com.example.glucoapp.data.db.daos

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Transaction
import androidx.room.Update
import com.example.glucoapp.data.db.models.InsulinType
import kotlinx.coroutines.flow.Flow


@Dao
interface InsulinTypeDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(insulinType: InsulinType)

    @Update
    suspend fun update(insulinType: InsulinType)

    @Delete
    suspend fun delete(insulinType: InsulinType)

    @Query("SELECT * FROM InsulinTypes")
    fun getAllInsulinTypes(): Flow<List<InsulinType>>

    @Query("SELECT * FROM InsulinTypes WHERE typeId = :typeId")
    fun getInsulinTypeById(typeId: Int): Flow<InsulinType>
    @Query("SELECT * FROM InsulinTypes WHERE userId = :userId")
    fun getAllInsulinTypesByUserId(userId: Int): Flow<List<InsulinType>>
    @Transaction
    suspend fun insertAndUpdateInsulinType(insulinType: InsulinType) {
        insert(insulinType)
        update(insulinType)
    }
}