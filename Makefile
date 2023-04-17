mastermind: my_mastermind.c
	gcc -o my_mastermind my_mastermind.c -I.

clean:
	rm -f *.o

fclean: clean
	rm -f my_mastermind

re: fclean all