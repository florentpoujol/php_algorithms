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
    // creating leftSize above and incrementing if in the loop 
    // is noticeably slower than using count()
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

// check that the array is indeed sorted
function check($array)
{
    $count = count($array);
    for ($i=1; $i < $count; $i++) { 
        if ($array[$i-1] > $array[$i]) {
            return false;
        }
    }
    return true;
}

$arrayCount = (int)$argv[1];
$arraySize = (int)$argv[2];

$data = [];
srand(time());
rand(); rand(); rand();

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
    // if (!check(quick_sort($data[$i]))) {
    //     echo "wrong";
    // }
    quick_sort($data[$i]);
}

$endPhpMergeTime = microtime(true);
$phpMergeDiff = $endPhpMergeTime - $starTime;

for ($i = 0; $i < $arrayCount; $i++) { 
    // if (!check(Sort\QuickSort::make($data[$i]))) {
    //     echo "wrong2";
    // }
    Sort\QuickSort::make($data[$i]);
}

$endExtMergeTime = microtime(true);
$extMergeDiff = $endExtMergeTime - $endPhpMergeTime;

for ($i = 0; $i < $arrayCount; $i++) { 
    sort($data[$i]);
}

$endSortTime = microtime(true);
$phpSortDiff = $endSortTime - $endExtMergeTime;

// execute the C script and retrive the last line of its input (the one that begins by "C")
$cTime = exec("./c/builds/quick_sort $arrayCount $arraySize | grep C");
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
*/
