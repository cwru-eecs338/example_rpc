CC = gcc
CFLAGS = -g -Wall #-Werror
LIBS = -pthread
CSOURCE = main.c producer.c consumer.c main_clnt.c main_xdr.c common.c
SSOURCE = main_svc.c main_xdr.c server.c common.c
RPCSRC = main.x
RPCGEN = main.h main_clnt.c main_svc.c main_xdr.c
CLIENT = main
SERVER = server
TEMPLATEDIR = source_templates
TEMPLATES = main_client.c main_server.c

all: $(CLIENT) $(SERVER)

main: $(CSOURCE) $(RPCGEN)
	$(CC) $(LIBS) $(CFLAGS) -o $(CLIENT) $(CSOURCE)

server: $(SSOURCE) $(RPCGEN)
	$(CC) $(LIBS) $(CFLAGS) -o $(SERVER) $(SSOURCE)

$(RPCGEN): $(RPCSRC)
	rpcgen $(RPCSRC)

templates: $(TEMPLATEDIR) $(RPCSRC)
	#cp $(RPCSRC) $(TEMPLATEDIR)
	#cd $(TEMPLATEDIR)
	#rpcgen -a $(RPCSRC)
	@(cp $(RPCSRC) $(TEMPLATEDIR) && cd $(TEMPLATEDIR) && exec rpcgen -a $(RPCSRC))
	#rm -f $(RPCSRC) $(RPCGEN)
	@(cd $(TEMPLATEDIR) && rm -f $(RPCSRC) $(RPCGEN) Makefile.main)
	#cd ../

$(TEMPLATEDIR):
	mkdir $(TEMPLATEDIR)

# 'clean' rule for remove non-source files
# To use, call 'make clean'
# Note: you don't have to call this in between every
# compilation, it's only in case you need to
# clean out your directory for some reason.
clean:
	@# Using the '@' sign suppresses echoing
	@# the line while the command is run
	rm -f $(CLIENT) $(SERVER) $(RPCGEN)
	
clean_templates: 
	rm -r $(TEMPLATEDIR)
	
clean_all: clean clean_templates