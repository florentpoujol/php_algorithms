namespace Sort;

class BinaryTreeNode
{
    /**
     * @var number|string
     */
    public key;

    /**
     * @var mixed
     */
    public value;

    /**
     * @var BinaryTreeNode
     */
    public parent;

    /**
     * @var BinaryTreeNode
     */
    public left;

    /**
     * @var BinaryTreeNode
     */
    public right;

    public function __construct(value = null, key = null, parent = null, left = null, right = null)
    {
        let this->value = value;
        //var theKey = key;
        if (key == null) {
            let key = value;
        }
        let this->key = key;
        let this->parent = parent;
        let this->left = left;
        let this->right= right;
    }

    public function toArray() -> array
    {
        var parent = null;
        if (this->parent) {
            let parent = this->parent;
            let parent = parent->key;
        }
        array a = [
            "k" : this->key, "v" : this->value,
            "p" : parent,
            "l" : null, "r" : null
        ];
        return a;
    }
}
