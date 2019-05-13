//
// Created by baha2 on 03-Sep-18.
//
#include "list.h"
#include "SystemRoom.h"
#include "Room.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


#define MM_STRING 4
#define MAX_SEATS 999
#define MAX_NUMBER 9999
#define DATE_STR 11
#define TIME_STR 6
#define FULL_TIME_STR 13
#define WORD_LEN 50

struct room
{
    char *building_name;
    int number;
    int size;
    bool multi_media;
    void *orders;
};

struct system_room {
    char *system_name;
    int number_of_rooms;
    void *rooms;
};



/**********************************Functions for List create *****/


ListElement roomCopy(ListElement room);
void roomFree (ListElement room);


ListElement roomCopy(ListElement room) {

    if(room==NULL) return NULL;
    Room new_room=malloc(sizeof(*new_room));
    if(new_room==NULL) return NULL;
    new_room->building_name=malloc(strlen(((Room)(room))->building_name)+1);
    if(new_room->building_name==NULL) return NULL;
    strcpy(new_room->building_name,((Room)(room))->building_name);
    new_room->multi_media=((Room)(room))->multi_media;
    new_room->number=((Room)(room))->number;
    new_room->size=((Room)(room))->size;
    new_room->orders=listCopy(((Room)room)->orders);
    if(new_room->orders==NULL) {
        free(new_room->building_name);
        free(new_room);
        return NULL;
    }


    return new_room;
}

void roomFree (ListElement room) {
    if(!room)
        return;
    roomDestroy(((Room)room),"15/10/3000","23:59");
}

/*******************************************************************/
int roomBeforeAux(ListElement room1, ListElement room2);


void lineToStrings (const char * str,char* name, char* size , char* number , char * mm);
void lineToStrings(const char *str, char *name, char *size, char *number, char *mm) {
    int is_str=0;
    int i = 0, j = 0;
    int value = 0;
    int ptr=0;
    while (ptr!=WORD_LEN) {
        number[ptr]='\0';
        size[ptr]='\0';
        name[ptr]='\0';
        mm[ptr]='\0';
        ptr++;
    }

    while (str[i]) {

        if (str[i] == '\n') return;
        if (str[i] <= ' ') {
            if (is_str == 1) {
                is_str = 0;
                j = 0;
                value+=1;

            }
            i++;
            continue;
        }
        is_str = 1;

        switch (value) {
            case 0 :
                name[j] = str[i];
                i++;
                j++;

                break;

            case 1:
                number[j] = str[i];
                i++;
                j++;

                break;
            case 2:
                size[j] = str[i];
                i++;
                j++;

                break;
            case 3:
                mm[j] = str[i];
                i++;
                j++;

                break;
            default:

                return;

        }


    }

}







// create a new system.
// the name of the system is system_name.
// if room_file is "" then no rooms exists in the initial system created, otherwise
// the system will have in its initial state all rooms that are defined in the room_file.
// in this case, one can assume that the room_file exists and legal. Its contents
// is exactly like the file rooms in HW1. can assume that each single word (of any kind) in the file
// is not longer than WORD_LEN characters.
// the function initializes the fields of the structure implements SystemRoom.
// system_name is copied by duplication. rooms should be a linked list that includes
// all rooms. There are no requests for rooms when creating the system.
// The list of the rooms is sorted exactly as the outpout of HW1 part A is sorted.
// (from small to big : num of seat places -> -MM before MM -> num of room -> name of building).
// Both parameters must not be NULL (handled by assert).
// return value :
// NULL if system can not be created due to any reason.
// otherwise, the new created system is returned.

SystemRoom systemCreate(char *system_name, char *room_file){
    assert( system_name && room_file);

    SystemRoom new_system= malloc(sizeof(*new_system));
    if(!new_system) return  NULL;

    new_system->system_name=malloc(strlen(system_name)+1);
    if(!new_system->system_name) {
        free(new_system);
        return NULL;
    }
    strcpy(new_system->system_name,system_name);

    List new_list=listCreate(roomCopy,roomFree);
    if (!new_list) {
        free(new_system->system_name);
        free(new_system);
        return NULL;
    }
    Room head=roomCreate("HEAD",1,1,false);
    if(!head) {
        listDestroy(new_list);
        free(new_system->system_name);
        free(new_system);
        return NULL;
    }


    listInsertFirst(new_list,head);
    roomDestroy(head,"28/04/2000","16:00");
    new_system->rooms=new_list;

    if (strcmp(room_file,"")==0) {
        new_system->number_of_rooms=0;
        return new_system;
    }

    FILE* stream=fopen(room_file,"r");
    if(!stream)
    {
        systemDestroy(new_system,"31/10/3000","15:00");
        return NULL;
    }
    int count=0,room_num,room_size;
    char name[WORD_LEN]="",mm[WORD_LEN]="",size[WORD_LEN]="",number[WORD_LEN]="";
    char buf[WORD_LEN*4]="";


    while(fgets(buf,WORD_LEN*4,stream)!=NULL) {

        lineToStrings(buf,name,size,number,mm);
        if (strcmp(name,"")==0) continue;
        room_num=atoi(number);
        room_size=atoi(size);
        bool mm_yes_orr_no = !(strcmp(mm,"Y"));
        if (  *mm == '\0') mm_yes_orr_no = true;
        systemRoomAdd(new_system, name, room_num, room_size, mm_yes_orr_no);
        count++;
    }
    new_system->number_of_rooms=count;
    fclose(stream);
    return new_system;

}


// deallocate all relevant memory of system (including all rooms, if any)
// and stop using it.
// system, current_date, current_time must not be NULL. (handled by assert).
// current_date is in the format "dd/mm/yyyy".
// current_time is in the format ""hh:mm" 24 hour time.
// can assume that all parameters are legal (if they are not NULL).
// The system can not be destroyed if there is a room that is currently in use.
// (room requests that are not relevant to the current time do not matter).
// in that case - SYSTEM_CAN_NOT_DESTROY is returned.
// otherwise - all system parts are destroyed, and SYSTEM_OK is returned.


SystemResult systemDestroy(SystemRoom system, char *current_date, char *current_time) {
    assert(system && current_date && current_date) ;

    for( ListElement ptr=listGetFirst(system->rooms) ; ptr !=NULL ; ptr=listGetNext(system->rooms)) {
        if (roomInUse(((Room)ptr), current_date, current_time)) return SYSTEM_CAN_NOT_DESTROY;
    }

    List list=system->rooms;
    listDestroy(list);
    free(system->system_name);
    free(system);
    return SYSTEM_OK;

}


// add a new room to the system.
// all parameters (except system) should be treated exactly as in roomCreateAux.
// system and building_name must not be NULL (handled by assert).
// the room should be added in the list of rooms. the list should be sorted
// as described in systemCreate.
// return value (in the following priority of order) :
// SYSTEM_ROOM_EXISTS - if such a room already exists (according to building_name and number,
//                      NO MATTER other parameters).
// SYSTEM_ILLEGAL_PARAMETER - if any relevant parameter is illegal.
// SYSTEM_MEMORY_ERROR - any memory problem.
// SYSTEM_OK - operation completes successfully.

SystemResult systemRoomAdd(SystemRoom system, char *building_name, int number, int size, bool mm) {

    assert(system && building_name );
    for( Room ptr=listGetFirst(system->rooms) ; ptr !=NULL ; ptr=listGetNext(system->rooms)) {
        if ((strcmp(ptr->building_name,building_name)==0) &&( ptr->number==number )) return SYSTEM_ROOM_EXISTS;

    }
    if(number<=0 || number>MAX_NUMBER || size <=0 || size>MAX_SEATS) {
        return  SYSTEM_ILLEGAL_PARAMETER;
    }

    Room new_room= roomCreate(building_name, number, size, mm);
    if (!new_room) return SYSTEM_MEMORY_ERROR;
    if(listInsertLast(system->rooms,(Room)new_room) != LIST_SUCCESS) {
        roomDestroy(new_room, "31/10/3000", "15:00");
        return SYSTEM_MEMORY_ERROR;
    }

    if(listSort(system->rooms,roomBeforeAux)!=LIST_SUCCESS) {
        roomDestroy(new_room, "31/10/3000", "15:00");
        return SYSTEM_MEMORY_ERROR;
    }
    roomDestroy(new_room, "31/10/3000", "15:00");
    return SYSTEM_OK;

}



// update the room size and mm according to the relevant parameters.
// the room is identified according to building_name and number.
// system and building_name must not be NULL. (handled by assert).
// update can be done only if the update does not decreases the number of seat numbers
// and multimedia is not omitted.
// return value (in this priority order) :
// SYSTEM_ROOM_NOT_EXIST - the room does not exist.
// SYSTEM_NOT_UPDATED - if size is lower than the current size or mm is false.
// SYSTEM_ILLEGAL_PARAMETER - size must not have more than 3 digits.
// SYSTEM_OK - operation completes successfully.

SystemResult systemRoomUpdate(SystemRoom system, char *building_name, int number, int size, bool mm) {

    assert(system && building_name);

    for (Room ptr = listGetFirst(system->rooms); ptr != NULL; ptr = listGetNext(system->rooms)) {
        if (strcmp(building_name, ptr->building_name) == 0 && number == ptr->number) {

            if (size < ptr->size || mm == false) return SYSTEM_NOT_UPDATED;
            if (size > MAX_SEATS) return SYSTEM_ILLEGAL_PARAMETER;

            if (roomUpdate(ptr, size, mm) != ROOM_OK)
                return SYSTEM_NOT_UPDATED;

            return SYSTEM_OK;

        }
    }
    return SYSTEM_ROOM_NOT_EXIST;

}

// remove a room from the system.
// system ,building_name, current_date, current_time must not be NULL (handled by assert).
// current_date is legal and in the format "dd/mm/yyyy".
// current_time is legal and in the format "hh:mm" (24 hours).
// the room can be removed only if it exists and it is not currently in use.
// requests for this room that are not relevant for the current time do not matter.
// return value (in the following priority of order) :
// SYSTEM_ROOM_NOT_EXIST - room does not exist.
// SYSTEM_ROOM_IN_USE - the room is in use. can not remove it.
// SYSTEM_OK - operation completes successfully.

SystemResult systemRoomRemove(SystemRoom system, char *building_name, int number,
                              char *current_date, char *current_time) {

    assert(system && building_name && current_date && current_time);

    for (ListElement ptr = listGetFirst(system->rooms); ptr != NULL; ptr = listGetNext(system->rooms)) {

        if (strcmp(((Room)ptr)->building_name, building_name) == 0 && ((Room)ptr)->number == number) {
            if (roomInUse(((Room)ptr), current_date, current_time)) {
                return SYSTEM_ROOM_IN_USE;
            }

            roomDestroy(((Room)ptr), current_date, current_time);
            listRemoveCurrent(system->rooms);
            return SYSTEM_OK;

        }
    }

    return SYSTEM_ROOM_NOT_EXIST;

}


// request for a room that has at least requested_seat_places places and
// if requested_mm is true then it must have multimedia.
// dates and times are given by parameters.
// the following parameters must not be null (handled by assert) :
// system, current_date, current_time, date_for_request, start_time_for_request,
// end_time_for_request, building_name, number.
// building_name and number are output parameters.
// all dates are in the format "dd/mm/yyyy".
// all times are in the format "hh:mm" (24 hours).
// can assume : start_time_for_request is chronologically before end_time_for_request.
// the criterions for choose the room are exactly as in HW1 part C.
// if current date/time is chronological after the requested date/time then the request is denied.
// if the request can be supported, then the relevant parts in the data types are updated
// accordingly.
// return value (in this priority order) :
// SYSTEM_CAN_NOT_ORDER - if the order can not be supported (not including memory problem).
//                      this includes also the case that the date/hour of the
//                      order request is greater than the date/hour of the order itself.
// SYSTEM_MEMORY_ERROR - memory problem
// SYSTEM_OK - the operation has been completed successfully. output is given by the
//             output parameters building_name and number.

SystemResult systemRoomRequest(SystemRoom system, char *current_date, char *current_time, char *date_for_request,
                               char *start_time_for_request, char *end_time_for_request, int requested_seat_places,
                               bool requested_mm,
                               char **building_name, int *number) {

    assert(system && current_date && current_time && date_for_request && start_time_for_request &&
           end_time_for_request && building_name && number);
    for (ListElement ptr = listGetFirst(system->rooms); ptr != NULL; ptr = listGetNext(system->rooms)) {
        if ((requested_seat_places > (((Room)ptr)->size)) || (requested_mm == true && ((Room)ptr)->multi_media == false)) continue;

        int result = roomOrder(((Room)ptr), current_date, current_time, date_for_request, start_time_for_request,
                               end_time_for_request);

        if (result == ROOM_OK) {
            if (roomBuildingNum(((Room)ptr), building_name, number) == ROOM_OK)
                return SYSTEM_OK;
            return SYSTEM_MEMORY_ERROR;
        }
        if (result == ROOM_MEMORY_ERROR) return SYSTEM_MEMORY_ERROR;

    }
    return SYSTEM_CAN_NOT_ORDER;

}


// remove a request that has been already approved upon room.
// the order field of room is updated accordingly.
// such a removal is possible if there exists such an order upon the given room that
// starts in the given date/start_time, as well as that the room is not currently in use
// for that request.
// the following parameters should not be NULL (handled by assert) :
// system, current_date, current_time, building_name, date_for_request,
// start_time_for_request.
// relevant dates are strings containing legal dates in the format "dd/mm/yyyy".
// relevant times are strings containing legal times in the format
// "hh:mm" (24 hour time).
// return value (in this priority order) :
// SYSTEM_ROOM_NOT_EXIST
// SYSTEM_ORDER_NOT_EXIST
// SYSTEM_ROOM_IN_USE
// ROOM_OK

SystemResult systemRemoveRequest(SystemRoom system, char *current_date, char *current_time,
                                 char *building_name, int number, char *date_for_request,
                                 char *start_time_for_request) {


    assert(system && current_date && current_time && building_name && date_for_request && start_time_for_request);
    for (ListElement ptr = listGetFirst(system->rooms); ptr != NULL; ptr = listGetNext(system->rooms)) {
        if (strcmp(((Room)ptr)->building_name, building_name) == 0 && ((Room)ptr)->number == number) {

            int result = roomRemoveOrder(((Room)ptr), current_date, current_time, date_for_request, start_time_for_request);
            if (result == ROOM_CAN_NOT_DISPLAY) return SYSTEM_ORDER_NOT_EXIST;

            if (result == ROOM_CAN_NOT_REMOVE) return SYSTEM_ROOM_IN_USE;

            if (result == ROOM_OK) return SYSTEM_OK;
        }

    }
    return SYSTEM_ROOM_NOT_EXIST;
}



// remove all orders of all rooms that have been already implemented, so now
// they are not longer relevant.
// NONE of the parameters should be NULL. (handled by assert).
// current_date is a string containing a legal date in the format "dd/mm/yyyy".
// current_time is a string containing a legal time in the format "hh:mm" (24 hour time).
// return value : the number of orders that have been removed in this operation.

int systemCleanPastOrders(SystemRoom system, char *current_date, char *current_time) {
    assert(system && current_date && current_time);
    int counter = 0;
    for (ListElement ptr = listGetFirst(system->rooms); ptr != NULL; ptr = listGetNext(system->rooms)) {
        counter += roomCleanOrders(((Room)ptr), current_date, current_time);
    }
    return counter;
}



// display the system.
// all the output is written to out_file - the name of the output file. (can include
// also a path). if out_file is "" then output will be displayed in standard output.
// out_file must not be NULL (handled by assert).
// the format of the output is as follows :
// <system_name> : 1 line. no leading/trailing blanks. in the end of line go to the beginning of
// next line.
// ********** : a line containig 10 * (end of line in the end).
// for each room, the room is displayed in the format as described in roomDisplay.
// after each room display there should be
// ********** : a line containig 10 * (end of line in the end).
// return value :
// SYSTEM_CAN_NOT_DISPLAY
// SYSTEM_OK

SystemResult systemDisplay(SystemRoom system, char *out_file) {

    assert(system && out_file);
    if (!out_file) {
        return SYSTEM_CAN_NOT_DISPLAY;

    }
    FILE *output;
    if (strcmp(out_file, "") == 0) {
        output = stdout;
    } else output = fopen(out_file, "w");


    fprintf(output, "%s\n**********\n", system->system_name);

    ListElement ptr = listGetFirst(system->rooms);
    ptr = listGetNext(system->rooms);
    for ( ; ptr != NULL; ptr = listGetNext(system->rooms))  {

        if (roomDisplay(((Room)ptr), output) != ROOM_OK) return SYSTEM_CAN_NOT_DISPLAY;
        fprintf(output, "**********\n");
    }
    if (output != stdout) {
        fclose(output);
    }

    return SYSTEM_OK;
}


// checks if room1 should appear before room2 in a sorted list, according to the
// criterions we used in  HW1. (num of places -> MM -> number of room ->
// building name).
// returns true or false accordingly.
// Either room1 or room2 must not be NULL. (handled by assert).
// assume here that room1 is not room2.

int roomBeforeAux(ListElement room1, ListElement room2) {
    assert (room1 && room2);
    if (((Room)room1)->size <( ((Room)room2))->size) return -1;
    if ((((Room)room1)->size == ((Room)room2)->size) && (!(((Room)room1)->multi_media) && ((Room)room2)->multi_media)) return -1;
    if ( (((Room)room1)->size ==( ((Room)room2))->size) && ((((Room)room1)->multi_media)== (((Room)room2)->multi_media)) && (((Room)room1)->number < ((Room)room2)->number)) {
        return -1;
    }
    if ( (((Room)room1)->size == ((Room)room2)->size) && ((((Room)room1)->multi_media)== (((Room)room2)->multi_media)) && (((Room)room1)->number == ((Room)room2)->number) &&
         strcmp(((Room)room1)->building_name,((Room)room2)->building_name)<0 ) {
        return -1;
    }

    return 1;

}
