#include <Data.h>

void DataValue::updateIfLargerThan(DataValue& I) {
	if(this->compareWith(I)==1)
		this->updateTo(I);
}

void DataValue::updateIfSmallerThan(DataValue& I) {
	if(this->compareWith(I)==-1)
		this->updateTo(I);
}

int IntDataValue::compareWith(DataValue &I) {
	if(type!=I.getType()) return 2;
	/*TODO|Upgrade: Can compare values of two different types. e.g. comparing 'int' with 'hexadecimal' type*/
	int Ivalue = I.objectReference()->getValue();
	if(this->value < Ivalue) return -1;
	else if(Ivalue == this->value) return 0;
	return 1;
}

void IntDataValue::updateTo(DataValue &I) {
	if(type!=I.getType()) return;
	this->value = I.objectReference()->getValue();
}

DataValue& IntDataValue::createCopy() {
	DataValue * newVal = new IntDataValue(value);
	return *newVal;
}

int DoubleDataValue::compareWith(DataValue& I) {
	if(type!=I.getType()) return 2;

	double Ivalue = I.objectReference()->getValue();
	if(this->value < I.value) return -1;
	else if(I.value == this->value) return 0;
	return 1;
}

void DoubleDataValue::updateTo(DataValue& I) {
	if(type!=I.getType()) return;
	this->value = I.objectReference()->getValue();
}

DataValue& DoubleDataValue::createCopy() {
	DataValue * newVal = new DoubleDataValue(value);
	return *newVal;
}

int StringDataValue::compareWith(DataValue &I) {
	if(type!=I.getType()) return 2;
	/*Can implement lexiographic comparisions*/
	return 2;
}

void StringDataValue::updateTo(DataValue &I) {
	if(type!=I.getType()) return;
	this->value = I.objectReference()->getValue();
}

DataValue& StringDataValue::createCopy() {
	DataValue * newVal = new StringDataValue(value);
	return *newVal;
}
