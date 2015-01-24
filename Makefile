all: xor random encrypt decrypt	zip

xor:
	gcc xor.c -o xor

random:
	gcc random.c -o random
	
encrypt:
	gcc encrypt.c -o encrypt

decrypt:
	gcc decrypt.c -o decrypt
	
zip:
	# gcc huffman.c -o huffman #-Wno_incompatible-pointer-types-discards-qualifiers
	
.PHONY: all xor random encrypt decrypt zip