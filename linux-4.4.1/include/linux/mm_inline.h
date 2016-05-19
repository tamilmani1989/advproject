#ifndef LINUX_MM_INLINE_H
#define LINUX_MM_INLINE_H

#include <linux/huge_mm.h>
#include <linux/swap.h>

/**
 * page_is_file_cache - should the page be on a file LRU or anon LRU?
 * @page: the page to test
 *
 * Returns 1 if @page is page cache page backed by a regular filesystem,
 * or 0 if @page is anonymous, tmpfs or otherwise ram or swap backed.
 * Used by functions that manipulate the LRU lists, to sort a page
 * onto the right LRU list.
 *
 * We would like to get this info without a page flag, but the state
 * needs to survive until the page is last deleted from the LRU, which
 * could be as far down as __page_cache_release.
 */




static inline int page_is_file_cache(struct page *page)
{
	return !PageSwapBacked(page);
}


static inline void calculatePageHeat(struct page *page, bool flag ) {
	
	__kernel_time_t diff;
	struct timespec tp;
	get_monotonic_boottime(&tp);
	if(page->refTime[1]!=0) {
		diff = tp.tv_sec - page->refTime[1];
		if(diff == 0)
			page->heat = 2000000;
		else
			page->heat = 2000000/diff;
		//printk("page heat:%d \t diff:%d \t cursec:%d \n",page->heat,diff,tp.tv_sec); 
	}
	else
	{
//		printk("Set page heat as 0\n");
		page->heat=0;
	}
	if(flag==true)
	{
		page->refTime[1] = page->refTime[0];
		page->refTime[0] = tp.tv_sec;
	}
	
}
# if 0
static inline void sortList(struct lruvec *lruvec, enum lru_list lru)
{
	struct list_head *head, *pos,*tmp;
	struct page *fixPg,*pg;
	int swp;
	head = &lruvec->lists[lru];
	for (pos = head->next; pos != head; pos = pos->next)
	{
		fixPg = list_entry(pos, struct page, lru);
		for (tmp = pos->next; tmp != head; tmp = tmp->next)
		{
			pg = list_entry(tmp, struct page, lru);
			if(pg->heat > fixPg->heat)
			{
				swp = pg->heat;
				pg->heat = fixPg->heat;
				fixPg->heat = swp;				
			}		
		}
	}
}

static inline void reorderList(struct page *page, struct lruvec *lruvec, enum lru_list lru)
{
	struct list_head *pos, *head, *marker;
	struct page *pg;
	head = &lruvec->lists[lru];
	marker = &page->lru;
	for (pos = (head)->next; pos != marker; pos = pos->next)
       	{
        	pg = list_entry(pos, struct page, lru);
                if (pos==head||page->heat > pg->heat)
                {
                                //printk("******** sikkitan********\n");
                                break;
                }
	}

	
	if(pos != marker && pos !=head ) {
		marker->prev->next = marker->next;
                marker->next->prev = marker->prev;
                marker->next=NULL;
               	marker->prev=NULL;

        	marker->prev = pos->prev;
              	marker->next = pos;
               	marker->prev->next = marker;
                pos->prev = marker;
	}

}

static inline void printList(struct page *page, struct lruvec *lruvec,enum lru_list lru)
{
	struct list_head *head=NULL,*pos;
	struct page *pg;
	int tmp1,tmp2;
	head = &lruvec->lists[lru];	
	printk("===============begin=================\n");	

	for(pos = head->next; pos!=head; pos = pos->next)
	{
		pg = list_entry(pos, struct page,lru);
		printk("***heat:%d*****:%d\n",page->heat,lru);

	}
}

static __always_inline void add_page_to_heat_list(struct page *page,
                                struct lruvec *lruvec, enum lru_list lru)
{
	struct list_head *pos,*newNode,*head;
	struct page *pg;
        int nr_pages = hpage_nr_pages(page);
        mem_cgroup_update_lru_size(lruvec, lru, nr_pages);
//        list_add(&page->lru, &lruvec->lists[lru]);
/*
	list_for_each(pos,&lruvec->lists[lru]) {
		page = list_entry(pos, struct page, lru);		
	}
*/
	//list_del(&page->lru);

	head = &lruvec->lists[lru];

	if (!list_empty(head)) {
		for (pos = head->next;pos!=head;head=head->next)
		{
			if(page == list_entry(pos,struct page, lru))
			{
				printk("*********MATCHED**********:\n");
				break;		
			}
		}
		if(pos!=head) {
			printk("TM checkpoint\n");
			pos->prev->next = pos->next;
			pos->next->prev = pos->prev; 
			pos->next=NULL;
			pos->prev=NULL;
		}			
		
		newNode = &page->lru;
	
		head = &lruvec->lists[lru];
	
		//list_add(&page->lru, &lruvec->lists[lru]);
	
		for (pos = (head)->next; pos != (head); pos = pos->next)
		{
			pg = list_entry(pos, struct page, lru);
			if (page->heat > pg->heat)
			{
				//printk("********HELLO********:%p\n",page);
				break;
			}
		}
		
		if(pos == head) {
			list_add_tail(&page->lru,&lruvec->lists[lru]);
		}
		else {
			newNode->prev = pos->prev;
			newNode->next = pos;
			newNode->prev->next = newNode;
			pos->prev = newNode;
			//printList(page,lruvec,lru);
		}	
	}

	else {
		//printk("*********HEAD ADD*************\n");
		list_add(&page->lru,&lruvec->lists[lru]);
	}
	//printList(page,lruvec,lru);
        __mod_zone_page_state(lruvec_zone(lruvec), NR_LRU_BASE + lru, nr_pages);
}

#endif
static __always_inline void add_page_to_lru_list(struct page *page,
				struct lruvec *lruvec, enum lru_list lru)
{
	int nr_pages = hpage_nr_pages(page);
	mem_cgroup_update_lru_size(lruvec, lru, nr_pages);
	list_add(&page->lru, &lruvec->lists[lru]);
	__mod_zone_page_state(lruvec_zone(lruvec), NR_LRU_BASE + lru, nr_pages);
}

static __always_inline void del_page_from_lru_list(struct page *page,
				struct lruvec *lruvec, enum lru_list lru)
{
	int nr_pages = hpage_nr_pages(page);
	mem_cgroup_update_lru_size(lruvec, lru, -nr_pages);
	list_del(&page->lru);
	__mod_zone_page_state(lruvec_zone(lruvec), NR_LRU_BASE + lru, -nr_pages);
}

/**
 * page_lru_base_type - which LRU list type should a page be on?
 * @page: the page to test
 *
 * Used for LRU list index arithmetic.
 *
 * Returns the base LRU type - file or anon - @page should be on.
 */
static inline enum lru_list page_lru_base_type(struct page *page)
{
	if (page_is_file_cache(page))
		return LRU_INACTIVE_FILE;
	return LRU_INACTIVE_ANON;
}

/**
 * page_off_lru - which LRU list was page on? clearing its lru flags.
 * @page: the page to test
 *
 * Returns the LRU list a page was on, as an index into the array of LRU
 * lists; and clears its Unevictable or Active flags, ready for freeing.
 */
static __always_inline enum lru_list page_off_lru(struct page *page)
{
	enum lru_list lru;

	if (PageUnevictable(page)) {
		__ClearPageUnevictable(page);
		lru = LRU_UNEVICTABLE;
	} else {
		lru = page_lru_base_type(page);
		if (PageActive(page)) {
			__ClearPageActive(page);
			lru += LRU_ACTIVE;
		}
	}
	return lru;
}

/**
 * page_lru - which LRU list should a page be on?
 * @page: the page to test
 *
 * Returns the LRU list a page should be on, as an index
 * into the array of LRU lists.
 */
static __always_inline enum lru_list page_lru(struct page *page)
{
	enum lru_list lru;

	if (PageUnevictable(page))
		lru = LRU_UNEVICTABLE;
	else {
		lru = page_lru_base_type(page);
		if (PageActive(page))
			lru += LRU_ACTIVE;
	}
	return lru;
}

#endif
