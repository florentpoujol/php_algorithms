namespace Sort;

class BinaryTree
{
    /**
     * @var BinaryTreeNode
     */
    public root = null;

    /**
     * @var BinaryTreeNode[]
     */
    public nodesPerKey = [];

    public function __construct(root = null)
    {
        let this->root = root;
    }

    /**
     * @param BinaryTreeNode newNode
     */
    public function insert(newNode)
    {
        if (this->root == null) {
            let this->root = newNode;
            return;
        }

        var root = this->root;
        while (root != null) {
            if (newNode->key < root->key) {
                if (root->left) {
                    let root = root->left;
                    continue;
                } else {
                    let root->left = newNode;
                    let newNode->parent = root;
                    break;
                }
            }

            if (newNode->key > root->key) {
                if (root->right) {
                    let root = root->right;
                    continue;
                } else {
                    let root->right = newNode;
                    let newNode->parent = root;
                    break;
                }
            }

            if (newNode->key == root->key) {
                // in this implementation, do not allow to add another node with the same key
                return;
            }
        }
    }

    /**
     * @param mixed key
     * @return BinaryTreeNode|void
     */
    public function find(key)
    {
        var root = this->root;
        while (root != null) {
            if (key == root->key) {
                return root;
            }

            if (key < root->key) {
                if (root->left) {
                    let root = root->left;
                    continue;
                }
                return null;
            }

            if (key > root->key) {
                if (root->right) {
                    let root = root->right;
                    continue;
                }
                return null;
            }
        }
    }

    public function balance()
    {
        let this->nodesPerKey = this->toArray();
        var sortedKeys = array_keys(this->nodesPerKey);
        let this->root = this->_balance(sortedKeys);
    }

    private function _balance(array sortedKeys) -> <Sort\BinaryTreeNode>
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
            array right = (array) array_splice(sortedKeys, middleId);
            // sortedKeys now represent the left values
            int middleKey = (int) array_shift(right);

            let node = <BinaryTreeNode> nodesPerKey[middleKey];
            let node->parent = null;

            let node->left = this->_balance(sortedKeys);
            var left = node->left;
            let left->parent = node;

            let node->right = this->_balance(right);
            var _right = node->right;
            let _right->parent = node;
        }

        return node;
    }

    public function toArray() -> array
    {
        array a = [];
        array flags = [];
        var node = this->root;

        while (node != null) {
            var key = node->key;
            if (! isset(flags[key])) {
                let flags[key] = 0;
            }

            if (flags[key] < 1) {
                let flags[key] = 1;

                if (node->left) {
                    let node = node->left;
                    continue;
                }
            }

            let a[key] = node;

            if (flags[key] < 2) {
                let flags[key] = 2;

                if (node->right) {
                    let node = node->right;
                    continue;
                }
            }

            let node = node->parent;
        }

        return a;
    }
}
