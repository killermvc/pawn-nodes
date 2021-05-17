#define RUN_TESTS
#define YSI_NO_HEAP_MALLOC
#define YSI_NO_CACHE_MESSAGE
#define YSI_NO_OPTIMISATION_MESSAGE
#define YSI_NO_VERSION_CHECK

//#include "nodes.inc"
#include <a_samp>
#include <YSI_Core\y_testing>

Test:test()
{
	testfn(1, 2, 3, 4, 5, 6);
}

testfn(param1, param2, ...)
{
	printf("count: %d", numargs());
}