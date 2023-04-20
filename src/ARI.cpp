/************************************************/
/* ARI.cpp, (c) Rene Puchinger                  */
/************************************************/

#include "ARI.h"
#include <iostream>

ARI_encoder::ARI_encoder() {
	in = NULL;
	out = NULL;
	model = new Model();
	if (model == NULL)
		throw Exception(Exception::ERR_MEMORY);
	low = 0;
	high = 0x7FFFFFFF;  /* maximal value that fits in 31 bits */
	bits_count = 0;
	total_bits = 0;
}

ARI_encoder::~ARI_encoder() {
	if (model)
		delete model;
}

void ARI_encoder::encode_symbol(unsigned long sym_low, unsigned long sym_high, unsigned long context_totals) {
	unsigned long range = (high - low + 1) / context_totals;
	high = low + sym_high * range - 1;
	low = low + sym_low * range;
	/* while the MSB of high and low match, output them and rescale low and high */
	while ((low & 0x40000000) == (high & 0x40000000)) {
		out->put_bit(low >> 30);
		total_bits++;
		while (bits_count > 0) {
			out->put_bit((~low & 0x7FFFFFFF) >> 30);
			total_bits++;
			bits_count--;
		}
		low <<= 1;
		low &= 0x7FFFFFFF;
		high <<= 1;
		high |= 1;
		high &= 0x7FFFFFFF;
	}
	/* if there is a danger of underflow, increase the underflow counter and rescale low and high */
	while (((low & 0x20000000) != 0) && ((high & 0x20000000) == 0)) {
		bits_count++;
		low &= 0x1FFFFFFF;
		low <<= 1;
		high |= 0x20000000;
		high <<= 1;
		high |= 1;
		high &= 0x7FFFFFFF;
	}
}

void ARI_encoder::encode(InputStream* _in, SimpleBitOutputStream* _out) {
	in = _in;
	out = _out;
	int symbol;
	unsigned long sym_low, sym_high, context_totals;
	while ((symbol = in->get_char()) != EOF) {
		std::cout << "Symbol: " << symbol << std::endl;
		model->add(this, symbol, &sym_low, &sym_high, &context_totals);
		encode_symbol(sym_low, sym_high, context_totals);
		
		std::cout << "Line" << std::endl;
		for(auto i = model->trie->nodes[0].down; 
			model->trie->nodes[i].right != -1 ; 
			i = model->trie->nodes[i].right){
			rece(i,1);
		}
	}
	/* output a special EOF marker */
	model->add(this, Model::ID_EOF, &sym_low, &sym_high, &context_totals);
	encode_symbol(sym_low, sym_high, context_totals);
	/* output the remaining MSBs */
	out->put_bit((low & 0x20000000) >> 29);
	bits_count++;
	while (bits_count-- > 0)
		out->put_bit((~low & 0x20000000) >> 29);
	out->flush();
	std::cout << model->trie->depth << std::endl;
	std::cout << "Trie" << std::endl;
	for(auto i = model->trie->nodes[0].down; model->trie->nodes[i].right != -1 ; i=model->trie->nodes[i].right){
		rece(i,1);
	}
	std::cout << model->trie->depth;
	std::cout << model->trie->nodes[0].right << std::endl;
}

void ARI_decoder::rece(int i, int deep){
	if(model->trie->nodes[i].count != -1){
		for(int j=0;j<deep;j++){
			std::cout << "\t";
		}
		if(model->trie->nodes[i].symbol != 10) std::cout << 
			static_cast<char>
				(model->trie->nodes[i].symbol) << ":  " << (model->trie->nodes[i].count) << std::endl;
		else{
			std::cout << "newline" << ":  " << (model->trie->nodes[i].count) << std::endl;
		}
		for(int j= model->trie->nodes[i].down; j!=-1; j=model->trie->nodes[j].down){
			rece(j, deep+1);
		}
	} 
}
void ARI_encoder::rece(int i, int deep){
	if(model->trie->nodes[i].count != -1){
		for(int j=0;j<deep;j++){
			std::cout << "\t\t";
		}
		if(model->trie->nodes[i].symbol != 10) std::cout << 
			static_cast<char>
				(model->trie->nodes[i].symbol) << ":  " << (model->trie->nodes[i].count) << ": " << (model->trie->nodes[i].vine) << ": " << (i) << std::endl;
		else{
			std::cout << "newline" << ":  " << (model->trie->nodes[i].count) << std::endl;
		}
		for(int j= model->trie->nodes[i].down; j!=-1; j=model->trie->nodes[j].down){
			rece(j, deep+1);
		}
	} 
}


/*****************************************************************************************/

ARI_decoder::ARI_decoder() {
	in = NULL;
	out = NULL;
	model = new Model();
	if (model == NULL)
		throw Exception(Exception::ERR_MEMORY);
	low = 0;
	high = 0x7FFFFFFF;  /* maximal value that fits in 31 bits */
}

ARI_decoder::~ARI_decoder() {
	if (model)
		delete model;
}

unsigned long ARI_decoder::get_count(unsigned long context_totals) {
	unsigned long range = (high - low + 1) / context_totals;
	return (buffer - low) / range;
}

void ARI_decoder::decode_symbol(unsigned long sym_low, unsigned long sym_high, unsigned long context_totals) {
	unsigned long range = (high - low + 1) / context_totals;
	unsigned long count = (buffer - low) / range;
	high = low + sym_high * range - 1;
	low = low + sym_low * range;
	while ((low & 0x40000000) == (high & 0x40000000)) {
		low <<= 1;
		low &= 0x7FFFFFFF;
		high <<= 1;
		high |= 1;
		high &= 0x7FFFFFFF;
		buffer <<= 1;
		buffer |= in->get_bit();
		buffer &= 0x7FFFFFFF;
	}
	while (((low & 0x20000000) != 0) && ((high & 0x20000000) == 0)) {
		low &= 0x1FFFFFFF;
		low <<= 1;
		high |= 0x20000000;
		high <<= 1;
		high |= 1;
		high &= 0x7FFFFFFF;
		buffer ^= 0x20000000;
		buffer <<= 1;
		buffer |= in->get_bit();
		buffer &= 0x7FFFFFFF;
	}
}

void ARI_decoder::decode(SimpleBitInputStream* _in, OutputStream* _out) {
	in = _in;
	out = _out;
	buffer = 0;
	/* read the beginning of the encoded number */
	for (int i = 0; i < 31; i++) {
		buffer = (buffer << 1) | in->get_bit();
	}
	int symbol;
	while ((symbol = model->add(this)) != Model::ID_EOF) {
		// for(auto i = model->trie->nodes[0].down; model->trie->nodes[i].right != -1 ; i=model->trie->nodes[i].right){
		// 	rece(i,1);
		// }
		std::cout << "Newline\n";
		if (symbol == Model::ID_FLUSH)
			model->flush();
		out->put_char((char) symbol);
	}
	out->flush();
}


