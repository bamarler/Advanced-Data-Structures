import random

class SkipListNode:
    def __init__(self, key):
        self.key = key
        self.right = None
        self.bottom = None

class SkipList:
    def __init__(self, max_level, p=0.5):
        self.max_level = max_level
        self.p = p
        self.head = SkipListNode(None)
        self.levels = 1
        self.length = 0
    
    def lookup(self, target):
        current = self.head
        
        for i in range(self.levels):
            while current.right and current.right.key <= target:
                current = current.right

            if current.bottom:
                current = current.bottom
            elif current.key == target:
                return current
            
        return None

    def random_height(self):
        level = 1
        while random.random() < self.p and level < self.max_level:
            level += 1
        return level
    
    def insert(self, target):
        update = [None] * self.max_level
        current = self.head
        
        for i in range(self.levels - 1, -1, -1):
            while current.right and current.right.key <= target:
                current = current.right

            update[i] = current

            if current.bottom:
                current = current.bottom
        
        height = self.random_height()

        if height > self.levels:
            for i in range(self.levels, height):
                new_head = SkipListNode(None)
                new_head.bottom = self.head
                self.head = new_head
                update[i] = new_head
            self.levels = height
        
        for i in range(height):
            new_node = SkipListNode(target)
            new_node.right = update[i].right
            update[i].right = new_node
            if i > 0:
                new_node.bottom = update[i - 1].right

        self.length += 1
    
    def delete(self, target):
        update = [None] * self.max_level
        current = self.head
        
        for i in range(self.levels - 1, -1, -1):
            while current.right and current.right.key < target:
                current = current.right

            if current.right and current.right.key == target:
                update[i] = current

            if current.bottom:
                current = current.bottom
        
        i = 0
        while i < self.max_level and update[i]:
            update[i].right = update[i].right.right
            i += 1

        if update[0]:
            self.length -= 1
    
    def visualize(self):
        # Initialize a matrix with self.levels rows and self.length columns
        matrix = [["  " for _ in range(self.length + 1)] for _ in range(self.levels)]

        heads = [None] * self.levels

        # Find the bottom level
        bottom_level = self.head
        for i in range(self.levels):
            if bottom_level:
                heads[i] = bottom_level
                bottom_level = bottom_level.bottom

        # Fill the bottom level in the matrix
        col_widths = []
        row = self.levels - 1
        col = 0
        bottom_level = heads[row]
        for j in range(self.length + 1):
            if bottom_level and bottom_level.key is not None:
                matrix[row][col] = str(bottom_level.key)
                col_widths.append(len(matrix[row][col]))
                col += 1
            bottom_level = bottom_level.right

        row -= 1
        while row >= 0:
            current = heads[row].right
            col = 0
            while current:
                # Find the column for this node by checking below
                while col < self.length and matrix[row + 1][col] != str(current.key):
                    #matrix[row][col] = " " * col_widths[col]
                    col += 1
                matrix[row][col] = str(current.key)
                current = current.right
            # Move to the next higher level
            row -= 1  # Move up a row


        for row in matrix:
            print(" | ".join(row))
        
        print("\n")


SL = SkipList(10)

SL.insert(20)
SL.visualize()
SL.insert(40)
SL.visualize()
SL.insert(10)
SL.visualize()
SL.insert(20)
SL.visualize()
SL.insert(5)
SL.visualize()
SL.insert(80)
SL.visualize()
SL.delete(20)
SL.visualize()
SL.insert(100)
SL.visualize()
SL.insert(20)
SL.visualize()
SL.insert(30)
SL.visualize()
SL.delete(5)
SL.visualize()
SL.insert(50)
SL.visualize()
SL.lookup(80)
