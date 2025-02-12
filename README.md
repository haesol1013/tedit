# Tedit

A simple CLI text editor in C, leveraging [PDCurses](https://github.com/wmcbrine/PDCurses) for console operations. This project demonstrates how to work with text input, display, and file I/O in a terminal environment.

## Features

- **Insert/Delete Text:** Supports inserting characters, backspacing, and creating new lines.
- **File Operations:** Load an existing file or save your current text to a file.
- **Status Bar:** Displays cursor position, total lines, and shortcut reminders.
- **Keyboard Shortcuts:**
    - **Ctrl+S**: Save the current file.
    - **Ctrl+Q**: Quit the editor.
- **Scrolling:** Keeps the cursor visible by adjusting the top line when you navigate beyond the visible screen area.

## Requirements

- A C compiler (C11 or higher recommended).
- CMake (v3.30 or higher).
- PDCurses library (included in `external/pdcurses/` as a static library named `pdcurses.a`).

## Building

1. **Clone the Repository**
   ```bash
   git clone https://github.com/haesol1013/tedit.git
   cd tedit
   ```
2. **Configure and Build with CMake**
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
   - The resulting binary will be placed in the `bin` directory under the project root.

3. **Verify the Executable**
    - Navigate to the `bin` folder and confirm that an executable named `tedit` (or `tedit.exe` on Windows) was created.

## Usage

```bash
./tedit [filename]
```

- **Optional [filename]:** If provided, Tedit attempts to load the specified file at startup.
- **Edit text:** Use regular characters, arrow keys, and the following shortcuts.
    - **Ctrl+S** to save changes.
    - **Ctrl+Q** to quit the application.
- **Saved file:** If no filename is given, the editor will save your work to `unnamed.txt`.

## Directory Structure

```
tedit/
├── CMakeLists.txt
├── include/
│   ├── editor.h
│   └── file_io.h
├── external/
│   └── pdcurses/
│       ├── curses.h
│       └── pdcurses.a
├── src/
│   ├── main.c
│   ├── editor.c
│   └── file_io.c
└── bin/
```