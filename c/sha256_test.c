/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding SHA1
	          implementation. These tests do not encompass the full
	          range of available test vectors, however, if the tests
	          pass it is very, very likely that the code is correct
	          and was compiled properly. This code also serves as
	          example usage of the functions.
*********************************************************************/

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "argconfig.h"
#include "sha256.h"

const char program_desc[]  =
    "Perform simple IO testing on the AFU";

struct config {
    char *file;
};

static const struct config defaults = {
    .file = NULL,
};

static const struct argconfig_commandline_options command_line_options[] = {
    {"f",    "STRING", CFG_STRING, &defaults.file, required_argument, NULL},
    {"file", "STRING", CFG_STRING, &defaults.file, required_argument,
     "a file with input data for the SHA-256 encoder."},
      {0}
};

int sha256_test()
{
	BYTE text1[] = {"abc"};
	BYTE text2[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
	BYTE text3[] = {"aaaaaaaaaa"};
	BYTE hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
	BYTE hash2[SHA256_BLOCK_SIZE] = {0x24,0x8d,0x6a,0x61,0xd2,0x06,0x38,0xb8,0xe5,0xc0,0x26,0x93,0x0c,0x3e,0x60,0x39,
	                                 0xa3,0x3c,0xe4,0x59,0x64,0xff,0x21,0x67,0xf6,0xec,0xed,0xd4,0x19,0xdb,0x06,0xc1};
	BYTE hash3[SHA256_BLOCK_SIZE] = {0xcd,0xc7,0x6e,0x5c,0x99,0x14,0xfb,0x92,0x81,0xa1,0xc7,0xe2,0x84,0xd7,0x3e,0x67,
	                                 0xf1,0x80,0x9a,0x48,0xa4,0x97,0x20,0x0e,0x04,0x6d,0x39,0xcc,0xc7,0x11,0x2c,0xd0};
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int idx;
	int pass = 1;

	sha256_init(&ctx);
	sha256_update(&ctx, text1, strlen(text1));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash1, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	sha256_update(&ctx, text2, strlen(text2));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash2, buf, SHA256_BLOCK_SIZE);

	sha256_init(&ctx);
	for (idx = 0; idx < 100000; ++idx)
	   sha256_update(&ctx, text3, strlen(text3));
	sha256_final(&ctx, buf);
	pass = pass && !memcmp(hash3, buf, SHA256_BLOCK_SIZE);

	return(pass);
}

int sha256_file(char *file)
{
  int fd = open(file, O_RDONLY);

  struct stat buf;
  fstat(fd, &buf);

  SHA256_CTX ctx;
  BYTE sha256[SHA256_BLOCK_SIZE];

  void *addr;

  printf("Hello\n");

  addr = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED,
                  fd, 0);

	sha256_init(&ctx);
	sha256_update(&ctx, addr, buf.st_size-1);
	sha256_final(&ctx, sha256);

	for (unsigned i=0;i<SHA256_BLOCK_SIZE;i++)
	  printf("%d\t0x%02x\n",i, sha256[i]);
  
}


int main (int argc, char *argv[])
{
  struct config cfg;

        argconfig_parse(argc, argv, program_desc, command_line_options,
                    &defaults, &cfg, sizeof(cfg));

	if (cfg.file == NULL)
	  printf("SHA-256 tests: %s\n", sha256_test() ? "SUCCEEDED" : "FAILED");
	else {
	  printf("SHA-256 on file: %s\n", cfg.file);
	  sha256_file(cfg.file);
		 }
	return(0);
}
