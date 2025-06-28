#include "terrain.h"
#include <mcpp/mcpp.h>

ChangeNode::ChangeNode(mcpp::Coordinate c, mcpp::BlockType b)
    : coord(c), blockType(b), next(nullptr) {}

ChangeList::ChangeList() : head(nullptr), tail(nullptr) {}

// Destructor for ChangeList
ChangeList::~ChangeList() {
    while (head != nullptr) {
        ChangeNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// Add a change to the list
void ChangeList::addChange(mcpp::Coordinate coord, mcpp::BlockType blockType) {
    ChangeNode* newNode = new ChangeNode(coord, blockType);
    if (tail != nullptr) {
        tail->next = newNode;
    } else {
        head = newNode;  // If the list is empty, the new node is the head
    }
    tail = newNode;  // Update the tail to the new node
}

void ChangeList::revertChanges(mcpp::MinecraftConnection& mc) {
    ChangeNode* current = head;
    while (current != nullptr) {
        mc.setBlock(current->coord, current->blockType);  // Restore the block
        current = current->next;  // Move to the next node
    }
}
