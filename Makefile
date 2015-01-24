all: xor random encrypt decrypt	zip

xor:
	gcc xor.c -o bin/xor

random:
	gcc random.c -o bin/random
	
encrypt:
	gcc encrypt.c -o bin/encrypt

decrypt:
	gcc decrypt.c -o bin/decrypt
	
zip:
	# gcc huffman.c -o bin/huffman #-Wno_incompatible-pointer-types-discards-qualifiers
	
.PHONY: all xor random encrypt decrypt zip