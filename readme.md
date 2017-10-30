# PHP Algorithms

This project is the occasion for me to implement various search and sort algorithms in PHP, C and Zephir (to be compiled as a PHP extension) and to compare the performance of the same implementation on various technologies.

## How to

- Build Zephir extension with:

```
./build.sh z
```

- Build C script with:

```
./build.sh c nameofthefile
# The C script must be in `c/` folder and the build is created in the `c/builds/` folder.
# or
./build.sh cc nameofthefile
```

- Run C or PHP script with:

```
./run.sh c nameofthefile testCount arraySize
./run.sh php nameofthefile testCount arraySize
# testCount and arraySize must be numeric.
```

## Benchmarks

The benchmarks are done when executing the PHP (7.1.10) script via cli and passing two arguments: the test count and the array size.

Each functions (the PHP one from the same file, the one from the PHP extension, the C one and the built-in sort() or in_array()) must sort or search through [test count] arrays of [array size] elements each.

The time it takes each functions to process all arrays is measured and displayed.


### Binary search


```
Array count: 50
Array size: 1000000

php simple:      0.462 s     naive implementation that uses array_slice()
php opti:        0.000138 s  pass start id and size as arg (loop)

php in_array:    0.0482 s

zephir simple:   0.468 s
zephir opti:     0.0000851 s

CPP opti (rec):  0.419 s     recursive version
CPP opti (loop): 0.416 s

C (rec):         0.000008 s
C (loop):        0.000006 s

// --------------------------------------------------

Array count: 100
Array size: 2000000

php simple:      1.800 s
php opti:        0.000234 s

php in_array:    0.170 s

zephir simple:   1.802 s
zephir opti:     0.0000958 s

CPP opti (rec):  1.608 s
CPP opti (loop): 1.601 s

C (rec):         0.000022 s
C (loop):        0.000014 s

// --------------------------------------------------

Array count: 200
Array size: 3000000

php simple:      5.457 s
php opti:        0.000451 s

php in_array:    0.566 s

zephir simple:   5.453 s
zephir opti:     0.000174 s

CPP opti (rec):  4.851 s
CPP opti (loop): 4.824 s

C (rec):         0.000062 s
C (loop):        0.000035 s
```

The zephir code has the same implementation as PHP and very similar times, except for the loop implementation which is about twice faster.

The fastest PHP implementation is up to several hundreds times faster than the built-in `in_array()`, which still is 10 times faster than the naive implementation.

C is by far the fastest. The loop implementation can be up to twice faster than the recursive one.



### Merge sort

```
Array count: 50
Array size: 10000

php simple:         0.643 s
php opti:           0.420 s

zepir simple:       0.735 s
zepir opti:         0.322 s

php built-in sort:  0.0340 s

C:                  0.038747 s

// --------------------------------------------------

Array count: 30
Array size: 100000

php simple:         4.512 s
php opti:           2.892 s

zepir simple:       4.930 s
zepir opti:         2.179 s

php built-in sort:  0.256 s

C:                  0.274866 s

// --------------------------------------------------

Array count: 20
Array size: 500000

php simple:         17.249 s
php opti:           10.754 s

zepir simple:       17.755 s
zepir opti:         7.980 s

php built-in sort:  1.003 s

C:                  1.016297 s

// --------------------------------------------------

Array count: 10
Array size: 1000000

php simple:         17.439 s
php opti:           11.263 s

zepir simple:       18.629 s
zepir opti:         8.428 s

php built-in sort:  1.119 s

C:                  1.082885 s
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

In each cases, only two trees are created. One totally unbalanced, and one totally balanced (complete).  
The `find` function are tested by repeatedly searching for the same set of targets.  
I experimented a bit by creating a looping and recursive version or each find and sort functions, to see which perform better.

PHP userland:
```
Tree count: 100
Tree size: 33000
build data time :               2.6075768470764 s
---
Unbalanced BT FindLoop():       0.00073504447937012 s
Unbalanced BT FindRecurse():    0.0024600028991699 s
Unbalanced BT SortLoop():       3.5795381069183 s
Unbalanced BT SortRecurse():    3.1952500343323 s  Gives a segfault when tree size is > 33000

Balanced BT FindLoop():         0.00031495094299316 s
Balanced BT FindRecurse():      0.00077199935913086 s
Balanced BT SortLoop():         4.2059578895569 s (1.3 micro sec / node)
Balanced BT SortRecurse():      4.5488350391388 s

in_array with sorted values:    0.00043106079101562 s
in_array with random values:    0.001957893371582 s
sort with sorted values:        0.24364900588989 s


Tree count: 100
Tree size: 70000    Building an unsorted tree would give a segfault over that number
build data time :               11.158700942993 s
---
Unbalanced BT FindLoop():       0.00080418586730957 s
Unbalanced BT FindRecurse():    0.0034639835357666 s
Unbalanced BT SortLoop():       13.770484924316 s
Unbalanced BT SortRecurse():    No value (has been commented out)

Balanced BT FindLoop():         0.00031590461730957 s
Balanced BT FindRecurse():      0.00084114074707031 s
Balanced BT SortLoop():         19.596042871475 s (2.8 micro sec / node)
Balanced BT SortRecurse():      18.877624988556 s

in_array with sorted values:    0.00083398818969727 s
in_array with random values:    0.0042519569396973 s
sort with sorted values:        0.57456016540527 s
```

Zephir:
```
Tree count: 1
Tree size: 6000   
build data time :               0.12625503540039 s
---
Unbalanced BT FindLoop():       0.00018906593322754 s
Unbalanced BT FindRecurse():    0.005328893661499 s
Unbalanced BT SortLoop():       0.0057790279388428 s
Unbalanced BT SortRecurse():    0.15836095809937 s   Give a segfault when tree size > 6000 and tree count > 1

Balanced BT FindLoop():         2.1934509277344E-5 s
Balanced BT FindRecurse():      1.3113021850586E-5 s
Balanced BT SortLoop():         0.0059330463409424 s (0.99 micro sec / node)
Balanced BT SortRecurse():      0.0039708614349365 s

Tree count: 1
Tree size: 40000
build data time :               3.6859300136566 s
---
Unbalanced BT FindLoop():       0.00082898139953613 s
Unbalanced BT FindRecurse():    0.1431028842926 s
Unbalanced BT SortLoop():       0.037585020065308 s
Unbalanced BT SortRecurse():    No data (commented out)

Balanced BT FindLoop():         2.4080276489258E-5 s
Balanced BT FindRecurse():      2.9087066650391E-5 s
Balanced BT SortLoop():         0.049489974975586 s (1.23 micro sec / node)
Balanced BT SortRecurse():      0.044035911560059 s
```

C:
```
Tree count: 100 
Tree size: 70000 
--
Unbalanced BT Build time:      9.030995 s
Unbalanced BT FindLoop:        0.013572 s
Unbalanced BT FindRecurse:     0.021325 s
Unbalanced BT SortLoop:        0.111348 s
Unbalanced BT SortRecurse:     0.067735 s (9.6 nano sec / node)

Balanced BT Build:            0.026317 s
Balanced BT Find Loop:        0.000037 s (0.37 nano sec to find the target node)
Balanced BT Find Recurse:     0.000081 s
Balanced BT sort Loop:        0.415712 s (0.059 micro sec / node)
Balanced BT sort Recurse:     0.135533 s (0.019 micro sec / node)


Tree count: 200 
Tree size: 1000000 
--
Balanced BT Build time:       0.879148 s
Balanced BT Find Loop:        0.000218 s 
Balanced BT Find Recurse:     0.000210 s
(average of 1 nano sec to find the desired node)

Balanced BT sort Loop:        20.691364 s (0.1 micro sec / node)
Balanced BT sort Recurse:     20.165499 s
```

In PHP and Zephir, traversing the tree with recursive functions is not necessarily slower. 
It is always slower for the Find() function but typically a little faster when you have to traverse the whole tree.  
However they are always the first to give segfault errors.

Looping through (the Sort functions) unbalanced trees is consistently a little faster than the balanced tree.

As with merge and quick sort, Zephir is up to two times faster but fails with segfault error with much smaller trees, not sure why.

C is as expected much faster. In this last example (Sort a balanced tree), it is at least 10 to 30 times faster than the PHP userland.
