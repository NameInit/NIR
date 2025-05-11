repeat_string() {
	local size=$1 output_file="$2" str="$3"
	for ((i=0; i<size; i++)); do echo -n "$str"; done > "$output_file"
}

length=${#1}
dir="$2"

for ((i=1, kb=100; i<=9; i++, kb+=100)); do
	repeat_string $((kb*1024/length)) "$dir/${kb}kb_repeat_test$i" "$1"
done

for ((i=10, mb=1; mb<=10; i++, mb++)); do
	repeat_string $((mb*1024*1024/length)) "$dir/${mb}mb_repeat_test$i" "$1"
done