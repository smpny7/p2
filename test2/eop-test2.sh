#!/bin/bash
#
# Exercise of Programming 1
# An automatic test Script for final examination
#
#   Sunao Hara 2020-06-02
#

#set -e
#SCR_DIR=/home/users/edu2019/lect/eop/20/test1
SCR_DIR=`dirname $0`/.data
RES_DIR=./result-`date +%y%m%d-%H%M%S`

FILES_ARE_VALID=1

function exe_suffix() {
    _uname=$(uname -a)
    if [ $(expr "$_uname" : Darwin.*) -gt 1 ]; then        echo "_macos"
    elif [ $(expr "$_uname" : .*C[yY][Gg].*) -gt 1 ]; then echo "_cygwin.exe"
    elif [ $(expr "$_uname" : MINGW.*) -gt 1 ]; then       echo "_mingw.exe"
    elif [ $(expr "$_uname" : .*Lin.*) -gt 1 ]; then       echo "_linux"
    else echo "Warning: Unknown OS" 1>&2;                  echo "_linux"
    fi
}

DBSAMPLE=db-sample$(exe_suffix)
if type gsed > /dev/null 2>&1; then
	SEDCMD=gsed
else
	SEDCMD=sed
fi

function md5file() {
	if [ -x /sbin/md5 ]; then
		/sbin/md5 < $1
	else
		md5sum $1 | cut -f1 -d" "
	fi
}

function output_ok() {
	echo -e "...\x1B[32;1m" "OK" "\x1B[m"
}

function output_warn() {
	echo -e "...\x1B[32;1m" "OK" "\x1B[33;1m" "*WARNING: $1" "\x1B[m"
}

function output_ng() {
	echo -e "...\x1B[31;1m" "NG: $1" "\x1B[m"
}

function validate_file() {
	mode=$1
	target=$2
	message=$3
	echo -n "  $message: $target "
	if [ $mode -eq 0 ]; then
		# Check only file is exists or not
		if [ -f $target ]; then
			echo -n `md5file $target`
			output_ok
			return 0
		else
			output_ng "the file is not exist."
			FILES_ARE_VALID=0
			return 1
		fi
	elif [ $mode -eq 1 ]; then
		# Check by md5 checksum
		TARGET_BASENAME=`basename $target`
		REF_MD5=`grep $TARGET_BASENAME $SCR_DIR/md5.txt | cut -f1 -d" "`
		TARGET_MD5=`md5file $target`
		echo -n $TARGET_MD5
		if [ x"$REF_MD5" == x"$TARGET_MD5" ]; then
			output_ok
			return 0
		else
			output_ng "checksum is invalid."
			FILES_ARE_VALID=0
			return 1
		fi
	fi
}

function run_and_validate() {
	USR_CMD_FILE=$1
	DIFF_RESULT=${RES_DIR}/diff-`basename $USR_CMD_FILE`
	REF_RESULT=${RES_DIR}/$$.result_ref.log,`basename $USR_CMD_FILE`
	USR_RESULT=${RES_DIR}/$$.result_${USER}.log,`basename $USR_CMD_FILE`

	$SCR_DIR/$DBSAMPLE  < $USR_CMD_FILE > ${REF_RESULT} 2> ${REF_RESULT}.err
	$RES_DIR/eop1-$USER < $USR_CMD_FILE > ${USR_RESULT} 2> ${USR_RESULT}.err
	diff -bB -y ${REF_RESULT} ${USR_RESULT} > ${DIFF_RESULT}
	return $?
}

function run_and_validate_filt() {
	USR_CMD_FILE=$1
	DIFF_RESULT=${RES_DIR}/diff-`basename $USR_CMD_FILE`,filt
	REF_RESULT=${RES_DIR}/$$.result_ref_filt.log,`basename $USR_CMD_FILE`
	USR_RESULT=${RES_DIR}/$$.result_${USER}_filt.log,`basename $USR_CMD_FILE`

	$SCR_DIR/$DBSAMPLE  < $USR_CMD_FILE 2> /dev/null | $SEDCMD -Ef ${SCR_DIR}/filter_command_p.conf > ${REF_RESULT}
	$RES_DIR/eop1-$USER < $USR_CMD_FILE 2> /dev/null | $SEDCMD -Ef ${SCR_DIR}/filter_command_p.conf > ${USR_RESULT}
	diff -bB -y ${REF_RESULT} ${USR_RESULT} > ${DIFF_RESULT}
	return $?
}

function test_by_file() {
	N=$1
	CMD_FILE=$3
	USR_CMD_FILE=${RES_DIR}/${USER}_`basename $CMD_FILE`

	echo -e "\x1B[36;1mTEST $2:\x1B[m " $CMD_FILE
	
	if [ $N -gt 2886 ]; then
		shuf -n $N $SCR_DIR/sample_x4.csv > $USR_CMD_FILE
		cat $CMD_FILE >> $USR_CMD_FILE
	elif [ $N -gt 0 ]; then
		shuf -n $N $SCR_DIR/sample.csv > $USR_CMD_FILE
		cat $CMD_FILE >> $USR_CMD_FILE
	else
		cat $SCR_DIR/sample.csv $CMD_FILE > $USR_CMD_FILE
	fi

	run_and_validate $USR_CMD_FILE

	if [ $? = 0 ]; then
		output_ok
	else
		run_and_validate_filt $USR_CMD_FILE
		if [ $? = 0 ]; then
			output_warn "The outputs are almost the same, but have small differences. Please check your diff result."
		else	
			output_ng "The outputs are not identical. Please check your diff result."
		fi
	fi
	
	rm $RES_DIR/$$.*
}

# Validate test files
#( cd $SCR_DIR && md5sum -c md5.txt ) || exit -1

# Check your id
#clear # clear the screen
echo "1. Environment"
echo "--------------"
echo "  Script:" `md5sum $0`
echo "  Date:" `date +'%FT%T%z'`
echo "  Host:" `hostname`
echo "  Directory:" $PWD
echo ""
echo "2. Student ID (or name)"
echo "-----------------------"
echo "  " $USER

# Check the files
echo ""
echo "3. Check test files"
echo "-------------------"
validate_file 0 eop1-${USER}.c "(1) Source File"
validate_file 1 $SCR_DIR/$DBSAMPLE  "(2) Reference application"
validate_file 1 $SCR_DIR/sample.csv "(3-1) Sample CSV file"
validate_file 1 $SCR_DIR/sample_x4.csv "(3-2) Sample CSV file (over 10,000 data)"
validate_file 1 $SCR_DIR/eop1-test1_1.txt "(4-1) Sample command file part 1-1"
validate_file 1 $SCR_DIR/eop1-test1_2.txt "(4-2) Sample command file part 1-2"
validate_file 1 $SCR_DIR/eop1-test1_3.txt "(4-3) Sample command file part 1-3"
validate_file 1 $SCR_DIR/eop1-test1_4.txt "(4-4) Sample command file part 1-4"
validate_file 1 $SCR_DIR/eop1-test2_1.txt "(4-5) Sample command file part 2-1"
validate_file 1 $SCR_DIR/eop1-test2_2.txt "(4-6) Sample command file part 2-2"
validate_file 1 $SCR_DIR/eop1-test2_3.txt "(4-7) Sample command file part 2-3"
validate_file 1 $SCR_DIR/eop1-test3_1.txt "(4-8) Sample command file part 3-1"
validate_file 1 $SCR_DIR/eop1-test3_2.txt "(4-9) Sample command file part 3-2"
validate_file 1 $SCR_DIR/eop1-test3_3.txt "(4-10) Sample command file part 3-3"
validate_file 1 $SCR_DIR/eop1-test4_1.txt "(4-11) Sample command file part 4-1"
validate_file 1 $SCR_DIR/eop1-test4_2.txt "(4-12) Sample command file part 4-2"

# Prepare result directory
mkdir -p ${RES_DIR}

# Delete old file
# if [ -f eop1-$USER ]; then rm eop1-$USER; fi
# for v in 1_1 1_2 1_3 1_4 2_1 2_2 2_3 3_1 3_2 3_3 4_1 4_2; do
# 	if [ -f ${USER}_eop1-test$v.txt ]; then rm ${USER}_eop1-test$v.txt; fi
# 	if [ -f diff-${USER}_eop1-test$v.txt ]; then rm diff-${USER}_eop1-test$v.txt; fi
# done

# Check the validation result
if [ $FILES_ARE_VALID -ne 1 ]; then
	output_ng "ERROR: The test process is aborted."
	echo ""
	exit -1
fi

# Compile
echo ""
echo "4. Compile"
echo "----------"
gcc -Wall -o ${RES_DIR}/eop1-$USER eop1-${USER}.c && ls -la ${RES_DIR}/eop1-${USER} eop1-${USER}.c

if [ ${RES_DIR}/eop1-$USER -nt eop1-$USER.c ]; then
	output_ok
else
	output_ng "Compile failed?"
	exit -1
fi

# Execute
echo ""
echo "5. Execute"
echo "----------"

test_by_file 5 "1 1/4" $SCR_DIR/eop1-test1_1.txt
test_by_file 5 "1 2/4" $SCR_DIR/eop1-test1_2.txt
test_by_file 5 "1 3/4" $SCR_DIR/eop1-test1_3.txt
test_by_file 5 "1 4/4" $SCR_DIR/eop1-test1_4.txt
test_by_file 5 "2 1/3" $SCR_DIR/eop1-test2_1.txt
test_by_file 5 "2 2/3" $SCR_DIR/eop1-test2_2.txt
test_by_file 5 "2 3/3" $SCR_DIR/eop1-test2_3.txt
test_by_file 0 "3 1/3" $SCR_DIR/eop1-test3_1.txt
test_by_file 0 "3 2/3" $SCR_DIR/eop1-test3_2.txt
test_by_file 0 "3 3/3" $SCR_DIR/eop1-test3_3.txt
test_by_file 10000 "4 1/2" $SCR_DIR/eop1-test4_1.txt
test_by_file 10000 "4 2/2" $SCR_DIR/eop1-test4_2.txt

# Finish test script
echo -e "\nFinish all.\x1B[m"
