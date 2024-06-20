import queue

class Node:
    def __init__(self, node_id):
        self.id = node_id
        self.incoming_buffer = queue.Queue()

    def get_id(self):
        return self.id

    def send_message(self, message, receiver):
        receiver.incoming_buffer.put(message)
        print(f"Message received by Node: {receiver.get_id()}")

    def process_messages(self):
        if not self.incoming_buffer.empty():
            print("Processing received messages ...")
            while not self.incoming_buffer.empty():
                print(self.incoming_buffer.get())
            print("Processed all messages.")

class EthernetChannel:
    def __init__(self):
        self.nodes = {}
        self.is_connected = False

    def connect(self):
        self.is_connected = True
        print("Network connected!")

    def disconnect(self):
        self.is_connected = False
        print("Network disconnected!")

    def add_node(self, node):
        if node.get_id() in self.nodes:
            raise ValueError(f"Error: Node with id {node.get_id()} already exists on the network.")
        self.nodes[node.get_id()] = node
        print(f"New node with id: {node.get_id()} added.")

    def send_message(self, sender_node_id, receiver_node_id, message):
        if sender_node_id not in self.nodes or receiver_node_id not in self.nodes:
            raise ValueError("Error: Sender and/or Receiver node doesn't exist on the channel.")
        if not self.is_connected:
            print("Info: Network is not connected.")
        self.nodes[sender_node_id].send_message(message, self.nodes[receiver_node_id])

    def access_node(self, node_id):
        if node_id in self.nodes:
            return self.nodes[node_id]
        else:
            raise ValueError(f"Error: Node with id {node_id} doesn't exist.")

class Simulator:
    def __init__(self, channel):
        self.channel = channel

    def setup(self):
        # Sets up the ethernet simulator with two nodes with ids 1 & 2.
        node_1 = Node(1)
        node_2 = Node(2)
        self.channel.add_node(node_1)
        self.channel.add_node(node_2)
        self.channel.connect()

    def run(self, messages):
        for message in messages:
            sender, receiver, msg = message
            self.channel.send_message(sender, receiver, msg)
        
        # Display all the messages received by Node 1.
        self.channel.access_node(1).process_messages()
        # Display all the messages received by Node 2.
        self.channel.access_node(2).process_messages()

if __name__ == "__main__":
    channel = EthernetChannel()
    sim = Simulator(channel)
    sim.setup()
    
    message_transmission = [
        (1, 2, "Hello from Node 1 to Node 2."),
        (2, 1, "Hello from Node 2 to Node 1")
    ]
    
    sim.run(message_transmission)
