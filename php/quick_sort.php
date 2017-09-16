<?php

function quick_sort($array, $size = null)
{
    if ($size === null) {
        $size = count($array);
    }

    if ($size <= 1) {
        return $array;
    }

    // select pivot (median of three)
    $first = $array[0];
    $last = $array[$size - 1];
    if ($first > $last) {
        $temp = $first;
        $first = $last;
        $last = $temp;
    }

    if ($size === 2) {
        return [$first, $last];
    }

    $pivot = $array[ (int)($size/2) ];
    if ($pivot < $first) {
        $pivot = $first;
    } elseif ($pivot > $last) {
        $pivot = $last;
    }
    
    // moves values
    $left = [];
    $middles = [];
    $right = [];
    foreach ($array as $value) { // a for loop is 50% slower 
        if ($value < $pivot) {
            $left[] = $value;
        } elseif ($value > $pivot) {
            $right[] = $value;
        } else {
            $middles[] = $value;
        }        
    }

    $leftSize = count($left);
    if ($leftSize > 1) {
        $left = quick_sort($left, $leftSize);
    }

    $rightSize = count($right);
    if ($rightSize > 1) {
        $right = quick_sort($right, $rightSize);
    }

    return array_merge($left, $middles, $right);
}

// ----------------------------------------
// tests / stats

ini_set("memory_limit", "1G");
ini_set("max_execution_time", "120");
$arrayCount = 30;
$arraySize = 100000;

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
    Sort\Sort::mergeSort($data[$i]);    
}

$endExtMergeTime = microtime(true);
$extMergeDiff = $endExtMergeTime - $endPhpMergeTime;

for ($i = 0; $i < $arrayCount; $i++) { 
    sort($data[$i]);
}

$endSortTime = microtime(true);
$phpSortDiff = $endSortTime - $endExtMergeTime;

$str = <<<EOL
<pre>
Array count: $arrayCount
Array size: $arraySize
php merge_sort : $phpMergeDiff s
ext mergeSort : $extMergeDiff s
built-in sort : $phpSortDiff s
</pre>
EOL;
echo "$str";

/*
Array count: 50
Array size: 10000
php merge_sort : 1.6033179759979 s
ext mergeSort : 0.75007104873657 s
built-in sort : 0.064254999160767 s

Array count: 30
Array size: 100000
php merge_sort : 10.968606948853 s
ext mergeSort : 5.0370688438416 s
built-in sort : 0.48219299316406 s

Array count: 10
Array size: 500000
php merge_sort : 20.224661111832 s
ext mergeSort : 8.9544038772583 s
built-in sort : 0.93342590332031 s


*/

/*
Array count: 10
Array size: 10000
Build data time : 0.060626029968262
quick_sort time : 0.21846914291382
sort time : 0.014840841293335

Array count: 10
Array size: 100000
Build data time : 0.42583203315735
quick_sort time : 2.5013740062714
sort time : 0.18328595161438

Array count: 10
Array size: 500000
Build data time : 2.2172510623932
quick_sort time : 14.667098999023
sort time : 1.0235970020294
*/