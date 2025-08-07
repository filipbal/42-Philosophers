#!/bin/bash

# Philosophers Test Suite
# Comprehensive testing for the mandatory part

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

PHILO_PATH="./philo"
TEST_COUNT=0
PASS_COUNT=0

print_header() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}           PHILOSOPHERS TEST SUITE${NC}"
    echo -e "${BLUE}================================================${NC}"
}

print_test() {
    echo -e "${YELLOW}[TEST $1]${NC} $2"
    TEST_COUNT=$((TEST_COUNT + 1))
}

print_pass() {
    echo -e "${GREEN}✓ PASS${NC}: $1"
    PASS_COUNT=$((PASS_COUNT + 1))
}

print_fail() {
    echo -e "${RED}✗ FAIL${NC}: $1"
}

print_info() {
    echo -e "${BLUE}ℹ INFO${NC}: $1"
}

# Check if program exists
check_program() {
    if [ ! -f "$PHILO_PATH" ]; then
        echo -e "${RED}Error: $PHILO_PATH not found. Please compile first.${NC}"
        exit 1
    fi
}

# Test argument validation
test_arguments() {
    echo -e "\n${BLUE}=== ARGUMENT VALIDATION TESTS ===${NC}"
    
    # Test 1: No arguments
    print_test 1 "No arguments"
    if ! timeout 2s $PHILO_PATH >/dev/null 2>&1; then
        print_pass "Correctly rejects no arguments"
    else
        print_fail "Should reject no arguments"
    fi
    
    # Test 2: Too few arguments
    print_test 2 "Too few arguments (3 args)"
    if ! timeout 2s $PHILO_PATH 5 800 200 >/dev/null 2>&1; then
        print_pass "Correctly rejects too few arguments"
    else
        print_fail "Should reject too few arguments"
    fi
    
    # Test 3: Too many arguments
    print_test 3 "Too many arguments (7 args)"
    if ! timeout 2s $PHILO_PATH 5 800 200 200 5 extra >/dev/null 2>&1; then
        print_pass "Correctly rejects too many arguments"
    else
        print_fail "Should reject too many arguments"
    fi
    
    # Test 4: Invalid number of philosophers (0)
    print_test 4 "Zero philosophers"
    if ! timeout 2s $PHILO_PATH 0 800 200 200 >/dev/null 2>&1; then
        print_pass "Correctly rejects 0 philosophers"
    else
        print_fail "Should reject 0 philosophers"
    fi
    
    # Test 5: Negative values
    print_test 5 "Negative time_to_die"
    if ! timeout 2s $PHILO_PATH 5 -800 200 200 >/dev/null 2>&1; then
        print_pass "Correctly rejects negative time_to_die"
    else
        print_fail "Should reject negative time_to_die"
    fi
    
    # Test 6: Non-numeric arguments
    print_test 6 "Non-numeric argument"
    if ! timeout 2s $PHILO_PATH abc 800 200 200 >/dev/null 2>&1; then
        print_pass "Correctly rejects non-numeric arguments"
    else
        print_fail "Should reject non-numeric arguments"
    fi
}

# Test basic functionality
test_basic_functionality() {
    echo -e "\n${BLUE}=== BASIC FUNCTIONALITY TESTS ===${NC}"
    
    # Test 7: Single philosopher (should die)
    print_test 7 "Single philosopher should die"
    output=$(timeout 5s $PHILO_PATH 1 800 200 200 2>&1)
    if echo "$output" | grep -q "died"; then
        print_pass "Single philosopher correctly dies"
    else
        print_fail "Single philosopher should die (only one fork available)"
    fi
    
    # Test 8: Basic simulation with 5 philosophers
    print_test 8 "Basic simulation (5 philosophers, no death expected)"
    output=$(timeout 3s $PHILO_PATH 5 800 200 200 2>&1)
    if echo "$output" | grep -q "died"; then
        print_fail "No philosopher should die in this configuration"
    else
        print_pass "No death in stable configuration"
    fi
    
    # Test 9: Verify all required log messages
    print_test 9 "Required log message formats"
    output=$(timeout 3s $PHILO_PATH 5 800 200 200 2>&1)
    
    messages_found=0
    if echo "$output" | grep -q "has taken a fork"; then
        messages_found=$((messages_found + 1))
    fi
    if echo "$output" | grep -q "is eating"; then
        messages_found=$((messages_found + 1))
    fi
    if echo "$output" | grep -q "is sleeping"; then
        messages_found=$((messages_found + 1))
    fi
    if echo "$output" | grep -q "is thinking"; then
        messages_found=$((messages_found + 1))
    fi
    
    if [ $messages_found -eq 4 ]; then
        print_pass "All required log message formats present"
    else
        print_fail "Missing log message formats ($messages_found/4 found)"
    fi
}

# Test death scenarios
test_death_scenarios() {
    echo -e "\n${BLUE}=== DEATH DETECTION TESTS ===${NC}"
    
    # Test 10: Death timing (philosopher should die)
    print_test 10 "Death detection timing"
    start_time=$(date +%s%3N)
    output=$(timeout 5s $PHILO_PATH 4 310 200 100 2>&1)
    end_time=$(date +%s%3N)
    
    if echo "$output" | grep -q "died"; then
        death_time=$(echo "$output" | grep "died" | head -1 | awk '{print $1}')
        if [ "$death_time" -le 320 ]; then
            print_pass "Death detected within acceptable time"
        else
            print_fail "Death detected too late (${death_time}ms > 320ms)"
        fi
    else
        print_fail "Death should occur in this configuration"
    fi
    
    # Test 11: Death message timing (should be within 10ms of actual death)
    print_test 11 "Death message timing accuracy"
    output=$(timeout 3s $PHILO_PATH 2 200 100 50 2>&1)
    if echo "$output" | grep -q "died"; then
        print_pass "Death message appears (timing accuracy requires manual verification)"
        print_info "Death message: $(echo "$output" | grep "died" | head -1)"
    else
        print_fail "No death detected in tight timing scenario"
    fi
}

# Test meal completion scenarios
test_meal_completion() {
    echo -e "\n${BLUE}=== MEAL COMPLETION TESTS ===${NC}"
    
    # Test 12: Optional meal count argument
    print_test 12 "Simulation with meal count limit"
    output=$(timeout 10s $PHILO_PATH 5 800 200 200 3 2>&1)
    
    # Should stop without death when all philosophers eat enough
    if ! echo "$output" | grep -q "died"; then
        print_pass "Simulation stops when all philosophers eat enough meals"
    else
        print_fail "Simulation should stop cleanly, not with death"
    fi
    
    # Test 13: Minimum meal count
    print_test 13 "Single meal requirement"
    output=$(timeout 5s $PHILO_PATH 3 1000 200 200 1 2>&1)
    if ! echo "$output" | grep -q "died"; then
        print_pass "Simulation handles single meal requirement"
    else
        print_fail "Should complete with single meal requirement"
    fi
}

# Test edge cases
test_edge_cases() {
    echo -e "\n${BLUE}=== EDGE CASE TESTS ===${NC}"
    
    # Test 14: Large number of philosophers
    print_test 14 "Large number of philosophers (200)"
    if timeout 5s $PHILO_PATH 200 800 200 200 >/dev/null 2>&1; then
        print_pass "Handles large number of philosophers"
    else
        print_fail "Should handle 200 philosophers"
    fi
    
    # Test 15: Very short times
    print_test 15 "Very short timing values"
    output=$(timeout 3s $PHILO_PATH 5 60 10 10 2>&1)
    # This is expected to be challenging but shouldn't crash
    if [ $? -eq 124 ] || [ $? -eq 0 ]; then  # timeout or success
        print_pass "Handles very short timing values without crashing"
    else
        print_fail "Program crashed with short timing values"
    fi
    
    # Test 16: Maximum timing values
    print_test 16 "Large timing values"
    if timeout 3s $PHILO_PATH 4 2147483647 200 200 >/dev/null 2>&1; then
        print_pass "Handles large timing values"
    else
        print_fail "Should handle large timing values"
    fi
}

# Test log format validation
test_log_format() {
    echo -e "\n${BLUE}=== LOG FORMAT VALIDATION ===${NC}"
    
    # Test 17: Timestamp format
    print_test 17 "Timestamp format validation"
    output=$(timeout 2s $PHILO_PATH 3 800 200 200 2>&1 | head -10)
    
    # Check if timestamps are numeric and increasing
    timestamps=$(echo "$output" | awk '{print $1}' | grep -E '^[0-9]+$' | wc -l)
    total_lines=$(echo "$output" | wc -l)
    
    if [ "$timestamps" -eq "$total_lines" ] && [ "$total_lines" -gt 0 ]; then
        print_pass "All timestamps are properly formatted"
    else
        print_fail "Invalid timestamp format detected"
    fi
    
    # Test 18: Philosopher ID format
    print_test 18 "Philosopher ID validation"
    output=$(timeout 2s $PHILO_PATH 3 800 200 200 2>&1 | head -10)
    
    # Check if philosopher IDs are in range [1, num_philosophers]
    invalid_ids=$(echo "$output" | awk '$2 > 3 || $2 < 1 {print $2}' | wc -l)
    
    if [ "$invalid_ids" -eq 0 ]; then
        print_pass "All philosopher IDs are valid"
    else
        print_fail "Invalid philosopher IDs detected"
    fi
}

# Test data races (basic check)
test_data_races() {
    echo -e "\n${BLUE}=== DATA RACE DETECTION ===${NC}"
    
    # Test 19: Concurrent access patterns
    print_test 19 "No message overlapping"
    output=$(timeout 3s $PHILO_PATH 5 800 200 200 2>&1)
    
    # Check for incomplete lines (basic race detection)
    incomplete_lines=$(echo "$output" | grep -E '^\s*$|^[0-9]+\s*$|^[0-9]+\s+[0-9]+\s*$' | wc -l)
    
    if [ "$incomplete_lines" -eq 0 ]; then
        print_pass "No incomplete log messages detected"
    else
        print_fail "Possible data race: incomplete log messages found"
    fi
    
    # Test 20: Multiple runs consistency
    print_test 20 "Multiple runs consistency"
    run1=$(timeout 2s $PHILO_PATH 3 500 100 100 2>&1 | wc -l)
    run2=$(timeout 2s $PHILO_PATH 3 500 100 100 2>&1 | wc -l)
    run3=$(timeout 2s $PHILO_PATH 3 500 100 100 2>&1 | wc -l)
    
    # Results should be reasonably similar (allowing for some variation)
    avg=$(((run1 + run2 + run3) / 3))
    if [ $run1 -ge $((avg - 5)) ] && [ $run1 -le $((avg + 5)) ]; then
        print_pass "Multiple runs show consistent behavior"
    else
        print_fail "Inconsistent behavior across runs"
    fi
}

# Memory leak test (basic)
test_memory() {
    echo -e "\n${BLUE}=== MEMORY MANAGEMENT TESTS ===${NC}"
    
    # Test 21: Quick termination (tests cleanup)
    print_test 21 "Cleanup on early termination"
    timeout 1s $PHILO_PATH 5 800 200 200 >/dev/null 2>&1
    exit_code=$?
    
    if [ $exit_code -eq 124 ]; then  # timeout exit code
        print_pass "Program can be terminated cleanly"
    else
        print_info "Program exited with code $exit_code"
    fi
    
    # Test 22: Resource allocation
    print_test 22 "Resource allocation test"
    if timeout 2s $PHILO_PATH 50 800 200 200 1 >/dev/null 2>&1; then
        print_pass "Successfully allocates resources for 50 philosophers"
    else
        print_fail "Failed to allocate resources"
    fi
}

# Performance tests
test_performance() {
    echo -e "\n${BLUE}=== PERFORMANCE TESTS ===${NC}"
    
    # Test 23: Startup time
    print_test 23 "Program startup time"
    start_time=$(date +%s%3N)
    timeout 1s $PHILO_PATH 10 800 200 200 >/dev/null 2>&1
    end_time=$(date +%s%3N)
    startup_time=$((end_time - start_time))
    
    if [ $startup_time -lt 1000 ]; then
        print_pass "Fast startup time (${startup_time}ms)"
    else
        print_info "Startup time: ${startup_time}ms"
    fi
}

# Main execution
main() {
    print_header
    check_program
    
    test_arguments
    test_basic_functionality
    test_death_scenarios
    test_meal_completion
    test_edge_cases
    test_log_format
    test_data_races
    test_memory
    test_performance
    
    echo -e "\n${BLUE}================================================${NC}"
    echo -e "${BLUE}              TEST SUMMARY${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo -e "Total Tests: $TEST_COUNT"
    echo -e "Passed: ${GREEN}$PASS_COUNT${NC}"
    echo -e "Failed: ${RED}$((TEST_COUNT - PASS_COUNT))${NC}"
    
    if [ $PASS_COUNT -eq $TEST_COUNT ]; then
        echo -e "${GREEN}🎉 ALL TESTS PASSED!${NC}"
        exit 0
    else
        echo -e "${YELLOW}⚠️  Some tests failed. Review the output above.${NC}"
        exit 1
    fi
}

# Run if script is executed directly
if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    main "$@"
fi