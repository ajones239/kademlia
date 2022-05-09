typedef string kademlia_id_t<>;
typedef string host_t<>;

struct kademlia_ping_t {
    kademlia_id_t id;
    host_t host;
    unsigned long proto;
};

struct kademlia_data {
    char data<>;
};

struct kademlia_store_t {
    kademlia_id_t key;
    struct kademlia_data data;
};

struct kademlia_internal_host {
    host_t host;
};

struct kademlia_internal_host {
    kademlia_id_t host;
};

struct kademlia_internal_protos {
    unsigned long protos<>;
};

struct kademlia_find_node_t {
    struct kademlia_internal_id ids<>;
    struct kademlia_internal_host hosts<>;
    struct kademlia_internal_protos protos;
    int numNodes;
};

struct kademlia_find_value_t {
    struct kademlia_internal_id ids<>;
    struct kademlia_internal_host hosts<>;
    struct kademlia_internal_protos protos;
    int numNodes;
    struct kademlia_data data;
};

program MESSAGE_PROG {
    version MESSAGE_VERS {
        int kademlia_ping(kademlia_ping_t) = 1;

        int kademlia_store(kademlia_store_t) = 2;

        kademlia_find_node_t kademlia_find_node(kademlia_id_t) = 3;

        kademlia_find_value_t kademlia_find_value(kademlia_id_t) = 4;
    } = 1;
} = 0x20000001;
