<?php

function merge_sort($left, $size = null)
{
    if ($size === null) {
        $size = count($left);
    }

    if ($size <= 1) {
        return $left;
    }

    $middleId = (int)($size / 2); // the cast also floors the value when $size is odd

    // get all the elements to the right of $middleId (inclusive)
    $right = array_splice($left, $middleId); // array_splice here is faster than building $right manually
    
    // no need to compute again the size of the array, nor to call merge_sort() if the size is 1
    $leftSize = $middleId;
    if ($leftSize > 1) {
        $left = merge_sort($left, $leftSize);
    }

    $rightSize = $size - $middleId;
    if ($rightSize > 1) {
        $right = merge_sort($right, $rightSize);
    }

    // sort content of $left and $right
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

    // add the remaining values from $left and $right in the new sorted array
    // we know that by that time, the remaining values in $left are all smaller than the ones in $right
    
    for ($i = $leftId; $i < $leftSize; $i++) {
        $newArray[] = $left[$i];
    }
    for ($i = $rightId; $i < $rightSize; $i++) {
        $newArray[] = $right[$i];
    }
    return $newArray;

    // this is much faster than using array_slice twice and array_merge
    // return array_merge($newArray,
    //     array_slice($left, $leftId),
    //     array_slice($right, $rightId)
    // );
}

// ----------------------------------------
// tests / stats

ini_set("memory_limit", "1G");
// ini_set("max_execution_time", "120");
$arrayCount = (int)$argv[1];
$arraySize = (int)$argv[2];

$data = [];

// build an array of $arrayCount arrays of $arraySize size
for ($i = 0; $i < $arrayCount; $i++) { 
    $a = [];
    for ($j = 0; $j < $arraySize; $j++) { 
        $a[] = rand(0, $arraySize);
    }
    $data[] = $a;
}

$starTime = microtime(true);

for ($i = 0; $i < $arrayCount; $i++) { 
    merge_sort($data[$i]);
}

$endPhpMergeTime = microtime(true);
$phpMergeDiff = $endPhpMergeTime - $starTime;

for ($i = 0; $i < $arrayCount; $i++) { 
    Sort\MergeSort::make($data[$i]);    
}

$endExtMergeTime = microtime(true);
$extMergeDiff = $endExtMergeTime - $endPhpMergeTime;

for ($i = 0; $i < $arrayCount; $i++) { 
    sort($data[$i]);
}

$endSortTime = microtime(true);
$phpSortDiff = $endSortTime - $endExtMergeTime;

// execute the C script and retrive the last line of its input (the one that begins by "C")
$cTime = exec("./c/builds/merge_sort $arrayCount $arraySize | grep C");
// calculating C time with PHP whould gives a time slightly higher

$str = <<<EOL
<pre>
Array count: $arrayCount
Array size: $arraySize
php userland:           $phpMergeDiff s
php extension (zephir): $extMergeDiff s
php built-in sort:      $phpSortDiff s
$cTime
</pre>
EOL;
echo "$str";

/*
Array count: 50
Array size: 10000
php userland:           1.5883460044861 s
php extension (zephir): 0.75912380218506 s
php built-in sort:      0.064247131347656 s
C:                      0.076066 s

Array count: 30
Array size: 100000
php userland:           11.057584047318 s
php extension (zephir): 5.1014478206635 s
php built-in sort:      0.48250508308411 s
C:                      0.539713 s

Array count: 10
Array size: 500000
php userland:           20.288812160492 s
php extension (zephir): 9.1355178356171 s
php built-in sort:      0.9337911605835 s
C:                      0.981725 s

Array count: 5
Array size: 1000000
php userland:           23.012610912323 s
php extension (zephir): 9.6566219329834 s
php built-in sort:      0.98485493659973 s
C:                      1.025418 s

*/