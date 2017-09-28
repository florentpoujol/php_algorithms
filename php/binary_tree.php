<?php

require_once 'BinaryTree.php';

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

function buildBT(array $sortedValues, bool $randomize = true): BinaryTree
{
    $tree = new BinaryTree;
    $size = count($sortedValues);
    $lastNode = null;

    if ($randomize) {
        shuffle($sortedValues);
    }

    while ($size-- > 0) {
        $value = array_shift($sortedValues);
        $node = new BinaryTreeNode($value);
        if ($randomize) {
            $tree->insert($node);
        } else {
            // consider values sorted ASC
            // it is prohibitively slow to insert them
            // while traversing the whole tree (which is just like a linked list in that case)
            if ($lastNode) {
                $lastNode->right = $node;
                $node->parent = $lastNode;
            } else {
                $tree->insert($node);
            }
            $lastNode = $node;
        }
    }
    return $tree;
}

function buildEBT(array $sortedValues, bool $randomize = true): Sort\BinaryTree
{
    $tree = new Sort\BinaryTree;
    $size = count($sortedValues);
    $lastNode = null;

    if ($randomize) {
        shuffle($sortedValues);
    }

    while ($size-- > 0) {
        $value = array_shift($sortedValues);
        $node = new Sort\BinaryTreeNode($value);
        if ($randomize) {
            $tree->insert($node);
        } else {
            // consider values sorted ASC
            // it is prohibitively slow to insert them
            // while traversing the whole tree (which is just like a linked list in that case)
            if ($lastNode) {
                $lastNode->right = $node;
                $node->parent = $lastNode;
            } else {
                $tree->insert($node);
            }
            $lastNode = $node;
        }
    }
    return $tree;
}

// ----------------------------------------
// tests / stats

/*
compare:
- search time in BT vs BST vs balanced BST
- insertion in AVL vs BST + balancing
*/
ini_set("memory_limit", "2G");
//ini_set("max_execution_time", "120");
$argv = $argv ?? [];

$treeCount = (int)($argv[1] ?? $_GET['treeCount'] ?? 1);
$treeSize = (int)($argv[2] ?? $_GET['treeSize'] ?? 10); // node count


//$starTime = microtime(true);
register_time_var('start');

if (false) {
    $seedValues = range(0, $treeSize-1); // not random values

    // ubt = unbalanced BT
    $ubt = buildBT($seedValues, false); // do not randomize values

    echo "First tree built \n";
    // bbt = balanced BT
    $bbt = buildBT($seedValues); // values randomized before building the tree
    echo "Second tree built \n";

    $bbt->balance();
    echo "Second tree balanced \n";

    $targets = range(0, $treeCount);
    shuffle($targets);

    register_time_var('buildData');

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($ubt->findLoop($target) === null) {
            var_dump("value not found", $target);
            $ubt->print();
        }
    }

    register_time_var('unbalancedBT_FindLoop');

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($ubt->findRecurse($target) === null) {
            var_dump("value not found", $target);
            $ubt->print();
        }
    }

    register_time_var('unbalancedBT_FindRecurse');

    for ($i = 0; $i < $treeCount; $i++) {
        $ubt->sortLoop();
    }

    register_time_var('unbalancedBT_SortLoop');

    /*for ($i = 0; $i < $treeCount; $i++) {
        // gives a segfault when treeSize > 33000
        $ubt->sortRecurse();
    }*/

    register_time_var('unbalancedBT_SortRecurse');

    // -----------------------------------------
    // BALANCED

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($bbt->findLoop($target) === null) {
            var_dump("value not found", $target);
            $bbt->print();
        }
    }

    register_time_var('balancedBT_FindLoop');

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($bbt->findRecurse($target) === null) {
            var_dump("value not found", $target);
            $bbt->print();
        }
    }

    register_time_var('balancedBT_FindRecurse');

    for ($i = 0; $i < $treeCount; $i++) {
        $bbt->sortLoop();
    }

    register_time_var('balancedBT_SortLoop');

    for ($i = 0; $i < $treeCount; $i++) {
        $bbt->sortRecurse();
    }

    register_time_var('balancedBT_SortRecurse');

    // ---------------------
    // array

    $seedValues = range(0, $treeSize - 1);

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        in_array($target, $seedValues);
    }

    register_time_var('in_array_sorted_values');

    for ($i = 0; $i < $treeCount; $i++) {
        sort($seedValues);
    }

    register_time_var('sort_sorted_values');

    shuffle($seedValues);

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        in_array($target, $seedValues);
    }

    register_time_var('in_array_random_values');

    for ($i = 0; $i < $treeCount; $i++) {
        shuffle($seedValues);
        sort($seedValues);
    }

    register_time_var('sort_random_values');

    extract(get_time_diffs());

    $str = <<<EOL
Tree count: $treeCount
Tree size: $treeSize
build data time :               $buildDataDiff s
---
PHP userland:
Unbalanced BT FindLoop():       $unbalancedBT_FindLoopDiff s
Unbalanced BT FindRecurse():    $unbalancedBT_FindRecurseDiff s
Unbalanced BT SortLoop():       $unbalancedBT_SortLoopDiff s
Unbalanced BT SortRecurse():    $unbalancedBT_SortRecurseDiff s

Balanced BT FindLoop():         $balancedBT_FindLoopDiff s
Balanced BT FindRecurse():      $balancedBT_FindRecurseDiff s
Balanced BT SortLoop():         $balancedBT_SortLoopDiff s
Balanced BT SortRecurse():      $balancedBT_SortRecurseDiff s

in_array with sorted values:    $in_array_sorted_valuesDiff s
sort with sorted values:        $sort_sorted_valuesDiff s
in_array with random values:    $in_array_random_valuesDiff s
sort with random values:        $sort_random_valuesDiff s (note: time is for suffle + sort)
EOL;
    echo "$str";

}
else {
    // -------
    // extension
    $seedValues = range(0, $treeSize - 1);

    $ubt = buildEBT($seedValues, false); // do not randomize values

    echo "First extension tree built \n";
    // bbt = balanced BT

    $bbt = buildEBT($seedValues); // values randomized before building the tree
    echo "Second extension tree built \n";

    $bbt->balance();
    echo "Second extension tree balanced \n";

    $targets = range(0, $treeSize - 1);
    shuffle($targets);

    register_time_var('ext_buildData');

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($ubt->findLoop($target) === null) {
            var_dump("value not found", $target);
        }
    }

    register_time_var('ext_unbalancedBT_FindLoop');

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($ubt->findRecurse($target) === null) {
            var_dump("value not found", $target);
        }
    }

    register_time_var('ext_unbalancedBT_FindRecurse');

    for ($i = 0; $i < $treeCount; $i++) {
        $ubt->sortLoop();
    }

    register_time_var('ext_unbalancedBT_SortLoop');

    /*for ($i = 0; $i < $treeCount; $i++) {
        $ubt->sortRecurse();
    }*/

    // register_time_var('ext_unbalancedBT_SortRecurse');

    // -----------------------------------------
    // BALANCED

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($bbt->findLoop($target) === null) {
            var_dump("value not found", $target);
        }
    }

    register_time_var('ext_balancedBT_FindLoop');

    for ($i = 0; $i < $treeCount; $i++) {
        $target = $targets[$i];
        if ($bbt->findRecurse($target) === null) {
            var_dump("value not found", $target);
        }
    }

    register_time_var('ext_balancedBT_FindRecurse');

    for ($i = 0; $i < $treeCount; $i++) {
        $bbt->sortLoop();
    }

    register_time_var('ext_balancedBT_SortLoop');

    for ($i = 0; $i < $treeCount; $i++) {
        $bbt->sortRecurse();
    }

    register_time_var('ext_balancedBT_SortRecurse');

    extract(get_time_diffs());

    $str = <<<EOL
<pre>
Tree count: $treeCount
Tree size: $treeSize
build data time :               $ext_buildDataDiff s
---
Zephir extension:
Unbalanced BT FindLoop():       $ext_unbalancedBT_FindLoopDiff s
Unbalanced BT FindRecurse():    $ext_unbalancedBT_FindRecurseDiff s
Unbalanced BT SortLoop():       $ext_unbalancedBT_SortLoopDiff s
Unbalanced BT SortRecurse():    $ext_unbalancedBT_SortRecurseDiff s

Balanced BT FindLoop():         $ext_balancedBT_FindLoopDiff s
Balanced BT FindRecurse():      $ext_balancedBT_FindRecurseDiff s
Balanced BT SortLoop():         $ext_balancedBT_SortLoopDiff s
Balanced BT SortRecurse():      $ext_balancedBT_SortRecurseDiff s
</pre>
EOL;
    echo "$str";

}
