/* Simple Ethernet Simulator Design
Design Requirements:
1. Two nodes capable of sending and receiving messages.
2. An ethernet channel that transports the messages between the nodes.
3. A simulator that traverses through the events as time proceeds.
*/

#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>

class Node {
 public:
    Node(int id) : id_(id) {}
    ~Node() = default;

    int get_id() const {
        return id_;
    }
    void send_message(std::string message, Node &receiver) {
        // Receiver node incoming buffer queue receivers the message.
        receiver.incoming_buffer_.push(message);
        std::cout << "Message received by Node: " << receiver.get_id() << std::endl;
    }
    void process_messages() {
        if (!incoming_buffer_.empty()) {
            std::cout << "Processing received messages ..." << std::endl;
            while (!incoming_buffer_.empty()) {
                std::cout << incoming_buffer_.front() << std::endl;
                incoming_buffer_.pop();
            }
            std::cout << "Processed all messages." << std::endl;
        }
    }
 private:
    int id_;
    std::queue<std::string> incoming_buffer_;
};

class EthernetChannel {
 public:
    EthernetChannel() : is_connected_(false) {}
    void connect() {
        is_connected_ = true;
        std::cout << "Network connected!" << std::endl;
    }
    void disconnect() {
        is_connected_ = false;
        std::cout << "Network disconnected!" << std::endl;
    }
    void add_node(Node &node) {
        if (nodes.find(node.get_id()) != nodes.end()) {
            std::cerr << "Error: Node with id " << node.get_id() << " already exists on the network. Change the node id before proceeding." << std::endl;
            std::exit(1);
        }
        nodes.insert({node.get_id(), node});
        std::cout << "New node with id: " << node.get_id() << " added." << std::endl;
    }
    void send_message(int sender_node_id, int receiver_node_id, std::string message) {
        if (nodes.find(sender_node_id) == nodes.end() || nodes.find(receiver_node_id) == nodes.end()) {
            std::cerr << "Error: Sender and/or Receiver node doesn't exist on the channel." << std::endl;
            std::exit(1);
        }
        if (!is_connected_) {
            std::cout << "Info: Network is not connected." << std::endl;
        }
        nodes.at(sender_node_id).send_message(message, nodes.at(receiver_node_id));
    }
    Node access_node(int node_id) {
        return nodes.at(node_id);
    }
 private:
    std::unordered_map<int, Node> nodes;
    bool is_connected_;
};

class Simulator {
 public:
    Simulator(EthernetChannel &channel) : channel_(channel) {}
    void setup() {
        // Sets up the ethernet simulator with two nodes with ids 1 & 2.
        Node node_1(1);
        Node node_2(2);
        channel_.add_node(node_1);
        channel_.add_node(node_2);
        channel_.connect();

    }
    void run(std::vector<std::tuple<int, int, std::string>> messages) {
        for (const auto &message : messages) {
            channel_.send_message(std::get<0>(message), std::get<1>(message), std::get<2>(message));
        }
        // Display all the messages received by Node 1.
        channel_.access_node(1).process_messages();
        // Display all the message received by Node 2.
        channel_.access_node(2).process_messages();
    }
 private:
    EthernetChannel channel_;
};

int main() {
    EthernetChannel channel;
    Simulator sim(channel);
    sim.setup();
    
    std::vector<std::tuple<int, int, std::string>> message_transmission{
        std::make_tuple<int, int, std::string>(1, 2, "Hello from Node 1 to Node 2."),
        std::make_tuple<int, int, std::string>(2, 1, "Hello from Node 2 to Node 1")
    };
    sim.run(message_transmission);
    return 0;
}