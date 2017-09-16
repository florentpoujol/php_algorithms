# PHP Algorithms

This project is the occasion for me to implement various search and sort algorithms in PHP,  C and Zephir (to be compiled as a php extension) and to compare the performance of the same implementation on various technologies.

## How to

- Build zephir extension with:

```
./build z
```

- Build c script with:

```
.build c nameofthefile
# The C script must be in C/ folder and the build is created in the C/builds/ folder.
```

- Run C or PHP script with:

```
./run c nameofthefile arrayCount arraySize
./run php nameofthefile arrayCount arraySize
# arrayCount and arraySize must be numeric.
```

## Benchmarks

The benchmarks are done when executing the php (7.0.x) script via cli and passing two arguments: the array count and the array size.

Each functions (the PHP (userland) one from the same file, the one from the PHP extension, the built-in sort() and the C one) must sort or search through [array count] arrays of [array size] elements each.

The time it takes to perform each is mesured and displayed.

### Takeaway

- The PHP (userland) function is by far the slowest (20 times slowe than the built-in sort())
- The function from the extension is at least 50% faster for a very similar code
- The C function, despite using a probably "naive" implementation of each algo is way faster
- The PHP built-in sort() function is the fastest, usually 10 times faster than the extension but only slighly faster than the C function

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