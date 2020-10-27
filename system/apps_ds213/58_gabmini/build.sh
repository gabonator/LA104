if [ "$1" == "ds203" ]; then
	exec ./build_ds203.sh
elif [ "$1" == "ds213" ]; then
	exec ./build_ds213.sh
fi

echo "Unknown device!"
exit 1
