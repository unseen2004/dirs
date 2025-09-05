# LEGO Builder 3D

A fully-featured 3D LEGO building game engine that allows users to build with virtual LEGO bricks in a realistic 3D environment. Built from scratch using modern C++ and OpenGL.

## Features

### Core Engine
- **3D Graphics Engine**: Modern OpenGL 3.3+ renderer with Phong lighting model
- **Camera System**: Free-look camera with WASD movement and mouse controls
- **Real-time Rendering**: Smooth 60 FPS rendering with proper depth testing
- **Custom Math Library**: Vec3 and Mat4 classes for 3D transformations

### LEGO Building System
- **Multiple Brick Types**: 6 different brick sizes (1x1, 1x2, 2x2, 2x4, 2x6, 2x8)
- **7 Brick Colors**: Red, Blue, Green, Yellow, White, Black, Gray
- **Snap-to-Grid Placement**: Automatic grid alignment for realistic building
- **Collision Detection**: Prevents overlapping brick placement
- **Interactive Preview**: Real-time preview of brick placement

### User Experience
- **Intuitive Controls**: Easy-to-learn keyboard and mouse controls
- **Undo/Redo System**: Full history with up to 50 operations
- **Save/Load Models**: Persistent storage in human-readable format
- **Interactive UI**: Console-based status and instruction display

## Building the Project

### Requirements
- **Compiler**: C++17 compatible (GCC 7+, Clang 5+, MSVC 2017+)
- **Build System**: CMake 3.16+
- **Graphics**: OpenGL 3.3+ support
- **Dependencies**: 
  - GLFW3 (windowing)
  - OpenGL development libraries

### Ubuntu/Debian Installation
```bash
sudo apt update
sudo apt install -y libgl1-mesa-dev libglfw3-dev libglu1-mesa-dev cmake g++
```

### Build Instructions
```bash
git clone <repository-url>
cd lego_builder_3d
mkdir build
cd build
cmake ..
make
./bin/LegoBuilder3D
```

## Controls

### Camera Movement
- **WASD**: Move camera forward/back/left/right
- **Mouse + Right-click**: Look around
- **Mouse Scroll**: Zoom in/out

### Building Controls
- **1-6**: Select brick type (1x1, 1x2, 2x2, 2x4, 2x6, 2x8)
- **C**: Cycle through brick colors
- **Tab**: Cycle through brick types
- **Left-click**: Place brick at cursor position
- **Space**: Toggle placement mode

### System Controls
- **Ctrl+Z**: Undo last action
- **Ctrl+Shift+Z**: Redo action
- **Ctrl+S**: Save model (prompts for filename)
- **Ctrl+O**: Load model (prompts for filename)
- **ESC**: Exit application

## File Format

Models are saved in a simple text format:
```
# LEGO Builder 3D Model File
version 1.0

0 0 0 2x4 red
1.6 0 0 2x2 blue
0 0.96 0 1x1 green
```

Format: `x y z brick_type brick_color`

## Architecture

The engine follows a modular design:

### Core Systems
- **Window**: GLFW-based window management and OpenGL context
- **Renderer**: Mesh rendering, shader management, and OpenGL abstraction
- **Camera**: 3D camera with perspective projection and view transformations
- **Math**: Custom 3D math library for vectors and matrices

### LEGO Systems
- **LegoSystem**: Brick management, placement validation, and rendering
- **ModelIO**: Save/load functionality for persistent models
- **InputHandler**: User input processing and brick selection

### Support Systems
- **UIRenderer**: Console-based user interface and status display
- **Grid**: Reference grid for spatial orientation (planned feature)

## Sample Models

The `resources/` directory contains sample LEGO models:
- `sample_castle.lego`: A small castle structure demonstrating various brick types

## Technical Details

### Rendering Pipeline
1. **Vertex Processing**: Custom Vertex structure with position, normal, and UV coordinates
2. **Shader System**: Vertex and fragment shaders with Phong lighting
3. **Lighting**: Point light with ambient, diffuse, and specular components
4. **Materials**: Per-brick color materials with realistic lighting response

### Performance Optimizations
- **Mesh Instancing**: Shared geometry between brick types
- **Efficient Collision**: Grid-based spatial optimization
- **Memory Management**: RAII and smart pointers throughout

### Cross-Platform Support
- Pure C++17 with standard libraries
- OpenGL for graphics (works on Windows, Linux, macOS)
- GLFW for windowing (cross-platform)
- CMake build system

## Extensibility

The engine is designed for easy extension:

### Adding New Brick Types
1. Add enum value to `BrickType`
2. Implement geometry in `Brick::createBrickMesh()`
3. Update string conversion functions

### Adding New Features
- **Advanced Lighting**: Multiple light sources, shadows
- **Physics**: Gravity simulation, structural analysis
- **Textures**: Realistic LEGO brick textures
- **UI**: ImGui integration for advanced interface
- **Networking**: Multi-player building sessions

## Development Status

### Completed Features ✅
- Core 3D engine with OpenGL rendering
- Full LEGO brick system with 6 types and 7 colors
- Interactive camera and input system
- Undo/redo functionality with history
- Save/load system with text format
- Grid-based placement and collision detection
- Real-time lighting and materials

### Planned Features 🚧
- Grid floor for spatial reference
- Advanced UI with ImGui
- Texture support for realistic materials
- Additional brick types (slopes, curves, special pieces)
- Export to common 3D formats (OBJ, STL)

## Contributing

This project demonstrates modern C++ game engine development principles:
- Object-oriented design with clear separation of concerns
- Memory-safe RAII patterns
- Modular architecture for easy extension
- Cross-platform compatibility

## License

MIT License - Feel free to use this project for learning, modification, or commercial purposes.

---

**LEGO Builder 3D** - Building imagination in three dimensions! 🧱