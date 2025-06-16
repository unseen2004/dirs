CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET = binomial_heap
SOURCE = binomial_heap.cpp

.PHONY: all run visualize clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

run: $(TARGET)
	./$(TARGET)

visualize: run
	python3 visualize_results.py

clean:
	rm -f $(TARGET) *.csv *.png

install-python-deps:
	pip3 install pandas matplotlib numpy

help:
	@echo "Available targets:"
	@echo "  all              - Compile the program"
	@echo "  run              - Compile and run the experiment"
	@echo "  visualize        - Run experiment and generate visualizations"
	@echo "  clean            - Remove generated files"
	@echo "  install-python-deps - Install required Python packages"
	@echo "  help             - Show this help message"