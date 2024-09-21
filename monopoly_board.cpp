#include <iostream>
#include <string>
#include <utility>

using namespace std;

// Data class to store a string and an integer
class MonopolyBoard {
public:
    string propertyName;
    string propertyColor;
    int value;
    int rent;

    // Constructor to initialize default values using initializer list
    MonopolyBoard() : value(0), rent(0) {
    }

    // Constructor with given param values
    MonopolyBoard(string propertyName, string propertyColor, const int value, const int rent) {
        this->propertyName = std::move(propertyName);
        this->propertyColor = std::move(propertyColor);
        this->value = value;
        this->rent = rent;
    }

    // Determine if two objects are equal
    [[nodiscard]] bool isEqual(const MonopolyBoard &other) const {
        return (this->propertyName == other.propertyName && this->propertyColor == other.propertyColor && this->value ==
                other.value && this->rent == other.rent);
    }

    // Get the color of the property
    [[nodiscard]] string getColor() const {
        return propertyColor;
    }

    // Display compact node information
    void print() const {
        cout << "(" << propertyName << ", " << propertyColor << ", " << value << ", " << rent << ")" << endl;
    }
};

// Operator overload to lexicographically compare property names
bool operator<(const MonopolyBoard &lhs, const MonopolyBoard &rhs) {
    return lhs.propertyName < rhs.propertyName;
}

// Operator overload to lexicographically compare property names
bool operator>(const MonopolyBoard &lhs, const MonopolyBoard &rhs) {
    return rhs < lhs;
}

// Operator overload for prettier print
std::ostream &operator<<(std::ostream &os, const MonopolyBoard &board) {
    cout << "Name: " << board.propertyName << endl;
    cout << "Color: " << board.propertyColor << endl;
    cout << "Value: $" << board.value << endl;
    cout << "Rent: $" << board.rent;
    return os;
}

// Template Node class
template<typename T>
class Node {
public:
    T data;
    Node *nextNode;

    explicit Node(T value) {
        data = value;
        nextNode = nullptr;
    }
};

// Template LinkedList class
template<typename T>
class CircularLinkedList {
private:
    Node<T> *headNode;

public:
    CircularLinkedList() {
        headNode = nullptr;
    }

    /**
    * Insert a node at the head of a circular linked list.
    *
    * @param value The node to insert.
    */
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

    /**
    * Insert a node at the tail of a circular linked list.
    *
    * @param value The node to insert.
    */
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

    /**
    * Insert a node in the middle of a circular linked list.
    *
    * @param value The node to insert.
    * @param position The 1-based index to insert the node at.
    *
    * @throws invalid_argument Thrown if position is less than 1 or greater than the size of the list
    */
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

    // Delete the node at the head of a circular linked list
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

    // Delete the node at the tail of a circular linked list
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

    /**
    * Delete a node in the middle of a circular linked list.
    *
    * @param position The 1-based index to insert the node at.
    *
    * @throws invalid_argument Thrown if position is less than 1 or greater than the size of the list.
    */
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

    /**
    * Search a circular linked list for a specific node.
    *
    * @param value The node to search for.
    * @param print Display node information if found or an error message.
    *
    * @return The node found in the search or `nullptr` if not found
    */
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

    /**
    * Displays the nodes in a linked list.
    *
    * @param circular Print circular linked list if 'true' or linear linked list if 'false'.
    *
    */
    void printList(const bool circular = true) {
        if (isListEmpty()) {
            cout << "No properties on the board!" << endl;
            return;
        }

        Node<T> *temp = headNode;
        // Display non-circular linked list
        if (!circular) {
            // Traverse through the linked list
            while (temp != nullptr) {
                temp->data.print(); // Print the object's data
                cout << " -> ";
                temp = temp->nextNode; // Update temp to the next node
            }
            cout << "NULL"; // Last node links to nullptr
            return;
        }

        temp = headNode;
        // Traverse through circular linked list
        do {
            temp->data.print();
            cout << " -> ";
            temp = temp->nextNode;
        } while (temp != headNode);
        headNode->data.print(); // Last node links back to the head node
    }

    // Reverse the nodes in a circular linked list
    void reverseCLList() {
        // To reverse the list we need to know the current and previous nodes
        Node<T> *curr = headNode;
        Node<T> *prev = nullptr;

        // Traverse through the circular linked list
        do {
            Node<T> *next = curr->nextNode; // Keep track of the original next node
            curr->nextNode = prev; // Reverse the next node position
            prev = curr; // Set the previous node to the current node
            curr = next; // Update the current node to the original next node
        } while (curr != headNode); // Break loop if reached end of list

        headNode->nextNode = prev; // Link the last node back to the start
        headNode = prev; // Update the headNode
    }

    // Sort a circular linked list using Bubble Sort
    void sortCLList() {
        // Don't sort if the list is empty or only has one node
        if (isListEmpty() || headNode->nextNode == headNode) {
            return;
        }

        Node<T> *end = nullptr;
        bool swapped;

        // Bubble Sort algorithm
        do {
            swapped = false;
            Node<T> *curr = headNode;

            // Traverse the list until we reach the end
            while (curr->nextNode != headNode && curr->nextNode != end) {
                // Checks property names lexicographically (alphabetical)
                if (curr->data > curr->nextNode->data) {
                    // Swap data between current and next node
                    auto temp = curr->data;
                    curr->data = curr->nextNode->data;
                    curr->nextNode->data = temp;
                    swapped = true;
                }
                curr = curr->nextNode; // Set the current node to the next node
            }
            end = curr; // Shorten the sorted portion of the list
        } while (swapped);
    }

    // Display information about the head node
    void printHeadNode() {
        if (isListEmpty()) {
            cout << "List is empty! Can't print head node!" << endl;
            return;
        }

        // Print out important head node information
        cout << headNode->data << endl;
    }

    // Display information about the tail node
    void printLastNode() {
        if (isListEmpty()) {
            cout << "List is empty! Can't print tail node!" << endl;
            return;
        }

        Node<T> *temp = getLastNode(); // Get the tail node
        // Print out important tail node information
        cout << temp->data << endl;
    }

    /**
    * Determine if a linked list is empty.
    *
    * @return `true` if the list is empty or `false` if it contains nodes.
    */
    bool isListEmpty() {
        // If the headNode is null, then the list is empty
        return headNode == nullptr;
    }

    // Count the amount of nodes in a circular linked list
    int countNodes() {
        if (isListEmpty()) {
            return 0;
        }

        int count = 0; // Initialize count as zero
        Node<T> *temp = headNode; // Initialize a temporary pointer to the head of the list

        // Traverse through the circular linked list
        do {
            temp = temp->nextNode; // update temp pointer to next node
            count++; // Increment the count
        } while (temp != headNode);

        return count;
    }

    // Extra function to return the last node of a circular linked list
    Node<T> *getLastNode() {
        Node<T> *temp = headNode;

        while (temp->nextNode != headNode) {
            temp = temp->nextNode;
        }

        return temp;
    }

    // Extra function to return the middle node of a circular linked list
    Node<T> *getMiddle() {
        Node<T> *fast = headNode;
        Node<T> *slow = headNode;

        do {
            slow = slow->nextNode;
            fast = fast->nextNode->nextNode;
        } while (fast != headNode && fast->nextNode != headNode);

        return slow;
    }

    // Method to de-circularize a list
    void convertCLList() {
        if (isListEmpty()) {
            return;
        }

        Node<T> *tail = getLastNode(); // Get the tail node
        tail->nextNode = nullptr; // Link the tail node to NULL
    }

    /**
    * Update the data of a specific node in a circular linked list.
    *
    * @param value The node whose data needs to be updated in the list.
    * @param update The node containing updated data.
    */
    void updateNodeValue(T value, T update) {
        if (isListEmpty()) {
            cout << "List is empty! Can't update node value!" << endl;
            return;
        }

        // Search the list for the node
        Node<T> *searchNode = search(value);
        if (searchNode == nullptr) {
            cout << "Node not found on the board! Nothing to update!" << endl;
            return;
        }
        searchNode->data = update; // Update the node data
    }

    /**
    * Update the data of a node in a circular linked list with a position-based approach.
    *
    * @param position The position of the node in the list.
    * @param update The node containing the updated information.
    *
    * @throws invalid_argument Thrown if position is less than 1 or greater than the size of the list.
    * @overload
    */
    void updateNodeValue(int position, T update) {
        if (isListEmpty()) {
            cout << "List is empty! Nothing to update!" << endl;
            return;
        }

        if (position < 1 || position > countNodes()) {
            throw invalid_argument("Position must be greater than 0 and at most equal to the size of the list!");
        }

        int count = 1;

        // Traverse through the circular linked list
        Node<T> *temp = headNode;
        do {
            // Check if indicated position is reached
            if (position == count) {
                temp->data = update; // Update the data
                return;
            }
            // Increment positions and counters
            count++;
            temp = temp->nextNode;
        } while (temp != headNode);
    }

    // Display all nodes with a certain color
    void displaySpecificColorNode(string color) {
        if (isListEmpty()) {
            return;
        }

        auto new_list = new CircularLinkedList<T>; // Create a new linked list
        bool match = false; // Keep track if the node was found in the list

        Node<T> *temp = headNode;

        // Traverse through the circular linked list
        do {
            // Check if the property has the wanted color
            if (temp->data.getColor() == color) {
                new_list->insertAtHead(temp->data); // Insert the node at the head of the new list
                new_list->printHeadNode(); // Display information about the added node
                cout << endl;
                match = true;
            }
            temp = temp->nextNode; // Update temp pointer to the next node
        } while (temp != headNode);

        // Text displayed if no nodes contained the property color
        if (!match) {
            cout << "No properties found with that property color!" << endl << endl;
        }
    }

    // Merge two circular linked lists
    void mergeCLList(const CircularLinkedList<T> &other) {
        if (other.headNode == nullptr) {
            return;
        }

        if (isListEmpty()) {
            headNode = other.headNode;
            return;
        }

        // Copy second list to a new list to avoid altering it
        auto new_list = new CircularLinkedList<T>;
        Node<T> *temp = other.headNode;
        do {
            new_list->insertAtTail(temp->data); // Insert the nodes into new list
            temp = temp->nextNode;
        } while (temp != other.headNode);

        // Get the tail of both lists
        Node<T> *tail = getLastNode();
        Node<T> *tail2 = new_list->getLastNode();

        tail->nextNode = new_list->headNode; // Link the tail of the first list to the head of the second list
        tail2->nextNode = headNode; // Link the tail of the second list back to the head of the first list
    }
};

// Main function to demonstrate the LinkedList class
int main() {
    // Create a new circular linked list object
    CircularLinkedList<MonopolyBoard> list;

    // Insert node at the head
    list.insertAtHead(MonopolyBoard("Boardwalk", "Dark Blue", 400, 50));
    cout << "List after head insertion:" << endl;
    list.printList();
    cout << endl;

    // Delete the head node
    list.deleteAtHead();
    cout << "List after head deletion:" << endl;
    list.printList();
    cout << "MonopolyBoard size: " << list.countNodes() << endl;
    cout << endl;

    // Insert node at the head
    list.insertAtHead(MonopolyBoard("Boardwalk", "Dark Blue", 400, 50));
    cout << "List after head insertion:" << endl;
    list.printList();
    cout << endl;

    // Delete the tail node
    list.deleteAtTail();
    cout << "List after tail deletion:" << endl;
    list.printList();
    cout << endl;

    // Insert node at the head
    list.insertAtHead(MonopolyBoard("Boardwalk", "Dark Blue", 400, 50));
    cout << "List after head insertion:" << endl;
    list.printList();
    cout << endl;

    // Insert node at the head
    list.insertAtHead(MonopolyBoard("Park Place", "Dark Blue", 175, 35));
    cout << "List after head insertion:" << endl;
    list.printList();
    cout << endl;

    // Insert node at the head
    list.insertAtHead(MonopolyBoard("Tennessee Avenue", "Orange", 90, 14));
    cout << "List after head insertion:" << endl;
    list.printList();
    cout << endl;

    // Insert node at the tail
    list.insertAtTail(MonopolyBoard("New York Avenue", "Orange", 100, 16));
    cout << "List after tail insertion:" << endl;
    list.printList();
    cout << endl;

    // Insert node at a specific position
    list.insertAtPosition(MonopolyBoard("Pacific Avenue", "Green", 150, 26), 3);
    cout << "List after position insertion:" << endl;
    list.printList();
    cout << endl;

    // Delete node at a specific position
    list.deleteAtPosition(3);
    cout << "List after position deletion:" << endl;
    list.printList();
    cout << endl;

    // Sort the circular linked list lexicographically
    list.sortCLList();
    cout << "List after being sorted:" << endl;
    list.printList();
    cout << endl;

    // Print out head node information
    cout << "First property information:" << endl;
    list.printHeadNode();
    cout << endl;

    // Print out tail node information
    cout << "Last property information:" << endl;
    list.printLastNode();
    cout << endl;

    // Reverse the circular linked list
    list.reverseCLList();
    cout << "List after being reversed:" << endl;
    list.printList();
    cout << endl;

    // Delete the head node
    list.deleteAtHead();
    cout << "List after head deletion:" << endl;
    list.printList();
    // Print out the list size
    cout << "MonopolyBoard size: " << list.countNodes() << endl;
    cout << endl;

    // Delete the tail node
    list.deleteAtTail();
    cout << "List after tail deletion:" << endl;
    list.printList();
    // Print out the list size
    cout << "MonopolyBoard size: " << list.countNodes() << endl;
    cout << endl;

    // Search the list for a specific node
    cout << "Searching for property..." << endl;
    list.search(MonopolyBoard("Boardwalk", "Dark Blue", 400, 50), true);
    cout << endl;

    cout << "Searching for property..." << endl;
    list.search(MonopolyBoard("New York Avenue", "Orange", 100, 16), true);
    cout << endl;

    // Create a new circular linked list object
    CircularLinkedList<MonopolyBoard> list2;
    list2.insertAtHead(MonopolyBoard("Marvin Gardens", "Yellow", 140, 24));
    list2.insertAtTail(MonopolyBoard("Atlantic Avenue", "Yellow", 130, 22));

    // Merge the two circular linked lists
    list.mergeCLList(list2);
    cout << "List after concatenation:" << endl;
    list.printList();
    cout << endl;

    // Display nodes of a specific color
    cout << "Properties found with Yellow color:" << endl;
    list.displaySpecificColorNode("Yellow");

    cout << "Properties found with Black color:" << endl;
    list.displaySpecificColorNode("Black");

    // Update specific node values
    list.updateNodeValue(MonopolyBoard("Atlantic Avenue", "Yellow", 130, 22),
                         MonopolyBoard("Pennsylvania Avenue", "Green", 160, 28));
    cout << "List after node update:" << endl;
    list.printList();
    cout << endl;

    list.updateNodeValue(MonopolyBoard(), MonopolyBoard());
    cout << "List after node update:" << endl;
    list.printList();
    cout << endl;

    list.updateNodeValue(4, MonopolyBoard("Virginia Avenue", "Pink", 80, 12));
    cout << "List after node update:" << endl;
    list.printList();
    cout << endl;

    // Convert circular linked list to linear linked list
    list.convertCLList();
    cout << "List after conversion:" << endl;
    list.printList(false); // Print linear linked list
    cout << endl;

    return 0;
}
