#ifndef _OBFS_OBFSUTIL_H
#define _OBFS_OBFSUTIL_H
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
#ifndef _MSC_VER
#define VLA(TYPE, SIZE, NAME) TYPE NAME[SIZE]
#define VLA_FREE(NAME)
#else
#define VLA(TYPE, SIZE, NAME) TYPE* NAME = (TYPE*)malloc(sizeof(TYPE) * SIZE)
#define VLA_FREE(NAME) free(NAME)
#endif

typedef struct shift128plus_ctx{
    uint64_t v[2];
}shift128plus_ctx;

    int get_head_size(char* plaindata, int size, int def_size);

    void init_shift128plus(void);

    uint64_t xorshift128plus(void);

    int ss_md5_hmac(char* auth, char* msg, int msg_len, uint8_t* iv, int enc_iv_len, uint8_t* enc_key, int enc_key_len);

    int ss_sha1_hmac(char* auth, char* msg, int msg_len, uint8_t* iv, int enc_iv_len, uint8_t* enc_key, int enc_key_len);

    int data_size_list_compare(const void* a, const void* b);

    int find_pos(int arr[], int length, int key);

    void memintcopy_lt(void* mem, uint32_t val);

    uint64_t shift128plus_next(shift128plus_ctx *ctx);

    void shift128plus_init_from_bin(shift128plus_ctx *ctx, uint8_t *bin, int bin_size);

    void shift128plus_init_from_bin_datalen(shift128plus_ctx *ctx, uint8_t *bin, int bin_size, int datalen, int init_loop);

#ifdef __cplusplus
}
#endif
#endif // _OBFS_OBFSUTIL_H
