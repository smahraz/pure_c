#! bash

compile_and_test () {
  echo At $1:
  cc -I../include -L.. $1.c -o $1 -lpure_c || exit

  ./$1 || echo "wrong exit code"

  valgrind ./$1 2>/dev/null 1>/dev/null || echo "leak"
}



tests=(
  queue_test
)


TEST_WORKING_DIR=$(pwd)
LIB_NAME="libpure_c.a"


cd ..
make re
cd $TEST_WORKING_DIR

for test in "${tests[@]}";
do
  compile_and_test $test
  rm -fv $test
done



