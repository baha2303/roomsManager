#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "Room.h"
#include "list.h"

//------------------------------------------------------------------------------------------


#define MM_STRING 4
#define MAX_SEATS 999
#define MAX_NUMBER 9999
#define DATE_STR 11
#define TIME_STR 6
#define FULL_TIME_STR 13


struct room
{
    char *building_name;
    int number;
    int size;
    bool multi_media;
    void *orders;
};
ListElement copyOrder (ListElement order);
void freeOrder (ListElement order);
/**** Function used by list ***************************/

typedef struct order_t {
    char date[DATE_STR];
    char start_time[TIME_STR];
    char end_time[TIME_STR];
} *Order ;




ListElement copyOrder (ListElement order) {

    if(order==NULL) return NULL;
    Order new_order=malloc(sizeof(*new_order));
    if(new_order==NULL) return NULL;
    strcpy(new_order->date,((Order)order)->date);
    strcpy(new_order->end_time,((Order)order)->end_time);
    strcpy(new_order->start_time,((Order)order)->start_time);
    return ((Order)new_order);
}

void freeOrder (ListElement order) {
    free(order);

}
/****************************************************/


static void dateCopy (const char* date1,const char* time1, char* date_str);
static char* nameDuplicate (const char* name );
static int compareDates(char* date1,char* time1,char* date2 ,char* time2);



/* compare two dates/time dd/mm/yyyy hh:mm
 * if date1>date2 return >0
    if date1<date2 return <0
    if date1= date 2 return 0
    */

static int compareDates(char* date1,char* time1,char* date2 ,char* time2) {

    assert(date1 && time1 && date2 && time2);

    char date1_str[FULL_TIME_STR]={0},date2_str[FULL_TIME_STR]={0};
    dateCopy(date1,time1,date1_str);
    dateCopy(date2,time2,date2_str);
    if(strcmp(date1_str,date2_str) > 0 ) return 1;
    if(strcmp(date1_str,date2_str) == 0) return 0;
    return -1;

}


static void dateCopy (const char* date1,const char* time1, char* date_str) {

    date_str[0]=date1[6];
    date_str[1]=date1[7];
    date_str[2]=date1[8];
    date_str[3]=date1[9];
    date_str[4]=date1[3];
    date_str[5]=date1[4];
    date_str[6]=date1[0];
    date_str[7]=date1[1];
    date_str[8]=time1[0];
    date_str[9]=time1[1];
    date_str[10]=time1[3];
    date_str[11]=time1[4];
    date_str[12]=time1[5];



}



static char* nameDuplicate (const char* name ) {
    char* duplicate= malloc(strlen(name)+1);
    return duplicate ? strcpy(duplicate,name) : NULL ;
}


Room roomCreate(char *building_name, int number, int size, bool mm) {

    if (number <= 0 || number > MAX_NUMBER || size <= 0 || size > MAX_SEATS) {

        return NULL;

    }

    Room new_room = malloc(sizeof(*new_room));
    if (!new_room) {
        return NULL;
    }

    new_room->building_name = nameDuplicate(building_name);
    if (!(new_room->building_name)) {
        free(new_room->building_name);
        free(new_room);
        return NULL;
    }

    new_room->number = number;
    new_room->size = size;
    new_room->multi_media = mm;

    List new_list = listCreate(copyOrder, freeOrder);
    if (!new_list) {
        free(new_room->building_name);
        free(new_room);
        return NULL;
    }
    Order head=malloc(sizeof(*head));
    if(!head) {
        listDestroy(new_list);
        free(new_room->building_name);
        free(new_room);
        return NULL;
    }
    strcpy(head->date,"01/01/0001");
    strcpy(head->start_time,"00:01");
    strcpy(head->end_time,"00:02");
    new_room->orders=new_list;
    listInsertFirst(new_room->orders,head);
    free(head);
    return new_room;
}



// deallocate all relevant memory of room (including all relevant orders, if any)
// and stop using it.
// room, current_date, current_time must not be NULL. (handled by assert).
// current_date is in the format "dd/mm/yyyy".
// current_time is in the format ""hh:mm" 24 hour time.
// can assume that all parameters are legal (if they are not NULL).
// return value (in this priority order) :
// ROOM_IN_USE - if the room is currently in use it can not be destroyed.
//              remark - if there is an order upon the room, but the room is not
//              right now in use, then no problem to deallocate it.
// ROOM_OK - operation completes successfully.

RoomResult roomDestroy(Room room, char *current_date, char *current_time) {

    assert(room && current_date && current_time);
    if(roomInUse(room,current_date,current_time))
        return ROOM_IN_USE;

    listDestroy(room->orders);

    free(room->building_name);
    free(room);
    return ROOM_OK;
}

// update the room size and/or mm according to the parameters.
// room must not be NULL. (handled by assert).
// size should be greater or equal to the current size, otherwise update can not be done.
// mm should be true ONLY, otherwise update can not be done.
// return value (in this priority order) :
// ROOM_NOT_UPDATED - as mentioned above.
// ROOM_ILLEGAL_PARAMETER - if size has more than 3 digits. (no update in this case).
// ROOM_OK - operation completes successfully.

RoomResult roomUpdate(Room room, int size, bool mm) {
    assert (room) ;
    if ( !mm || size < room->size) {
        return ROOM_NOT_UPDATED ;

    }

    if (size>MAX_SEATS) {
        return  ROOM_ILLEGAL_PARAMETER;
    }

    room->size=size;
    room->multi_media= true;
    return ROOM_OK;

}

// get the name of the building and the number of the given room.
// all parameters not be NULL. (handled by assert).
// building_name and number are output parameters.
// building_name should be copied by duplication. no assumptions concerning its length.
// return value (in this priority order) :
// ROOM_MEMORY_ERROR in case of any memory problem.
// ROOM_OK if operation completes successfully.
RoomResult roomBuildingNum(Room room, char **building_name, int *number){
    assert(room && building_name && number);
    *number=room->number;
    *building_name=malloc(strlen(room->building_name)+1);
    if(building_name[0]==NULL) {
        return ROOM_MEMORY_ERROR;
    }
    strcpy(*building_name,room->building_name);
    return ROOM_OK;

}

// checks if room1 should appear before room2 in a sorted list, according to the
// criterions we used in  HW1. (num of places -> MM -> number of room ->
// building name).
// returns true or false accordingly.
// Either room1 or room2 must not be NULL. (handled by assert).
// assume here that room1 is not room2.

bool roomBefore(Room room1, Room room2) {
    assert (room1 && room2);
    if (room1->size < room2->size) return true;
    if ((room1->size == room2->size) && (!(room1->multi_media) && (room2->multi_media)) ) return true;
    if ( (room1->size == room2->size) && ((room1->multi_media)== (room2->multi_media)) && (room1->number < room2->number)) {
        return true;
    }
    if ( (room1->size == room2->size) && ((room1->multi_media)== (room2->multi_media)) && (room1->number == room2->number) &&
         strcmp(room1->building_name,room2->building_name)<0 ) {
        return true;
    }

    return false;

}

// check if room is currently in use according to current_date and current_time.
// room, current_date, current_time must not be NULL. (handled by assert).
// current_date is in the format "dd/mm/yyyy".
// current_time is in the format ""hh:mm" 24 hour time.
// can assume that all parameters are legal (if they are not NULL).
// return value : true or false.

bool roomInUse(Room room, char *current_date, char *current_time) {

    assert(room&& current_date && current_time);

        for(ListElement ptr=listGetFirst(room->orders); ptr!=NULL ; ptr=listGetNext(room->orders) ) {
        if (compareDates(current_date, current_time, ((Order)ptr)->date, ((Order)ptr)->start_time) >= 0 &&
            compareDates(current_date, current_time, ((Order)ptr)->date,((Order)ptr)->end_time) <= 0)
            return true;
    }
    return false;



}



// check if room can be ordered for the required date and start_time/end_time.
// if so, then the room is updated in its order field.
// the field order should keep all approved orders for this room. The orders MUST be kept
// in a chronological sorted manner regarding the date/start_time of the
// order contents, which means - first chronological date/start_time is in the
// beginning of the list, last chronological date/start_time is in the end of the list.
// NONE of the parameters should be NULL. (handled by assert).
// date_order and date are strings containing legal dates in the format "dd/mm/yyyy".
// time_order, start_time, end_time are strings containing legal times in the format
// "hh:mm" (24 hour time).
// can assume that start_time is chronologically before (not equal) end_time.
// return value (in this priority order) :
// ROOM_CAN_NOT_ORDER - if the order can not be fullfilled (not including memory problem).
//                      this includes also the case that the date/hour of the
//                      order request is greater than the date/hour of the order itself
// ROOM_MEMORY_ERROR - memory problem
// ROOM_OK - the operation has been completed successfully.

RoomResult roomOrder(Room room, char *date_order, char *time_order, char *date,
                     char *start_time, char *end_time) {

    assert(room && date_order && time_order&&date && start_time && end_time) ;


    if (compareDates(date_order,time_order,date,start_time)>0) return ROOM_CAN_NOT_ORDER;
    if(roomInUse(room,date,start_time) || roomInUse(room,date,end_time)) return ROOM_CAN_NOT_ORDER;

    Order new_order=malloc(sizeof(*new_order));
    if(!new_order)
        return ROOM_MEMORY_ERROR;

    strcpy(new_order->date,date);
    strcpy(new_order->start_time,start_time);
    strcpy(new_order->end_time,end_time);

    ListElement ptr=listGetFirst(room->orders);

    while(ptr) {

        if(compareDates(date,start_time,((Order)ptr)->date,((Order)ptr)->end_time) > 0) {
            ptr=listGetNext(room->orders);
            continue;
        }
        else {
        if( listInsertBeforeCurrent(room->orders,new_order)!=LIST_SUCCESS) {
            free(new_order);
            return  ROOM_MEMORY_ERROR;

        }
        free(new_order);
            return ROOM_OK;
        }
    }

    if(listInsertLast(room->orders,new_order)!=LIST_SUCCESS) {
        free(new_order);
        return ROOM_MEMORY_ERROR;
    }
    free(new_order);
    return ROOM_OK;
}



// remove an order that has been already approved upon room.
// the order field of room is updated accordingly.
// such a removal is possible if there exists such an order upon the given room that
// starts in the given date/start_time, as well as that the room is not currently in use
// upon this order. (other order do not matter).
// NONE of the parameters should be NULL. (handled by assert).
// current_date and date are strings containing legal dates in the format "dd/mm/yyyy".
// current_time and start_time are strings containing legal times in the format
// "hh:mm" (24 hour time).
// return value (in this priority order) :
// ROOM_CAN_NOT_REMOVE
// ROOM_OK

RoomResult roomRemoveOrder(Room room, char *current_date, char *current_time,
                           char *date, char *start_time) {


    assert(room && current_time && current_date && date && start_time);

    ListElement ptr=listGetFirst(room->orders);

    while (ptr) {


        if( compareDates(((Order)ptr)->date,((Order)ptr)->start_time,date,start_time)==0 )
        {

            if (compareDates(current_date, current_time, ((Order)ptr)->date, ((Order)ptr)->start_time) >= 0 &&
                compareDates(current_date, current_time, ((Order)ptr)->date, ((Order)ptr)->end_time) <= 0)
                return ROOM_CAN_NOT_REMOVE;
            else {

                listRemoveCurrent(room->orders);

                return ROOM_OK;
            }

        }
        ptr=listGetNext(room->orders);

    }

    return ROOM_CAN_NOT_REMOVE;
}




// remove all orders of room that have been already implemented and now
// they are not longer relevant.
// NONE of the parameters should be NULL. (handled by assert).
// current_date is a string containing a legal date in the format "dd/mm/yyyy".
// current_time is a string containing a legal time in the format "hh:mm" (24 hour time).
// return value : the number of orders that have been removed in this operation.

int roomCleanOrders(Room room, char *current_date, char *current_time) {

    assert(room && current_time && current_date);
    int removed=0;
    ListElement ptr=listGetFirst(room->orders);
    ptr=listGetNext(room->orders);
    while(ptr) {
        if(compareDates(((Order)ptr)->date,((Order)ptr)->end_time,current_date,current_time) < 0 ) {
            if(roomRemoveOrder(room,current_date,current_time,((Order)ptr)->date,((Order)ptr)->start_time)==ROOM_OK) {

                removed++;
                ptr=listGetFirst(room->orders);
                ptr=listGetNext(room->orders);
                continue;
            }
        }

        ptr=listGetNext(room->orders);

    }

    return removed;



}


// display a room details.
// room and out_file must not be NULL. (handled by assert).
// assuming room is legal (unless it's NULL).
// out_file is the output_file to which the output is written. assuming it's already
// opened for writing in the appropriate place. the operation does not open the file, nor closes it.
// If cannot write the complete output
// to the file, then ROOM_CAN_NOT_DISPLAY is returned, otherwise ROOM_OK is returned.
// the format of the output is as follows :
// first line :
// <building_name>  <room_number>  <number_of_seat_places> <MM (if has a multimedia) or -MM (if does not have)>
// no leading zeros. no spaces in the beginning or end of the line. in the end of line going to
// the beginning of next line. one space only between every 2 successive words in the line.
// for each order of allocation there will be displayed a whole line in the following format :
// <date> <start_time> <end_time>
// date is displayed in the format dd/mm/yyyy
// time is displayed in the format  hh:mm (24 hours)
// no leading zeros. no spaces in the beginning or end of the line. in the end of line going to
// the beginning of next line. one space only between every 2 successive words in the line.
// all lines concerning orders of allocation are displayed in a chronological sorted
// manner regarding the date/start_time of the order contents, which means - the order with
// the first chronological date/start_time is displayed in the upper line,
// and the order with the last chronological date/start_time is displayed in the lower line.
// If no orders at all, then no lines of orders are displayed.
// return value (as already mentioned) :
// ROOM_CAN_NOT_DISPLAY
// ROOM_OK

RoomResult roomDisplay(Room room, FILE *out_file) {

    assert(room&&out_file);
    char multimedia_str[MM_STRING];
    if(room->multi_media) {
        strcpy(multimedia_str,"MM");
    } else strcpy(multimedia_str,"-MM");
    if(!out_file) {
        return ROOM_CAN_NOT_DISPLAY;
    }
    fprintf(out_file,"%s %d %d %s\n",room->building_name,room->number,room->size,multimedia_str);

    ListElement ptr=listGetFirst(room->orders);
    ptr=listGetNext(room->orders);
    while(ptr) {

        fprintf(out_file,"%s %s %s\n",((Order)ptr)->date,((Order)ptr)->start_time,((Order)ptr)->end_time);
        ptr=listGetNext(room->orders);
    }




    return  ROOM_OK;
}
