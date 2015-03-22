#include "base64.h"
#include "mem.h"
#include "bits.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

static const char *base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="; 

base64_t
makeBase64Type(void *data, unsigned int data_len)
{
	base64_t new_base64;
	new_base64.mem = makeMem(data_len);
	memCpy(&new_base64.mem, data, data_len);
	return new_base64;
}

//反转字符串每个字节的高位和低位
static inline void
reverse_bytes(char *bytes, unsigned int len)
{
	for(int i = 0; i < len; i++) {
		char ch = bytes[i];
		int temp;
		for(int j = 0; j < BITS_PER_BYTE / 2; j++) {
			temp = bits_test(&ch, j);
			bits_test(&ch, BITS_PER_BYTE - j - 1) ? bits_set(&ch, j) : bits_clr(&ch, j);
			temp ? bits_set(&ch, BITS_PER_BYTE - j -1) : bits_clr(&ch, BITS_PER_BYTE - j - 1);
		}
		bytes[i] = ch;
	}
}

unsigned int 
getBase64Len(base64_t base64)
{
	return getMemIndex(base64.mem);
}

void *
getBase64Ptr(base64_t base64)
{
	return getMemPtr(&base64.mem, 0, 0);
}

void 
destroyBase64Type(base64_t base64)
{
	destroyMem(base64.mem);
}

base64_t
base64Encode(base64_t enc)
{
	base64_t dec;
	int encode_size = getBase64Len(enc) * BITS_PER_BYTE / 6;
	char ch = 0;
	while(encode_size % 4 != 0)
		encode_size++;
	if(encode_size == 0) {
		dec.mem = makeMem(1);
		return dec;
	}
	dec.mem = makeMem(encode_size);
	reverse_bytes(getMemPtr(&enc.mem, 0, 0), getBase64Len(enc));
	for(int i = 0; ; i++) {
		if(i == getBase64Len(enc) * BITS_PER_BYTE) {
			while(i % 6 != 0) {
				bits_clr(&ch, 5 - i % 6);
				i++;
			}
			memSet(&dec.mem, base[(int)ch], 1);
			break;
		}
		if(i % 6 == 0) {
			if(i != 0)
				memSet(&dec.mem, base[(int)ch], 1);
			ch = 0;
		}
		bits_test(getMemPtr(&enc.mem, 0, 0), i) ? bits_set(&ch, 5 - i % 6) : bits_clr(&ch, 5 - i % 6);
	}
	while(getBase64Len(dec) < encode_size)
		memSet(&dec.mem, '=', 1);
	return dec;
}

static inline char
find_char_pos(char ch)
{
	char *c = strchr(base, ch);
	assert(c != NULL);
	return c - base;
}

base64_t
base64Decode(base64_t dec)
{
	base64_t enc;
	//XXX:这里*Mem函数无法保证enc的内存在访问时是足够的，必须分配足够的内存
	if(getBase64Len(dec) == 0) {
		enc.mem = makeMem(1);
		return enc;
	}
	enc.mem = makeMem(getBase64Len(dec) / 4 * 3);
	int bit_index = 0;
	char ch;
	int pad = 0;
	for(int i = 0; i < getBase64Len(dec); i++) {
		ch = *GET_TYPE_MEM(&dec.mem, char, i);
		if(ch == '=')
			++pad;
		ch = find_char_pos(ch);
		for(int j = 0; j < 6; j++) {
			bits_test(&ch, 5 - j) ? 
				bits_set(getMemPtr(&enc.mem, 0, 0), bit_index++) : 
				bits_clr(getMemPtr(&enc.mem, 0, 0), bit_index++);
		}
	}
	bit_index -= pad * BITS_PER_BYTE;
	setMemIndex(&enc.mem, bit_index / BITS_PER_BYTE);
	reverse_bytes(getMemPtr(&enc.mem, 0, 0), getBase64Len(enc));
	return enc;
}

static void
deal_with_file(const char *in_file, const char *out_file, int encode_flag)
{
	FILE *in_f = fopen(in_file, "rb");
	FILE *out_f = fopen(out_file, "wb");
	assert(in_f != NULL); 
	assert(out_f != NULL);
	mem_t mem = makeMem(1);
	while(1) {
		char ch = getc(in_f);
		if(feof(in_f))
			break;
		memSet(&mem, ch, 1);
	}
	if(getMemIndex(mem) == 0)
		goto end_of_encode;
	base64_t enc = makeBase64Type(getMemPtr(&mem, 0, 0), getMemIndex(mem));
	base64_t code;
	if(encode_flag) {
		code = base64Encode(enc);
	} else {
		code = base64Decode(enc);
	}

	fwrite(getBase64Ptr(code), getBase64Len(code), 1, out_f);
	destroyBase64Type(enc);
	destroyBase64Type(code);
end_of_encode:
	destroyMem(mem);
	fclose(in_f);
	fclose(out_f);
}
	
void
base64EncodeFile(const char *in_file, const char *out_file)
{
	deal_with_file(in_file, out_file, 1);
}

void
base64DecodeFile(const char *in_file, const char *out_file)
{
	deal_with_file(in_file, out_file, 0);
}

