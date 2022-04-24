typedef string kademlia_id_t<>;
typedef string host_t<>;

struct kademlia_ping_t {
    kademlia_id_t id;
    host_t host;
    int port;
    int proto;
};

struct kademlia_store_t {
    kademlia_id_t key;
    char data<>;
};

struct kademlia_find_node_t {
    kademlia_id_t ids<>;
    host_t hosts<>;
    int ports<>;
    int protos<>;
    int numNodes;
};

struct kademlia_find_value_t {
    kademlia_id_t ids<>;
    host_t hosts<>;
    int ports<>;
    int protos<>;
    int numNodes;
    char data<>;
};

program MESSAGE_PROG {
    version MESSAGE_VERS {
        int kademlia_ping(kademlia_ping_t) = 1;

        int kademlia_store(kademlia_store_t) = 2;

        kademlia_find_node_t kademlia_find_node(kademlia_id_t) = 3;

        kademlia_find_value_t kademlia_find_value(kademlia_id_t) = 4;
    } = 1;
} = 0x20000001;
