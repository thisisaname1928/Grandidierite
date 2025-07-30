# Page Allocation

I use buddy allocation for allocating page.

**How headers work:** 
At initialization, allocation read through memory map, then init all headers for all of the pages on memory (so that we can access headers arrays by page index, so much faster on deallocating/allocating). The memory effiency of this way is 99.8%.

Struct of header:

```

typedef struct {

uint64_t offset;

uint64_t size; // in 4KB

} BlockHeader;

```

  

Allocate pages by: `void* allocPages(uint64_t *N)`

`allocPages` will set N into how much pages it has been allocated, return a pointer to allocated address.

  

Deallocate pages by: `void freePages(uint64_t N)`

N should be a power of 2, because by forcing N to be a power of 2, we won't need more headers for storing info about allocated blocks.
