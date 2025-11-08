# Example Input / Output

### 🧮 Sample Input:

- State: FL  
- Min population: 0
- Max population: 5000000
- Number of cities: 250 
- Neighbors per city: 6
- Algorithm used: A*
- Start city: Miami  
- End city: Tampa


---

### 📊 Sample Output:

Shortest path: Miami -> Orlando -> Tampa -> END
Shortest-path distance using Dijkstra algorithm: 384 km
Shortest-path distance using A* algorithm: 376 km
Straight-line distance: 347 km
Dijkstra runtime: 12 ms
A* runtime: 9 ms
Memory usage: 24,500 KB


---

### 🖼️ Visualization Example:
The output graph will be visualized in BRIDGES, with:
- **Red nodes** → cities on the shortest path  
- **Blue nodes** → all other cities  
- **Edges** → represent distances between neighboring cities

