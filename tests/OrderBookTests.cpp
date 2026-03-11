//
// Created by timot on 03/03/2026.
//
#include "OrderBook.hpp"
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