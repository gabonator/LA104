if [ "$1" == "ds203" ]; then
	./build_ds203.sh
elif [ "$1" == "ds213" ]; then
	./build_ds213.sh
elif [ "$1" == "la104" ]; then
	./build_la104.sh
else
	echo "Unknown device!"
fi
