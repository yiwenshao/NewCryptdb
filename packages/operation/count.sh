## specify the database name, and get the total count of all the tables in that database
function mysql_command(){
    cmd=\'$1\'
    #single quoted ${cmd} is not recognised
    final="mysql -uroot  -h127.0.0.1 -e ${cmd}"
    #eval can not be ommited here, can not just use $final only for this command
    res=`eval $final`
    echo $res
}

function show_table_counts(){
    db=$1
    raw=`mysql -uroot  -h127.0.0.1 -e "use $db;show tables;"| awk '{print $1}'`
    arr=($raw)
    echo $arr
    len=${#arr[@]}
    for((i=1;i<$len;i++))
    do
        res=`mysql_command "SELECT COUNT(*) AS total_count FROM $1.${arr[$i]}"`
        echo ${arr[$i]},$res >> res
    done
}

name="tpcc1000"

if [ $# = 1 ];then
    name=$1
fi

show_table_counts $name
cat res
rm res

#res=`mysql_command 'show databases'`
#echo $res
##only support numeric return value
#echo $?
