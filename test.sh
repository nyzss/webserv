#!/bin/bash

DEFAULT_ITERATIONS=1

send_request() {
    local port=$1
    local iteration=$2
    local request_id=$3
    local body="Request ID: $request_id, Iteration: $iteration, Port: $port"
    echo "Iteration $iteration: Sending request to port $port (ID: $request_id)"
    curl -s -o /dev/null -w "%{http_code}" -X POST -H "Content-Type: text/plain" -d "$body" "http://0.0.0.0:$port"
}

run_iteration() {
    local iteration=$1
    local results=()

    for port in "${ports[@]}"; do
        local request_id="req_${iteration}_${port}"
        result=$(send_request $port $iteration $request_id)
        results+=("Port $port (ID: $request_id): $result")
    done

    # Print results for this iteration
    for result in "${results[@]}"; do
        echo "$result"
    done
}

ports=(8080 8081 8082 8083 8084)

iterations=${1:-$DEFAULT_ITERATIONS}

echo "Running test for $iterations iterations"

for ((i=1; i<=iterations; i++)); do
    echo "Iteration $i"
    run_iteration $i
    echo "------------------------"
done

echo "All tests completed"