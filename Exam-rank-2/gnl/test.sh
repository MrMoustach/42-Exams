gcc -Wall -Werror -Wextra gnl.c main.c -o get_next_line
./get_next_line < gnl.c > yours_.res
cat -e yours_.res > yours.res
cat -e < gnl.c > original.res
diff -y --suppress-common-line original.res yours.res
rm -rf original.res yours_.res yours.res get_next_line