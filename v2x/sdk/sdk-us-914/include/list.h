#ifndef __GN_LIST_H__
#define __GN_LIST_H__

#undef NULL
#define NULL 0

#define off_setof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

//#define container_of(ptr, type, member) ( { const typeof( ((type *)0)->member ) *__mptr = (ptr); (type *)( (char *)__mptr - off_setof(type,member) ); } )
#define container_of(ptr, type, member) ( { typeof( ((type *)0)->member ) *__mptr = (ptr); (type *)( (char *)__mptr - off_setof(type,member) ); } )

static inline void prefetch(const void *x) {;} 
//static inline void prefetchw(const void *x) {;} 

#define LIST_POISON1 ((void *) 0x00100100) 
#define LIST_POISON2 ((void *) 0x00200200) 

struct list_head {
	struct list_head *next, *prev;
};
 
#define LIST_HEAD_INIT(name) { &(name), &(name) }

//#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}

#define list_entry(ptr, type, member) container_of(ptr, type, member)

//#define list_for_each_safe(pos, n, head) for(pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)
#define list_for_each_safe(pos, n, head) for(({pos = (head)->next; n = pos->next;}); pos != (head); ({pos = n; n = pos->next;}))
					
//#define list_for_each_entry(pos, head, member) for(pos = list_entry((head)->next, typeof(*pos), member); prefetch(pos->member.next), &pos->member != (head); pos = list_entry(pos->member.next, typeof(*pos), member))
#define list_for_each_entry(pos, head, member) for(pos = list_entry((head)->next, typeof(*pos), member); ({prefetch(pos->member.next); &pos->member != (head);}); pos = list_entry(pos->member.next, typeof(*pos), member))
					
//#define list_for_each_entry_safe(pos, n, head, member) for(pos = list_entry((head)->next, typeof(*pos), member), n = list_entry(pos->member.next, typeof(*pos), member); &pos->member != (head); pos = n, n = list_entry(n->member.next, typeof(*n), member))
#define list_for_each_entry_safe(pos, n, head, member) for(({pos = list_entry((head)->next, typeof(*pos), member); n = list_entry(pos->member.next, typeof(*pos), member);}); &pos->member != (head); ({pos = n; n = list_entry(n->member.next, typeof(*n), member);}))
					
struct hlist_head {
	struct hlist_node *first;
};
 
struct hlist_node {
	struct hlist_node *next, **pprev;
};

#define HLIST_HEAD_INIT { .first = NULL }
#define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)

#endif
