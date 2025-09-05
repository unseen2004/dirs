# LEGO Builder 3D

A 3D LEGO building game engine that allows users to build with virtual LEGO bricks in a realistic 3D environment.

## Features

- **3D Graphics Engine**: Modern OpenGL-based renderer with proper lighting and materials
- **LEGO Brick System**: Various brick types (2x1, 2x2, 2x4, etc.) with authentic connection system
- **Intuitive Building**: Snap-to-grid placement with visual feedback
- **Interactive Camera**: Free-look and orbit camera modes for easy navigation
- **User Interface**: Clean, minimal UI for brick selection and building controls
- **Save/Load**: Persistent storage of created models

## Building

### Requirements
- C++17 compatible compiler
- CMake 3.16+
- OpenGL 3.3+ support
- GLFW3
- GLM (included)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
./bin/LegoBuilder3D
```

## Controls

- **Mouse**: Look around (right-click drag) / Place bricks (left-click)
- **WASD**: Camera movement
- **Scroll**: Zoom in/out
- **1-9**: Select different brick types
- **R**: Rotate brick before placing
- **Ctrl+Z**: Undo
- **Ctrl+S**: Save model
- **Ctrl+O**: Load model

## Architecture

The engine is built with modularity in mind:

- **Engine Core**: Renderer, Scene Manager, Input Handler
- **LEGO System**: Brick Models, Connection Logic, Physics
- **UI System**: Brick Palette, Building Interface
- **Asset Manager**: Model Loading, Texture Management

## License

MIT License - See LICENSE file for details