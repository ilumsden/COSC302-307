# Homework 5:

## Problem 1:

Consider the directed graph given by `V={0, 1, 2, 3, 4, 5}`, `E={[0, 2], [0, 3], [0, 4], [2, 1], [2, 4], [3, 5], [4, 0]}`.

Determine the vertex visitation order that results from breadth-first traversal starting first from vertex 2 then vertex 3, 4, and 5. Use the breadth-first search function shown below. See the `graph1_handout` (lecture material) for layout details. Assume all vertex colors are reset to __WHITE__ before any run. Process lowest numbered vertex first in case of a tie.

Instead of producing lots of arrays that indicate when the color is changed for which vertex, draw trees that represent the traversal process. Then extract the corresponding vertex visitation order.

```
void graph::bfsearch(int source) {
    queue<int> Q;
    Q.push(source);

    while (!Q.empty()) {
        int i = Q.front();
        Q.pop();

        if (vcolor[i] == BLACK)
            continue;

        vcolor[i] = BLACK;
        for (int k = 0; k < E[i].size(); k++)
            Q.push(E[i][k]);
    }
}
```
