# ds_and_algo

Learning repository for data structures and algorithms in C++.

-----------

Requirements:
- CMake 3.15+
- A C++23-capable compiler (clang or gcc)
- clang-format and clang-tidy

Build (normal):

```bash
cmake -S . -B build
cmake --build build -j
```

Enable clang-tidy during local builds (opt-in):

```bash
cmake -S . -B build -DENABLE_CLANG_TIDY=ON
cmake --build build -j
```
# ds_and_algo