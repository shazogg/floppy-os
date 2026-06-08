
# Floppy OS

## Overview

Floppy OS is a lightweight, modular operating system designed for embedded devices, specifically targeting the M5Stick-C platform (ESP32-based). It features a custom display driver, file management, GPIO control, and hardware abstraction, all written in modern C++ and organized for extensibility.

## Features

- **Custom Display Driver:** Supports ST7789 V2 LCD with canvas drawing, font rendering, and image display.
- **File System Management:** Secure file operations using SPIFFS and AES encryption.
- **GPIO Abstraction:** Easy digital and analog pin control.
- **Hardware Utilities:** PSRAM checks, memory management, and serial communication.
- **Command Interface:** Serial command parsing for file operations and system control.
- **Splash Screen & UI:** Custom splash screen, icon rendering, and font tools.
- **Modular Libraries:** Each hardware feature is encapsulated in its own library for clarity and reuse.
- **Python Tools:** Scripts for font/image conversion to C++ arrays for display.

## Project Structure

```
floppy-os-main/
├── include/           # Project-wide headers
├── lib/               # Modular libraries (Display, Files, GPIO, Hardware, SPI)
│   ├── FloppyDisplay/ # Display driver, fonts, tools
│   ├── FloppyFiles/   # File system and encryption
│   ├── FloppyGPIO/    # GPIO abstraction
│   ├── FloppyHardware/# Hardware utilities
│   ├── FloppySPI/     # SPI communication
├── src/               # Main application and OS logic
│   ├── main.cpp       # Entry point
│   └── app/           # OS core, commands, splash, utils, assets
├── test/              # Test files
├── partitions.csv      # Partition table for ESP32
├── platformio.ini      # PlatformIO config
├── README.md           # Project documentation
```

## Installation

1. **Clone the repository:**
	```bash
	git clone https://github.com/shazogg/floppy-os-main.git
	cd floppy-os-main
	```

2. **Set up PlatformIO:**
	- Install [PlatformIO](https://platformio.org/) (VS Code recommended).
	- Activate the PlatformIO environment:
	  - **Unix:**
		 ```bash
		 source ~/.platformio/penv/bin/activate
		 ```
	  - **Windows:**
		 ```powershell
		 source ~/.platformio/penv/bin/Activate.ps1
		 ```

3. **Install Python dependencies:**
	```bash
	pip install python-dotenv pillow numpy
	```

4. **Build and upload:**
	- Connect your M5Stick-C device.
	- Use PlatformIO to build and upload the firmware:
	  ```bash
	  pio run --target upload
	  ```

## Usage

- On boot, Floppy OS displays a splash screen and version info.
- Use the serial interface to send commands (e.g., `read_file`, `write_file`).
- File operations are encrypted and managed via SPIFFS.
- Display driver supports custom fonts and images (see Python tools for conversion).

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Credits

- Developed by Shawn (shazogg)
- Uses PlatformIO, M5Stick-C, ESP32, and open-source libraries.

---
For questions or contributions, please open an issue or pull request.
