# Monopoly-Board

## Overview

Monopoly board data class in C++ implemented using circular linked lists. Each node represents a property on the board. \
Tasks implemented: **Insert**, **Delete**, **Search**, **Display**, **Reverse**, **Sort**, **Merge**, **Update**,
**Count**, and **Convert**. \
Additionally, there are helper functions to get the information of the head and tail nodes.

### Monopoly Board Representation - Data Class

Each node/property contains:

- `propertyName` : std::string
- `propertyColor` : std::string
- `value` : int
- `rent` : int

Operator overloads:

- Operator `>` and operator `<` overloaded to lexicographically compare property names
- Operator `<<` overloaded to display relevant property information nicely

### Circular Linked List - Container Class

- Written using generic templates
- Easily adaptable depending on the data type
- Stores a pointer to the head node of the linked list

## Usage

When the program runs, it will execute the code in the `main` function. Provided in the function are already many
examples for creating circular linked list objects and utilizing the implemented methods to manipulate them.
Every function is documented to facilitate interactions with the user/programmer. The section below contains detailed
instructions on how to build and run the project.

## Build

CMake configuration files are included on the repository. To begin building with CMake, make sure that it's installed on the system with
a proper build environment for C++.
Now, open a terminal window in the project directory and execute the following code:

```
cmake -B build
cmake --build build
```

If the project was built successfully, you should now find a `Monopoly_Board` file inside the `build` folder which can be
run by the command `./Monopoly_Board`. There is also the option of creating a new CLion project with these files and building/running through the IDE.

Alternatively, to compile using g++ directly, execute the following code in a terminal shell while in the project directory:
```
mkdir build 
cd build
g++ ../monopoly_board.cpp -o Monopoly_Board
```

This should also output a `Monopoly_Board` file inside the `build` folder if successful. Run the same command from the CMake instructions to execute the resulting file.

## Runtime Analysis

### Insertion

#### Insert at Head:

```c++
void insertAtHead(T value) {
    auto *newNode = new Node<T>(value); // Allocate memory for a new node

    if (isListEmpty()) {
        headNode = newNode; // Update the headNode
        headNode->nextNode = headNode; // Link the last node back to the head
    } else {
        Node<T> *temp = getLastNode(); // Get the tail node

        newNode->nextNode = headNode; // Link the new node's next value to the current head
        headNode = newNode; // Update the headNode
        temp->nextNode = headNode; // Link the last node back to the head
    }
}
```

- Big(O) = **O(N)**
- Calls `getLastNode()` which contains a simple while loop that traverses through the whole linked list to retrieve the tail
  node
- All other operations are simple assignment/comparison that take up O(1) time complexity
- Best case is O(1) if the list is empty because it doesn't need to do any traversal

#### Insert at Tail:

```c++
void insertAtTail(T value) {
    auto *newNode = new Node<T>(value); // Allocate memory for a new node

    if (isListEmpty()) {
        headNode = newNode;
        headNode->nextNode = headNode; // List is now one element, link head back to itself
    } else {
        Node<T> *temp = getLastNode(); // Get the tail node

        temp->nextNode = newNode; // Link the previous tail node to the new tail node
        newNode->nextNode = headNode; // Link the tail node back to the head node
    }
}
```

- Big(O) = **O(N)**
- Similar to the function above, it needs to traverse the whole list with `getLastNode()` and therefore ends up at O(N)
- The traversal contains the only dominant term as the rest are simple operations costing O(1)
- Creating a new node and adjusting the pointers all cost O(1) and is the best case when the list is empty

#### Insert at Position:

```c++
void insertAtPosition(T value, const int position) {
    const int size = countNodes(); // Get size of the linked list

    if (position < 1 || position > size) {
        throw invalid_argument("Position must be greater than 0 and at most equal to the size of the list!");
    }

    if (position == 1) {
        insertAtHead(value);
        return;
    }

    if (position == size) {
        insertAtTail(value);
        return;
    }

    int count = 1; // Initialize count to 1
    auto *newNode = new Node<T>(value); // Allocate memory for the new node

    // Keep track of current and previous nodes
    Node<T> *temp = headNode;
    Node<T> *prev = nullptr;

    // Traverse the circular linked list
    do {
        if (count == position) {
            newNode->nextNode = temp; // Link the new node to the current node
            prev->nextNode = newNode; // Link the previous node to the new node
            return;
        }
        count++;
        prev = temp; // Set previous node to current node
        temp = temp->nextNode; // Set current node to the next node
    } while (temp != headNode);
}
```

- Big(O) = **O(N)**
- Calls `countNodes()` which contains a simple while loop to traverse the circular linked list taking O(N) time
- Traversing to the desired position could take O(1) best case or O(N) worst case depending on the position
- Total time complexity dominated by O(N), however, could take O(1) best case if position bounds were not checked

### Deletion

#### Delete at Head:

```c++
void deleteAtHead() {
    if (isListEmpty()) {
        cout << "List is empty! Nothing to delete!" << endl;
        return;
    }

    Node<T> *temp = getLastNode(); // Get the tail node

    // Handle only one node in list
    if (temp == headNode) {
        delete headNode;
        headNode = nullptr; // List is now empty
        return;
    }

    temp->nextNode = headNode->nextNode; // Link tail node to head's next node
    delete headNode; // Delete the head node
    headNode = temp->nextNode; // Update the head node to the next node
}
```

- Big(O) = **O(N)**
- Just like the other functions before, as the tail needs to link back to the head, a whole O(N) traversal is required\
- The other operations are simple and only cost O(1) and therefore are dominated by O(N)
- Best case is O(1) if there is a single element in the list as no traversal is required
- Worst case could also be O(1) if the function was overloaded to allow passing a tail pointer

#### Delete at Tail:

```c++
void deleteAtTail() {
    if (isListEmpty()) {
        cout << "List is empty! Nothing to delete!" << endl;
        return;
    }

    // Check if the circular linked list only has one node
    if (headNode->nextNode == headNode) {
        delete headNode;
        headNode = nullptr;
        return;
    }

    Node<T> *temp = headNode;
    // Traverse circular linked list until the node before the last
    while (temp->nextNode->nextNode != headNode) {
        temp = temp->nextNode;
    }

    delete temp->nextNode; // Delete last node and deallocate memory
    temp->nextNode = headNode; // Link the new last node back to the head
}
```

- Big(O) = **O(N)**
- Most of the operations are simple and only cost O(1) 
- Needs to traverse to N-2, costing O(N) and dominating the time complexity
- Best case is O(1) if the list is empty or only has one element

#### Delete at Position:

```c++
void deleteAtPosition(const int position) {
    if (isListEmpty()) {
        cout << "List is empty! Nothing to delete!" << endl;
        return;
    }

    const int size = countNodes(); // Get size of the linked list

    if (position < 1 || position > size) {
        throw invalid_argument("Position must be greater than 0 and at most equal to the size of the list!");
    }

    if (position == 1) {
        deleteAtHead();
        return;
    }

    if (position == size) {
        deleteAtTail();
        return;
    }

    int count = 1; // Initialize count to 1

    // Keep track of current and previous nodes
    Node<T> *temp = headNode;
    Node<T> *prev = nullptr;

    // Traverse the circular linked list
    do {
        if (count == position) {
            prev->nextNode = temp->nextNode;
            // Link the previous node to the node after the node marked for deletion
            delete temp; // Delete the current node
            return;
        }
        count++;
        prev = temp; // Set previous node to current node
        temp = temp->nextNode; // Set current node to the next node
    } while (temp != headNode);
}
```

- Big(O) = **O(N)**
- Size is determined by traversing through the whole list, costing O(N)
- Traversing to the desired position could take O(1) best case and O(N) worst case
- Worst case for whole function is O(N) + O(N) = O(N)
- Best case could be O(1) depending on the position and if size wasn't checked

### Search

```c++
Node<T> *search(T value, const bool print = false) {
    Node<T> *temp = headNode;
    int position = 1;

    // Traverse through the circular linked list
    do {
        // Check if all the data matches the specified node
        if (temp->data.isEqual(value)) {
            if (print) {
                cout << "Property found on the board at property location " << position << "!" << endl;
                cout << temp->data << endl;
            }
            return temp;
        }
        // Update positions for next iteration
        temp = temp->nextNode;
        position++;
    } while (temp != headNode);

    if (print) {
        cout << "Property not found on the board!" << endl;
    }
    return nullptr;
}
```

- Big(O) = **O(N)**
- Mostly simple assignment and comparison operations costing O(1)
- Traversal through the circular linked list costs O(N) and is the dominating term
- Best case would be O(1) if the value is found at the first node

