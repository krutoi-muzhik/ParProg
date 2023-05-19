CC = mpicc
CRUN = mpirun
THREADS = 4
RUNOPTS = -np $(THREADS)
TARGET = Summ
N = 16

.PHONY:
	all

all: compile run

compile: $(TARGET)

$(TARGET): $(TARGET).c $(TARGET).h
	$(CC) $(TARGET).c -o $(TARGET)

run: $(TARGET)
	$(CRUN) $(RUNOPTS) ./$(TARGET) $(N)

time: $(TARGET)
	time -p $(CRUN) $(RUNOPTS) ./$(TARGET) $(N)

test: $(TARGET)
	./test.sh
