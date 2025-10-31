# 🧭 Dijkstra’s Algorithm 

## 🧩 What Is Dijkstra’s Algorithm?

**Dijkstra’s Algorithm** is a **shortest path algorithm** used to find the **minimum distance** between a starting node and all other nodes in a **weighted graph** (a graph where each edge has a distance or cost).

It’s commonly used in:
- GPS navigation (finding the fastest route)
- Network routing (finding efficient data paths)
- Map applications (like Google Maps)



## 🌉 Basic Idea

Imagine you’re planning a road trip between cities.  
Each **city** is a **node**, and each **road** is a **connection (edge)** with a **distance**.

Dijkstra’s algorithm finds the **shortest route** from your start city to your destination by:
1. Starting with a distance of `0` for your starting city.
2. Setting all other cities to have a distance of `∞` (unknown for now).
3. Repeatedly visiting the city that’s **closest** to the start.
4. Updating (relaxing) the distances of its neighboring cities if a shorter route is found.
5. Continuing until all cities have been visited or the destination is reached.

---

### 📚 Reference
-- For a more detailed explanation of the algorithm, see the following articles:  
-- [Dijkstra’s Algorithm to find Shortest Paths from a Source to all](https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-greedy-algo-7/)  
[DSA Dijkstra's Algorithm](https://www.w3schools.com/dsa/dsa_algo_graphs_dijkstra.php)
[Why use Dijkstra's Algorithm if Breadth First Search (BFS) can do the same thing faster?](https://stackoverflow.com/questions/3818079/why-use-dijkstras-algorithm-if-breadth-first-search-bfs-can-do-the-same-thing)
[Dijkstra's Algorithm](https://www.programiz.com/dsa/dijkstra-algorithm)
[Dijkstra Shortest Path](https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html)

## 🧠 Step-by-Step Example

Let’s say we have these cities and distances:

| From | To | Distance |
|------|----|-----------|
| A | B | 4 |
| A | C | 2 |
| B | C | 1 |
| B | D | 5 |
| C | D | 8 |
| D | E | 2 |

We start at **A** and want to find the shortest path to **E**.

1. Start with `A` → distance = 0  
   All others = ∞  
2. From `A`, we can reach:
   - `B` with distance 4  
   - `C` with distance 2  
3. The smallest distance is `C (2)`, so we move there next.  
4. From `C`, we can go to `B` (distance 3 total) or `D` (distance 10).  
   We update `B` to 3 since that’s shorter than 4.  
5. Continue visiting the next closest city, and keep updating paths.  
6. Finally, we get:  
A → C → B → D → E

And the shortest distance from **A** to **E** = 12.

---

## ⚙️ Algorithm Steps 

| Step | Action |
|------|---------|
| 1 | Initialize all distances as ∞ and set the start node to 0 |
| 2 | Use a **priority queue (min-heap)** to pick the next closest node |
| 3 | For each neighbor, calculate new distance = current + edge weight |
| 4 | If the new distance is smaller, update it |
| 5 | Repeat until all nodes are processed or destination is reached |
| 6 | Reconstruct the path using the "previous" node map |

---

## 📦 Data Structures Used

Dijkstra’s algorithm relies on several key data structures to stay **efficient** and **optimal**.

| Data Structure | Purpose | Efficiency Impact |
|----------------|----------|-------------------|
| **Priority Queue (Min-Heap)** | Selects the city with the smallest known distance next. | Ensures efficient selection in `O(log V)` time. |
| **Adjacency List** | Stores each city’s neighbors and edge distances. | Reduces space complexity compared to adjacency matrix (`O(V + E)` vs `O(V²)`). |
| **Map / Unordered Map** | Tracks shortest distance (`dist`) and previous city (`prev`). | Provides fast lookups and updates for distance tracking. |
| **Vector** | Used to store and return the final path. | Allows easy reversal and ordered traversal. |

**In this project**, we used:
- `map<string, double>` → for `dist` (stores shortest known distances)  
- `map<string, string>` → for `prev` (stores parent city for path reconstruction)  
- `priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>>` → for efficiently finding the next closest city  
- `map<string, vector<pair<string, double>>>` → as our adjacency list to store connections  

This combination gives an **overall time complexity of `O((V + E) log V)`**,  
which is **optimal for sparse graphs** like city road maps.


# 💻 Our Project’s Implementation

Now let’s look at how our C++ program applies these steps.

### 🧩 Function Header
```cpp
vector<string> dijkstra(GraphAdjList<string, double, double>& city_graph,
                     map<string, double>& edge_weights,
                     string startVertex,
                     string endVertex)
