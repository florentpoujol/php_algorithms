# PHP Algorithms

This project is the occasion for me to implement various search and sort algorithms in PHP, C and Zephir (to be compiled as a PHP extension) and to compare the performance of the same implementation on various technologies.

## How to

- Build Zephir extension with:

```
./build z
```

- Build C script with:

```
./build c nameofthefile
# The C script must be in `c/` folder and the build is created in the `c/builds/` folder.
```

- Run C or PHP script with:

```
./run c nameofthefile arrayCount arraySize
./run php nameofthefile arrayCount arraySize
# arrayCount and arraySize must be numeric.
```

## Benchmarks

The benchmarks are done when executing the PHP (7.0.x) script via cli and passing two arguments: the array count and the array size.

Each functions (the PHP one from the same file, the one from the PHP extension, the C one and the built-in sort() or in_array()) must sort or search through [array count] arrays of [array size] elements each.

The time it takes each functions to process all arrays is mesured and displayed.

### Takeaway

- The PHP (userland) function is by far the slowest (20 times slower than the built-in PHP functions).
- The function from the extension is at least 50% faster for a very similar code.
- The C function, despite using a probably "naive" implementation of each algo is almost the fastest.
- The PHP built-in PHP function is the fastest, only slighly faster than the C function but usually 10 times faster than the userland.

### Binary search

Here, as the code is very simple, the extension is only marginally faster than the userland.

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
php userland:           20.105852127075 s
php extension (zephir): 8.7928829193115 s
php built-in sort:      0.93890905380249 s
C:                      0.926520 s  

Array count: 5
Array size: 1000000
php userland:           21.140818834305 s
php extension (zephir): 9.0098340511322 s
php built-in sort:      0.99025511741638 s
C:                      0.965595 s 
```

### Quick sort

```
Array count: 50
Array size: 10000
php userland:           0.80580806732178 s
php extension (zephir): 0.35681986808777 s
php built-in sort:      0.064460039138794 s
C:                      0.072400 s

Array count: 30
Array size: 100000
php userland:           5.7532091140747 s
php extension (zephir): 2.5272769927979 s
php built-in sort:      0.4820020198822 s
C:                      0.526823 s

Array count: 10
Array size: 500000
php userland:           10.721113920212 s
php extension (zephir): 4.752336025238 s
php built-in sort:      0.93213200569153 s
C:                      0.981925 s

Array count: 5
Array size: 1000000
php userland:           11.271620988846 s
php extension (zephir): 5.0043919086456 s
php built-in sort:      0.98648905754089 s
C:                      1.029710 s
```

### Binary Tree

It take 5 seconds to build an unbalanced tree from 10000 sorted values.
Only 0.04 s to build from random values. (0.02 s to balance).

```
Array count: 50
Array size: 10000
build data time :           0.10264301300049 s
php BT search :             0.00020813941955566 s
php Balanced BT search :    0.00013303756713867 s
php built-in in_array() :   0.00026392936706543 s
php BT sort :               0.40845394134521 s
php Balanced BT sort :      0.39661407470703 s
php built-in sort() :       0.034395933151245 s

Array count: 30
Array size: 100000
build data time :           3.1138999462128 s
php BT search :             0.00017213821411133 s
php Balanced BT search :    0.00014305114746094 s
php built-in in_array() :   0.0014889240264893 s
php BT sort :               12.102861881256 s
php Balanced BT sort :      12.167613983154 s
php built-in sort() :       0.2691650390625 s


Array count: 10
Array size: 500000
build data time :           94.416689157486 s
php BT search :             0.00007700 s (7.7009201049805E-5 s)
php Balanced BT search :    0.00003290 s (3.2901763916016E-5 s)
php built-in in_array() :   0.00006008 s (6.0081481933594E-5 s)
php BT sort :               95.364817857742 s
php Balanced BT sort :      92.899277210236 s
php built-in sort() :       0.54810690879822 s

Array count: 1
Array size: 500000
build data time :           95.854732036591 s
php BT search :             2.0027160644531E-5 s
php Balanced BT search :    5.9604644775391E-6 s
php built-in in_array() :   8.8930130004883E-5 s
php BT sort :               9.8844139575958 s
php Balanced BT sort :      9.3771319389343 s
php built-in sort() :       0.099236011505127 s

Incremental build time :
Tree size   BT   Balanced BT    Total
100000      1.2     1.8         3.0  s
150000      2.9     4.2         7.2  s
200000      5.4     7.5         12.9 s
250000      8.8     11.9        20.8 s

```
 