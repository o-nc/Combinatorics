
#ifndef COMBINATORICS_LOCKFREECOUNTER_HPP
#define COMBINATORICS_LOCKFREECOUNTER_HPP

#include <atomic>
#include <generator>
#include <memory>
#include <unordered_map>

//check what naming convention want to use. what is used by std?
// check structs for alignment and being always lock free
// change memory orders
// change bucket_count
namespace combis {

    // add constraint on type T, must implement operator+(T const& other);
    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    class LockfreeCounter {
        struct Node {
            Key const key {};
            std::atomic<T> count {};

            Node() = delete; // invariant: deleted bc we need key to have a valid value at all time that a Node object is in scope
            Node(Key const& key, T const& value) : key(key), count(value) {}

            void increment(T const& value) {
                auto old = count.load(std::memory_order_seq_cst);
                while (not count.compare_exchange_weak(old, old + value)) { // memory order?
                    // busy retry
                }
            }
        };

        class LockfreeForwardBucket {
        public:
            struct LinkedNode;
            using LinkedNodePtr = std::atomic<std::shared_ptr<LinkedNode>>;
        private:
            LinkedNodePtr head = nullptr;

            void increment_at(LinkedNodePtr& nodePtr, Key const& key, T const& value) {
                auto linkedNode = nodePtr.load(std::memory_order_seq_cst);
                if (linkedNode == nullptr) {
                    auto newNodePtr = std::make_shared<LinkedNode>(key, value);
                    if (auto const inserted = nodePtr.compare_exchange_strong(linkedNode, newNodePtr); inserted) // memory order?
                        return;
                }
                // from this point, linkedNode is guaranteed not to be nullptr and to have a valid constructed node
                if (linkedNode->node.key == key) {
                    linkedNode->node.increment(value);
                    return;
                }
                else
                    increment_at(linkedNode->next, key, value);
            }
        public:
            struct LinkedNode {
                Node node;
                LinkedNodePtr next = nullptr;
                LinkedNode(Key const& key, T const& value) : node(key, value) {}
            };

            void increment(Key const& key, T const& value) {
                increment_at(head, key, value);
            }

            std::generator<Node const&> nodes() const {
                // precondition: assumes that no thread is modifying the container
                auto linkedNode = head.load(std::memory_order_seq_cst);
                while (linkedNode != nullptr) {
                    co_yield linkedNode->node;
                    linkedNode = linkedNode->next.load(std::memory_order_seq_cst);
                }
            }
        };

        static constexpr std::size_t bucket_count {100uz}; // how many hashes do we expect? are we able to prevent collisions (larger if no, smaller if yes)?
        using Buckets = std::array<LockfreeForwardBucket, bucket_count>;
        Buckets buckets;

    public:
        void increment(Key const& key, T const& value) {
            auto const hash = Hash()(key);
            auto const index = hash % bucket_count;
            buckets[index].increment(key, value);
        }

        // public interface to convert into a non-atomic std::unordered_map<Key, T>.
        using unordered_map = std::unordered_map<Key, T, Hash, KeyEqual>;
        explicit operator unordered_map() const {
            // precondition: assumes that no thread is modifying the container
            unordered_map result;
            for (auto const& bucket : buckets)
                for (auto const& node : bucket.nodes())
                    result[node.key] = node.count.load(std::memory_order_seq_cst);
            return result;
        }
    };
}

#endif
