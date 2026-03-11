//
// Created by timot on 03/03/2026.
//
#include "OrderBook.hpp"
#include <random>
#include <chrono>
#include <iostream>
#include <gtest/gtest.h>

TEST(BasicTests, AddAndDelete) {
    OrderBook book;
    EXPECT_TRUE(book.addOrder({1, Side::SELL, 100, 10}));
    EXPECT_FALSE(book.addOrder({1, Side::SELL, 100, 10}));
    EXPECT_FALSE(book.addOrder({2, Side::SELL, 00, 10}));
    EXPECT_FALSE(book.addOrder({3, Side::SELL, 100, 00}));
    EXPECT_TRUE(book.cancelOrder(1));
    EXPECT_FALSE(book.cancelOrder(1));
}

TEST(BasicTests, FullMatch) {
    OrderBook book;
    EXPECT_TRUE(book.addOrder({1, Side::SELL, 100, 10}));
    EXPECT_TRUE(book.addOrder({2, Side::BUY, 100, 10}));
    EXPECT_FALSE(book.cancelOrder(1));
    EXPECT_FALSE(book.cancelOrder(2));
}

TEST(BasicTests, PartialMatch) {
    OrderBook book;
    EXPECT_TRUE(book.addOrder({1, Side::SELL, 100, 20}));
    EXPECT_TRUE(book.addOrder({2, Side::BUY, 100, 5}));
    EXPECT_TRUE(book.cancelOrder(1));
    EXPECT_FALSE(book.cancelOrder(2));
}

TEST(BasicTests, SellLower) {
    OrderBook book;
    EXPECT_TRUE(book.addOrder({1, Side::BUY, 100, 10}));
    EXPECT_TRUE(book.addOrder({2, Side::BUY, 105, 10}));
    EXPECT_TRUE(book.addOrder({3, Side::SELL, 95, 25}));
    EXPECT_FALSE(book.cancelOrder(1));
    EXPECT_FALSE(book.cancelOrder(2));
    EXPECT_TRUE(book.cancelOrder(3));
}

TEST(PerformanceTests, MassiveStressTest) {
    OrderBook book;
    const int NUM_ORDERS = 1000000;

    std::mt19937 gen(42);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<uint32_t> priceDist(90, 110);
    std::uniform_int_distribution<uint32_t> qtyDist(1, 100);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; ++i) {
        Side randomSide = (sideDist(gen) == 0) ? Side::BUY : Side::SELL;
        uint32_t randomPrice = priceDist(gen);
        uint32_t randomQty = qtyDist(gen);

        book.addOrder({static_cast<uint64_t>(i), randomSide, randomPrice, randomQty});
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    double seconds = diff.count();

    double ops = NUM_ORDERS / seconds;

    std::cout << "[  RESULT  ] Temps total : " << seconds << " secondes." << std::endl;
    std::cout << "[  RESULT  ] Vitesse : " << ops << " ordres / seconde." << std::endl;

    EXPECT_TRUE(true);
}