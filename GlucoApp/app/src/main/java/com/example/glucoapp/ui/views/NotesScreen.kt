package com.example.glucoapp.ui.views

import android.util.Log
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Delete
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavController
import com.example.glucoapp.data.UserPreferences
import com.example.glucoapp.data.db.models.Note
import com.example.glucoapp.navigation.Screen
import com.example.glucoapp.ui.viewmodels.NoteViewModel
import javax.inject.Inject
import com.example.glucoapp.R
import androidx.compose.ui.res.stringResource
@Composable
fun NotesScreen(
    navController: NavController,
    viewModel: NoteViewModel = hiltViewModel(),
    userPreferences: UserPreferences
) {
    val userId by viewModel.userId.collectAsState(initial = null)
    val notes by viewModel.notes.collectAsState()
    val isDoctor by userPreferences.isDoctorLoggedIn.collectAsState(initial = false)

    LaunchedEffect(userId) {
        userId?.let {
            viewModel.loadNotesByUserId(it)
        }
    }

    Box(modifier = Modifier.fillMaxSize()) {
        LazyColumn(modifier = Modifier.fillMaxSize()) {
            items(notes) { note ->
                NoteItem(note = note, isDoctor = isDoctor, onDeleteClick = {
                    if (!isDoctor) {
                        viewModel.deleteNote(note)
                    }
                })
            }
        }

        if (!isDoctor) {
            FloatingActionButton(
                onClick = { navController.navigate(Screen.AddNote.route) },
                modifier = Modifier
                    .align(Alignment.BottomEnd)
                    .padding(16.dp)
            ) {
                Icon(Icons.Filled.Add, stringResource(R.string.add_note))
            }
        }
    }
}

@Composable
fun NoteItem(note: Note, isDoctor: Boolean, onDeleteClick: () -> Unit) {
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .padding(8.dp),
        elevation = CardDefaults.cardElevation(defaultElevation = 4.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp)
        ) {
            Text(
                text = note.noteText ?: stringResource(R.string.no_note_text),
                fontWeight = FontWeight.Bold,
                style = MaterialTheme.typography.headlineSmall
            )
            Spacer(modifier = Modifier.padding(4.dp))

            Text(
                text = stringResource(R.string.glucose, note.glucoseLevel ?: 0),
                color = MaterialTheme.colorScheme.primary,
                style = MaterialTheme.typography.bodyLarge
            )
            Spacer(modifier = Modifier.padding(2.dp))

            note.InsulinAmount?.let {
                Text(text = stringResource(R.string.insulin_amount, it))
                Spacer(modifier = Modifier.padding(2.dp))
            }
            note.WW?.let {
                Text(text = stringResource(R.string.ww, it))
                Spacer(modifier = Modifier.padding(2.dp))
            }
            note.WBT?.let {
                Text(text = stringResource(R.string.wbt, it))
                Spacer(modifier = Modifier.padding(2.dp))
            }
            note.mealId?.let {
                Text(text = stringResource(R.string.meal_added))
                Spacer(modifier = Modifier.padding(2.dp))
            }
            note.activityId?.let {
                Text(text = stringResource(R.string.activity_added))
                Spacer(modifier = Modifier.padding(2.dp))
            }

            Box(
                modifier = Modifier.fillMaxWidth(),
                contentAlignment = Alignment.CenterEnd
            ) {
                if (!isDoctor) {
                    IconButton(onClick = onDeleteClick) {
                        Icon(Icons.Filled.Delete, contentDescription = stringResource(R.string.delete))
                    }
                }
            }
        }
    }
}