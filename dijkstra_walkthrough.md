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

### 🔄 Traditional Dijkstra

| Data Structure | Purpose | Complexity / Cons |
|----------------|---------|------------------|
| Vector / Array | Stores all nodes and distances | Finding the next minimum distance node is `O(V)` → slow for large graphs |
| Adjacency Matrix | Stores all edge weights | Space `O(V²)` → wasteful for sparse graphs |
| Array / Map for distance | Tracks shortest distances | Simple, but combined with linear search → overall `O(V²)` |

**Cons:**  
- Slow on large graphs  
- High memory usage for sparse graphs  
- Not suitable for real-time routing

Our dijkstra’s algorithm relies on several advanced data structures to stay **efficient** and **optimal**.


### ⚡ Optimized Implementation 


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


**Benefits:**  
- Much faster than linear search (`O((V + E) log V)`)  
- Memory-efficient for sparse graphs  
- Optimized for real-time shortest path calculations  


# 💻 Our Project’s Implementation

Now let’s look at how our C++ program applies these steps.

### 🧩 Function Header
```cpp
vector<string> dijkstra(GraphAdjList<string, double, double>& city_graph,
                     map<string, double>& edge_weights,
                     string startVertex,
                     string endVertex)
```
**Explanation:**

- `city_graph` → the graph of cities (nodes) and their connections.
- `edge_weights` → map storing distances between cities, e.g., `"Miami, Orlando"`.
- `startVertex` → starting city.
- `endVertex` → destination city.
- Returns a `vector<string>` of cities along the shortest path.

### 🧱 Step 1: Initialize Distances and Previous Nodes

```cpp
map<string,double> dist;
map<string,string> prev;

// Initialize all distances to infinity, except the start city
unordered_map<string, Element<double>*>* vertexMap = city_graph.getVertices();
for (auto it = vertexMap->begin(); it != vertexMap->end(); ++it) {
    string cityName = it->first;
    dist[cityName] = numeric_limits<double>::infinity();
    prev[cityName] = "";
}
dist[startVertex] = 0.0;

```
**Explanation:**
- `dist[city]` → stores the **current shortest known distance** from the start city.
- `prev[city]` → stores the **previous city on the path** to reconstruct the shortest path later.
- All cities are initialized to `∞` except the start city, which is set to 0.

### 🧩 Step 2: Build an Adjacency List

```cpp
map<string, vector<pair<string, double>>> adj;

for (auto edgeIt = edge_weights.begin(); edgeIt != edge_weights.end(); ++edgeIt) {
    string key = edgeIt->first;
    double weight = edgeIt->second;

    size_t comma_pos1 = key.find(",");
    size_t comma_pos2 = key.find(",", comma_pos1 + 2);
    string city1 = key.substr(0, comma_pos2);
    string city2 = key.substr(comma_pos2 + 2);

    adj[city1].push_back({city2, weight});
    adj[city2].push_back({city1, weight});
}
```

**Explanation:**
- Converts the `edge_weights` map into an **adjacency list** for fast neighbor lookup.
- Each city maps to a vector of pairs: `{neighbor_city, distance}`.
- This allows **O(1) access** to a city’s neighbors during the main loop.

### ⚡ Step 3: Initialize Priority Queue (Min-Heap)

```cpp
priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
pq.push({0.0, startVertex});
```

**Explanation:**
- Priority queue always gives the **closest city** next.
- Each element contains `{current_distance, city_name}`.
- Min-heap ensures **efficient selection** in `O(log V)` time per extraction.


### 🧮 Step 4: Main Loop — Relaxing Edges

```cpp
while (!pq.empty()) {
    auto [current_dist, u] = pq.top();
    pq.pop();

    if (u == endVertex) break;
    if (current_dist > dist[u]) continue;

    for (auto neighbor : adj[u]) {
        double alt = dist[u] + neighbor.second;
        if (alt < dist[neighbor.first]) {
            dist[neighbor.first] = alt;
            prev[neighbor.first] = u;
            pq.push({alt, neighbor.first});
        }
    }
}
```

**Explanation:**
- Extract the **closest unvisited city** `u` from the priority queue.
- Stop if `u` is the **destination city**.
- Skip if a **shorter path to `u` is already known**.
- Loop through all neighbors of `u`:
  - Compute `alt = distance to u + edge weight to neighbor`.
  - If `alt` is smaller than the neighbor’s current distance:
    - Update `dist[neighbor]`.
    - Set `prev[neighbor] = u`.
    - Push neighbor into the priority queue.

### 🧵 Step 5: Reconstruct the Shortest Path

```cpp
vector<string> path;
string current = endVertex;

while (current != "") {
    path.push_back(current);
    current = prev[current];
}

reverse(path.begin(), path.end());
return path;
```

**Explanation:**
- Start at `endVertex` and follow the `prev` map backwards.
- Collect all cities along the path in the `path` vector.
- Reverse the vector to get the path from **start → end**.
- Return the ordered list of cities as the **shortest path**.

### ⏱️ Step 6: Runtime and Efficiency**

- **Min-heap priority queue**: `O((V + E) log V)`
- **Adjacency list**: `O(V + E)` memory → efficient for sparse graphs
- **Overall**: much faster and memory-efficient than the traditional linear search (`O(V²)`)


### 📚 References
For a more detailed explanation of Dijkstra’s Algorithm, see the following articles:

- [GeeksforGeeks — *Dijkstra’s Algorithm to Find Shortest Paths from a Source to All*](https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-greedy-algo-7/)
- [W3Schools — *DSA Dijkstra's Algorithm*](https://www.w3schools.com/dsa/dsa_algo_graphs_dijkstra.php)
- [Stack Overflow — *Why Use Dijkstra's Algorithm if Breadth First Search (BFS) Can Do the Same Thing Faster?*](https://stackoverflow.com/questions/3818079/why-use-dijkstras-algorithm-if-breadth-first-search-bfs-can-do-the-same-thing)
- [Programiz — *Dijkstra's Algorithm Explained*](https://www.programiz.com/dsa/dijkstra-algorithm)
- [USFCA Visualization — *Dijkstra Shortest Path Interactive Demo*](https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html)

