namespace ZephirAlgos;

class BinaryTree
{
    /**
     * @var BinaryTreeNode
     */
    public root;

    /**
     * @var BinaryTreeNode[]
     */
    public nodesPerKey = [];

    public function __construct(var root = null)
    {
        let this->root = root;
    }

    /**
     * @param BinaryTreeNode newNode
     */
    public function insert(<BinaryTreeNode> newNode) -> void
    {
        if (this->root === null) {
            let this->root = newNode;
            return;
        }

        var root = this->root;

        while (root !== null) {
            if (newNode->key < root->key) {
                if (root->left !== null) {
                    let root = root->left;
                    continue;
                } else {
                    let root->left = newNode;
                    let newNode->parent = root;
                    return;
                }
            }

            if (newNode->key > root->key) {
                if (root->right !== null) {
                    let root = root->right;
                    continue;
                } else {
                    let root->right = newNode;
                    let newNode->parent = root;
                    return;
                }
            }

            return;
        }
    }

    /**
     * @param int key
     * @return BinaryTreeNode|null
     */
    public function findLoop(int! key)
    {
        var node = this->root;

        loop {
            if (key === node->key) {
                return node;
            }

            if (key < node->key) {
                if (node->left !== null) {
                    let node = node->left;
                    continue;
                }
            } elseif (node->right !== null) {
                let node = node->right;
                continue;
            }

            return null;
        }
    }

    /**
     * @param int key
     * @param BinaryTreeNode node
     * @return BinaryTreeNode|null
     */
    public function findRecurse(int! key, <BinaryTreeNode> node = null)
    {
        if (node === null) {
            let node = this->root;
        }
        if (key === node->key) {
            return node;
        }

        if (key < node->key) {
            if (node->left !== null) {
                return this->findRecurse(key, node->left);
            }
        } elseif (node->right !== null) {
            return this->findRecurse(key, node->right);
        }

        return null;
    }

    public function balance() -> void
    {
        this->sortLoop();
        var sortedKeys = array_keys(this->nodesPerKey);
        let this->root = this->_balance(sortedKeys);
    }

    private function _balance(array sortedKeys) -> <BinaryTreeNode>
    {
        // to get a balanced tree, build it from a sorted array
        // split the array in two and take the middle value
        // the middle value become the new root
        // its left and right sub-trees are built from the remaining arrays

        int size = count(sortedKeys);
        if (size <= 0) {
            return;
        }

        array nodesPerKey = this->nodesPerKey;
        var node = null;

        if (size == 1) {
            let node = <BinaryTreeNode> nodesPerKey[ sortedKeys[0] ];
            let node->parent = null;
            let node->left = null;
            let node->right = null;
        }
        elseif (size == 2) {
            let node = <BinaryTreeNode> nodesPerKey[ sortedKeys[1] ];
            let node->parent = null;
            let node->right = null;

            let node->left = <BinaryTreeNode> nodesPerKey[ sortedKeys[0] ];
            var left = node->left;
            let left->parent = node;
            let left->left = null;
            let left->right = null;
        }
        else { // size >= 3
            int middleId = (int)(size / 2);
            array rightKeys = (array) array_splice(sortedKeys, middleId);
            // sortedKeys now represent the left values
            int middleKey = (int) array_shift(rightKeys);

            let node = nodesPerKey[middleKey];
            let node->parent = null;

            let node->left = this->_balance(sortedKeys);
            var left = node->left;
            let left->parent = node;

            let node->right = this->_balance(rightKeys);
            var right = node->right;
            let right->parent = node;
        }

        return node;
    }

    public function sortLoop() -> void
    {
        array flags = [];
        var node = this->root;
        let this->nodesPerKey = [];

        while (node !== null) { // isset is not valid here ?
            var key = node->key;

            if (! isset(flags[key])) {
                let flags[key] = 0;
            }

            if (flags[key] < 1) {
                let flags[key] = 1;

                if (node->left !== null) {
                    let node = node->left;
                    continue;
                }
            }

            if (flags[key] < 2) {
                let flags[key] = 2;

                let this->nodesPerKey[key] = node;
            }

            if (flags[key] < 3) {
                let flags[key] = 3;

                if (node->right !== null) {
                    let node = node->right;
                    continue;
                }
            }

            let node = node->parent;
        }
    }

    public function sortRecurse(<BinaryTreeNode> node = null) -> void
    {
        if (node === null) {
            let node = this->root;
            let this->nodesPerKey = [];
        }

        if (node->left !== null) {
            this->sortRecurse(node->left);
        }

        let this->nodesPerKey[node->key] = node;

        if (node->right !== null) {
            this->sortRecurse(node->right);
        }
    }

    /**
     * @return array | null
     */
    public function print(<BinaryTreeNode> node = null) -> array | null
    {
        boolean isRoot = false;

        if (node === null) { // isset not valid here
            let isRoot = true;
            let node = this->root;
            //echo "node is null, set node \n";
            //echo node->key."\n";
        }

        array nodeArray = (array) node->toArray();

        if (node->left !== null) {
            let nodeArray["l"] = this->print(node->left);
            //echo "has left \n";
        }

        if (node->right !== null) {
            let nodeArray["r"] = this->print(node->right);
        }

        if (isRoot) {
            echo "<pre>" . print_r(nodeArray, true) . "</pre>";
            return null;
        } else {
            return nodeArray;
        }
    }

    public function destroy() -> void
    {
        // it is twice slower to use $this->traverse() than a custom loop
        var key, node;
        for key, node in this->nodesPerKey {
            unset node->parent;
            unset node->left;
            unset node->right;
            unset this->nodesPerKey[key];
        }
        unset this->nodesPerKey;
        unset this->root;
    }
}
