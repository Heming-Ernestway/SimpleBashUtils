#!/bin/bash

success_cnt=0
fail_cnt=0
test_number=1
test_file="tests/grep_history.txt"
non_existent_file="tests/this_file_does_not_exist.txt"
pattern="w"
pattern_file1="tests/template1.txt"
pattern_file2="tests/template2.txt"
template1="w"
template2="Q"
options=("v" "c" "l" "n" "h" "i")
echo "" > tests/log.txt

test() {
    ./s21_grep $test_case > tests/s21_grep.txt
    grep $test_case > tests/grep.txt

    difference=$(diff tests/s21_grep.txt tests/grep.txt)
    
    current_test="Test $test_number: $test_case"

    if [[ $? -ne 0 ]]; then
        echo "$current_test: Fail"
        echo "$test_case" >> tests/log.txt
        ((fail_cnt++))
    else
        echo "$current_test: Success"
        ((success_cnt++))
    fi

    rm tests/s21_grep.txt tests/grep.txt
}

for opt in ${options[@]}; do
    test_case="-$opt $pattern $test_file"
    test
    ((test_number++))
done

for opt in ${options[@]}; do
    test_case="-$opt -e $template1 -e $template2 $test_file"
    test
    ((test_number++))
done

for opt in ${options[@]}; do
    test_case="-$opt -f $pattern_file1 -f $pattern_file2 $test_file"
    test
    ((test_number++))
done

for opt in ${options[@]}; do
    test_case="-$opt $pattern -s $non_existent_file"
    test
    ((test_number++))
done

for opt1 in ${options[@]}; do
    for opt2 in ${options[@]}; do
        if [[ $opt1 != $opt2 ]]; then
            test_case="-$opt1$opt2 $pattern $test_file"
            test
            ((test_number++))
        fi
    done
done



# for opt1 in ${options[@]}; do
#     for opt2 in ${options[@]}; do
#         for opt3 in ${options[@]}; do
#             if [[ $opt1 != $opt2 && $opt1 != $opt3 && $opt2 != $opt3 ]]; then
#                 test_case="-$opt1 -$opt2 -$opt3 $pattern $test_file"
#                 test
#                 ((test_number++))
#             fi
#         done
#     done
# done

# for opt1 in ${options[@]}; do
#     for opt2 in ${options[@]}; do
#         for opt3 in ${options[@]}; do
#             for opt4 in ${options[@]}; do
#                 if [[ $opt1 != $opt2 && $opt1 != $opt3 && $opt1 != $opt4 && $opt2 != $opt3 && $opt2 != $opt4 && $opt3 != $opt4 ]]; then
#                     test_case="-$opt1 -$opt2 -$opt3 -$opt4 $pattern $test_file"
#                     test
#                     ((test_number++))
#                 fi
#             done
#         done
#     done
# done


# for opt1 in ${options[@]}; do
#     for opt2 in ${options[@]}; do
#         for opt3 in ${options[@]}; do
#             for opt4 in ${options[@]}; do
#                 for opt5 in ${options[@]}; do
#                     if [[ $opt1 != $opt2 && $opt1 != $opt3 && $opt1 != $opt4 && $opt1 != $opt5 && $opt2 != $opt3 && $opt2 != $opt4 && $opt2 != $opt5 && $opt3 != $opt4 && $opt3 != $opt5 && $opt4 != $opt5 ]]; then
#                         test_case="-$opt1 -$opt2 -$opt3 -$opt4 -$opt5 $pattern $test_file"
#                         test
#                         ((test_number++))
#                     fi
#                 done
#             done
#         done
#     done
# done


# for opt1 in ${options[@]}; do
#     for opt2 in ${options[@]}; do
#         for opt3 in ${options[@]}; do
#             for opt4 in ${options[@]}; do
#                 for opt5 in ${options[@]}; do
#                     for opt6 in ${options[@]}; do
#                         if [[ $opt1 != $opt2 && $opt1 != $opt3 && $opt1 != $opt4 && $opt1 != $opt5 && $opt1 != $opt6 && $opt2 != $opt3 && $opt2 != $opt4 && $opt2 != $opt5 && $opt2 != $opt6 && $opt3 != $opt4 && $opt3 != $opt5 && $opt3 != $opt6 && $opt4 != $opt5 && $opt4 != $opt6 && $opt5 != $opt6 ]]; then
#                             test_case="-$opt1 -$opt2 -$opt3 -$opt4 -$opt5 -$opt6 $pattern $test_file"
#                             test
#                             ((test_number++))
#                         fi
#                     done
#                 done
#             done
#         done
#     done
# done

echo "Success: $success_cnt"
echo "Fail: $fail_cnt"