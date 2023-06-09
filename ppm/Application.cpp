#include "Application.h"
#include <iostream>
#include <chrono>

Application::Application(int argc, char** argv) {
    enc = NULL;  // encoder
    dec = NULL;  // decoder
    
    if (argc != 4 || (argv[1][0] != 'c' && argv[1][0] != 'd')) {
        std::cout << "Usage:   ppm [c|d] [file_in] [file_out]\n";
        std::cout << "Where:   c - compress\n";
        std::cout << "         d - decompress\n";
        std::cout << "Example: ppm c book.txt book.cmp\n";
        std::cout << "         ppm d book.cmp book.txt\n";
        state = IDLE;
    } else {
        if (argv[1][0] == 'c') {
            enc = new ARI_encoder;
            state = ENCODE;
            fn_in = argv[2];
            fn_out = argv[3];
        } else if (argv[1][0] == 'd') {
            dec = new ARI_decoder;
            state = DECODE;
            fn_in = argv[2];
            fn_out = argv[3];
        }
    }
}

void Application::run() {
	if (state == ENCODE) {
		FileInputStream* fin = new FileInputStream(fn_in);
		FileOutputStream* fout = new FileOutputStream(fn_out);
		SimpleBitOutputStream* bout = new SimpleBitOutputStream(fout);
        
        auto start = std::chrono::high_resolution_clock::now();
		enc->encode(fin, bout);
        auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "File compressed successfully: " << duration.count()  << "ms" << std::endl;
        if (fin) delete fin;
		if (fout) delete fout;
		if (bout) delete bout;
	} else if (state == DECODE) {
		FileInputStream* fin = new FileInputStream(fn_in);
		SimpleBitInputStream* bin = new SimpleBitInputStream(fin);
		FileOutputStream* fout = new FileOutputStream(fn_out);

        auto start = std::chrono::high_resolution_clock::now();
		dec->decode(bin, fout);
        auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "File decompressed successfully: " << duration.count()  << "ms" << std::endl;
		if (fin) delete fin;
		if (bin) delete bin;
		if (fout) delete fout;
	}
}

Application::~Application() {
	if (enc)
		delete enc;
	if (dec)
		delete dec;
}
