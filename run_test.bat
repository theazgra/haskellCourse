@echo off

echo "Running automaton tests..."
ghc -e ":load test\automaton_test.hs" -e "runTestTT tests"

echo "Running regex tests..."
ghc -e ":load test\regex_test.hs" -e "runTestTT tests"