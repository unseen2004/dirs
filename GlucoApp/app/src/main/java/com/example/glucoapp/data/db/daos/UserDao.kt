package com.example.glucoapp.data.db.daos

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Transaction
import androidx.room.Update
import com.example.glucoapp.data.db.models.Note
import com.example.glucoapp.data.db.models.User
import kotlinx.coroutines.flow.Flow

@Dao
interface UserDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(user: User): Long

    @Update
    suspend fun update(user: User)

    @Delete
    suspend fun delete(user: User)

    @Query("SELECT * FROM Users WHERE userId = :userId")
    fun getUserById(userId: Int): Flow<User>


    @Query("SELECT * FROM users WHERE username = :username")
    fun getUserByUsername(username: String): Flow<User?>

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertNote(note: Note): Long

    @Transaction
    suspend fun insertUserAndNote(user: User, note: Note) {
        val userId = insert(user)
        insertNote(note.copy(userId = userId.toInt()))
    }


    @Query("UPDATE Users SET passwordHash = :newPasswordHash WHERE userId = :userId")
    suspend fun updatePassword(userId: Int, newPasswordHash: String)

    @Query("UPDATE Users SET username = :newUsername WHERE userId = :userId")
    suspend fun updateUsername(userId: Int, newUsername: String)

    @Query("UPDATE Users SET doctorPasswordHash = :newDoctorPasswordHash WHERE userId = :userId")
    suspend fun updateDoctorPassword(userId: Int, newDoctorPasswordHash: String)
    @Query("DELETE FROM Users WHERE userId = :userId")
    suspend fun clearCurrentUser(userId: Int)
    @Transaction
    suspend fun deleteUserAndRelatedData(userId: Int) {
        deleteUserById(userId)
        deleteNotesByUserId(userId)
        deleteMealsByUserId(userId)
        deleteActivitiesByUserId(userId)
    }
    @Query("DELETE FROM Users WHERE userId = :userId")
    suspend fun deleteUserById(userId: Int)

    @Query("DELETE FROM Notes WHERE userId = :userId")
    suspend fun deleteNotesByUserId(userId: Int)

    @Query("DELETE FROM Meals WHERE userId = :userId")
    suspend fun deleteMealsByUserId(userId: Int)

    @Query("DELETE FROM Activities WHERE userId = :userId")
    suspend fun deleteActivitiesByUserId(userId: Int)

}