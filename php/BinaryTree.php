<?php


class BinaryTreeNode
{
    /**
     * @var number|string
     */
    public $key;

    /**
     * @var mixed
     */
    public $value;

    /**
     * @var BinaryTreeNode
     */
    public $parent;

    /**
     * @var BinaryTreeNode
     */
    public $left;

    /**
     * @var BinaryTreeNode
     */
    public $right;

    public function __construct($value = null, $key = null, BinaryTreeNode $parent = null, BinaryTreeNode $left = null, BinaryTreeNode $right = null)
    {
        $this->value = $value;
        $this->key = $key ?? $value;
        $this->parent = $parent;
        $this->left = $left;
        $this->right= $right;
    }

    public function toArray(): array
    {
        $parent = null;
        if ($this->parent) {
            $parent = $this->parent->key;
        }
        return [
            'k' => $this->key, 'v' => $this->value,
            'p' => $parent,
            'l' => null, 'r' => null,
        ];
    }
}


class BinaryTree
{
    /**
     * @var BinaryTreeNode
     */
    public $root;

    /**
     * @var BinaryTreeNode[]
     */
    public $nodesPerKey = [];

    public function __construct(BinaryTreeNode $root = null)
    {
        $this->root = $root;
    }

    /**
     * Traverse the tree in the order provided, giving each node as argument to the callback.
     * Continue to traverse until the end or a callback returns a non null value.
     *
     * @param callable $callback
     * @param int $order  "post" or "pre"
     * @return mixed
     */
    public function traverse(Callable $callback, string $order = 'in', BinaryTreeNode $node = null)
    {
        // this implementation, not using a recursive call to traverse
        // is ORDERS OF MAGNITUDE faster than using recursive calls
        if ($node === null) {
            $node = $this->root;
        }

        if ($order === 'pre') {
            $return = $callback($node);
            if ($return !== null) {
                return $return;
            }
        }

        if ($node->left) {
            $return = $this->traverse($callback, $order, $node->left);
            if ($return !== null) {
                return $return;
            }
        }

        if ($order === 'in') {
            $return = $callback($node);
            if ($return !== null) {
                return $return;
            }
        }

        if ($node->right) {
            $return = $this->traverse($callback, $order, $node->right);
            if ($return !== null) {
                return $return;
            }
        }

        if ($order === 'post') {
            $return = $callback($node);
            if ($return !== null) {
                return $return;
            }
        }
    }

    /**
     * @param BinaryTreeNode $newNode
     */
    public function insert(BinaryTreeNode $newNode)
    {
        if ($this->root === null) {
            $this->root = $newNode;
            return;
        }

        $root = $this->root;
        while ($root !== null) {
            if ($newNode->key < $root->key) {
                if ($root->left) {
                    $root = $root->left;
                    continue;
                } else {
                    $root->left = $newNode;
                    $newNode->parent = $root;
                    break;
                }
            }

            if ($newNode->key > $root->key) {
                if ($root->right) {
                    $root = $root->right;
                    continue;
                } else {
                    $root->right = $newNode;
                    $newNode->parent = $root;
                    break;
                }
            }

            if ($newNode->key === $root->key) {
                // in this implementation, do not allow to add another node with the same key
                return;
            }
        }
    }

    /**
     * @param mixed $key
     * @return BinaryTreeNode|void
     */
    public function find($key)
    {
        $root = $this->root;
        while ($root !== null) {
            if ($key === $root->key) {
                return $root;
            }

            if ($key < $root->key) {
                if ($root->left) {
                    $root = $root->left;
                    continue;
                }
                return null;
            }

            if ($key > $root->key) {
                if ($root->right) {
                    $root = $root->right;
                    continue;
                }
                return null;
            }
        }
    }

    public function balance()
    {
        $this->nodesPerKey = $this->toArray();
//        ksort($this->nodesPerKey);
        $sortedKeys = array_keys($this->nodesPerKey);
        $this->root = $this->_balance($sortedKeys);
    }

    private function _balance(array $sortedKeys): BinaryTreeNode
    {
        // to get a balanced tree, build it from a sorted array
        // split the array in two and take the middle value
        // the middle value become the new root
        // its left and right sub-trees are built from the remaining arrays

        $size = count($sortedKeys);
        if ($size <= 0) {
            return null;
        }

        $nodesPerKey = $this->nodesPerKey;
        $node = null;

        if ($size === 1) {
            $node = $nodesPerKey[ $sortedKeys[0] ];
            $node->parent = null;
            $node->left = null;
            $node->right = null;
        }
        elseif ($size === 2) {
            $node = $nodesPerKey[ $sortedKeys[1] ];
            $node->parent = null;
            $node->right = null;

            $node->left = $nodesPerKey[ $sortedKeys[0] ];
            $node->left->parent = $node;
            $node->left->left = null;
            $node->left->right = null;
        }
        else { // $size >= 3
            $middleId = (int)($size / 2);
            $right = array_splice($sortedKeys, $middleId);
            // $sortedKeys now represent the left values
            $middleKey = array_shift($right);

            $node = $nodesPerKey[$middleKey];
            $node->parent = null;

            $node->left = $this->_balance($sortedKeys);
            $node->left->parent = $node;

            $node->right = $this->_balance($right);
            $node->right->parent = $node;
        }

        return $node;
    }

    public function toArray(): array
    {
        // it is twice slower to use $this->traverse() than a custom loop
        $a = [];
        $flags = [];
        $node = $this->root;

        while ($node !== null) {
            $key = $node->key;
            if (! isset($flags[$key])) {
                $flags[$key] = 0;
            }

            if ($flags[$key] < 1) {
                $flags[$key] = 1;

                if ($node->left) {
                    $node = $node->left;
                    continue;
                }
            }

            $a[$key] = $node;

            if ($flags[$key] < 2) {
                $flags[$key] = 2;

                if ($node->right) {
                    $node = $node->right;
                    continue;
                }
            }

            $node = $node->parent;
        }

        return $a;
    }

    public function print()
    {
        $a = [];
        $this->traverse(
            function (BinaryTreeNode $node) use (&$a) {
                $nodeInfo = $node->toArray();
                if ($node->left) {
                    $nodeInfo['l'] = $a[$node->left->key];
                }
                if ($node->right) {
                    $nodeInfo['r'] = $a[$node->right->key];
                }
                $a[$node->key] = $nodeInfo;
            }
        , 'post');

        echo '<pre>' . print_r($a[$this->root->key], true) . '</pre>';
    }
}
