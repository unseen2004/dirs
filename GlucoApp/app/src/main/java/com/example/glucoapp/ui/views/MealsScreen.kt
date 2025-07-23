package com.example.glucoapp.ui.views

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material3.*
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
import com.example.glucoapp.navigation.Screen
import com.example.glucoapp.ui.viewmodels.MealsViewModel
import com.example.glucoapp.data.db.models.Meal
import com.example.glucoapp.R
import androidx.compose.ui.res.stringResource
@Composable
fun MealsScreen(
    navController: NavController,
    viewModel: MealsViewModel = hiltViewModel(),
    isDoctor: Boolean = false
) {
    val userId by viewModel.userId.collectAsState(initial = null)
    val meals by viewModel.meals.collectAsState()

    LaunchedEffect(userId) {
        userId?.let {
            viewModel.loadMealsByUserId(it)
        }
    }

    Box(modifier = Modifier.fillMaxSize()) {
        LazyColumn(modifier = Modifier.fillMaxSize()) {
            items(meals) { meal ->
                MealItem(meal = meal, onDeleteClick = {
                    if (!isDoctor) {
                        viewModel.deleteMeal(meal)
                    }
                })
            }
        }
        if (!isDoctor) {
            FloatingActionButton(
                onClick = { navController.navigate(Screen.AddMeal.route) },
                modifier = Modifier
                    .align(Alignment.BottomEnd)
                    .padding(16.dp)
            ) {
                Icon(Icons.Filled.Add, stringResource(R.string.add_meal))
            }
        }
    }
}

@Composable
fun MealItem(meal: Meal, onDeleteClick: () -> Unit) {
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
                text = meal.foodName ?: stringResource(R.string.unknown_meal),
                fontWeight = FontWeight.Bold,
                style = MaterialTheme.typography.headlineSmall
            )
            Spacer(modifier = Modifier.padding(4.dp))

            meal.protein?.let {
                Text(text = stringResource(R.string.protein, it))
                Spacer(modifier = Modifier.padding(2.dp))
            }
            meal.carbs?.let {
                Text(text = stringResource(R.string.carbs, it))
                Spacer(modifier = Modifier.padding(2.dp))
            }
            meal.fat?.let {
                Text(text = stringResource(R.string.fat, it))
                Spacer(modifier = Modifier.padding(2.dp))
            }
            meal.kcal?.let {
                Text(text = stringResource(R.string.kcal, it))
            }

            Box(
                modifier = Modifier.fillMaxWidth(),
                contentAlignment = Alignment.CenterEnd
            ) {
                IconButton(onClick = onDeleteClick) {
                    Icon(Icons.Filled.Delete, contentDescription = stringResource(R.string.delete))
                }
            }
        }
    }
}