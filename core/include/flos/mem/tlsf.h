/**
 * @file    core/mm/tlsf.h
 * @brief   Two-Level Segregated Fit memory allocator
 * @version 0.1
 * @date    09.06.2014.
 *
 * @author Armands Skolmeisters  (Arseniuss) <arseniuss@arseniuss.id.lv>
 * @copyright Public Domain
 */

#ifndef __flos__MEM_TLSF_H__
#    define __flos__MEM_TLSF_H__

#    include <flos/types.h>

typedef void *tlsf_t;
typedef void *pool_t;

tlsf_t tlsf_create(void *mem);
tlsf_t tlsf_create_in_pool(void *mem, size_t bytes);
void tlsf_destroy(tlsf_t tlsf);
pool_t tlsf_get_pool(tlsf_t tlsf);

pool_t tlsf_add_pool(tlsf_t tlsf, void *mem, size_t bytes);
void tlsf_remove_pool(tlsf_t tlsf, pool_t pool);

void *tlsf_malloc(tlsf_t tlsf, size_t size);
void *tlsf_malign(tlsf_t tlsf, size_t size, size_t align);
void *tlsf_realloc(tlsf_t tlsf, size_t size, void *ptr);
void tlsf_free(tlsf_t tlsf, void *ptr);

typedef void (*tlsf_walker) (void *ptr, size_t size, int used, void *user);
void tlsf_walk_pool(pool_t pool, tlsf_walker walker, void *user);
/* Returns nonzero if any internal consistency check fails. */
int tlsf_check(tlsf_t tlsf);
int tlsf_check_pool(pool_t pool);

#endif /* !__flos__MM_TLSF_H__ */
