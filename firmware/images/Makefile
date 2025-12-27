
-include Makefile.conf

SRCs := $(shell ls *.puml)
IMGs := $(SRCs:.puml=.png)

PLANTUML ?= plantuml

all:		$(IMGs)

%.png:	%.puml
		@echo "[IMG] $@"
		@$(PLANTUML) --progress-bar --png $<
		@echo ""

clean:
		@rm -fv $(IMGs)
