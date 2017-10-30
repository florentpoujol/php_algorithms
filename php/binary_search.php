<?php

function binary_search_simple(array $array, int $target): bool
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
        return binary_search_simple(array_slice($array, 0, $middleId), $target);
    }

    return binary_search_simple(array_slice($array, $middleId), $target);    
}

// the recursive version was a little slower
function binary_search_opti(array $array, int $start, int $size, int $target): bool
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
            // left side, middleId excluded
            $size = $middleId - $start;
            // $start doesn't change value (which may be != 0)
        } else {
            // right side, middleId excluded
            $size -= ($middleId - $start + 1);
            $start = $middleId + 1;
        }
    }

    return false;
}


// ----------------------------------------
// tests / stats

require_once "utils.php";

$arrayCount = (int)$argv[1];
$arraySize = (int)$argv[2];

$data = [];
$targets = [];

$array = range(0, $arraySize - 1);

// build an array of $arrayCount arrays of $arraySize size
for ($i = 0; $i < $arrayCount; $i++) { 
    $targets[] = rand(0, $arraySize - 1); // target is always wihtin range
}

register_time_var("start");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! binary_search_simple($array, $targets[$i])) {
        echo "wrong php simple t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("php_simple");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! binary_search_opti($array, 0, $arraySize - 1, $targets[$i])) {
        echo "wrong php opti t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("php_opti");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! in_array($targets[$i], $array)) {
        echo "wrong in_array t=".$targets[$i];
    }
}

register_time_var("in_array");

// --------------------

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! ZephirAlgos\BinarySearch::simple($array, $targets[$i])) {
        echo "wrong zephir simple t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("zephir_simple");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! ZephirAlgos\BinarySearch::opti($array, 0, $arraySize - 1, $targets[$i])) {
        echo "wrong zephir opti t=".$targets[$i]." ".in_array($targets[$i], $array)."\n";
    }
}

register_time_var("zephir_opti");

// --------------------

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! CppAlgos\BinarySearch::opti_recursive($array, 0, $arraySize, $targets[$i])) {
        echo "wrong CPP opti recursive t=".$targets[$i]." \n";
        break;
    }
}

register_time_var("CPP_simple");

for ($i = 0; $i < $arrayCount; $i++) { 
    if (! CppAlgos\BinarySearch::opti($array, 0, $arraySize, $targets[$i])) {
        echo "wrong CPP opti loop t=".$targets[$i]." \n";
        break;
    }
}

register_time_var("CPP_opti");


// execute the C script and retrive the last line of its input (the one that begins by "C")
$cTime = shell_exec("./c/builds/binary_search $arrayCount $arraySize | grep C");
// calculating C time with PHP whould gives a time slightly higher

extract(get_time_diffs());

$str = <<<EOL
<pre>
Array count: $arrayCount
Array size: $arraySize

php simple:      $php_simpleDiff s
php opti:        $php_optiDiff s

php in_array:    $in_arrayDiff s

zephir simple:   $zephir_simpleDiff s
zephir opti:     $zephir_optiDiff s

CPP opti (rec):  $CPP_simpleDiff s   recursive version
CPP opti (loop): $CPP_optiDiff s

$cTime
</pre>
EOL;
echo "$str";
