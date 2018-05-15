SUBDIRS = src analyze

all: subdirs test
	@echo "After Build, run 'make analysis' to run analysis"

test: 
	@echo "About to self-test"; ./src/hw1

analysis: 
	@cd analyze && make analysis

subdirs:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done


