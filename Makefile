store: store.c
		gcc -Werror -Wall store.c -o build/store -pthread && ./build/store