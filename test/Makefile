
all:
	@echo -n "Building '$(file)'... " 
	@g++ -g3 -O0 -o $(file) -I../include -L../Debug/ $(file).cpp -lcli++
	@echo "done."
	
run: all
	@echo "Running '$(file)'..."
	@echo "-------------------------------------------------------------------------------"
	@./$(file)
	@echo "-------------------------------------------------------------------------------"
	@echo "Run done."

.PHONY: clean run

clean:
	rm -f *.o && find . -maxdepth 1 -executable -type f -delete
