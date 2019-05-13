#ifndef ROOM_H
#define ROOM_H

#include <stdio.h>
#include <stdbool.h>

//------------------------------------------------------------------------------------------

typedef struct room *Room;

//------------------------------------------------------------------------------------------

typedef enum { ROOM_OK , ROOM_IN_USE , ROOM_MEMORY_ERROR , ROOM_ILLEGAL_PARAMETER , ROOM_NOT_UPDATED ,
               ROOM_CAN_NOT_ORDER , ROOM_CAN_NOT_REMOVE, ROOM_CAN_NOT_DISPLAY } RoomResult;

//------------------------------------------------------------------------------------------

// create a new room with the details of the parameters building_name , number (number of the
// room), size (number of seat places), mm (multi_media).
// the room is created with no orders yet.
// building_name should be copied into the new crated room by duplication.
// building_name must not be NULL. (handled by assert).
// number should be great than 0 with no more than 4 digits.
// size should be great than 0 with no more than 3 digits.
// if cannot create due to any kind of reason (including illegal parameters, not including
// assert case), then NULL is returned. otherwise the new created room is returned.

Room roomCreate(char *building_name, int number, int size, bool mm);

//------------------------------------------------------------------------------------------

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

RoomResult roomDestroy(Room room, char *current_date, char *current_time);

//------------------------------------------------------------------------------------------

// update the room size and/or mm according to the parameters.
// room must not be NULL. (handled by assert).
// size should be greater or equal to the current size, otherwise update can not be done.
// mm should be true ONLY, otherwise update can not be done.
// return value (in this priority order) : 
// ROOM_NOT_UPDATED - as mentioned above.
// ROOM_ILLEGAL_PARAMETER - if size has more than 3 digits. (no update in this case).
// ROOM_OK - operation completes successfully.

RoomResult roomUpdate(Room room, int size, bool mm);

//------------------------------------------------------------------------------------------

// get the name of the building and the number of the given room.
// all parameters not be NULL. (handled by assert).
// building_name and number are output parameters.
// building_name should be copied by duplication. no assumptions concerning its length.
// return value (in this priority order) :
// ROOM_MEMORY_ERROR in case of any memory problem.
// ROOM_OK if operation completes successfully.

RoomResult roomBuildingNum(Room room, char **building_name, int *number);

//------------------------------------------------------------------------------------------

// checks if room1 should appear before room2 in a sorted list, according to the
// criterions we used in  HW1. (num of places -> MM -> number of room ->
// building name).
// returns true or false accordingly.
// Either room1 or room2 must not be NULL. (handled by assert).
// assume here that room1 is not room2.

bool roomBefore(Room room1, Room room2);

//------------------------------------------------------------------------------------------

// check if room is currently in use according to current_date and current_time.
// room, current_date, current_time must not be NULL. (handled by assert).
// current_date is in the format "dd/mm/yyyy".
// current_time is in the format ""hh:mm" 24 hour time.
// can assume that all parameters are legal (if they are not NULL).
// return value : true or false.

bool roomInUse(Room room, char *current_date, char *current_time);

//------------------------------------------------------------------------------------------

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
                     char *start_time, char *end_time);

//------------------------------------------------------------------------------------------

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
                           char *date, char *start_time);

//------------------------------------------------------------------------------------------

// remove all orders of room that have been already implemented and now
// they are not longer relevant.
// NONE of the parameters should be NULL. (handled by assert).
// current_date is a string containing a legal date in the format "dd/mm/yyyy".
// current_time is a string containing a legal time in the format "hh:mm" (24 hour time).
// return value : the number of orders that have been removed in this operation.

int roomCleanOrders(Room room, char *current_date, char *current_time);

//------------------------------------------------------------------------------------------

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

RoomResult roomDisplay(Room room, FILE *out_file);


//------------------------------------------------------------------------------------------

#include "Room1.h"

//------------------------------------------------------------------------------------------


#endif // ifndef ROOM_H
