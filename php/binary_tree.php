<?php

require_once 'BinaryTree.php';

function buildBT(array $randomValues): BinaryTree
{
    $tree = new BinaryTree;
    $size = count($randomValues);
    while ($size-- > 0) {
        $value = array_pop($randomValues);
        $node = new BinaryTreeNode($value);
        $tree->insert($node);
    }
    return $tree;
}

function buildEBT(array $randomValues): Sort\BinaryTree
{
    $tree = new Sort\BinaryTree;
    $size = count($randomValues);
    while ($size-- > 0) {
        $value = array_pop($randomValues);
        $node = new Sort\BinaryTreeNode($value);
        $tree->insert($node);
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


$starTime = microtime(true);

$randomValues = range(0, $treeSize-1);
shuffle($randomValues);

$bt = buildBT($randomValues);

shuffle($randomValues);
$bst = buildBT($randomValues);
$bst->balance();

shuffle($randomValues);
$ebt = buildEBT($randomValues);
//var_dump($ebt->toArray());
$ebt->balance();

shuffle($randomValues);

$buildDataTime = microtime(true);
$buildDataDiff = $buildDataTime - $starTime;


for ($i = 0; $i < $treeCount; $i++) {
    $target = mt_rand(0, $treeSize-1);
    if ($bt->find($target) === null) {
        var_dump("value not found bt", $target);
        $bt->print();
    }
}

$bstSearchTime = microtime(true);
$bstSearchDiff = $bstSearchTime - $buildDataTime;

for ($i = 0; $i < $treeCount; $i++) {
    $target = mt_rand(0, $treeSize-1);
    if ($bst->find($target) === null) {
        var_dump("value not found bst ", $target);
        $bst->print();
    }
}

$bbstSearchTime = microtime(true);
$bbstSearchDiff = $bbstSearchTime - $bstSearchTime;

for ($i = 0; $i < $treeCount; $i++) {
    $target = mt_rand(0, $treeSize-1);
    if ($ebt->find($target) === null) {
        var_dump("value not found ebt ", $target);
        $ebt->print();
    }
}

$ebtSearchTime = microtime(true);
$ebtSearchDiff = $ebtSearchTime - $bbstSearchTime;

for ($i = 0; $i < $treeCount; $i++) {
    $target = mt_rand(0, $treeSize-1);
    in_array($target, $randomValues);
}

$inArraySearchTime = microtime(true);
$inArraySearchDiff = $inArraySearchTime - $ebtSearchTime;

// -------------------

for ($i = 0; $i < $treeCount; $i++) {
    $bt->toArray();
}

$bstSortTime = microtime(true);
$bstSortDiff = $bstSortTime - $inArraySearchTime;

for ($i = 0; $i < $treeCount; $i++) {
    $bst->toArray();
}

$bbstSortTime = microtime(true);
$bbstSortDiff = $bbstSortTime - $bstSortTime;

for ($i = 0; $i < $treeCount; $i++) {
    $ebt->toArray();
}

$ebtSortTime = microtime(true);
$ebtSortDiff = $ebtSortTime - $bbstSortTime;

for ($i = 0; $i < $treeCount; $i++) {
    sort($randomValues);
}

$sortTime = microtime(true);
$sortDiff = $sortTime - $ebtSortTime;


// execute the C script and retrive the last line of its input (the one that begins by "C")
// $cTime = exec("./c/builds/merge_sort $treeCount $treeSize | grep C");
$cTime = "";
// calculating C time with PHP whould gives a time slightly higher

$str = <<<EOL
<pre>
Array count: $treeCount
Array size: $treeSize
build data time :           $buildDataDiff s
---
php BT search :             $bstSearchDiff s
php Balanced BT search :    $bbstSearchDiff s
PHP Extension search :      $ebtSearchDiff s
php built-in in_array() :   $inArraySearchDiff s
---
php BT sort :               $bstSortDiff s
php Balanced BT sort :      $bbstSortDiff s
PHP Extension sort :        $ebtSortDiff s
php built-in sort() :       $sortDiff s
$cTime
</pre>
EOL;
echo "$str";