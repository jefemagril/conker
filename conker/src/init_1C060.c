#include <libaudio.h>
#include "n_libaudio.h"
#include <os_internal.h>
#include <ultraerror.h>

void n_alEvtqNew(ALEventQueue *evtq, N_ALEventListItem *items, s32 itemCount)
{
    s32 i;
    ALLink *item, *to;

    evtq->eventCount     = 0;
    evtq->allocList.next = 0;
    evtq->allocList.prev = 0;
    evtq->freeList.next  = 0;
    evtq->freeList.prev  = 0;

    for (i = 0; i < itemCount; i++) {
        item = &items[i];
        to = &evtq->freeList;
        // effectively alLink, TODO: macro this?
        item->next = to->next;
        item->prev = to;
        if (to->next) {
            to->next->prev = item;
        }
        to->next = item;
    }

}

ALMicroTime n_alEvtqNextEvent(ALEventQueue *evtq, N_ALEvent *evt)
{
    N_ALEventListItem *item;
    ALMicroTime delta;
    ALLink *unlink;
    ALLink *linkItem;
    ALLink *freeList;

    /* Conker: no osSetIntMask (PD/libreultra always mask). */
    item = (N_ALEventListItem *)evtq->allocList.next;

    if (item) {
        unlink = (ALLink *)item;
        if (unlink->next != 0) {
            unlink->next->prev = unlink->prev;
        }
        if (unlink->prev != 0) {
            unlink->prev->next = unlink->next;
        }
        bcopy(&item->evt, evt, sizeof(*evt));
        linkItem = (ALLink *)item;
        freeList = &evtq->freeList;
        linkItem->next = freeList->next;
        linkItem->prev = freeList;
        if (freeList->next != 0) {
            freeList->next->prev = linkItem;
        }
        freeList->next = linkItem;
        delta = item->delta;
    } else {
        /* sct 11/28/95 - overflowed the event queue */
        evt->type = -1;
        delta = 0;
    }

    return delta;
}

void n_alEvtqPostEvent(ALEventQueue *evtq, N_ALEvent *evt, ALMicroTime delta, s32 arg3)
{
    N_ALEventListItem *item;
    N_ALEventListItem *nextItem;
    ALLink *node;
    s32 postAtEnd;
    OSIntMask mask;
    ALLink *unlink;
    ALLink *element;
    ALLink *after;
    ALLink *element2;
    ALLink *after2;

    postAtEnd = 0;

    /* Conker: mask only when arg3 bit1 set (PD always masks). */
    if (arg3 & 2) {
        mask = osSetIntMask(1);
    }

    item = (N_ALEventListItem *)evtq->freeList.next;

    if (item == 0) {
        if (arg3 & 2) {
            osSetIntMask(mask);
        }
        return;
    }

    /* Conker: bit0 allows posting the last free item (PD uses !arg3). */
    if ((item->node.next == 0) && ((arg3 & 1) == 0)) {
        if (arg3 & 2) {
            osSetIntMask(mask);
        }
        return;
    }

    unlink = (ALLink *)item;
    if (unlink->next != 0) {
        unlink->next->prev = unlink->prev;
    }
    if (unlink->prev != 0) {
        unlink->prev->next = unlink->next;
    }
    bcopy(evt, &item->evt, sizeof(*evt));

    if (delta == AL_EVTQ_END) {
        postAtEnd = -1;
    }

    for (node = &evtq->allocList; node != 0; node = node->next) {
        if (node->next == 0) { /* end of the list */
            if (postAtEnd != 0) {
                item->delta = 0;
            } else {
                item->delta = delta;
            }

            element = (ALLink *)item;
            after = node;
            element->next = after->next;
            element->prev = after;
            if (after->next != 0) {
                after->next->prev = element;
            }
            after->next = element;
            break;
        } else {
            nextItem = (N_ALEventListItem *)node->next;

            if (delta < nextItem->delta) {
                item->delta = delta;
                nextItem->delta -= delta;

                element2 = (ALLink *)item;
                after2 = node;
                element2->next = after2->next;
                element2->prev = after2;
                if (after2->next != 0) {
                    after2->next->prev = element2;
                }
                after2->next = element2;
                break;
            }

            delta -= nextItem->delta;
        }
    }

    if (arg3 & 2) {
        osSetIntMask(mask);
    }
}

ALMicroTime n_alEvtqFlushType(ALEventQueue *evtq, s16 type)
{
    N_ALEventListItem *current;
    N_ALEventListItem *next;
    N_ALEventListItem *item;
    N_ALEventListItem *nextEvent;
    OSIntMask mask;
    ALMicroTime firstDelta;
    ALMicroTime itemTime;
    ALLink *unlink;
    ALLink *linkItem;
    ALEventQueue *queue;

    /* Conker: also returns absolute time of the first flushed event. */
    firstDelta = 0;
    itemTime = 0;
    mask = osSetIntMask(1);

    current = (N_ALEventListItem *)evtq->allocList.next;
    if (current != 0) {
        do {
            next = (N_ALEventListItem *)current->node.next;
            item = current;
            nextEvent = next;
            itemTime += item->delta;

            if (item->evt.type == type) {
                if (firstDelta == 0) {
                    firstDelta = itemTime;
                }
                if (nextEvent != 0) {
                    nextEvent->delta += item->delta;
                }

                unlink = (ALLink *)current;
                if (unlink->next != 0) {
                    unlink->next->prev = unlink->prev;
                }
                if (unlink->prev != 0) {
                    unlink->prev->next = unlink->next;
                }

                linkItem = (ALLink *)current;
                queue = evtq;
                linkItem->next = queue->freeList.next;
                linkItem->prev = &queue->freeList;
                if (queue->freeList.next != 0) {
                    queue->freeList.next->prev = linkItem;
                }
                queue->freeList.next = linkItem;
            }

            current = next;
        } while (current != 0);
    }

    osSetIntMask(mask);
    return firstDelta;
}
