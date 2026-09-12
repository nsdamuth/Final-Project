#!/usr/bin/env bash

set -u

SOURCE="${1:-crypt.cpp}"
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
    echo "Usage: ./test_crypt.sh [source-file]"
    exit 1
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

PROGRAM="$TMP_DIR/crypt"
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

# ------------------------------------------------------------
# Test 1: Known encryption using FEATHER
# ------------------------------------------------------------
printf 'Hello, World!\n' > "$TMP_DIR/input.txt"

set +e
OUTPUT="$(cd "$TMP_DIR" && ./crypt -kFEATHER input.txt encrypted.txt 2>&1)"
STATUS=$?
set -e

EXPECTED='Yhuup, Gpmut!'

if [[ $STATUS -eq 0 && -f "$TMP_DIR/encrypted.txt" ]]; then
    ACTUAL="$(cat "$TMP_DIR/encrypted.txt")"

    if [[ "$ACTUAL" == "$EXPECTED" ]]; then
        pass "Known text encrypts correctly with FEATHER"
    else
        fail "Known text encrypts correctly with FEATHER"
        printf '  Expected: %s\n' "$EXPECTED"
        printf '  Actual:   %s\n' "$ACTUAL"
    fi
else
    fail "Known text encrypts correctly with FEATHER"
    printf '  Exit status: %s\n' "$STATUS"
    printf '  Program output: %s\n' "$OUTPUT"
fi

# ------------------------------------------------------------
# Test 2: Known decryption using FEATHER
# ------------------------------------------------------------
printf 'Yhuup, Gpmut!\n' > "$TMP_DIR/encrypted_input.txt"

set +e
OUTPUT="$(cd "$TMP_DIR" && ./crypt -d -kFEATHER encrypted_input.txt decrypted.txt 2>&1)"
STATUS=$?
set -e

EXPECTED='Hello, World!'

if [[ $STATUS -eq 0 && -f "$TMP_DIR/decrypted.txt" ]]; then
    ACTUAL="$(cat "$TMP_DIR/decrypted.txt")"

    if [[ "$ACTUAL" == "$EXPECTED" ]]; then
        pass "Known text decrypts correctly with FEATHER"
    else
        fail "Known text decrypts correctly with FEATHER"
        printf '  Expected: %s\n' "$EXPECTED"
        printf '  Actual:   %s\n' "$ACTUAL"
    fi
else
    fail "Known text decrypts correctly with FEATHER"
    printf '  Exit status: %s\n' "$STATUS"
    printf '  Program output: %s\n' "$OUTPUT"
fi

# ------------------------------------------------------------
# Test 3: Round trip preserves mixed text exactly
# ------------------------------------------------------------
cat > "$TMP_DIR/roundtrip.txt" <<'EOF'
C++ is Fun!
Line 2 has numbers: 12345.
Punctuation stays: , . ! ? ; :
EOF

set +e
(
    cd "$TMP_DIR" &&
    ./crypt -kFEATHER roundtrip.txt roundtrip.enc &&
    ./crypt -d -kFEATHER roundtrip.enc roundtrip.out
) >"$TMP_DIR/roundtrip.log" 2>&1
STATUS=$?
set -e

if [[ $STATUS -eq 0 ]] && cmp -s "$TMP_DIR/roundtrip.txt" "$TMP_DIR/roundtrip.out"; then
    pass "Encrypting and then decrypting restores the original file"
else
    fail "Encrypting and then decrypting restores the original file"
    cat "$TMP_DIR/roundtrip.log"

    if [[ -f "$TMP_DIR/roundtrip.out" ]]; then
        echo "  Difference:"
        diff -u "$TMP_DIR/roundtrip.txt" "$TMP_DIR/roundtrip.out" || true
    fi
fi

# ------------------------------------------------------------
# Test 4: Missing keyword should be an error
# ------------------------------------------------------------
printf 'Test\n' > "$TMP_DIR/no_keyword.txt"

set +e
OUTPUT="$(cd "$TMP_DIR" && ./crypt no_keyword.txt output.txt 2>&1)"
STATUS=$?
set -e

if [[ $STATUS -ne 0 && "$OUTPUT" == *"keyword"* ]]; then
    pass "Missing keyword is rejected"
else
    fail "Missing keyword is rejected"
    printf '  Exit status: %s\n' "$STATUS"
    printf '  Output: %s\n' "$OUTPUT"
fi

# ------------------------------------------------------------
# Test 5: Missing input/output filenames should be an error
# ------------------------------------------------------------
set +e
OUTPUT="$(cd "$TMP_DIR" && ./crypt -kFEATHER 2>&1)"
STATUS=$?
set -e

if [[ $STATUS -ne 0 ]]; then
    pass "Missing input/output filenames are rejected"
else
    fail "Missing input/output filenames are rejected"
    printf '  Exit status: %s\n' "$STATUS"
    printf '  Output: %s\n' "$OUTPUT"
fi

# ------------------------------------------------------------
# Test 6: Missing input file should be an error
# ------------------------------------------------------------
set +e
OUTPUT="$(cd "$TMP_DIR" && ./crypt -kFEATHER does_not_exist.txt output.txt 2>&1)"
STATUS=$?
set -e

if [[ $STATUS -ne 0 && "$OUTPUT" == *"Could not open input file."* ]]; then
    pass "Missing input file is handled"
else
    fail "Missing input file is handled"
    printf '  Exit status: %s\n' "$STATUS"
    printf '  Output: %s\n' "$OUTPUT"
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
