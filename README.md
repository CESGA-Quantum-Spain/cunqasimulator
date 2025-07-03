
* **Modules**
```
module load qmio/hpc gcc/12.3.0 hpcx-ompi cmake/3.27.6 nlohmann_json/3.11.3 impi/2021.13.0 ninja/1.9.0
```

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