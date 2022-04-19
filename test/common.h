#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <spvm/context.h>
#include <spvm/state.h>

static const int* load_source(const char* fname, size_t* src_size) {
	FILE* f = fopen(fname, "rb");
	if (f == 0) {
		printf("Failed to load file %s\n", fname);
		return 0;
	}

	fseek(f, 0, SEEK_END);
	long file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	size_t el_count = (file_size / sizeof(int));
	const int* ret = (const int*) malloc(el_count * sizeof(int));
	fread((void*)ret, el_count, sizeof(int), f);
	fclose(f);

	*src_size = el_count;
	return ret;
}

struct test_setup {
	spvm_context_t ctx;
	spvm_source spv;
	spvm_program_t program;
	spvm_state_t state;
};

void setup_init(struct test_setup* setup, const char* spv_file) {
	// context holds all opcode functions
	setup->ctx = spvm_context_initialize();

	// load source code
	size_t spv_length = 0;
	setup->spv = load_source("phi.spv", &spv_length);

	// create a program and a state
	setup->program = spvm_program_create(setup->ctx, setup->spv, spv_length);
	setup->state = spvm_state_create(setup->program);
}

void setup_run_main(struct test_setup* setup) {
	spvm_word fnMain = spvm_state_get_result_location(setup->state, "main");
	spvm_state_prepare(setup->state, fnMain);
	spvm_state_call_function(setup->state);
}

void setup_destroy(struct test_setup* setup) {
	spvm_state_delete(setup->state);
	spvm_program_delete(setup->program);
	spvm_context_deinitialize(setup->ctx);
	free((void*) setup->spv);
	memset(setup, 0x0, sizeof(*setup));
}
