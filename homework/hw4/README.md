# Homework 4:

# Problem 1: for both COSC302 and COSC307 students

Show that mergsort is `O(N logN)` for all data. That is, use telescoping to solve `T(N) = 2T(N/2) + cN`.

# Problem 2: for COSC307 students ONLY

Redo Problem 1 using brute force substitution. That is, first substitute the expression for `T(N/2)` with its recurrence. Then, repeat for `T(N/4)`, followed by `T(N/8)`. Each time simplify the resulting expression. Now let `N/2` be step `k=1`, `N/4` be step `k=2`, and `N/8` be step `k=3`. Identify the substitution pattern and rewrite the original recurrence relation accordingly. Determine the maximum value of `k` and simplify to obtain the expected `O(N logN)` cost (Note: this sounds worse than it is.).
