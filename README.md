# Multi-Project Repository 📚

<p align="center">
  <img src="https://img.shields.io/badge/C++-20-blue?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/Python-3.8+-blue?style=for-the-badge&logo=python&logoColor=white" alt="Python" />
  <img src="https://img.shields.io/badge/SDL2-2.0-blue?style=for-the-badge" alt="SDL2" />
</p>

---

## Table of Contents
- [Overview](#overview)
- [Projects](#projects)
  - [15puzzle 🎲](#15puzzle)
  - [AI 🤖](#ai)
    - [K-means Image Compression](#k-means-image-compression)
    - [VisualizeCNN](#visualizecnn)
  - [Crypto 🔐](#crypto)
  - [ParallelProblems ⚡](#parallelproblems)
    - [Customer Service (C++)](#customer-service-c)
    - [GameOfLife (SDL2 & THREAD)](#gameoflife-sdl2--thread)
    - [ParalelTransform (C++)](#paraleltransform-c)
    - [ThreadQS (C++)](#threadqs-c)
- [How to Run / Build](#how-to-run--build)
- [Contributing](#contributing)
- [License](#license)

---

## Overview
This repository hosts a collection of projects spanning various domains, including puzzle games, artificial intelligence, cryptography, and parallel programming. Each project is contained in its own folder with source code, configuration files, and assets as needed.

---

## Projects

### 15puzzle 🎲
- **Language:** C++
- **Description:** A classic 15-puzzle implementation.
- **Location:** `15puzzle/`
- **Usage:** Compile `main.cpp` using your preferred C++ compiler.

### AI 🤖

#### K-means Image Compression
- **Language:** Python (Jupyter Notebook)
- **Description:** Implements k-means clustering for image compression.
- **Location:** `AI/K-means_img_compression/`
- **Usage:** Open `k_mean.ipynb` in Jupyter Notebook or VS Code with Python extension.  
  - **Data:** The folder contains sample data (e.g., `data/picture.png`).

#### VisualizeCNN
- **Language:** Python (Jupyter Notebook)
- **Description:** Contains a notebook for visualizing Convolutional Neural Networks.
- **Location:** `AI/VisualizeCNN/`
- **Usage:** Open `main.ipynb` with Jupyter Notebook.  
  - **Requirements:** Install dependencies from `requirements.txt`.

### Crypto 🔐
- **Language:** Python (Jupyter Notebook)
- **Description:** Notebook for cracking ciphertexts and exploring cryptographic techniques.
- **Location:** `Crypto/CrackingCipherText.ipynb`
- **Usage:** Open the notebook in Jupyter to run and analyze the code.

### ParallelProblems ⚡

#### Customer Service (C++)
- **Language:** C++
- **Description:** A C++ project simulating a customer service scenario.
- **Location:** `ParallelProblems/costumerService.cpp`
- **Usage:** Compile using your preferred C++ compiler.

#### GameOfLife (SDL2 & THREAD)
- **Language:** C++ (with SDL2 and threading)
- **Description:** Implementation of Conway’s Game of Life using SDL2 for graphics and threading for performance.
- **Location:** `ParallelProblems/GameOfLife(SDL2,THREAD)/`
- **Usage:**  
  1. Navigate to the folder and use the provided `CMakeLists.txt`:
     ```bash
     mkdir build && cd build
     cmake ..
     cmake --build . --config Release
     ```
  2. Run the resulting executable (ensure SDL2 is installed on your system).

#### ParalelTransform (C++)
- **Language:** C++
- **Description:** Demonstrates parallel processing via transformation algorithms.
- **Location:** `ParallelProblems/ParalelTransform.cpp`
- **Usage:** Compile the file using your C++ compiler with appropriate flags for parallel processing (e.g., OpenMP if used).

#### ThreadQS (C++)
- **Language:** C++
- **Description:** An implementation of a multi-threaded QuickSort.
- **Location:** `ParallelProblems/ThreadQS.cpp`
- **Usage:** Compile and run the code with a compiler that supports C++11 (or later) and threading.

---

## How to Run / Build

### For C++ Projects
1. **Compile via Command Line (e.g., for 15puzzle):**
   ```bash
   g++ -std=c++20 -O2 15puzzle/main.cpp -o 15puzzle.exe
   ./15puzzle.exe
   ```

2. **Using CMake (recommended for multi-file projects like GameOfLife):**
   ```bash
   cd ParallelProblems/GameOfLife\(SDL2,THREAD\)  # Adjust path as needed
   mkdir build && cd build
   cmake ..
   cmake --build . --config Release
   ./MyExecutable.exe   # Name depends on your CMakeLists.txt
   ```

### For Python / Jupyter Notebooks
1. **Install Dependencies:**
   Navigate to the relevant project folder (e.g., AI/VisualizeCNN/) and run:
   ```bash
   pip install -r requirements.txt
   ```

2. **Launch Jupyter Notebook:**
   ```bash
   jupyter notebook
   ```
   Then, open the corresponding `.ipynb` file (e.g., `main.ipynb` or `k_mean.ipynb`).


---

## License
This project is licensed under the MIT License.
