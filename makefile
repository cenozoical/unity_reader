target:
	
	-rm -rf build
	
	mkdir build
	cd build; \
	cmake ../src; \
	make
	
	
	./build/unity_reader test/test_input test/test_output
