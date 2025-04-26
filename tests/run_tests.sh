#!/bin/bash

cleanup() {
    kill $TELLER_PID 2>/dev/null
    rm -f /tmp/bank_*_pipe
}

# Test 1: Single customer
echo "=== TEST 1: SINGLE CUSTOMER ==="
rm -f /tmp/bank_*_pipe  # Clear old pipes
./bin/teller 3 2 &
TELLER_PID=$!
sleep 2  # Increased wait time

./bin/customer 100
sleep 1
cleanup

# Test 2: Terminal contention
echo -e "\n=== TEST 2: TERMINAL CONTENTION ==="
rm -f /tmp/bank_*_pipe
./bin/teller 3 1 &
TELLER_PID=$!
sleep 2  # Ensure teller is ready

for i in {101..105}; do
    ./bin/customer $i &
    sleep 0.1
done
wait
cleanup