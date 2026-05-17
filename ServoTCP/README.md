
## How to Build and Run (Windows)
1. Open the x64 Native Tools Command Prompt for VS 2022
2. Navigate to this directory.
3. Open VS Code by typing `code .`.
4. Open the integrated terminal and type to build:
   cl.exe /EHsc main.cpp PicoController.cpp ws2_32.lib /Fe:RobotEngine.exe
5. Type to run: .\RobotEngine.exe
