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

    public function __construct(int! value = null)
    {
        let this->value = value;
        let this->key = value;
    }

    public function toArray() -> array
    {
        var parentKey = null;
        if (this->parent !== null) {
            var _parent = this->parent;
            let parentKey = _parent->key;
        }
        return [
            "k" : this->key, "v" : this->value,
            "p" : parentKey,
            "l" : null, "r" : null
        ];
    }
}
