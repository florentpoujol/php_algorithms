<?php

function binary_search($array, $target)
{
    $arraySize = count($array);
    
    if ($arraySize === 0) {
        return false;
    } elseif ($arraySize === 1) {
        return ($target === $array[0]);
    }

    $middleId = (int)($arraySize / 2); // (int) cast and floors the value for when $arraySize is odd
    $middleValue = $array[$middleId];
    
    if ($target === $middleValue) {
        return true;
    } elseif ($target < $middleValue) {
        return binary_search(array_slice($array, 0, $middleId), $target);
    } else {
        return binary_search(array_slice($array, $middleId), $target);
    }
}

// ----------------------------------------
// tests / stats

/*

Array count: 100
Array size: 100000
BS time : 0.071372032165527
in_array time : 0.014373064041138

Array count: 50
Array size: 500000
BS time : 0.28307700157166
in_array time : 0.036340951919556

// the optimized version (now deleted) didn't use the count() or array_slice() method but as shown it was much slower
*/