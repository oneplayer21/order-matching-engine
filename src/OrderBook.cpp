//
// Created by timot on 03/03/2026.
//
#include "OrderBook.hpp"



    bool OrderBook::addOrder(Order order) {
        // If BUY -> Attempts to match with existing Asks.
        // If SELL -> Attempts to match with existing Bids.
        if (order.price > 0 && order.quantity > 0 && Index.find(order.id) == Index.end()) {
            if (order.side == Side::BUY) {
                processBuyOrder(order);
            }
            else if (order.side == Side::SELL) {
                processSellOrder(order);
            }
            return true;
        }
    return false;

    }

    bool OrderBook::cancelOrder(uint64_t orderId) {
        auto found = Index.find(orderId);
        //if no orderId matches we return false;
        if (found != Index.end()) {
            const Info& info = found->second;
            if (info.side == Side::BUY) {
                bids[info.price].erase(info.iterator);
                //if there is no more bids with this price we delete the node from the bids map
                if (bids[info.price].empty()) {
                    bids.erase(info.price);
                }
            }
            else {
                asks[info.price].erase(info.iterator);
                //if there is no more asks with this price we delete the node from the asks map
                if (asks[info.price].empty()) {
                    asks.erase(info.price);
                }
            }
            //We do not forget to delete the ID from the hashmap
            Index.erase(found);
            return true;
        }
        return false;
    }

    void OrderBook::printBook() {
        std::cout << "----- ASKS (Vendeurs) -----" << std::endl;
        for (auto ask = asks.rbegin(); ask != asks.rend(); ++ask) {
            uint32_t totalQty = 0;
            auto tempQueue = ask->second;
            while(!tempQueue.empty()) {
                totalQty += tempQueue.front().quantity;
                tempQueue.pop_front();
            }
            std::cout << "Prix: " << ask->first << " | Qté totale: " << totalQty << " | Nb Ordres: " << ask->second.size() << std::endl;
        }

        std::cout << "----- BIDS (Acheteurs) -----" << std::endl;
        for (auto bid = bids.begin(); bid != bids.end(); ++bid) {
            uint32_t totalQty = 0;
            auto tempQueue = bid->second;
            while(!tempQueue.empty()) {
                totalQty += tempQueue.front().quantity;
                tempQueue.pop_front();
            }
            std::cout << "Prix: " << bid->first << " | Qté totale: " << totalQty << " | Nb Ordres: " << bid->second.size() << std::endl;
        }
        std::cout << "---------------------------" << std::endl;
    }

    uint32_t OrderBook::matchOrder(Order& order, uint32_t remainingQuantity, std::list<Order>& list) {
        //While we still have stocks to buy/sell and there are still asks/bids for the price we continue to loop
        while (remainingQuantity > 0 && !list.empty()) {
            if (remainingQuantity > list.front().quantity) {
                //If we have more to sell/buy than we can then we close the position and delete the order from the hashmap & the correspoding map
                remainingQuantity -= list.front().quantity;
                auto temp = list.front().id;
                list.pop_front();
                Index.erase(temp);
            }
            else if (remainingQuantity <= list.front().quantity) {
                //If we have less or the correct quantity then we update the quantity in the list and leave the loop
                list.front().quantity -= remainingQuantity;
                remainingQuantity = 0;
                if (list.front().quantity == 0) {
                    //If we buy everything avalaible we also delete the listed order from the hashmap & the corresponding map
                    auto temp = list.front().id;
                    list.pop_front();
                    Index.erase(temp);
                }
                break;
            }
            else {
                break;
            }
        }
        return remainingQuantity;
    }
    void OrderBook::processBuyOrder(Order& order) {
        //While quantity is > 0 and there are asks avalaible we loop
        while (order.quantity > 0 && !asks.empty()) {
            auto currentAsk = asks.begin();
            if (order.price < currentAsk->first) {
                //If the price of the highest seller avalaible is > to our price then we stop
                break;
            }
            order.quantity = matchOrder(order, order.quantity, currentAsk->second);

            if (currentAsk->second.empty())
                //If we bought everything at that price we delete the node from the map
                asks.erase(currentAsk);
        }

        if (order.quantity > 0) {
            //If we haven't fulfilled the order and there are no asks at a good price remaining we add the remaining quantity to our bid table
            bids[order.price].push_back(order);
            Index[order.id] = Info(std::prev(bids[order.price].end()), order.side, order.price);
        }
    }

    void OrderBook::processSellOrder(Order& order) {
        //Same reasoning as processBuyOrder except this time we check if the highestBid is lower than the order's price
        while (order.quantity > 0 && !bids.empty()) {
            auto currentBid = bids.begin();
            if (order.price > currentBid->first) {

                break;
            }
            order.quantity = matchOrder(order, order.quantity, currentBid->second);

            if (currentBid->second.empty())
                bids.erase(currentBid);
        }

        if (order.quantity > 0) {
            asks[order.price].push_back(order);
            Index[order.id] = Info(std::prev(asks[order.price].end()), order.side, order.price);
        }
    }