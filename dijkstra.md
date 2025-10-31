# 🧭 Dijkstra’s Algorithm Explained (Beginner-Friendly)

## 🧩 What Is Dijkstra’s Algorithm?

**Dijkstra’s Algorithm** is a **shortest path algorithm** used to find the **minimum distance** between a starting node and all other nodes in a **weighted graph** (a graph where each edge has a distance or cost).

It’s commonly used in:
- GPS navigation (finding the fastest route)
- Network routing (finding efficient data paths)
- Map applications (like Google Maps)

---

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

You start at **A**, and you want to find the shortest path to **E**.

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


Shortest distance from **A** to **E** = 12.

---

## ⚙️ Algorithm Steps (Generalized)

| Step | Action |
|------|---------|
| 1 | Initialize all distances as ∞ and set the start node to 0 |
| 2 | Use a **priority queue (min-heap)** to pick the next closest node |
| 3 | For each neighbor, calculate new distance = current + edge weight |
| 4 | If the new distance is smaller, update it |
| 5 | Repeat until all nodes are processed or destination is reached |
| 6 | Reconstruct the path using the "previous" node map |

---

# 💻 Your Project’s Implementation

Now let’s look at how your C++ program applies these steps.

### 🧩 Function Header
```cpp
vector<string> dijkstra(GraphAdjList<string, double, double>& city_graph,
                     map<string, double>& edge_weights,
                     string startVertex,
                     string endVertex)
