#!/bin/bash

# Start teller system
./bin/teller 3 2 &  # 3 tellers, 2 terminals
TELLER_PID=$!
sleep 1  # Wait for initialization

# Test 1: Single customer
echo "Test 1: Single customer"
./bin/customer 100
sleep 1

# Test 2: Multiple customers
echo "Test 2: Parallel customers"
for i in {101..105}; do
    ./bin/customer $i &
done
wait

# Cleanup
kill $TELLER_PID
echo "All tests completed"