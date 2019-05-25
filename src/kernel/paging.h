#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>
#include <stdbool.h>

#define MAXPHYADDR 48
#define assert_size(x, size) _Static_assert(sizeof(x) == size, "Size assertion failed")


//See Intel SDM Vol 3A 4-27 Table 4-19
struct __attribute__((__packed__)) p1_entry {
	bool present : 1;
	bool writable : 1;
	bool supervisor : 1;
	bool pwt : 1;
	bool pcd : 1;
	bool accessed : 1;
	bool dirty : 1;
	bool pat : 1;
	bool global : 1;
	uint8_t : 3;
	uint64_t addr : MAXPHYADDR - 12; // Need to bit shift by 11
	uint64_t : 51 - MAXPHYADDR + 1;
	uint8_t : 7;
	uint8_t protection : 4;
	bool xd : 1;
};
assert_size(struct p1_entry, 8);

//See Intel SDM Vol 3A 4-26 Table 4-18
struct __attribute__((__packed__)) p2_entry {
	bool present : 1;
	bool writable : 1;
	bool supervisor : 1;
	bool pwt : 1;
	bool pcd : 1;
	bool accessed : 1;
	bool : 1;
	bool page_size : 1;
	uint8_t : 3;
	uint64_t addr : MAXPHYADDR - 12; // Need to bit shift by 11
	uint64_t : 51 - MAXPHYADDR + 1;
	uint16_t : 11;
	bool xd : 1;
};
assert_size(struct p2_entry, 8);

//See Intel SDM Vol 3A 4-25 Table 4-16
struct __attribute__((__packed__)) p3_entry {
	bool present : 1;
	bool writable : 1;
	bool supervisor : 1;
	bool pwt : 1;
	bool pcd : 1;
	bool accessed : 1;
	bool : 1;
	bool page_size : 1;
	uint8_t : 3;
	uint64_t addr : MAXPHYADDR - 12; // Need to bit shift by 11
	uint64_t : 51 - MAXPHYADDR + 1;
	uint16_t : 11;
	bool xd : 1;
};
assert_size(struct p3_entry, 8);

//See Intel SDM Vol 3A 4-23 Table 4-14
struct __attribute__((__packed__)) p4_entry {
	bool present : 1;
	bool writable : 1;
	bool supervisor : 1;
	bool pwt : 1;
	bool pcd : 1;
	bool accessed : 1;
	bool : 1;
	bool : 1;
	uint8_t : 3;
	uint64_t addr : MAXPHYADDR - 12; // Need to bit shift by 11
	uint64_t : 51 - MAXPHYADDR + 1;
	uint16_t : 11;
	bool xd : 1;
};
assert_size(struct p4_entry, 8);


struct p1_entry
pg_mk_p1(bool writable, bool supervisor, bool pwt,
	bool pcd, bool pat , bool global, void* addr,
	uint8_t protection, bool xd);

struct p2_entry
pg_mk_p2_p1(bool writable, bool supervisor, bool pwt,
	bool pcd, void* addr, bool xd);

struct p3_entry
pg_mk_p3_p2(bool writable, bool supervisor, bool pwt,
	bool pcd, void* addr, bool xd);

struct p4_entry
pg_mk_p4_p3(bool writable, bool supervisor, bool pwt,
	bool pcd, void* addr, bool xd);

struct uint8_t* pg_get_page(struct p1_entry* pte);
struct p1_entry* pg_get_p1(struct p2_entry* pde);
struct p2_entry* pg_get_p2(struct p3_entry* pdpte);
struct p3_entry* pg_get_p3(struct p4_entry* pml4e);
void flush_page_cache(void);


extern struct p4_entry p4_table;

#endif /* __PAGING_H__ */
