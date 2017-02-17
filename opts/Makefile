CC = gcc
DIR = ../lib
DIROBJ = ../obj
CFLAGS = -g -I$(DIR) -I. -c -Wall
LFLAGS = -g
OBJECTS = chains.o copyprop.o cse.o deadasgs.o fillslots.o flow.o io.o livedeadvars.o loops.o misc.o motion.o opt.o peephole.o regalloc.o reverse.o $(DIROBJ)/unreachable.o vars.o vect.o

opt: $(OBJECTS)
	$(CC) $(LFLAGS) -o opt $(OBJECTS)

chains.o: chains.c $(DIR)/misc.h $(DIR)/opt.h opts.h $(DIR)/vect.h
	$(CC) $(CFLAGS) chains.c

copyprop.o: copyprop.c $(DIR)/misc.h $(DIR)/opt.h $(DIR)/vars.h
	$(CC) $(CFLAGS) copyprop.c

deadasgs.o: deadasgs.c $(DIR)/opt.h $(DIR)/vars.h analysis.h opts.h
	$(CC) $(CFLAGS) deadasgs.c

fillslots.o: fillslots.c $(DIR)/opt.h opts.h $(DIR)/misc.h
	$(CC) $(CFLAGS) fillslots.c

flow.o: $(DIR)/flow.c $(DIR)/flow.h $(DIR)/opt.h
	$(CC) $(CFLAGS) $(DIR)/flow.c

io.o: $(DIR)/io.c $(DIR)/io.h analysis.h $(DIR)/misc.h $(DIR)/opt.h $(DIR)/vars.h
	$(CC) $(CFLAGS) $(DIR)/io.c

livedeadvars.o: livedeadvars.c analysis.h $(DIR)/misc.h $(DIR)/opt.h $(DIR)/vars.h opts.h
	$(CC) $(CFLAGS) livedeadvars.c

cse.o: cse.c $(DIR)/vars.h $(DIR)/misc.h $(DIR)/opt.h
	$(CC) $(CFLAGS) cse.c

loops.o: loops.c analysis.h $(DIR)/misc.h $(DIR)/opt.h
	$(CC) $(CFLAGS) loops.c

misc.o: $(DIR)/misc.c $(DIR)/misc.h $(DIR)/opt.h $(DIR)/vars.h
	$(CC) $(CFLAGS) $(DIR)/misc.c

motion.o: motion.c $(DIR)/opt.h opts.h $(DIR)/misc.h $(DIR)/vars.h
	$(CC) $(CFLAGS) motion.c

opt.o: $(DIR)/opt.c $(DIR)/opt.h
	$(CC) $(CFLAGS) $(DIR)/opt.c

peephole.o: peephole.c $(DIR)/misc.h $(DIR)/vars.h $(DIR)/opt.h
	$(CC) $(CFLAGS) peephole.c

regalloc.o: regalloc.c $(DIR)/opt.h opts.h $(DIR)/misc.h $(DIR)/vars.h
	$(CC) $(CFLAGS) regalloc.c

reverse.o: reverse.c $(DIR)/opt.h opts.h $(DIR)/misc.h
	$(CC) $(CFLAGS) reverse.c

vars.o: $(DIR)/vars.c $(DIR)/vars.h $(DIR)/opt.h
	$(CC) $(CFLAGS) $(DIR)/vars.c

vect.o: $(DIR)/vect.c $(DIR)/vect.h $(DIR)/opt.h
	$(CC) $(CFLAGS) $(DIR)/vect.c
