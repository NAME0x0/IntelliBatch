# Testing Framework for ITD

This directory contains tests for the Integrated Terminal Desktop project.

## Directory Structure

- `unit/`: Unit tests for individual components
- `integration/`: Integration tests for component interactions
- `data/`: Test data files
- `mocks/`: Mock objects for testing

## Running Tests

Tests can be run using CMake's CTest:

```bash
cd build
cmake --build . --target test
```

Or run specific tests:

```bash
cd build
ctest -R <test_pattern>
```

## Writing Tests

Tests are written using Google Test framework. When adding a new test:

1. Create a new file in the appropriate directory
2. Name the file with a `_test` suffix, e.g., `terminal_test.cpp`
3. Include the Google Test header and necessary project headers
4. Write test fixtures and cases
5. Add the test to `tests/CMakeLists.txt`

## Test Coverage

To generate a test coverage report:

```bash
cd build
cmake -DENABLE_COVERAGE=ON ..
cmake --build . --target coverage
```

The coverage report will be available in `build/coverage/index.html`.
