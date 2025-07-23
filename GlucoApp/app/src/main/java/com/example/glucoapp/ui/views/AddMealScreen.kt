package com.example.glucoapp.ui.views

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Check
import androidx.compose.material.icons.filled.Close
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavController
import com.example.glucoapp.data.db.models.Meal
import com.example.glucoapp.data.db.models.Ingredient
import com.example.glucoapp.navigation.Screen
import com.example.glucoapp.ui.viewmodels.MealsViewModel
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.filled.Check
import androidx.compose.material.icons.filled.Close
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import com.example.glucoapp.R
import androidx.compose.ui.res.stringResource

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun AddMealScreen(
    navController: NavController,
    viewModel: MealsViewModel = hiltViewModel()
) {
    var mealName by remember { mutableStateOf("") }
    var protein by remember { mutableStateOf("") }
    var carbs by remember { mutableStateOf("") }
    var fat by remember { mutableStateOf("") }
    var kcal by remember { mutableStateOf("") }
    val userId by viewModel.userId.collectAsState(initial = null)
    var showIngredientDialog by remember { mutableStateOf(false) }
    val ingredients by viewModel.ingredients.collectAsState(initial = emptyList())

    LaunchedEffect(Unit) {
        viewModel.loadIngredients()
    }

    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text(stringResource(R.string.add_meal_title)) },
                navigationIcon = {
                    IconButton(onClick = { navController.navigate(Screen.Main.route) }) {
                        Icon(Icons.Filled.Close, stringResource(R.string.cancel))
                    }
                },
                actions = {
                    IconButton(onClick = {
                        userId?.let {
                            val newMeal = Meal(
                                userId = it,
                                foodName = mealName,
                                protein = protein.toDoubleOrNull(),
                                carbs = carbs.toDoubleOrNull(),
                                fat = fat.toDoubleOrNull(),
                                kcal = kcal.toDoubleOrNull()
                            )
                            viewModel.insertMeal(newMeal)
                            navController.navigate(Screen.Main.route)
                        }
                    }) {
                        Icon(Icons.Filled.Check, stringResource(R.string.save))
                    }
                }
            )
        }
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            OutlinedTextField(
                value = mealName,
                onValueChange = { mealName = it },
                label = { Text(stringResource(R.string.meal_name)) },
                modifier = Modifier.fillMaxWidth()
            )
            Spacer(modifier = Modifier.height(8.dp))

            OutlinedTextField(
                value = protein,
                onValueChange = { protein = it },
                label = { Text(stringResource(R.string.protein)) },
                modifier = Modifier.fillMaxWidth()
            )
            Spacer(modifier = Modifier.height(8.dp))

            OutlinedTextField(
                value = carbs,
                onValueChange = { carbs = it },
                label = { Text(stringResource(R.string.carbs)) },
                modifier = Modifier.fillMaxWidth()
            )
            Spacer(modifier = Modifier.height(8.dp))

            OutlinedTextField(
                value = fat,
                onValueChange = { fat = it },
                label = { Text(stringResource(R.string.fat)) },
                modifier = Modifier.fillMaxWidth()
            )
            Spacer(modifier = Modifier.height(8.dp))

            OutlinedTextField(
                value = kcal,
                onValueChange = { kcal = it },
                label = { Text(stringResource(R.string.kcal)) },
                modifier = Modifier.fillMaxWidth()
            )
            Spacer(modifier = Modifier.height(8.dp))

            Button(
                onClick = { showIngredientDialog = true },
                modifier = Modifier.fillMaxWidth()
            ) {
                Text(stringResource(R.string.add_ingredients))
            }
        }
    }

    if (showIngredientDialog) {
        IngredientSelectionDialog(
            ingredients = ingredients,
            onDismiss = { showIngredientDialog = false },
            onIngredientsSelected = { selectedIngredients ->
                val totalProtein = selectedIngredients.sumOf { it.protein.toDouble() }
                val totalCarbs = selectedIngredients.sumOf { it.carbs.toDouble() }
                val totalFat = selectedIngredients.sumOf { it.fat.toDouble() }
                val totalKcal = selectedIngredients.sumOf { it.kcal.toDouble() }

                protein = totalProtein.toString()
                carbs = totalCarbs.toString()
                fat = totalFat.toString()
                kcal = totalKcal.toString()

                showIngredientDialog = false
            }
        )
    }
}

@Composable
fun IngredientSelectionDialog(
    ingredients: List<Ingredient>,
    onDismiss: () -> Unit,
    onIngredientsSelected: (List<Ingredient>) -> Unit
) {
    var selectedIngredients by remember { mutableStateOf(emptyList<Ingredient>()) }

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text(stringResource(R.string.select_ingredients)) },
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
                        Checkbox(
                            checked = selectedIngredients.contains(ingredient),
                            onCheckedChange = {
                                if (it) {
                                    selectedIngredients = selectedIngredients + ingredient
                                } else {
                                    selectedIngredients = selectedIngredients - ingredient
                                }
                            }
                        )
                    }
                }
            }
        },
        confirmButton = {
            Button(onClick = { onIngredientsSelected(selectedIngredients) }) {
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