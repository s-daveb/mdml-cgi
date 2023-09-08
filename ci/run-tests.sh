#!/bin/sh

BASE_DIR="$(cd "$(dirname "$0")"; pwd)/..";
BASE_DIR="$(readlink -f ${BASE_DIR})"

PROGRAM="$(basename $0)"
RESULT=1

print_help() {
	/bin/echo -n "$PROGRAM: "
	echo "Executes the project test suite under the specified directory"
	echo
	echo 'Usage:'
	echo "$PROGRAM <builddir>"

	exit 255
}

if [ $# -ne 1 ]; then
	echo "Invalid number of parameters!"
	echo
	print_help
fi

RUNDIR="${BASE_DIR}/$1"
shift

cd $RUNDIR
ctest
RESULT=$?

cd $OLDPWD

(exit $RESULT)
