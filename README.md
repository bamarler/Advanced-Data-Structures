# Advanced Data Structures

## Description
This project was used as a place to implement the data structures discussed in Northeastern's CS 5800, Graduate Algorithms, course. Specifically, I implemented Binomial Heaps, Hash Tables, Red Black Trees, Skip Lists, and a Push Relabel flow. As a note, this was my first attempt to code in C++ so please forgive any code smells.

### Skip List
This was a simple data structure that I implemented in python. It uses levels to improve runtime for lookups, insertions, and deletions. The list as a whole can be thought of a graph with the head at the top left, and it navigates by moving as far right as it can at each level from top to bottom. When a new item is inserted, it chooses a random height and corrects the linkages. An example sequence of events is below

```
Generated elements: [15, 96, 58, 9, 35, 75, 71, 82, 73, 15, 49, 95, 74, 82, 5, 18, 52, 60, 88, 13, 58, 34, 23, 14, 100, 85, 40, 31, 81, 49]

Initial SkipList:
                                            40
                        18                  40
                        18                  40                      60
            14          18          34  35  40                      60                      82          88
            14          18  23      34  35  40              58      60                      82          88      96
5  9  13  14  15  15  18  23  31  34  35  40  49  49  52  58  58  60  71  73  74  75  81  82  82  85  88  95  96  100

Enter a command ('insert', 'delete', or press Enter to exit): insert
Enter a value to insert: 78
Inserted 78. Updated SkipList:
                                            40
                        18                  40
                        18                  40                      60
            14          18          34  35  40                      60                  78      82          88
            14          18  23      34  35  40              58      60                  78      82          88      96
5  9  13  14  15  15  18  23  31  34  35  40  49  49  52  58  58  60  71  73  74  75  78  81  82  82  85  88  95  96  100

Enter a command ('insert', 'delete', or press Enter to exit): delete
Enter a value to delete: 18
Deleted 18. Updated SkipList:
                                        40
                                        40
                                        40                      60
            14                  34  35  40                      60                  78      82          88
            14          23      34  35  40              58      60                  78      82          88      96
5  9  13  14  15  15  23  31  34  35  40  49  49  52  58  58  60  71  73  74  75  78  81  82  82  85  88  95  96  100
```

### Binomial Heap
This was the first datastructure I decided to implement in C++. It involved maintaining a linked list of rooted min heaps. I implemented the Insert, Minimum, Extract Min, Decrease Key, and Delete operations. Because after every operation the min heaps are maintained, the extract min operation is quick and could be used in various priority queue implementations. Even though the underlying code works, the visualization is a little iffy. Below is an example stream of operations.

```
Heap after all insertions:
|-- Key: 0 (Order: 1)
    |-- Key: 67 (Order: 0)

|-- Key: 5 (Order: 3)
    |-- Key: 8 (Order: 2)
    |-- Key: 14 (Order: 1)
    |-- Key: 82 (Order: 0)
        |-- Key: 67 (Order: 1)
        |-- Key: 88 (Order: 0)
        |-- Key: 63 (Order: 0)
            |-- Key: 84 (Order: 0)


What do you want to do? (Insert, Minimum, ExtractMin, DecreaseKey, DeleteNode, end)
Insert
Enter the key to insert: 33
|-- Key: 33 (Order: 0)

|-- Key: 0 (Order: 1)
    |-- Key: 67 (Order: 0)

|-- Key: 5 (Order: 3)
    |-- Key: 8 (Order: 2)
    |-- Key: 14 (Order: 1)
    |-- Key: 82 (Order: 0)
        |-- Key: 67 (Order: 1)
        |-- Key: 88 (Order: 0)
        |-- Key: 63 (Order: 0)
            |-- Key: 84 (Order: 0)


What do you want to do? (Insert, Minimum, ExtractMin, DecreaseKey, DeleteNode, end)
Insert
Enter the key to insert: 22
|-- Key: 0 (Order: 2)
    |-- Key: 22 (Order: 1)
    |-- Key: 67 (Order: 0)
        |-- Key: 33 (Order: 0)

|-- Key: 5 (Order: 3)
    |-- Key: 8 (Order: 2)
    |-- Key: 14 (Order: 1)
    |-- Key: 82 (Order: 0)
        |-- Key: 67 (Order: 1)
        |-- Key: 88 (Order: 0)
        |-- Key: 63 (Order: 0)
            |-- Key: 84 (Order: 0)


What do you want to do? (Insert, Minimum, ExtractMin, DecreaseKey, DeleteNode, end)
Minimum
Minimum key: 0

What do you want to do? (Insert, Minimum, ExtractMin, DecreaseKey, DeleteNode, end)
ExtractMin
Minimum element extracted.
|-- Key: 67 (Order: 0)

|-- Key: 22 (Order: 1)
    |-- Key: 33 (Order: 0)

|-- Key: 5 (Order: 3)
    |-- Key: 8 (Order: 2)
    |-- Key: 14 (Order: 1)
    |-- Key: 82 (Order: 0)
        |-- Key: 67 (Order: 1)
        |-- Key: 88 (Order: 0)
        |-- Key: 63 (Order: 0)
            |-- Key: 84 (Order: 0)


What do you want to do? (Insert, Minimum, ExtractMin, DecreaseKey, DeleteNode, end)
DecreaseKey
Enter the key to decrease: 82
Enter the new key: 2
|-- Key: 67 (Order: 0)

|-- Key: 22 (Order: 1)
    |-- Key: 33 (Order: 0)

|-- Key: 2 (Order: 3)
    |-- Key: 8 (Order: 2)
    |-- Key: 14 (Order: 1)
    |-- Key: 5 (Order: 0)
        |-- Key: 67 (Order: 1)
        |-- Key: 88 (Order: 0)
        |-- Key: 63 (Order: 0)
            |-- Key: 84 (Order: 0)


What do you want to do? (Insert, Minimum, ExtractMin, DecreaseKey, DeleteNode, end)
DeleteNode
Enter the key to delete: 2
|-- Key: 5 (Order: 1)
    |-- Key: 67 (Order: 0)

|-- Key: 8 (Order: 3)
    |-- Key: 14 (Order: 2)
    |-- Key: 67 (Order: 1)
    |-- Key: 88 (Order: 0)
        |-- Key: 22 (Order: 1)
        |-- Key: 63 (Order: 0)
        |-- Key: 84 (Order: 0)
            |-- Key: 33 (Order: 0)
```

### Hash Table
Again, the hash table data structure was implemented in C++. I decided to test two different hash function, a simple randomHash and a slightly more complex MurmurHash. Implementing MurmurHash was interesting because it allowed me to take advantage of the languages bitshift and xor operators. To test the hash functions and data structure as a whole, I read in "Alice In Wonderland" and stored the words. As analysis, I printed a histogram of collision list lengths and the variance. An example with a size of 100 is below.

```
Enter the size of the Hash table: 100

Reading Alice in Wonderland...

Histogram of Collision Lists' Lengths:
13 | * (1)
14 | ******* (7)
15 | **** (4)
16 | * (1)
17 | ****** (6)
18 | ******* (7)
19 | ****** (6)
20 | *** (3)
21 | ************* (13)
22 | *************** (15)
23 | ********** (10)
24 | **** (4)
25 | ********* (9)
26 | ** (2)
27 | ***** (5)
28 | *** (3)
29 | ** (2)
30 | * (1)
31 |  (0)
32 |  (0)
33 | * (1)

Variance of collision list lengths: 17.1475

Lengths of the 10% longest lists are: 33 30 29 29 28 28 28 27 27 27
```

### Red Black Tree
This was complicated data structure. In essence, a red black tree is heap that always maintains a height of log(n), preventing a worst case height of n in a standard heap. It does this by coloring each node as red or black and following a set of rules to shift the roots around. I implemented the insert, search, min, max, successor and predecessor operations. Unfortunately, I did not implement a visualization and do not have the time now :(

### Push Relabel
This was less so a data structure, but an implementation of the push relabel algorithm to determine max flow for a graph. Again I chose to complete this in C++. I used the Boost library to construct the graphs, and the built in 'push_relabel_max_flow' function to compare against my function. An example test output is below.

```
Capacities:
0 -> 1 : 16
0 -> 2 : 13
1 -> 2 : 10
1 -> 3 : 12
2 -> 1 : 4
2 -> 4 : 14
3 -> 2 : 9
3 -> 5 : 20
4 -> 3 : 7
4 -> 5 : 4
Expected Max flow: 23
Residuals:
0 -> 1 : 13
0 -> 2 : 10
1 -> 2 : 1
1 -> 3 : 12
2 -> 1 : 0
2 -> 4 : 11
3 -> 2 : 0
3 -> 5 : 19
4 -> 3 : 7
4 -> 5 : 4
Actual Max flow: 23
Residuals:
0 -> 1 : 16
0 -> 2 : 7
1 -> 2 : 4
1 -> 3 : 12
2 -> 1 : 0
2 -> 4 : 11
3 -> 2 : 0
3 -> 5 : 19
4 -> 3 : 7
4 -> 5 : 4
```
