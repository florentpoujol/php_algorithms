<?php

function merge_sort_simple(array $left): array
{
    $size = count($left);
    if ($size <= 1) {
        return $left;
    }

    $middleId = (int)($size / 2); // the cast also floors the value when $size is odd

    // get all the elements to the right of $middleId (inclusive)
    $right = array_splice($left, $middleId);
    
    // recursively and independentely merge left and right
    $left = merge_sort_simple($left);
    $right = merge_sort_simple($right);

    // left and right are now sorted but right may contain value < to some values in left
    // merge these in order in a new array
    $newArray = [];
    $leftId = 0;
    $leftSize = count($left);
    $rightId = 0;
    $rightSize = count($right);

    while ($leftId < $leftSize && $rightId < $rightSize) {
        if ($left[$leftId] <= $right[$rightId]) {
            $newArray[] = $left[$leftId];
            $leftId++;
        } else {
            $newArray[] = $right[$rightId];
            $rightId++;
        }
    }

    // so now, just add the remaining values from left and right in the new sorted array
    // we know by that time that the remaining values in $left if any are all smaller than the ones in $right (and vice-versa)
    return array_merge(
        $newArray,
        array_slice($left, $leftId),
        array_slice($right, $rightId)
    );
}


function merge_sort_opti(array $array, int $startId = 0, int $size = null): array
{
    if ($size === null) {
        // should only happen the first time
        $size = count($array);
    }

    if ($size <= 1) {
        return [$array[$start]];
    }

    $middleId = $startId + (int)($size / 2); // the cast also floors the value when $size is odd
  
    // no need to compute again the size of the array, nor to call merge_sort() if the size is 1
    $left = [$array[$startId]]; // when size == 1
    $leftSize = $middleId - $startId;
    if ($leftSize > 1) {
        $left = merge_sort_opti($array, $startId, $leftSize);
    }

    $right = [$array[$middleId]];
    $rightSize = $size - ($middleId - $startId);
    if ($rightSize > 1) {
        $right = merge_sort_opti($array, $middleId, $rightSize);
    }

    // this time, leave left and right alone intact
    // just copy values to the newArray
    // and keep track up to which id left or right has been "emptied"
    $newArray = [];
    $leftId = 0;
    $rightId = 0;
    while ($leftId < $leftSize && $rightId < $rightSize) {
        if ($left[$leftId] <= $right[$rightId]) {
            $newArray[] = $left[$leftId];
            $leftId++;
        } else {
            $newArray[] = $right[$rightId];
            $rightId++;
        }
    }

    // now only a part of left or right needs to be merged in the new array
    // could use array_merge() and array_slice() but this is much faster
    for ($i = $leftId; $i < $leftSize; $i++) {
        $newArray[] = $left[$i];
    }
    for ($i = $rightId; $i < $rightSize; $i++) {
        $newArray[] = $right[$i];
    }
    return $newArray;
}


// ----------------------------------------
// tests / stats

require "utils.php";

// ini_set("memory_limit", "1G");
$arrayCount = (int)$argv[1];
$arraySize = (int)$argv[2];

$array = range(0, $arraySize - 1);
shuffle($array);

$debug = false; // for isSorted();

register_time_var("start");

for ($i = 0; $i < $arrayCount; $i++) { 
    $_array = merge_sort_simple($array);
    if (! isSorted($_array, "php_simple")) {
        break;
    }
}

register_time_var("php_simple");

for ($i = 0; $i < $arrayCount; $i++) { 
    $_array = merge_sort_opti($array);
    if (! isSorted($_array, "php_opti")) {
        break;
    }
}

register_time_var("php_opti");


for ($i = 0; $i < $arrayCount; $i++) { 
    $_array = ZephirAlgos\MergeSort::simple($array);    
    if (! isSorted($_array, "zephir_simple")) {
        break;
    }
}

register_time_var("zephir_simple");

for ($i = 0; $i < $arrayCount; $i++) { 
    $_array = ZephirAlgos\MergeSort::opti($array);    
    if (! isSorted($_array, "zephir_opti")) {
        break;
    }
}

register_time_var("zephir_opti");


for ($i = 0; $i < $arrayCount; $i++) { 
    sort($array);
}

register_time_var("php_sort");
// shuffle($array);
// register_time_var("shuffle3");

// for ($i = 0; $i < $arrayCount; $i++) { 
//     CppAlgos::mergeSort1($array);
// }

// register_time_var("cpp_merge1");
// shuffle($array);
// register_time_var("shuffle4");


// execute the C script and retrive the last line of its input (the one that begins by "C")
$cTime = exec("./c/builds/merge_sort $arrayCount $arraySize | grep C:");
// calculating C time with PHP whould gives a time slightly higher

extract(get_time_diffs());

$str = <<<EOL
<pre>
Array count: $arrayCount
Array size: $arraySize

php simple:         $php_simpleDiff s
php opti:           $php_optiDiff s

zepir simple:       $zephir_simpleDiff s
zepir opti:         $zephir_optiDiff s

php built-in sort:  $php_sortDiff s

$cTime
</pre>
EOL;
echo "$str";

/*
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

*/
