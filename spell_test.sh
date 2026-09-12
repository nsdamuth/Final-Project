#!/usr/bin/env bash

set -u

SOURCE="${1:-spell_check.cpp}"
DICT="/usr/share/dict/words"

PASS=0
FAIL=0

pass() {
    printf 'PASS: %s\n' "$1"
    PASS=$((PASS + 1))
}

fail() {
    printf 'FAIL: %s\n' "$1"
    FAIL=$((FAIL + 1))
}

if [[ ! -f "$SOURCE" ]]; then
    echo "ERROR: Could not find source file: $SOURCE"
    echo "Usage: ./unit_test.sh [source-file]"
    exit 1
fi

if [[ ! -r "$DICT" ]]; then
    echo "ERROR: Dictionary file is not available at $DICT"
    echo "The program being tested requires that file."
    exit 1
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

PROGRAM="$TMP_DIR/spellcheck"
COMPILE_LOG="$TMP_DIR/compile.log"

echo "Compiling $SOURCE..."

if g++ -std=c++17 -Wall -Wextra -pedantic "$SOURCE" -o "$PROGRAM" \
    >"$COMPILE_LOG" 2>&1; then
    pass "Program compiles"
else
    fail "Program compiles"
    cat "$COMPILE_LOG"
    exit 1
fi

if [[ -s "$COMPILE_LOG" ]]; then
    echo
    echo "Compiler warnings:"
    cat "$COMPILE_LOG"
fi

echo
echo "Running tests..."

# Test 1: final.txt does not exist.
rm -f "$TMP_DIR/final.txt"

set +e
OUTPUT="$(cd "$TMP_DIR" && ./spellcheck 2>&1)"
STATUS=$?
set -e

if [[ $STATUS -ne 0 && "$OUTPUT" == *"Could not open file to check."* ]]; then
    pass "Missing input file is handled"
else
    fail "Missing input file is handled"
    printf '  Exit status: %s\n  Output:\n%s\n' "$STATUS" "$OUTPUT"
fi

# Test 2: empty file should contain no spelling errors.
: > "$TMP_DIR/final.txt"

OUTPUT="$(cd "$TMP_DIR" && ./spellcheck 2>&1)"
EXPECTED="Completed Spell check with 0 errors."

if [[ "$OUTPUT" == "$EXPECTED" ]]; then
    pass "Empty file reports 0 errors"
else
    fail "Empty file reports 0 errors"
    printf '  Expected:\n%s\n  Actual:\n%s\n' "$EXPECTED" "$OUTPUT"
fi

# Pull two real dictionary entries so the test works with the local dictionary.
VALID_ONE="$(awk 'NF { print; exit }' "$DICT")"
VALID_TWO="$(awk 'NF { count++; if (count == 2) { print; exit } }' "$DICT")"

# Test 3: known dictionary words should not be reported as errors.
printf '%s %s\n' "$VALID_ONE" "$VALID_TWO" > "$TMP_DIR/final.txt"

OUTPUT="$(cd "$TMP_DIR" && ./spellcheck 2>&1)"
EXPECTED="Completed Spell check with 0 errors."

if [[ "$OUTPUT" == "$EXPECTED" ]]; then
    pass "Known dictionary words report 0 errors"
else
    fail "Known dictionary words report 0 errors"
    printf '  Expected:\n%s\n  Actual:\n%s\n' "$EXPECTED" "$OUTPUT"
fi

# Build a token guaranteed not to exist in this machine's dictionary.
INVALID_WORD="zzzz_unit_test_not_a_dictionary_word_987654321"
while grep -Fxq "$INVALID_WORD" "$DICT"; do
    INVALID_WORD="x${INVALID_WORD}"
done

# Test 4: one unknown word should be printed and counted once.
printf '%s %s\n' "$VALID_ONE" "$INVALID_WORD" > "$TMP_DIR/final.txt"

OUTPUT="$(cd "$TMP_DIR" && ./spellcheck 2>&1)"
EXPECTED="$(printf '%s\nCompleted Spell check with 1 errors.' "$INVALID_WORD")"

if [[ "$OUTPUT" == "$EXPECTED" ]]; then
    pass "Unknown word is printed and counted"
else
    fail "Unknown word is printed and counted"
    printf '  Expected:\n%s\n  Actual:\n%s\n' "$EXPECTED" "$OUTPUT"
fi

echo
echo "----------------------------------------"
printf 'Passed: %d\n' "$PASS"
printf 'Failed: %d\n' "$FAIL"
echo "----------------------------------------"

if [[ "$FAIL" -eq 0 ]]; then
    echo "ALL TESTS PASSED"
    exit 0
else
    echo "ONE OR MORE TESTS FAILED"
    exit 1
fi
