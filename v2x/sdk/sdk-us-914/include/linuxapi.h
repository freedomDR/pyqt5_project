#ifndef LINUXAPI_H
#define LINUXAPI_H

#include <stdint.h>

#include <nx_api.h>
#include <libcli.h>
#include <craton/cli.h>

void linux_malloc_init(void);
void linux_tools_init(void);

/* Net APIs */
char * addr_to_string_v4(ULONG addr, char* buf, size_t size);
ULONG string_to_addr_v4(char *s);
char * addr_to_string_v6(NXD_ADDRESS ip_address, char* buf, size_t size);
int string_to_addr_v6(const char *s, NXD_ADDRESS *ip_address);

void linux_api_init(void);
void linux_api_exit(void);

void *__linux_malloc(uint64_t len, const char *caller_name, const char *file_name, const int lineno);
void __linux_free(void *ptr, const char *caller_name, const char *file_name, const int lineno);
void * linux_realloc(void * ptr, int32_t len);

// define linux_malloc(len) __linux_malloc(len, __FUNCTION__, __BASE_FILE__, __LINE__)
// define linux_free(ptr) __linux_free(ptr, __FUNCTION__, __BASE_FILE__, __LINE__)
static inline void *linux_malloc(uint64_t len)
{
	return __linux_malloc(len, __FUNCTION__, __BASE_FILE__, __LINE__);
}

static inline void linux_free(void *ptr)
{
	__linux_free(ptr, __FUNCTION__, __BASE_FILE__, __LINE__);
}

void memory_print(void);
void hexdump(cli_instance_t *cli, char *desc, void *addr, uint32_t len);

#endif
