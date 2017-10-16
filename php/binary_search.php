<?php

function binary_search(array $array, int $target)
{
    $size = count($array);
    
    if ($size === 0) {
        return false;
    }
    if ($size === 1) {
        return ($target === $array[0]);
    }

    $middleId = (int)($size / 2); // (int) cast and floors the value for when $size is odd
    $middleValue = $array[$middleId];
    
    if ($target === $middleValue) {
        return true;
    }

    if ($target < $middleValue) {
        return binary_search(array_slice($array, 0, $middleId), $target);
    }

    return binary_search(array_slice($array, $middleId), $target);    
}


function binary_search2(array $array, int $start, int $end, int $target)
{
    $size = $end - $start + 1;
    
    if ($size <= 0) {
        return false;
    }
    if ($size === 1) {
        return ($target === $array[$start]);
    }

    $middleId = $start + (int)($size / 2); // (int) cast and floors the value for when $size is odd
    $middleValue = $array[$middleId];
    
    if ($target === $middleValue) {
        return true;
    }

    if ($target < $middleValue) {
        return binary_search2($array, $start, $middleId - 1, $target);
    }

    return binary_search2($array, $middleId + 1, $end, $target);    
}


function binary_search3(array $array, int $start, int $size, int $target)
{
    if ($size <= 0) {
        return false;
    }
    if ($size === 1) {
        return ($target === $array[$start]);
    }

    $middleId = $start + (int)($size / 2); // (int) cast and floors the value for when $size is odd
    $middleValue = $array[$middleId];
    
    if ($target === $middleValue) {
        return true;
    }

    if ($target < $middleValue) {
        return binary_search3($array, $start, $middleId - $start, $target);
    }

    return binary_search3($array, $middleId + 1, $size - ($middleId - $start + 1), $target);    
}


function binary_search4(array $array, int $start, int $size, int $target)
{
    while ($size > 0) {
        if ($size === 1) {
            return ($target === $array[$start]);
        }

        $middleId = $start + (int)($size / 2); // (int) cast and floors the value for when $size is odd
        $middleValue = $array[$middleId];
        
        if ($target === $middleValue) {
            return true;
        }

        if ($target < $middleValue) {
            $size = $middleId - $start;
        } else {
            $size -= ($middleId - $start + 1);
            $start = $middleId + 1;
        }
    }

    return false;
}


// ----------------------------------------
// tests / stats

require_once "timeUtils.php";

$arrayCount = (int)$argv[1];
$arraySize = (int)$argv[2];

$data = [];
srand(time());
rand(); rand(); rand();
$targets = [];

$array = range(0, $arraySize - 1);

// build an array of $arrayCount arrays of $arraySize size
for ($i = 0; $i < $arrayCount; $i++) { 
    $targets[] = rand(0, $arraySize - 1); // target is always wihtin range
}

register_time_var("start");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! binary_search($array, $targets[$i])) {
        echo "wrong 1 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("php_BS1");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! binary_search2($array, 0, $arraySize - 1, $targets[$i])) {
        echo "wrong 2 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("php_BS2");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! binary_search3($array, 0, $arraySize, $targets[$i])) {
        echo "wrong 2.3 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
        break;
    }
}

register_time_var("php_BS3");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! binary_search4($array, 0, $arraySize, $targets[$i])) {
        echo "wrong 2.4 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
        break;
    }
}

register_time_var("php_BS4");



for ($i = 0; $i < $arrayCount; $i++) { 
    if (! Sort\BinarySearch::make($array, $targets[$i])) {
        echo "wrong 3 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("zephir_BS1");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! Sort\BinarySearch::make2($array, 0, $arraySize - 1, $targets[$i])) {
        echo "wrong 4 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("zephir_BS2");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! Sort\BinarySearch::make3($array, 0, $arraySize, $targets[$i])) {
        echo "wrong 4.1 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
        break;
    }
}

register_time_var("zephir_BS3");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! Sort\BinarySearch::make4($array, 0, $arraySize, $targets[$i])) {
        echo "wrong 4.1 t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
        break;
    }
}

register_time_var("zephir_BS4");



for ($i = 0; $i < $arrayCount; $i++) { 
    if (! in_array($targets[$i], $array)) {
        echo "wrong 5 t=".$targets[$i];
    }
}

register_time_var("in_array");




for ($i = 0; $i < $arrayCount; $i++) { 
    if (! CppAlgos::binarySearch3($array, 0, $arraySize, $targets[$i])) {
        echo "wrong 6 t=".$targets[$i]." \n";
        break;
    }
}

register_time_var("CPP_BS3");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! CppAlgos::binarySearch4($array, 0, $arraySize, $targets[$i])) {
        echo "wrong 7 t=".$targets[$i]." \n";
        break;
    }
}

register_time_var("CPP_BS4");



// execute the C script and retrive the last line of its input (the one that begins by "C")
$cTime = exec("./c/builds/binary_search $arrayCount $arraySize | grep C");
// calculating C time with PHP whould gives a time slightly higher

extract(get_time_diffs());

$str = <<<EOL
<pre>
Array count: $arrayCount
Array size: $arraySize

php BS1:        $php_BS1Diff s
php BS2:        $php_BS2Diff s
php BS3:        $php_BS3Diff s
php BS4:        $php_BS4Diff s

zephir BS1:     $zephir_BS1Diff s
zephir BS2:     $zephir_BS2Diff s
zephir BS3:     $zephir_BS3Diff s
zephir BS4:     $zephir_BS4Diff s

CPP BS1:        $CPP_BS3Diff s
CPP BS2:        $CPP_BS4Diff s

php in_array:   $in_arrayDiff s

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