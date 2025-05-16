build:
	mkdir -p out
	cl /Fe:out/nso-winevt-extractor.exe main.c
	cp LICENSE COPYRIGHT README.md out/
