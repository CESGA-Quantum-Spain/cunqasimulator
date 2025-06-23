
* **Standard way (slower)**
```console
cmake -B build/ 
cmake --build build/
cmake --install build/
```

* **Using [Ninja](https://ninja-build.org/) (faster)**
```console
cmake -G Ninja -B build/
ninja -C build -j $(nproc)
cmake --install build/
```