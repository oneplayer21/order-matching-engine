# Low-Latency Limit Order Matching Engine

A high-performance Limit Order Book (LOB) and matching engine implemented in modern **C++17**. This project simulates the core infrastructure of a financial exchange (e.g., Nasdaq, Euronext, Binance), matching buy and sell orders based on strict **Price-Time Priority (FIFO)**.

## Core Features

* **Price-Time Priority Matching:** Executes orders strictly based on the best available price, followed by the time of arrival.
* **$O(1)$ Order Cancellation:** Achieves constant-time order lookups and deletions using a hybrid Hashmap and Doubly Linked-List architecture.
* **Partial & Full Matching:** Dynamically handles complex liquidity consumption across multiple price levels.
* **Memory Safety:** Modern C++ memory management without raw pointers (`new`/`delete`) to prevent memory leaks during high-frequency operations.
* **Edge-Case Protection:** Rejects invalid orders (e.g., zero quantity, zero price, duplicate IDs) before they reach the core engine.

## Architecture & Data Structures

In a financial matching engine, the speed of order insertion, matching, and cancellation is critical.

* **Order Books (Bids & Asks):** Implemented using `std::map` balanced binary search trees to maintain ordered price levels.
    * `Bids` are sorted in descending order (`std::greater`).
    * `Asks` are sorted in ascending order (`std::less`).
* **Price Levels (FIFO Queues):** Instead of standard queues, `std::list` (doubly linked lists) are used to store orders at a specific price level. This allows for mid-queue deletions without rebuilding the entire queue.
* **Order Indexing ($O(1)$ lookup):**
  A `std::unordered_map` tracks the exact memory location (iterator) of every active order. When a cancellation request arrives, the engine finds the order in $O(1)$ and erases it from the `std::list` in $O(1)$.


## Performance

Benchmarked system performance achieving a **throughput of ~5 million orders per second (avg. 200ns per order)** by compiling heavily optimized C++17 Release builds.
## Prerequisites & Building

This project uses **CMake** for build automation.

### Requirements
* A C++17 compatible compiler (GCC, Clang, or MSVC)
* CMake (>= 3.14)

### Build Instructions
```bash
# Clone the repository
git clone https://github.com/oneplayer21/order-matching-engine.git
cd order-matching-engine

# Create a build directory
mkdir build && cd build

# Generate Makefiles and compile
cmake ..
make

```
### To run the test suite
```bash
# Inside the build directory
./run_tests
```