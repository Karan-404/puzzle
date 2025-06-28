#ifndef TERRAIN_H
#define TERRAIN_H

#include <mcpp/mcpp.h>

struct ChangeNode {
    mcpp::Coordinate coord;   
    mcpp::BlockType blockType; 
    ChangeNode* next;         

    // Constructor to initialize the node
    ChangeNode(mcpp::Coordinate c, mcpp::BlockType b);
};

// Linked list structure to store the changes
class ChangeList {
private:
    ChangeNode* head;   // Pointer to the first node
    ChangeNode* tail;   // Pointer to the last node

public:
    ChangeList();

    ~ChangeList();

    // Add a change to the list
    void addChange(mcpp::Coordinate coord, mcpp::BlockType blockType);

    // Revert the changes
    void revertChanges(mcpp::MinecraftConnection& mc);
};

#endif // TERRAIN_H
