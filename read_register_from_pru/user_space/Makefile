read_add_from_user_space: read_add_from_user_space.o
	gcc -o read_add_from_user_space read_add_from_user_space.o
	rm -rf *.o
read_add_from_user_space.o : read_add_from_user_space.c
	gcc -o read_add_from_user_space.o -c read_add_from_user_space.c 

clean:
	rm -rf *.o
	rm -rf read_add_from_user_space
