OK im gonna explain ...

## UF DSA Project 2: US City Graphs

1. This is our Proejct 2 for Data Strcutre and Algorithms 


## Features
- User selects a state, population range, number of cities, and neighbors per city.
- Graph visualization using [Bridges C++ API](http://bridgesuncc.github.io/).
- Calculates:
  - Shortest path distance (Dijkstra)
  - Shortest path distance (A*)

- Displays runtime and memory usage of the algorithm.
  
## Usage

1. In order to compile the project, you would need to setup and run BRIDGES Programs.
2. The instructions for C++/Java/Python is here: [BRIDGES:Getting Started](https://bridgesuncc.github.io/bridges_setup.html)
3. Since this project is programmed in C++ by VS Code, you should follow the tutorial here: [BRIDGES(C++) With VS Code:Getting Started](https://bridgesuncc.github.io/bridges_setup_cxx_vscode.html)
4. Several things the user should be careful while going through the tutorial:
   - Make sure you have the WSL Extension installed in VS Code.
   - In your task.json file, make sure to change the compiler from clang++ to g++.
   - Follow the tutorial steps carefully to avoid path or linking issues (remember
   - "-I../bridges-cxx-LATEST-x86_64-linux-gnu/bridges-cxx-3.5.0-x86_64-linux-gnu/include" should be the relative path of include folder
   - "-L../bridges-cxx-LATEST-x86_64-linux-gnu/bridges-cxx-3.5.0-x86_64-linux-gnu/lib" should be the relative path of library folder
   - Remember you are editing the program in Ubuntu linux system on your Windows machine, not your local path.

