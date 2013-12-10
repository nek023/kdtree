PRODUCT_NAME=kdtree.out

build:
	$(CXX) -std=c++11 -o $(PRODUCT_NAME) kdtree/main.cpp

run: build
	./$(PRODUCT_NAME)

clean:
	if [ -e $(PRODUCT_NAME) ]; then rm $(PRODUCT_NAME); fi;

