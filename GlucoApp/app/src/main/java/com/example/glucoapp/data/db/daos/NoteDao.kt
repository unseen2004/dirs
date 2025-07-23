package com.example.glucoapp.data.db.daos

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Update
import androidx.room.OnConflictStrategy
import com.example.glucoapp.data.db.models.Note
import kotlinx.coroutines.flow.Flow
import androidx.room.Transaction


@Dao
interface NoteDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(note: Note)

    @Update
    suspend fun update(note: Note)

    @Delete
    suspend fun delete(note: Note)

    @Query("SELECT * FROM Notes WHERE userId = :userId ORDER BY timestamp DESC")
    fun getNotesByUserId(userId: Int): Flow<List<Note>>

    @Transaction
    suspend fun insertAndUpdateNote(note: Note) {
        insert(note)
        update(note)
    }
}