run:
	@gcc wordle.c -o wordle.out;
	@./wordle.out;
clean:
	@rm -rf *.out;
push:
	@git add .
	@git commit -m "mzy"
	@git push origin main
