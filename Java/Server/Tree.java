// Tree element class
class TreeElem<T extends Comparable<T>> {
    T elem;
    TreeElem<T> left;
    TreeElem<T> right;

    TreeElem(T elem) {
        this.elem = elem;
        left = null;
        right = null;
    }

    @Override
    public String toString() {
        return elem.toString();
    }
}

// Tree handling class. Generic type T must implement Comparable to compare elements for insertion into the tree.
public class Tree<T extends Comparable<T>> {
    private TreeElem<T> root;

    public Tree() {
        root = null;
    }

    public void insert(T elem) {
        root = ins(elem, root);
    }

    private TreeElem<T> ins(T elem, TreeElem<T> node) {
        if (node == null) return new TreeElem<>(elem);
        if (elem.compareTo(node.elem) <= 0)
            node.left = ins(elem, node.left);
        else
            node.right = ins(elem, node.right);
        return node;
    }

    public boolean isElement(T elem) {
        return isElem(elem, root);
    }

    private boolean isElem(T elem, TreeElem<T> node) {
        if (node == null) return false;
        if (elem.compareTo(node.elem) == 0) return true;
        if (elem.compareTo(node.elem) < 0)
            return isElem(elem, node.left);
        else
            return isElem(elem, node.right);
    }

    @Override
    public String toString() {
        return toS(root);
    }

    private String toS(TreeElem<T> node) {
        if (node != null)
            return "(" + node.elem + ":" + toS(node.left) + ":" + toS(node.right) + ")";
        return "()";
    }

    public void delete(T elem) {
        root = del(elem, root);
    }

    private TreeElem<T> del(T elem, TreeElem<T> node) {
        if (node == null) {
            return null;
        }

        int comparison = elem.compareTo(node.elem);
        if (comparison < 0) {
            node.left = del(elem, node.left);
        } else if (comparison > 0) {
            node.right = del(elem, node.right);
        } else {
            // Leaf node
            if (node.left == null && node.right == null) {
                return null;
            }
            // One right child
            else if (node.left == null) {
                return node.right;
            }
            // One left child
            else if (node.right == null) {
                return node.left;
            }
            // Two children
            else {
                TreeElem<T> minRight = findMin(node.right);
                node.elem = minRight.elem;
                node.right = del(minRight.elem, node.right);
            }
        }
        return node;
    }

    private TreeElem<T> findMin(TreeElem<T> node) {
        while (node.left != null) {
            node = node.left;
        }
        return node;
    }
}