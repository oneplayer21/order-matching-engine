//
// Created by timot on 03/03/2026.
//
#pragma once
#ifndef ORDER_MATCHING_ENGINE_ORDERBOOK_HPP
#define ORDER_MATCHING_ENGINE_ORDERBOOK_HPP

//
// Created by timot on 03/03/2026.
//
#include <iostream>
#include <cstdint>
#include <map>
#include <queue>
#include <list>
#include <unordered_map>

enum class Side { BUY, SELL };

//Order informations
struct Order {
    uint64_t id;
    Side side;
    uint32_t price;
    uint32_t quantity;
};

//Informations for the hashmap
struct Info {
    std::list<Order>::iterator iterator;
    Side side;
    uint32_t price;

    Info() = default;

    Info(std::list<Order>::iterator end, Side side, uint32_t price) : iterator(end), side(side), price(price) {}
};

//OrderBook
class OrderBook {
private:
    // Bids (Buyers) : sorted by decreasing price (std::greater)
    std::map<uint32_t, std::list<Order>, std::greater<uint32_t>> bids;

    // Asks (Sellers) : sorted by increasing price (std::less par défaut)
    std::map<uint32_t, std::list<Order>, std::less<uint32_t>> asks;
    // Hashmap to find order and delete them quickly
    std::unordered_map<uint64_t, Info> Index;

    //Subfunction used to match orders
    uint32_t matchOrder(Order& order, uint32_t remainingQuantity, std::list<Order>& list);
    // Process used when buying an order
    void processBuyOrder(Order& order);
    // Process used when selling an order
    void processSellOrder(Order& order);


public:
    OrderBook() = default;
    // Adds and order and tries to match it
    bool addOrder(Order order);

    // Cancels an order and removes it from the OrderBook via its ID
    bool cancelOrder(uint64_t orderId);

    // Shows the current state of the OrderBook
    void printBook();

};


#endif //ORDER_MATCHING_ENGINE_ORDERBOOK_HPP