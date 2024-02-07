#include "common.h"
#include "oned_csr.h"
#include <mpi.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

static oned_csr_graph g;
static int64_t* g_oldq;
static int64_t* g_newq;
static unsigned long* g_visited;
static const int coalescing_size = 256;
static int64_t* g_outgoing;
static size_t* g_outgoing_counts /* 2x actual count */;
static MPI_Request* g_outgoing_reqs;
static int* g_outgoing_reqs_active;
static int64_t* g_recvbuf;

void make_graph_data_structure(const tuple_graph* const tg) {
  convert_graph_to_oned_csr(tg, &g);
  const size_t nlocalverts = g.nlocalverts;
  g_oldq = (int64_t*)xmalloc(nlocalverts * sizeof(int64_t));
  g_newq = (int64_t*)xmalloc(nlocalverts * sizeof(int64_t));
  const int ulong_bits = sizeof(unsigned long) * CHAR_BIT;
  int64_t visited_size = (nlocalverts + ulong_bits - 1) / ulong_bits;
  g_visited = (unsigned long*)xcalloc(visited_size, sizeof(unsigned long)); // Use calloc for zero-initialization
  g_outgoing = (int64_t*)xMPI_Alloc_mem(coalescing_size * size * 2 * sizeof(int64_t));
  g_outgoing_counts = (size_t*)xcalloc(size, sizeof(size_t)); // Use calloc for zero-initialization, note the comment "2x actual count" is handled by logic, not memory initialization
  g_outgoing_reqs = (MPI_Request*)xmalloc(size * sizeof(MPI_Request));
  g_outgoing_reqs_active = (int*)xcalloc(size, sizeof(int)); // Use calloc for zero-initialization
  g_recvbuf = (int64_t*)xMPI_Alloc_mem(coalescing_size * 2 * sizeof(int64_t));
}

void free_graph_data_structure(void) {
  free(g_oldq);
  g_oldq = NULL; // Nullify the pointer after freeing
  free(g_newq);
  g_newq = NULL; // Nullify the pointer after freeing
  free(g_visited);
  g_visited = NULL; // Nullify the pointer after freeing
  MPI_Free_mem(g_outgoing);
  g_outgoing = NULL; // Nullify the pointer after freeing
  free(g_outgoing_counts);
  g_outgoing_counts = NULL; // Nullify the pointer after freeing
  free(g_outgoing_reqs);
  g_outgoing_reqs = NULL; // Nullify the pointer after freeing
  free(g_outgoing_reqs_active);
  g_outgoing_reqs_active = NULL; // Nullify the pointer after freeing
  MPI_Free_mem(g_recvbuf);
  g_recvbuf = NULL; // Nullify the pointer after freeing
  free_oned_csr_graph(&g);
  // Ensure the structure 'g' is properly cleaned up in free_oned_csr_graph, including any internal dynamic allocations
}

// Rest of the code remains unchanged
