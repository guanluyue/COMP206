#!/bin/bash
#Gwen Guan
#Student ID: 260950108

errorMsg(){
	echo "Error: $1"
	echo -e  "Script syntax:\n	./covidata.sh -r procedure id range inputFile outputFile compareFile"
	echo -e "Legal usage examples:\n	./covidata.sh get 35 data.csv result.csv\n	./covidata.sh -r get 35 2020-01 2020-03 data.csv result.csv\n	./covidata.sh compare 10 data.csv result2.csv result.csv\n	./covidata.sh -r compare 10 2020-01 2020-03 data.csv result2.csv result.csv"
}

get(){
	if [[ -f $2 ]]
	then

		if [[ -f $3 ]]
		then
			rm $3
		fi

		awk -v pattern="$1" -v output="$3" 'BEGIN {FS=","} { if($1 ~ pattern) { print $0 >> output }}' < $2  

		if [[ -f $3 ]]
		then
			num_rows=$(grep -c "" $3)
			echo "rowcount,avgconf,avgdeaths,avgtests" >> $3
			awk -v row="$num_rows" -v output="$3" 'BEGIN {FS=","} { total1 += $6;total2 += $8;total3 += $11 } END {OFS=",";print row,total1/row,total2/row,total3/row >> output}' < $3
		else
			errorMsg "The input ID does not exist"
			exit 1
		fi
	else
		errorMsg "The input file does not exist"
		exit 1
	fi
}


compare(){
	awk -v pattern="$1" -v output="$3" 'BEGIN {FS=","} { if($1 ~ pattern) { print $0 >> output }}' < $2

	num_rows=$(grep -c "" $3)
	echo "rowcount,avgconf,avgdeaths,avgtests" > "outputstats.csv"
	awk -v row="$num_rows" -v output="outputnum.csv" 'BEGIN {FS=","} { total1 += $6;total2 += $8;total3 += $11 } END {OFS=",";print row,total1/row,total2/row,total3/row >> output}' < $3
	cat outputnum.csv >> outputstats.csv

	head -n -2 $4 >> $3
	cat outputstats.csv >> $3
	tail -n 2 $4 >> $3
	tail -n 1 $4 >> outputnum.csv

	echo "diffcount,diffavgconf,diffavgdeath,diffavgtests" >> $3
	awk -v output="$3" 'BEGIN {FS=","} { if (NR==1) { first=$1;second=$2;third=$3;fourth=$4 } else { one=$1;two=$2;three=$3;four=$4 }} END {OFS=",";print first-one,second-two,third-three,fourth-four >> output}' < outputnum.csv

	rm outputnum.csv outputstats.csv
}

stats(){
	num_rows=$(grep -c "" $1)
	#echo "rowcount,avgconf,avgdeaths,avgtests" >> $2
	awk -v row="$num_rows" -v output="$2" 'BEGIN {FS=","}\
		{ total1 += $6;total2 += $8;total3 += $11 }\
			END {OFS=",";print row,total1/row,total2/row,total3/row >> output}' < $1
}

rget(){
	if [[ -f $5 ]]
	then
		rm $5
	fi
	x="$2"
	y="$3"
	startyear=${x:0:4}
	startmonth=${x:5:2}
	endyear=${y:0:4}
	endmonth=${y:5:2}
	awk -v pattern="$1" -v output="getdata.csv" 'BEGIN {FS=","} { if($1 ~ pattern) { print $0 >> output }}' < $4


	while [[ $startyear -lt $endyear ]]
	do
			while [[ $startmonth -le 12 ]]
			do
				startday="01"
				if [ ${#startmonth} -lt 2 ]
				then
					startmonth=0$startmonth
				fi
				while [[ $startday -le 31 ]]
                                do
					if [ ${#startday} -lt 2 ]
                                        then
                                                startday=0$startday
                                        fi
					
                                        awk -v pattern="$startyear-$startmonth-$startday" -v output="$5" -v sy="$startyear" -v sm="$startmonth" -v ey="$endyear" -v em="$endmonth" -v firsthalf="firsthalf.csv" -v secondhalf="secondhalf.csv" 'BEGIN {FS=","}\
                                        { if ($5 ~ pattern)\
                                                if (substr($5,9,2) <= 15)\
                                                        { print $0 >> firsthalf;print $0 >> output }\
                                                        else {print $0 >> secondhalf; print $0 >> output}}' < getdata.csv
                                startday=$(expr $startday + 1)
				done
			if [[ -f firsthalf.csv ]]
                        then
                                stats firsthalf.csv allstats.csv
                                rm firsthalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> allstats.csv
                        fi
                        if [[ -f secondhalf.csv ]]
                        then
                                stats secondhalf.csv allstats.csv
                                rm secondhalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> allstats.csv
                        fi
			
			startmonth=$(expr $startmonth + 1)	
			done
			if [[ $startmonth -eq 13 ]]
			then
				startmonth="01"
			fi
			startyear=$(expr $startyear + 1)
	done



	if [[ $startyear == $endyear ]]
	then
		while [[ $startmonth -le $endmonth ]]
                        do
                                startday="01"
                                if [ ${#startmonth} -lt 2 ]
                                then
                                        startmonth=0$startmonth
                                fi

                                #month loop
                                while [[ $startday -le 31 ]]
                                do

                                        if [ ${#startday} -lt 2 ]
                                        then
                                                startday=0$startday
                                        fi

                                        awk -v pattern="$startyear-$startmonth-$startday" -v output="$5" -v sy="$startyear" -v sm="$startmonth" -v ey="$endyear" -v em="$endmonth" -v firsthalf="firsthalf.csv" -v secondhalf="secondhalf.csv" 'BEGIN {FS=","}\
                                        {echo "$5"; if ($5 ~ pattern)\
                                                if (substr($5,9,2) <= 15)\
                                                        { print $0 >> firsthalf;print $0 >> output }\
                                                        else {print $0 >> secondhalf;print $0 >> output}}' < getdata.csv

                                        startday=$(expr $startday + 1)
                                done

                        if [[ -f firsthalf.csv ]]
                        then
                                stats firsthalf.csv allstats.csv
                                rm firsthalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> allstats.csv
                        fi
                        if [[ -f secondhalf.csv ]]
                        then
                                stats secondhalf.csv allstats.csv
                                rm secondhalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> allstats.csv
                        fi
			startmonth=$(expr $startmonth + 1)
                        done

		fi
		
	echo "rowcount,avgconf,avgdeaths,avgtests" >> $5
	cat allstats.csv >> $5
	rm getdata.csv allstats.csv
} 


rcompare(){
if [[ -f $5 ]]
        then
                rm $5
        fi
        x="$2"
        y="$3"
        startyear=${x:0:4}
        startmonth=${x:5:2}
        endyear=${y:0:4}
        endmonth=${y:5:2}

       	awk -v pattern="$1" -v output="getdata.csv" 'BEGIN {FS=","} { if($1 ~ pattern) { print $0 >> output }}' < $4
	#awk -v pattern="$1" -v output="getcompdata.csv" 'BEGIN {FS=","} { if($1 ~ pattern) { print $0 >> output }}' < $5
	echo "rowcount,avgconf,avgdeaths,avgtests" >> outputstats.csv
	echo "rowcount,avgconf,avgdeaths,avgtests" >> compstats.csv
	echo "diffcount,diffavgcount,diffavgdeath,diffavgtests" >> diffstats.csv
	while [[ $startyear -lt $endyear ]]
        do
                        while [[ $startmonth -le 12 ]]
                        do
			startday="01"
                                if [ ${#startmonth} -lt 2 ]
                                then
                                        startmonth=0$startmonth
                                fi

                                #month loop
                                while [[ $startday -le 31 ]]
                                do
				if [ ${#startday} -lt 2 ]
                                        then
                                                startday=0$startday
                                        fi

                                        awk -v pattern="$startyear-$startmonth-$startday" -v output="$5" -v sy="$startyear" -v sm="$startmonth" -v ey="$endyear" -v em="$endmonth" -v firsthalf="firsthalf.csv" -v secondhalf="secondhalf.csv" 'BEGIN {FS=","}\
                                        {if ($5 ~ pattern)\
                                                if (substr($5,9,2) <= 15)\
                                                        { print $0 >> firsthalf;print $0 >> output }\
                                                        else {print $0 >> secondhalf;print $0 >> output}}' < getdata.csv
                                        awk -v pattern="$startyear-$startmonth-$startday" -v output="compdata.csv" -v sy="$startyear" -v sm="$startmonth" -v ey="$endyear" -v em="$endmonth" -v firsthalf="firsthalf1.csv" -v secondhalf="secondhalf1.csv" 'BEGIN {FS=","}\
                                        {if ($5 ~ pattern)\
                                                if (substr($5,9,2) <= 15)\
                                                        { print $0 >> firsthalf;print $0 >> output }\
                                                        else {print $0 >> secondhalf;print $0 >> output}}' < $6
                                        startday=$(expr $startday + 1)
                                done
                        if [[ -f firsthalf.csv ]]
                        then
                                stats firsthalf.csv outputstats.csv
                                tail -n 1 outputstats.csv >> firstcalcdiff.csv
                                rm firsthalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> outputstat.csv
                                echo "0,0,0,0" >> outputstats.csv
                                echo "0,0,0,0" >> firstcalcdiff.csv
                        fi
                        if [[ -f secondhalf.csv ]]
                        then
                                stats secondhalf.csv outputstats.csv
                                tail -n 1 outputstats.csv >> secondcalcdiff.csv
                                rm secondhalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> outputstats.csv
                                echo "0,0,0,0" >> secondcalcdiff.csv
                        fi
                        if [[ -f firsthalf1.csv ]]
                        then
                                stats firsthalf1.csv compstats.csv
                                tail -n 1 compstats.csv >> firstcalcdiff.csv
                                rm firsthalf1.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> outputstat.csv
                                echo "0,0,0,0" >> compstats.csv
                                echo "0,0,0,0" >> firstcalcdiff.csv
                        fi
                        if [[ -f secondhalf1.csv ]]
                        then
                                stats secondhalf1.csv compstats.csv
                                tail -n 1 compstats.csv >> secondcalcdiff.csv
                                rm secondhalf1.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> compstats.csv
				echo "0,0,0,0" >> secondcalcdiff.csv
			fi
			awk -v output="diffstats.csv" 'BEGIN {FS=","} { if (NR==1) { first=$1;second=$2;third=$3;fourth=$4 } else { one=$1;two=$2;three=$3;four=$4 }} END {OFS=",";print first-one,second-two,third-three,fourth-four >> output}' < firstcalcdiff.csv
                        awk -v output="diffstats.csv" 'BEGIN {FS=","} { if (NR==1) { first=$1;second=$2;third=$3;fourth=$4 } else { one=$1;two=$2;three=$3;four=$4 }} END {OFS=",";print first-one,second-two,third-three,fourth-four >> output}' < secondcalcdiff.csv
                        rm firstcalcdiff.csv secondcalcdiff.csv
			startmonth=$(expr $startmonth + 1)
		done
		 if [[ $startmonth -eq 13 ]]
                        then
                                startmonth="01"
                        fi
			startyear=$(expr $startyear + 1)
          
        done


	

if [[ $startyear -eq $endyear ]]
                then
                        while [[ $startmonth -le $endmonth ]]
                        do
                                startday="01"
                                if [ ${#startmonth} -lt 2 ]
                                then
                                        startmonth=0$startmonth
                                fi

                                #month loop
                                while [[ $startday -le 31 ]]
                                do

                                        if [ ${#startday} -lt 2 ]
                                        then
                                                startday=0$startday
                                        fi

                                        awk -v pattern="$startyear-$startmonth-$startday" -v output="$5" -v sy="$startyear" -v sm="$startmonth" -v ey="$endyear" -v em="$endmonth" -v firsthalf="firsthalf.csv" -v secondhalf="secondhalf.csv" 'BEGIN {FS=","}\
                                        { if ($5 ~ pattern)\
                                                if (substr($5,9,2) <= 15)\
                                                        { print $0 >> firsthalf;print $0 >> output }\
                                                        else {print $0 >> secondhalf;print $0 >> output}}' < getdata.csv

                                        awk -v pattern="$startyear-$startmonth-$startday" -v output="compdata.csv" -v sy="$startyear" -v sm="$startmonth" -v ey="$endyear" -v em="$endmonth" -v firsthalf="firsthalf1.csv" -v secondhalf="secondhalf1.csv" 'BEGIN {FS=","}\
                                        { if ($5 ~ pattern)\
                                                if (substr($5,9,2) <= 15)\
                                                        { print $0 >> firsthalf;print $0 >> output }\
                                                        else {print $0 >> secondhalf;print $0 >> output}}' < $6
                                        startday=$(expr $startday + 1)
                                done
if [[ -f firsthalf.csv ]]
                        then
                                stats firsthalf.csv outputstats.csv
                                tail -n 1 outputstats.csv >> firstcalcdiff.csv
                                rm firsthalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> outputstat.csv
                                echo "0,0,0,0" >> outputstats.csv
                                echo "0,0,0,0" >> firstcalcdiff.csv
                        fi
                        if [[ -f secondhalf.csv ]]
                        then
                                stats secondhalf.csv outputstats.csv
                                tail -n 1 outputstats.csv >> secondcalcdiff.csv
                                rm secondhalf.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> outputstats.csv
                                echo "0,0,0,0" >> secondcalcdiff.csv
                        fi
                        if [[ -f firsthalf1.csv ]]
                        then
                                stats firsthalf1.csv compstats.csv
                                tail -n 1 compstats.csv >> firstcalcdiff.csv
                                rm firsthalf1.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> outputstat.csv
                                echo "0,0,0,0" >> compstats.csv
                                echo "0,0,0,0" >> firstcalcdiff.csv
                        fi
                        if [[ -f secondhalf1.csv ]]
                        then
                                stats secondhalf1.csv compstats.csv
                                tail -n 1 compstats.csv >> secondcalcdiff.csv
                                rm secondhalf1.csv
                        else
                                #echo "rowcount,avgconf,avgdeaths,avgtests" >> allstats.csv
                                echo "0,0,0,0" >> compstats.csv
                                echo "0,0,0,0" >> secondcalcdiff.csv
                        fi
#do diff calculations!!
                        awk -v output="diffstats.csv" 'BEGIN {FS=","} { if (NR==1) { first=$1;second=$2;third=$3;fourth=$4 } else { one=$1;two=$2;three=$3;four=$4 }} END {OFS=",";print first-one,second-two,third-three,fourth-four >> output}' < firstcalcdiff.csv
                        awk -v output="diffstats.csv" 'BEGIN {FS=","} { if (NR==1) { first=$1;second=$2;third=$3;fourth=$4 } else { one=$1;two=$2;three=$3;four=$4 }} END {OFS=",";print first-one,second-two,third-three,fourth-four >> output}' < secondcalcdiff.csv
                        rm firstcalcdiff.csv secondcalcdiff.csv
                        startmonth=$(expr $startmonth + 1)
                        done

		fi

	cat compdata.csv >> $5
	cat outputstats.csv >> $5
                        cat compstats.csv >> $5
                        cat diffstats.csv >> $5
	if [[ -f compdata.csv ]]
	then
		rm compdata.csv
	fi
                        rm getdata.csv compstats.csv diffstats.csv outputstats.csv
			}
if [[ "$1" == "-r" ]]
then
	if [[ "$2" == "get" ]]
	then
		if [[ "$#" -eq 7 ]]
		then
			rget $3 $4 $5 $6 $7
		else
			errorMsg "Wrong number of arguments"
			exit 1
		fi
	elif [[ "$2" == "compare" ]]
	then
		if [[ "$#" -eq 8 ]]
		then
			rcompare $3 $4 $5 $6 $7 $8
		else
			errorMsg "Wrong number of arguments"
			exit 1
		fi
	else
		errorMsg "Wrong syntax"
		exit 1
	fi
elif [[ "$1" == "get" ]]
then
	if [[ "$#" -eq 4 ]]
	then
		get $2 $3 $4
	
	else
		errorMsg "Wrong number of arguments"
		exit 1
	fi
elif [[ "$1" == "compare" ]]
then
	if [[ "$#" -eq 5 ]]
	then
		compare $2 $3 $4 $5
	else
		errorMsg "Wrong number of arguments"
		exit 1
	fi
else
	

	errorMsg "Wrong syntax"
	exit 1
fi

