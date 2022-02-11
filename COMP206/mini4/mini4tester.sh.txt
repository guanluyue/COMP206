#!/bin/bash

# cleanup files
rm -f one.e one.d two.e two.d

# Create testing files
echo "One line message" > one.txt

echo "Dear mom," > two.txt
echo "  How are you?" >> two.txt
echo "Love, Bob." >> two.txt

# Test 1: Wrong number of arguments
echo "Test #1 - Wrong number of arguments. Program displays an error message."
./cipher 10 100 < test.txt
echo

# Test 2: Wrong switch
echo "Test #2 - Wrong switch. Program displays an error message."
./cipher -f 10 100 < test.txt
echo

# Test 3: Wrong LENGTH
echo "Test #3a - Wrong LENGTH value. Program displays an error message for too large."
./cipher -e 10 700 < test.txt
echo "Test #3b - Wrong LENGTH value, Program displays an error message for zero."
./cipher -e 10 0 < test.txt
echo

# Test 4: Wrong KEY
echo "Test #4a - KEY > LENGTH. Program displays an error message."
./cipher -e 100 10 < test.txt
echo "Test #4b - KEY == LENGTH. Program displays an error message."
./cipher -e 100 100 < test.txt
echo

# Test 5: Simple encryption - no errors
echo "Test #5 - Simple encryption test - no errors."
./cipher -e 5 16 < one.txt > one.e
cat one.e
echo

# Test 6: Simple decrypt of Test 5
echo "Test #6 - Simple decrypt of Test 5 - no errors - matches original file."
./cipher -d 5 16 < one.e > one.d
echo "Decrypted file:"
cat one.d
echo
echo "Original file:"
cat one.txt
echo

# Test 7: Harder encrypt - no errors
echo "Test #7 - Harder encrypt - no errors."
./cipher -e 10 35 < two.txt > two.e
cat two.e
echo

# Test 8: Harder decrypt - no errors
echo "Test #8 - Harder decrypt - no errors - matches original file."
./cipher -d 10 35 < two.e > two.d
echo "Decrypted file:"
cat two.d
echo
echo "Original file:"
cat two.txt
echo
echo "********* Tests are Done **********"

