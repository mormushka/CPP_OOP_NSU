#include <gtest/gtest.h>
#include "priority_queue.hpp"
#include <sstream>

// --- Basic construction ---
TEST(Basic, 1) {
    PriorityQueue pq;
    EXPECT_TRUE(pq.is_max_heap());
    EXPECT_TRUE(pq.empty());
}

TEST(Basic, 2) {
    std::vector<int> values = {3, 1, 4, 2};
    PriorityQueue pq(values, true); // max-heap
    EXPECT_EQ(pq.top(), 4);
    EXPECT_TRUE(pq.verify_heap());
}

TEST(Basic, 3) {
    std::vector<int> values = {3, 1, 4, 2};
    PriorityQueue pq(values, false); // min-heap
    EXPECT_EQ(pq.top(), 1);
    EXPECT_TRUE(pq.verify_heap());
}

TEST(Basic, 4) {
    PriorityQueue pq({3, 1, 4, 5, 5, 9, -3, 2}, false); // min-heap
    EXPECT_EQ(pq.top(), -3);
    EXPECT_TRUE(pq.verify_heap());
}

// --- Push / Pop / Top ---
TEST(PushPopTop, 5) {
    PriorityQueue pq(true);
    pq.push(10);
    pq.push(5);
    pq.push(20);
    EXPECT_EQ(pq.top(), 20);
    EXPECT_EQ(pq.size(), 3);
}

TEST(PushPopTop, 6) {
    PriorityQueue pq(false);
    pq.push(10);
    pq.push(5);
    pq.push(20);
    EXPECT_EQ(pq.top(), 5);
    EXPECT_EQ(pq.size(), 3);
}

TEST(PushPopTop, 7) {
    PriorityQueue pq(true);
    pq.push(10);
    pq.push(30);
    pq.push(20);
    EXPECT_EQ(pq.top(), 30);
    pq.pop();
    EXPECT_EQ(pq.top(), 20);
    EXPECT_EQ(pq.size(), 2);
}

TEST(Exception, 8) {
    PriorityQueue pq;
    EXPECT_THROW(pq.pop(), std::runtime_error);
}

TEST(Exception, 9) {
    PriorityQueue pq;
    EXPECT_THROW(pq.top(), std::runtime_error);
}

// --- Clear / Reserve ---
TEST(Clear, 10) {
    PriorityQueue pq;
    pq.push(1);
    pq.push(2);
    pq.clear();
    EXPECT_TRUE(pq.empty());
}

TEST(Reserve, 11) {
    PriorityQueue pq;
    pq.reserve(100);
    EXPECT_EQ(pq.size(), 0);
}

// --- Operators ---
TEST(Operators, 12) {
    PriorityQueue pq1(true);
    pq1.push(1);
    pq1.push(2);

    PriorityQueue pq2(true);
    pq2.push(1);
    pq2.push(2);

    EXPECT_TRUE(pq1 == pq2);
    EXPECT_FALSE(pq1 != pq2);
}

TEST(Operators, 13) {
    PriorityQueue pq1(true);
    pq1.push(1);

    PriorityQueue pq2(true);
    pq2.push(2);

    EXPECT_TRUE(pq1 != pq2);
}

// --- Output operator ---
TEST(OutputOperator, 14) {
    PriorityQueue pq(true);
    pq.push(10);
    pq.push(5);
    std::ostringstream oss;
    oss << pq;
    std::string output = oss.str();
    EXPECT_NE(output.find("MaxHeap"), std::string::npos);
    EXPECT_NE(output.find("10"), std::string::npos);
    EXPECT_NE(output.find("5"), std::string::npos);
}

// --- Verify heap ---
TEST(VerifyHeap, 15) {
    PriorityQueue pq(true);
    pq.push(10);
    pq.push(20);
    pq.push(5);
    EXPECT_TRUE(pq.verify_heap());
}

TEST(VerifyHeap, 16) {
    PriorityQueue pq(true);
    pq.push(10);
    pq.push(20);
    pq.push(5);

    pq.clear();
    pq.push(1);
    pq.push(100);
    std::ostringstream oss;
    EXPECT_TRUE(pq.verify_heap(oss));
}
