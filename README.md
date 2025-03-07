# Integrated Terminal Desktop (ITD)

A Windows-based terminal designed to replace the full desktop, combining terminal and desktop features for a modern, efficient user experience.

## Features

- **Widget Support**: Add clocks, system monitors, calendars, and personalize their look and layout
- **File Explorer**: Integrated Yazi for seamless file management within the terminal
- **Vim-Like Navigation**: Modal editing and keyboard-centric controls, reducing mouse dependency
- **Tiling and Transparency**: Organizes windows in a grid and supports see-through effects
- **Custom Taskbar**: Manages apps and notifications, adapting to multiple monitors
- **Lua Scripting**: Customize behavior and key bindings using Lua scripts
- **Fast Search**: Advanced indexing for quick access to files and applications

## Technical Implementation

- **Core**: Written in C++ for high performance with low resource usage
- **GUI**: Built with wxWidgets (Cross-Platform C++ GUI Library)
- **Terminal**: Uses TerminalWx (WxWidgets Terminal Widget)
- **Scripting**: Embedded Lua interpreter for customization and extensions
- **Indexing**: Custom file indexing service for fast search capabilities

## Requirements

- Windows 10/11 (64-bit)
- 2GB RAM minimum (4GB recommended)
- 100MB disk space
- Modern CPU (Intel Core i3 or equivalent)

## Building from Source

### Prerequisites

- Visual Studio 2019 or newer with C++ support
- CMake 3.15 or newer
- wxWidgets 3.2 or newer
- Lua 5.4

### Build Steps

1. Clone the repository:

   ```batch
   git clone <https://github.com/yourusername/ITD.git>
   cd ITD
   ```

2. Create a build directory:

   ```batch
   mkdir build
   cd build
   ```

3. Generate build files:

   ```batch
   cmake ..
   ```

4. Build the project:

   ```batch
   cmake --build . --config Release
   ```

5. Run the application:

   ```batch
   ./bin/ITD
   ```

## Project Structure

- `src/` - Source files
- `include/` - Header files
- `lib/` - Third-party libraries
- `res/` - Resources (icons, themes, etc.)
- `docs/` - Documentation
- `tests/` - Test files

## License

See the [LICENSE](LICENSE) file for details.
