#include "common.h"
#include <spvm/context.h>
#include <spvm/state.h>

int main()
{
	struct test_setup setup;
	setup_init(&setup, "phi.spv");
	setup_run_main(&setup);

	// find result
	spvm_result_t result = spvm_state_get_result(setup.state, "result");

	// check that result was set correctly
	assert(result->member_count == 1u);

	spvm_result_t type_info = spvm_state_get_type_info(setup.state->results,
		&setup.state->results[result->members[0].type]);
	assert(result->members[0].member_count == 0u);
	assert(type_info->value_type == spvm_value_type_int);
	assert(result->members[0].value.u == 7u);
}
