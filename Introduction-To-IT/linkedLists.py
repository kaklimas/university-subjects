from ctypes.wintypes import LARGE_INTEGER
import random

class Word:
    def __init__(self, string="", next=None):
        self.string = string
        self.next= next
    
    def printList(self):
        length = self.getLength()
        head = self
        while length > 0:
            if length == 1:
                print(head.string)
            else:
                print(head.string, end=" -> ")
            head = head.next
            length -= 1

    def getLength(self):
        head = self
        length = 0
        
        if head.is_cycle():
            head1 = self
            head2 = self

            while True:
                head1 = head1.next    
                head2 = head2.next
                head2 = head2.next
                if head1 == head2:
                    break
            head1 = head1.next
            length += 1
            while head1 != head2:
                head1 = head1.next
                length += 1
            
                
        else:
            while head is not None:
                head = head.next
                length += 1  
                
        return length

    def is_cycle(self):
        head1 = self
        head2 = self
        
        if head1 is None:
            return False
        
        while True:
            head1 = head1.next
            if head1 is None:
                return False
            
            head2 = head2.next
            if head2 is None:
                return False
            
            head2 = head2.next
            if head2 is None:
                return False
            
            if head1 == head2:
                return True
    
class Node:
    def __init__(self, val=None, next=None):
        self.next = next
        self.val = val
        

    def printList(self):
        start = self
        head = self
        while head is not None:
            if head.next is None:
                print(head.val)
            else:
                print(head.val, end=" -> ")
            head = head.next
    

    
    def is_cycle(self):
        head1 = self
        head2 = self
        
        if head1 is None:
            return False
        
        while True:
            head1 = head1.next
            if head1 is None:
                return False
            
            head2 = head2.next
            if head2 is None:
                return False
            
            head2 = head2.next
            if head2 is None:
                return False
            
            if head1 == head2:
                return True
    
    def insert(self, val):
        head = self
        
        while head.next is not None:
            head = head.next
        head.next = Node(val)
        
    # elements of list are sorted by nodes value
    def makeListUniq(self):
        curr = self
        prev = Node()
        
        print("List before: ", end="")
        self.printList()
        
        while curr is not None:
            if curr.val != prev.val:
                prev = curr
                curr = curr.next
            else:
                prev.next = curr.next
                curr = prev.next
        
        print("\n List after: ", end="")
        self.printList()
    
    def getLength(self):
        head = self
        length = 0
        
        if head.is_cycle():
            head1 = self
            head2 = self

            while True:
                head1 = head1.next    
                head2 = head2.next
                head2 = head2.next
                if head1 == head2:
                    break
            head1 = head1.next
            length += 1
            while head1 != head2:
                head1 = head1.next
                length += 1
            
                
        else:
            while head is not None:
                head = head.next
                length += 1  
                
        return length
    
def remove(nodeList, val):
    head = nodeList
    prev = None
    
    while head is not None and head.val != val:
        prev = head
        head = head.next
    
    if prev is None:
        return head.next

    else:
        prev.next = head.next
        head.next = None    
        return nodeList

#check if head1 contains node2
def contains(node1, node2):
    
    while node1 is not None:
        if node1.val == node2.val:
            tmp1 = node1
            tmp2 = node2
            while tmp2 is not None and tmp1.val == tmp2.val:
                tmp1 = tmp1.next
                tmp2 = tmp2.next
            if tmp2 is None:
                return True
        else:
            node1 = node1.next
    
    return False

def breakUp(node):
    rejected = 0
    
    evenPositive = Node()
    oddNegative = Node()
    
    body1 = evenPositive
    body2 = oddNegative
    
    while node is not None:
        if node.val < 0 and node.val % 2 == 1:
            body2.next = node
            body2 = body2.next
        elif node.val > 0 and node.val % 2 == 0:
            body1.next = node
            body1 = body1.next
        else:
            rejected += 1
        node = node.next
    
    body1.next = None
    body2.next = None
    if evenPositive.next is not None:
        evenPositive = evenPositive.next
    if oddNegative.next is not None:
        oddNegative = oddNegative.next
    
    evenPositive.printList()
    oddNegative.printList()
    
    return rejected

# Word class excercises

# string are uniq in list
def sortedAdd(node, string):
    # check wheter string2 is larger than string1
    def larger(string1, string2):
        n = len(string1)
        return string1[n-1] < string2[0]
    
    curr = node.next
    prev = node
    length = node.getLength()
    
    #find the smallest string
    while larger(prev.string, curr.string):
        prev = curr
        curr = curr.next
    
    
    while larger(curr.string, string) and length > 0:
        prev = curr
        curr = curr.next
        length -= 1
    
    new = Word(string, curr)
    prev.next = new
    return new

# Testing section
# tab = [Node(random.randint(1, 10)) for _ in range(10)]
# for i in range(0, len(tab)-1):
#     tab[i].next = tab[i+1]    
# tab[0].printList()

# a = Node(3)
# a.printList()
# a.insert(5)
# a.insert(6)
# a.insert(8)
# a.insert(11)
# a.printList()
# a = remove(a, 11)
# a.printList()

# a = Node(1, Node(2, Node(3, Node(4))))
# c = Node(1, Node(2, Node(3, Node(4))))
# b = Node(2, Node(2, Node(4)))
# print(contains(a, b))

# list = Node(-5, Node(3, Node(-12, Node(8, Node(2, Node(-1, Node(-3)))))))
# print(breakUp(list))

# a = Word("a", Word("b", Word("c")))
# tmp = a
# while tmp.next is not None:
#     tmp = tmp.next
# tmp.next = a

# a.printList()
# a = sortedAdd(a, "d")

# a = sortedAdd(a, "a")
# a.printList()
# End of testing section


class BST_Node:
    def __init__(self, val=None):
        self.left = None
        self.right = None
        self.val = val
        
    # node's values are uniq
    def insert(self, node):
        root = self
        prev = None
        while root is not None:
            prev = root
            if root.val > node.val:
                root = root.left
                
            else:
                root = root.right
        if prev.val > node.val:
            prev.left = node
        else:
            prev.right = node

def draw(node):
    root = node
    if root is None:
        return
    if root.left is not None:
        draw(root.left)
        print(root.left.val)
    if root.right is not None:
        draw(root.right)
        print(root.right.val)
    return

def getList(node):
    arrayList = []
    root = node
    
    def dive(root):
        if root is None:
            return
        if root.left is not None:
            dive(root.left)
            arrayList.append(root.left.val)
        if root.right is not None:
            dive(root.right)
            arrayList.append(root.right.val)
        return
    dive(root)
    print(arrayList)
    return arrayList    

A = BST_Node(4)
B = BST_Node(2)
C = BST_Node(5)
D = BST_Node(1)
E = BST_Node(3)
F = BST_Node(8)

A.left = B
A.right = C

B.left = D
B.right = E
C.right = F

draw(A)

getList(A)