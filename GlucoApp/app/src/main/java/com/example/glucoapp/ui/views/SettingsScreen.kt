package com.example.glucoapp.ui.views

import android.util.Log
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavController
import com.example.glucoapp.data.db.models.Activity
import com.example.glucoapp.data.db.models.InsulinType
import com.example.glucoapp.data.db.models.Ingredient
import com.example.glucoapp.ui.viewmodels.SettingsViewModel
import com.example.glucoapp.R
import androidx.compose.ui.res.stringResource
import kotlinx.coroutines.launch

@Composable
fun SettingsScreen(
    navController: NavController,
    viewModel: SettingsViewModel = hiltViewModel(),
    isDoctor: Boolean = false,
    language: String
) {
    var showAddInsulinDialog by remember { mutableStateOf(false) }
    var showAddIngredientDialog by remember { mutableStateOf(false) }
    var showDeleteInsulinDialog by remember { mutableStateOf(false) }
    var showDeleteIngredientDialog by remember { mutableStateOf(false) }
    var showAddActivityDialog by remember { mutableStateOf(false) }
    var showDeleteActivityDialog by remember { mutableStateOf(false) }
    var showDeleteUserDialog by remember { mutableStateOf(false) }
    val coroutineScope = rememberCoroutineScope()
    val currentLanguage by viewModel.currentLanguage.collectAsState()


    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        if (!isDoctor) {
            Button(onClick = { showAddInsulinDialog = true }) {
                Text(stringResource(R.string.add_insulin))
            }
            Spacer(modifier = Modifier.height(8.dp))
            Button(onClick = { showAddIngredientDialog = true }) {
                Text(stringResource(R.string.add_ingredient))
            }
            Spacer(modifier = Modifier.height(8.dp))
            Button(onClick = { showDeleteInsulinDialog = true }) {
                Text(stringResource(R.string.delete_insulin))
            }
            Spacer(modifier = Modifier.height(8.dp))
            Button(onClick = { showDeleteIngredientDialog = true }) {
                Text(stringResource(R.string.delete_ingredient))
            }
            Spacer(modifier = Modifier.height(8.dp))
            Button(onClick = {
                coroutineScope.launch {
                    val newLanguage = if (currentLanguage == "en") "ja" else "en"
                    viewModel.changeLanguage(newLanguage)
                    Log.d("SettingsScreen", "Button clicked. Current language: $currentLanguage, New language: $newLanguage")
                }
            }) {
                Text(stringResource(R.string.change_language))
            }
            Spacer(modifier = Modifier.height(8.dp))
            Button(onClick = { showDeleteUserDialog = true }) {
                Text(stringResource(R.string.delete_user))
            }
            Spacer(modifier = Modifier.height(8.dp))
            Button(onClick = { showAddActivityDialog = true }) {
                Text(stringResource(R.string.add_activity))
            }
            Spacer(modifier = Modifier.height(8.dp))
            Button(onClick = { showDeleteActivityDialog = true }) {
                Text(stringResource(R.string.delete_activity))
            }
            Spacer(modifier = Modifier.height(8.dp))
        }
        Button(onClick = {
            navController.navigate("login") {
                popUpTo("settings") { inclusive = true }
            }
        }) {
            Text(stringResource(R.string.logout))
        }
    }

    if (showAddInsulinDialog) {
        AddInsulinDialog(onDismiss = { showAddInsulinDialog = false }, viewModel = viewModel)
    }

    if (showAddIngredientDialog) {
        AddIngredientDialog(onDismiss = { showAddIngredientDialog = false }, viewModel = viewModel)
    }

    if (showDeleteInsulinDialog) {
        DeleteInsulinDialog(onDismiss = { showDeleteInsulinDialog = false }, viewModel = viewModel)
    }

    if (showDeleteIngredientDialog) {
        DeleteIngredientDialog(onDismiss = { showDeleteIngredientDialog = false }, viewModel = viewModel)
    }
    if (showDeleteUserDialog) {
        DeleteUserDialog(
            onDismiss = { showDeleteUserDialog = false },
            onConfirm = {
                viewModel.deleteUserAndLogout()
                navController.navigate("login") {
                    popUpTo("settings") { inclusive = true }
                }
            }
        )
    }
    if (showAddActivityDialog) {
        AddActivityDialog(onDismiss = { showAddActivityDialog = false }, viewModel = viewModel)
    }

    if (showDeleteActivityDialog) {
        DeleteActivityDialog(onDismiss = { showDeleteActivityDialog = false }, viewModel = viewModel)
    }
}

@Composable
fun DeleteUserDialog(onDismiss: () -> Unit, onConfirm: () -> Unit) {
    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.delete_user_title)) },
        text = { Text(stringResource(R.string.delete_user_confirmation)) },
        confirmButton = {
            Button(onClick = onConfirm) {
                Text(stringResource(R.string.delete))
            }
        },
        dismissButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.cancel))
            }
        }
    )
}

@Composable
fun AddActivityDialog(onDismiss: () -> Unit, viewModel: SettingsViewModel) {
    var activityType by remember { mutableStateOf("") }
    var duration by remember { mutableStateOf("") }
    var notes by remember { mutableStateOf("") }
    val userId by viewModel.userId.collectAsState(initial = null)

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.add_activity)) },
        text = {
            Column {
                OutlinedTextField(
                    value = activityType,
                    onValueChange = { activityType = it },
                    label = { Text(stringResource(R.string.activity_type)) },
                    modifier = Modifier.fillMaxWidth()

                )
                OutlinedTextField(
                    value = duration,
                    onValueChange = { duration = it },
                    label = { Text(stringResource(R.string.duration)) },
                    keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
                    modifier = Modifier.fillMaxWidth()
                )
                OutlinedTextField(
                    value = notes,
                    onValueChange = { notes = it },
                    label = { Text(stringResource(R.string.notes)) },
                    modifier = Modifier.fillMaxWidth()
                )
            }
        },
        confirmButton = {
            Button(onClick = {
                userId?.let {
                    viewModel.addActivity(Activity(
                        activityType = activityType,
                        duration = duration.toInt(),
                        notes = notes,
                        userId = it, // Use the actual user ID
                        timestamp = System.currentTimeMillis()
                    ))
                    onDismiss()
                }
            }) {
                Text(stringResource(R.string.add))
            }
        },
        dismissButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.cancel))
            }
        }
    )
}

@Composable
fun DeleteActivityDialog(onDismiss: () -> Unit, viewModel: SettingsViewModel) {
    val activities by viewModel.activities.collectAsState(initial = emptyList())

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.delete_activity)) },
        text = {
            LazyColumn {
                items(activities) { activity ->
                    Row(
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(8.dp),
                        horizontalArrangement = Arrangement.SpaceBetween
                    ) {
                        Text(activity.activityType)
                        Button(onClick = {
                            viewModel.deleteActivity(activity)
                            onDismiss()
                        }) {
                            Text(stringResource(R.string.delete))
                        }
                    }
                }
            }
        },
        confirmButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.close))
            }
        }
    )
}

@Composable
fun ChangePasswordDialog(
    onDismiss: () -> Unit,
    onSave: (String) -> Unit
) {
    var newPassword by remember { mutableStateOf("") }

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.change_password)) },
        text = {
            Column {
                OutlinedTextField(
                    value = newPassword,
                    onValueChange = { newPassword = it },
                    label = { Text(stringResource(R.string.new_password)) },
                    modifier = Modifier.fillMaxWidth()
                )
            }
        },
        confirmButton = {
            Button(onClick = { onSave(newPassword) }) {
                Text(stringResource(R.string.save))
            }
        },
        dismissButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.cancel))
            }
        }
    )
}

@Composable
fun ChangeDoctorPasswordDialog(
    onDismiss: () -> Unit,
    onSave: (String) -> Unit
) {
    var newPassword by remember { mutableStateOf("") }

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.change_doctor_password)) },
        text = {
            Column {
                OutlinedTextField(
                    value = newPassword,
                    onValueChange = { newPassword = it },
                    label = { Text(stringResource(R.string.new_doctor_password)) },
                    modifier = Modifier.fillMaxWidth()
                )
            }
        },
        confirmButton = {
            Button(onClick = { onSave(newPassword) }) {
                Text(stringResource(R.string.save))
            }
        },
        dismissButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.cancel))
            }
        }
    )
}

@Composable
fun AddInsulinDialog(onDismiss: () -> Unit, viewModel: SettingsViewModel) {
    var insulinName by remember { mutableStateOf("") }
    var typeName by remember { mutableStateOf("") }
    val userId by viewModel.userId.collectAsState(initial = null)

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.add_insulin)) },
        text = {
            Column {
                OutlinedTextField(
                    value = insulinName,
                    onValueChange = { insulinName = it },
                    label = { Text(stringResource(R.string.insulin_name)) },
                    modifier = Modifier.fillMaxWidth()
                )
                Spacer(modifier = Modifier.height(8.dp))
                OutlinedTextField(
                    value = typeName,
                    onValueChange = { typeName = it },
                    label = { Text(stringResource(R.string.type_name)) },
                    modifier = Modifier.fillMaxWidth()
                )
            }
        },
        confirmButton = {
            Button(
                onClick = {
                    userId?.let {
                        viewModel.addInsulinType(InsulinType(insulinName = insulinName, typeName = typeName, userId = it))
                        onDismiss()
                    }
                }
            ) {
                Text(stringResource(R.string.add))
            }
        },
        dismissButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.cancel))
            }
        }
    )
}

@Composable
fun AddIngredientDialog(onDismiss: () -> Unit, viewModel: SettingsViewModel) {
    var ingredientName by remember { mutableStateOf("") }
    var protein by remember { mutableStateOf("") }
    var carbs by remember { mutableStateOf("") }
    var fat by remember { mutableStateOf("") }
    var kcal by remember { mutableStateOf("") }
    val userId by viewModel.userId.collectAsState(initial = null)

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.add_ingredient)) },
        text = {
            Column {
                OutlinedTextField(
                    value = ingredientName,
                    onValueChange = { ingredientName = it },
                    label = { Text(stringResource(R.string.ingredient_name)) },
                    modifier = Modifier.fillMaxWidth()
                )
                OutlinedTextField(
                    value = protein,
                    onValueChange = { protein = it },
                    label = { Text(stringResource(R.string.protein)) },
                    modifier = Modifier.fillMaxWidth()
                )
                OutlinedTextField(
                    value = carbs,
                    onValueChange = { carbs = it },
                    label = { Text(stringResource(R.string.carbs)) },
                    modifier = Modifier.fillMaxWidth()
                )
                OutlinedTextField(
                    value = fat,
                    onValueChange = { fat = it },
                    label = { Text(stringResource(R.string.fat)) },
                    modifier = Modifier.fillMaxWidth()
                )
                OutlinedTextField(
                    value = kcal,
                    onValueChange = { kcal = it },
                    label = { Text(stringResource(R.string.kcal)) },
                    modifier = Modifier.fillMaxWidth()
                )
            }
        },
        confirmButton = {
            Button(onClick = {
                userId?.let {
                    viewModel.addIngredient(Ingredient(
                        foodName = ingredientName,
                        protein = protein.toFloat(),
                        carbs = carbs.toFloat(),
                        fat = fat.toFloat(),
                        kcal = kcal.toFloat(),
                        userId = it
                    ))
                }
                onDismiss()
            }) {
                Text(stringResource(R.string.add))
            }
        },
        dismissButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.cancel))
            }
        }
    )
}

@Composable
fun DeleteInsulinDialog(onDismiss: () -> Unit, viewModel: SettingsViewModel) {
    val insulinTypes by viewModel.insulinTypes.collectAsState(initial = emptyList())

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.delete_insulin)) },
        text = {
            LazyColumn {
                items(insulinTypes) { insulinType ->
                    Row(
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(8.dp),
                        horizontalArrangement = Arrangement.SpaceBetween
                    ) {
                        Text(insulinType.insulinName)
                        Button(onClick = {
                            viewModel.deleteInsulinType(insulinType)
                            onDismiss()
                        }) {
                            Text(stringResource(R.string.delete))
                        }
                    }
                }
            }
        },
        confirmButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.close))
            }
        }
    )
}

@Composable
fun DeleteIngredientDialog(onDismiss: () -> Unit, viewModel: SettingsViewModel) {
    val ingredients by viewModel.ingredients.collectAsState(initial = emptyList())

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.delete_ingredient)) },
        text = {
            LazyColumn {
                items(ingredients) { ingredient ->
                    Row(
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(8.dp),
                        horizontalArrangement = Arrangement.SpaceBetween
                    ) {
                        Text(ingredient.foodName)
                        Button(onClick = {
                            viewModel.deleteIngredient(ingredient)
                            onDismiss()
                        }) {
                            Text(stringResource(R.string.delete))
                        }
                    }
                }
            }
        },
        confirmButton = {
            Button(onClick = onDismiss) {
                Text(stringResource(R.string.close))
            }
        }
    )
}