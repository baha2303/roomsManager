#include <stdlib.h>
#include <string.h>
#include "Room.h"


int main()
{
    char name[]="Ulmannn";
    Room room1=roomCreate("Ulman",507,100,true);
    if(!room1)
        exit(1);
    Room room2=roomCreate(name,450,30,false);
    if(!room2) exit(2);
    Room room3=roomCreate("Daviss",200,200,false);
    if(!room3) exit(3);
    printf("Rooms created successfully\n");
    printf("Room1:\n");
    roomDisplay(room1,stdout);
    strcpy(name,"Ulman");
    printf("Room2:\n");
    roomDisplay(room2,stdout);
    if(roomDestroy(room2,"28/08/2018","16:00")==ROOM_OK) {
        printf("Room2 destroyed\n");
    }
    if(roomUpdate(room3,300,true)==ROOM_OK) {
        printf("Room3 updated\n");
    }
    printf("Room3:\n");
    roomDisplay(room3,stdout);
    if(roomBefore(room1,room3)) printf("Room 1 is first\n");
    else printf("Room 3 is first\n");
    if (roomDestroy(room3,"28/08/2018","16:02")==ROOM_OK) {
        printf("Room3 destroyed\n");
    }

    if(roomOrder(room1,"28/08/2018","16:03","29/08/2018","16:00","17:30")==ROOM_OK) {
        printf("Event added to Room1:\n");
    }
    if(roomOrder(room1,"28/08/2018","16:03","29/08/2018","16:00","17:30")==ROOM_CAN_NOT_ORDER) {
        printf("Cant order the Room1!!\n");
    }
    if(roomOrder(room1,"28/08/2018","16:03","29/08/2017","16:00","17:30")!=ROOM_OK) {
        printf("Cant order the Room1!!\n");
    }
    if (roomOrder(room1,"28/08/2018","16:03","30/08/2018","16:00","17:30")==ROOM_OK) {
        printf("Event added to Room1:\n");
    }
    if(roomOrder(room1,"28/08/2018","16:03","01/09/2018","16:00","17:30")==ROOM_OK) {
        printf("Event added to Room1:\n");
    }
    printf("Room1:\n");
    roomDisplay(room1,stdout);
    if(roomInUse(room1,"01/09/2018","16:59"))
        printf("Room is in use at 01/09/2018 16:59\n");

    if(roomRemoveOrder(room1,"01/09/2018","16:59","01/09/2018","16:00")!=ROOM_OK) {
        printf("Order cant be removed at this time\n");
    }

    if(roomRemoveOrder(room1,"25/08/2018","16:59","01/09/2018","16:00")==ROOM_OK)
        printf("Order removed\n");
    printf("Room1:\n");
    roomDisplay(room1,stdout);
    int events_deleted;
    events_deleted=roomCleanOrders(room1,"01/09/2018","16:59");

    printf("%d orders deleted\n",events_deleted);
    printf("Room1:\n");
    roomDisplay(room1,stdout);
    if (roomDestroy(room1,"01/09/2019","16:59")==ROOM_OK) {
        printf("room removed successfully\n");
    }



    return 0;
}
