/*
 Name:		List.ino
 Created:	2019/5/18 10:45:24
 Author:	qinbi
*/
#include "List.h"
List<int> intList;
int a = 100;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200U);
	intList.pushBack(12);
	intList.pushBack(13);
	intList.pushBack(a);
	intList.insert(++intList.begin(), 25);
}

// the loop function runs over and over again until power down or reset
void loop() {
	List<int>::Iterator iterator = intList.begin();
	for (; iterator != intList.end(); iterator++) {
		Serial.println(*iterator);
	}
}
