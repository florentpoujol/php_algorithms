# PHP Algorithms

This project is the occasion for me to implement various search and sort algorithms in PHP,  C and Zephir (to be compiled as a php extension) and to compare the performance of the same implementation on various technologies.

## How to

- Build Zephir extension with:

```
./build z
```

- Build C script with:

```
.build c nameofthefile
# The C script must be in `c/` folder and the build is created in the `c/builds/` folder.
```

- Run C or PHP script with:

```
./run c nameofthefile arrayCount arraySize
./run php nameofthefile arrayCount arraySize
# arrayCount and arraySize must be numeric.
```

## Benchmarks

The benchmarks are done when executing the php (7.0.x) script via cli and passing two arguments: the array count and the array size.

Each functions (the PHP one from the same file, the one from the PHP extension, the C one and the built-in sort()) must sort or search through [array count] arrays of [array size] elements each.

The time it takes each functions to process all arrays is mesured and displayed.

### Takeaway

- The PHP (userland) function is by far the slowest (20 times slower than the built-in sort()).
- The function from the extension is at least 50% faster for a very similar code.
- The C function, despite using a probably "naive" implementation of each algo is almost the fastest.
- The PHP built-in sort() function is the fastest, only slighly faster than the C function but usually 10 times faster than the extension.

### Binary search

```
Array count: 100
Array size: 100000
php userland:           0.07220196723938 s
php extension (zephir): 0.071554899215698 s
php built-in in_array:  0.010440111160278 s
C:                      0.000060 s

Array count: 100
Array size: 1000000
php userland:           1.180379152298 s
php extension (zephir): 1.1114928722382 s
php built-in in_array:  0.099914073944092 s
C:                      0.000081 s

Array count: 100
Array size: 2000000
php userland:           2.1361570358276 s
php extension (zephir): 2.1351580619812 s
php built-in in_array:  0.20601606369019 s
C:                      0.000089 s

```

### Merge sort

```
Array count: 50  
Array size: 10000  
PHP userland:           1.5883460044861 s  
PHP extension (zephir): 0.75912380218506 s  
PHP built-in sort:      0.064247131347656 s  
C:                      0.076066 s  

Array count: 30  
Array size: 100000  
PHP userland:           11.057584047318 s  
PHP extension (zephir): 5.1014478206635 s  
PHP built-in sort:      0.48250508308411 s  
C:                      0.539713 s  

Array count: 10  
Array size: 500000  
PHP userland:           20.288812160492 s  
PHP extension (zephir): 9.1355178356171 s  
PHP built-in sort:      0.9337911605835 s  
C:                      0.981725 s  

Array count: 5  
Array size: 1000000  
PHP userland:           23.012610912323 s  
PHP extension (zephir): 9.6566219329834 s  
PHP built-in sort:      0.98485493659973 s  
C:                      1.025418 s  
```

### Quick sort

C function not available (it doesn't work :().

```
Array count: 50
Array size: 10000
php userland:           0.80128002166748 s
php extension (zephir): 0.35245800018311 s
php built-in sort:      0.064351081848145 s

Array count: 30
Array size: 100000
php userland:           5.7431969642639 s
php extension (zephir): 2.5146708488464 s
php built-in sort:      0.48189902305603 s

Array count: 10
Array size: 500000
php userland:           10.714740037918 s
php extension (zephir): 4.7196428775787 s
php built-in sort:      0.93556809425354 s

Array count: 5
Array size: 1000000
php userland:           11.382132053375 s
php extension (zephir): 5.0132319927216 s
php built-in sort:      0.98790097236633 s
```