# FDF (Fil de Fer)

![Demo](demo.gif)

FDF is a 3D wireframe mesh viewer that renders height maps in different projections. This project demonstrates the use of the MLX (MiniLibX) graphics library to create an interactive 3D visualization tool.

## Features

- Multiple view modes:
  - Isometric view
  - Perspective view
  - Top view
- Interactive controls:
  - Zoom in/out (Page Up/Down)
  - Move up/down (W/S keys)
  - Move left/right (A/D keys)
  - Rotate view (Arrow keys)
  - Adjust line thickness (+/- keys)
  - Toggle color mode (Space bar)
  - Reset view (R key)
- Support for height maps with color information
- Automatic scaling to fit window
- Smooth line rendering with Bresenham's algorithm
- Memory leak protection and proper cleanup

## Requirements

- macOS (MLX library is macOS-specific)
- GCC compiler
- Make

## Installation

1. Clone the repository:
```bash
git clone [https://github.com/adshz/fdf.git]
cd fdf
```

2. Compile the project:
```bash
make
```

## Usage

Run the program with a map file as an argument:
```bash
./fdf [map_file]
```

### Map File Format

The map file should contain a grid of numbers representing height values. Each number can optionally be followed by a color in hexadecimal format (e.g., "0,0xFFFFFF").

Example map file:
```
0 1 2 3
1 2 3 4
2 3 4 5
```

Or with colors:
```
0,0xFFFFFF 1,0xFF0000 2,0x00FF00
1,0x0000FF 2,0xFFFF00 3,0xFF00FF
2,0x00FFFF 3,0xFFFFFF 4,0x000000
```

## Controls

- **ESC**: Exit program
- **Page Up/Down**: Zoom in/out
- **W/S**: Move up/down
- **A/D**: Move left/right
- **Arrow Keys**: Rotate view
- **+/-**: Increase/decrease line thickness
- **I**: Switch to Isometric view
- **P**: Switch to Perspective view
- **T**: Switch to Top view
- **Space**: Toggle color mode
- **R**: Reset view

## Project Structure

- `src/main.c`: Main program entry point
- `src/modules/ft_init/`: Initialization functions
- `src/modules/ft_transform/`: View transformation functions
- `src/modules/ft_parse/`: Map file parsing functions
- `src/modules/ft_render/`: Drawing functions
- `src/modules/ft_interact/`: User interaction functions
- `src/modules/ft_utils/`: Utility functions

## Error Handling

The program includes comprehensive error handling for:
- Invalid map files
- Memory allocation failures
- Window creation failures
- Invalid input parameters

## Memory Management

The program implements proper memory management with:
- Cleanup of MLX resources
- Freeing of allocated memory
- Protection against memory leaks

## License

This project is part of the 42 London and follows its guidelines and restrictions.

## Author

Created by szhong (42 London)