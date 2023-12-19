class Node:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None


class BinarySearchTree:
    def __init__(self):
        self.root = None

    def __contains__(self, value):
        cur = self.root
        while cur:
            if value < cur.value:
                cur = cur.left
            elif cur.value < value:
                cur = cur.right
            else:
                return True
        return False 

    def add(self, value):
        cur = self.root
        if cur is None:
            self.root = Node(value)
            return
        while cur:
            if value < cur.value:
                if cur.left is None:
                    cur.left = Node(value)
                    return
                else:
                    cur = cur.left
            elif cur.value < value:
                if cur.right is None:
                    cur.right = Node(value)
                    return
                else:
                    cur = cur.right
            else:
                return
