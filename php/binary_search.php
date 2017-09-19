<?php

function binary_search($array, $target)
{
    $arraySize = count($array);
    
    if ($arraySize === 0) {
        return false;
    }
    if ($arraySize === 1) {
        return ($target === $array[0]);
    }

    $middleId = (int)($arraySize / 2); // (int) cast and floors the value for when $arraySize is odd
    $middleValue = $array[$middleId];
    
    if ($target === $middleValue) {
        return true;
    }

    if ($target < $middleValue) {
        return binary_search(array_slice($array, 0, $middleId), $target);
    }

    return binary_search(array_slice($array, $middleId), $target);    
}

// ----------------------------------------
// tests / stats

// ini_set("memory_limit", "1G");
// ini_set("max_execution_time", "120");
$arrayCount = (int)$argv[1];
$arraySize = (int)$argv[2];

$data = [];
srand(time());
rand(); rand(); rand();
$targets = [];


// build an array of $arrayCount arrays of $arraySize size
for ($i = 0; $i < $arrayCount; $i++) { 
    $data[] = range(0, $arraySize-1);
    $targets[] = rand(0, $arraySize-1); // target is always wihtin range
}

$starTime = microtime(true);

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! binary_search($data[$i], $targets[$i])) {
        echo "wrong 1 i=$i ".in_array($targets[$i], $data[$i])."\n";
        var_dump($data[$i]);
    }
}

$endPhpMergeTime = microtime(true);
$phpMergeDiff = $endPhpMergeTime - $starTime;

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! Sort\BinarySearch::make($data[$i], $targets[$i])) {
        echo "wrong 2 i=$i ".in_array($targets[$i], $data[$i])."\n";
        var_dump($data[$i]);
    }
}

$endExtMergeTime = microtime(true);
$extMergeDiff = $endExtMergeTime - $endPhpMergeTime;

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! in_array($targets[$i], $data[$i])) {
        // condition is here just so that it consume the same time as for the two cases above
    }
}

$endSortTime = microtime(true);
$phpSortDiff = $endSortTime - $endExtMergeTime;

// execute the C script and retrive the last line of its input (the one that begins by "C")
$cTime = exec("./c/builds/binary_search $arrayCount $arraySize | grep C");

// calculating C time with PHP whould gives a time slightly higher

$str = <<<EOL
<pre>
Array count: $arrayCount
Array size: $arraySize
php userland:           $phpMergeDiff s
php extension (zephir): $extMergeDiff s
php built-in in_array:      $phpSortDiff s
$cTime
</pre>
EOL;
echo "$str";

/*
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
*/