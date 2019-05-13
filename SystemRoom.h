#ifndef SYSTEM_ROOM_H
#define SYSTEM_ROOM_H

#include <stdio.h>
#include <stdbool.h>

#define WORD_LEN 50

//------------------------------------------------------------------------------------------

typedef struct system_room *SystemRoom;

//------------------------------------------------------------------------------------------

typedef enum { SYSTEM_OK , SYSTEM_CAN_NOT_DESTROY , SYSTEM_MEMORY_ERROR ,
    SYSTEM_ROOM_EXISTS , SYSTEM_ILLEGAL_PARAMETER , SYSTEM_NOT_UPDATED ,
    SYSTEM_ROOM_NOT_EXIST , SYSTEM_ROOM_IN_USE , SYSTEM_CAN_NOT_ORDER ,
    SYSTEM_ORDER_NOT_EXIST , SYSTEM_CAN_NOT_DISPLAY } SystemResult;

//------------------------------------------------------------------------------------------

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

SystemRoom systemCreate(char *system_name, char *room_file);

//------------------------------------------------------------------------------------------

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

SystemResult systemDestroy(SystemRoom system, char *current_date, char *current_time);

//------------------------------------------------------------------------------------------

// add a new room to the system.
// all parameters (except system) should be treated exactly as in roomCreate.
// system and building_name must not be NULL (handled by assert).
// the room should be added in the list of rooms. the list should be sorted
// as described in systemCreate.
// return value (in the following priority of order) :
// SYSTEM_ROOM_EXISTS - if such a room already exists (according to building_name and number,
//                      NO MATTER other parameters).
// SYSTEM_ILLEGAL_PARAMETER - if any relevant parameter is illegal.
// SYSTEM_MEMORY_ERROR - any memory problem.
// SYSTEM_OK - operation completes successfully.

SystemResult systemRoomAdd(SystemRoom system, char *building_name, int number, int size, bool mm);

//------------------------------------------------------------------------------------------

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

SystemResult systemRoomUpdate(SystemRoom system, char *building_name, int number, int size, bool mm);

//------------------------------------------------------------------------------------------

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
                              char *current_date, char *current_time);

//------------------------------------------------------------------------------------------

// request for a room that has at least requested_seat_places places and
// if requested_mm is true then it must have mtltimedia.
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

SystemResult systemRoomRequest(SystemRoom system, char *current_date, char *current_time,
                               char *date_for_request, char *start_time_for_request,
                               char *end_time_for_request,
                               int requested_seat_places, bool requested_mm,
                               char **building_name, int *number);

//------------------------------------------------------------------------------------------

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
// SYSTEM_OK

SystemResult systemRemoveRequest(SystemRoom system, char *current_date, char *current_time,
                                 char *building_name, int number, char *date_for_request,
                                 char *start_time_for_request);

//------------------------------------------------------------------------------------------

// remove all orders of all rooms that have been already implemented, so now
// they are not longer relevant.
// NONE of the parameters should be NULL. (handled by assert).
// current_date is a string containing a legal date in the format "dd/mm/yyyy".
// current_time is a string containing a legal time in the format "hh:mm" (24 hour time).
// return value : the number of orders that have been removed in this operation.

int systemCleanPastOrders(SystemRoom system, char *current_date, char *current_time);

//------------------------------------------------------------------------------------------

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

SystemResult systemDisplay(SystemRoom system, char *out_file);


//------------------------------------------------------------------------------------------

#endif // ifndef SYSTEM_ROOM_H
