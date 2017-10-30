<?php

srand(time());
rand(); rand(); rand();


$timeVars = [];
function register_time_var(string $varName)
{
    global $timeVars;
    $timeVars[$varName] = microtime(true);
    echo "register_time_var: $varName \n";
}


function get_time_diffs(): array
{
    global $timeVars;
    $varNames = array_keys($timeVars);
    $diffs = [];
    $lastName = "";

    foreach ($varNames as $i => $name) {
        if ($i === 0) {
            $lastName = $name;
            continue;
        }

        $diffs[ $name . "Diff" ] = formatFloat( $timeVars[$name] - $timeVars[$lastName] );
        
        $lastName = $name;
    }

    return $diffs;
}


// don't use exponent
// cut the 
function formatFloat(float $num): string
{
    $str = (string)$num;
    $newStr = "";

    if (! strpos($str, "E")) {
        $pos = 0; // pos of the first non-0 digit
        for ($i=0; $i < strlen($str); $i++) { 
            if ($str[$i] !== "0" && $str[$i] !== ".") {
                $pos = $i;
                break;
            }
        }
        if ($pos === 0) {
            // positive time
            $pos = 2;
        }

        $newStr = number_format($num, $pos + 1);
    } else {
        $precision = (int)$str[strlen($str) - 1];
        $newStr = sprintf("%.".($precision+2)."f", $num);
    }

    return $newStr;
}


function isSorted(array $array, string $str = ""): bool
{
    global $debug;

    if ($debug === true) {
        $size = count($array) - 1;
        for ($i=0; $i < $size; $i++) { 
            if ($array[$i] > $array[$i + 1]) {
                echo "ERROR: array not sorted. ", $str;
                // var_dump($array);
                return false;
            }
        }
    }
    
    return true;
}
