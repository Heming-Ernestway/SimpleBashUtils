#!/bin/bash

success_cnt=0
fail_cnt=0
test_number=1
test_file="tests/1.txt"
options=("b" "e" "n" "s" "t" "v")
long_options=("number-nonblank" "squeeze-blank" "number")
echo "" > tests/log.txt

test() {
    ./s21_cat $test_case > s21_cat.txt
    cat $test_case > cat.txt

    difference=$(diff s21_cat.txt cat.txt)

    current_test="Test $test_number: $test_case"

    if [[ $? -ne 0 ]]; then
        echo "$current_test: Fail"
        echo "$test_case" >> tests/log.txt
        ((fail_cnt++))
    else
        echo "$current_test: Success"
        ((success_cnt++))
    fi
    rm s21_cat.txt cat.txt
}

long_opts_test() {
    ./s21_cat $long_test > s21_cat_long.txt
    if [[ $long_opt == "number-nonblank" ]]; then
        cat -b $test_file > cat_long.txt
    fi
    if [[ $long_opt == "squeeze-blank" ]]; then
        cat -s $test_file > cat_long.txt
    fi
    if [[ $long_opt == "number" ]]; then
        cat -n $test_file > cat_long.txt
    fi

    difference=$(diff s21_cat_long.txt cat_long.txt)

    current_test="Test $test_number: $long_test"

    if [[ $? -ne 0 ]]; then
        echo "$current_test: Fail"
        echo "$test_case" >> tests/log.txt
        ((fail_cnt++))
    else
        echo "$current_test: Success"
        ((success_cnt++))
    fi
    rm s21_cat_long.txt cat_long.txt
}


for long_opt in ${long_options[@]}; do
    long_test="--$long_opt $test_file"
    long_opts_test
    ((test_number++))
done

for opt in ${options[@]}; do
    test_case="-$opt $test_file"
    test
    ((test_number++))
done


# for opt1 in ${options[@]}; do
#     for opt2 in ${options[@]}; do
#         if [[ $opt1 != $opt2 ]]; then
#             test_case="-$opt1 -$opt2 $test_file"
#             test
#             ((test_number++))
#         fi
#     done
# done


# for opt1 in ${options[@]}; do
#     for opt2 in ${options[@]}; do
#         for opt3 in ${options[@]}; do
#             if [[ $opt1 != $opt2 && $opt1 != $opt3 && $opt2 != $opt3 ]]; then
#                 test_case="-$opt1 -$opt2 -$opt3 $test_file"
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
#                     test_case="-$opt1 -$opt2 -$opt3 -$opt4 $test_file"
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
#                         test_case="-$opt1 -$opt2 -$opt3 -$opt4 -$opt5 $test_file"
#                         test
#                         ((test_number++))
#                     fi
#                 done
#             done
#         done
#     done
# done

echo "Success: $success_cnt"
echo "Fail: $fail_cnt"