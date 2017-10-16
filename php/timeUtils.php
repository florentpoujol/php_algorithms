<?php

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

        $diffs[ $name . "Diff" ] = $timeVars[$name] - $timeVars[$lastName];
        $lastName = $name;
    }

    return $diffs;
}
