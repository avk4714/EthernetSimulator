#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>

enum class NodeType {
    SENDER = 0,
    RECEIVER = 1,
};

// TODO: Update the structure to align with TCP comms protocol.
struct NodeData {
    std::string data;
    int length;
};

class Node {
 public:
    Node(NodeType type);
    ~Node() = default;

    // TODO: Add methods for the node class.
    // * User can add a message to queue.
    // * User can get message from the node.
    // * User only inputs/outputs with a string.
    // * Node perfoms transformation of string to transmissible packets.
 private:
    std::vector<NodeData> outgoing_buffer_;
    std::vector<NodeData> incoming_buffer_;
};

#endif NODE_H   // NODE_H