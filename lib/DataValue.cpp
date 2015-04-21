#include <Data.h>
#include <iostream>

using namespace std;

/*
	Minimum Hash Value = 0
	Smaller Hash Value => higher preference
	Negative Hash Value => Undefined Value, non-comparable
*/

void DataValue::updateIfLargerThan(DataValue& I) {
	if(this->compareWith(I)==1)
		this->updateTo(I);
}

void DataValue::updateIfSmallerThan(DataValue& I) {
	if(this->compareWith(I)==-1)
		this->updateTo(I);
}

double IntDataValue::MIN_HASH = 0.0;
const int IntDataValue::type = 1; //Should be unique

int IntDataValue::compareWith(DataValue &I) {
	if(type!=I.getType()) return 2;
	/*TODO|Upgrade: Comparing different types, e.g. integer and string are non-comparable while hexadecimal is comparable to integer*/

	//Same Datatype case
	IntDataValue * Iptr = (IntDataValue*)I.objectReference();
	double Ivalue = Iptr->getHash();
	if(Ivalue<0 || this->HashValue<0) return 2;
	if(this->HashValue < Ivalue) return -1;
	else if(Ivalue == this->value) return 0;
	return 1;
}

void IntDataValue::updateTo(DataValue &I) {
	if(type!=I.getType()) return;
	IntDataValue * Iptr = (IntDataValue*)I.objectReference();
	this->value = Iptr->getValue();
}

DataValue& IntDataValue::createCopy() {
	DataValue * newVal = new IntDataValue(value);
	return *newVal;
}

void IntDataValue::setHash() {
	/*Define hash function such that more preferred value gets lesser HashValue*/
	HashValue = (double)value;
}

void IntDataValue::printDataValue() {
	cout << "(integer) " << value << " @ " << this << endl;
}

double DoubleDataValue::MIN_HASH = 0.0;
const int DoubleDataValue::type = 2; //Should be unique

int DoubleDataValue::compareWith(DataValue& I) {
	if(type!=I.getType()) return 2;

	DoubleDataValue * Iptr = (DoubleDataValue*)I.objectReference();
	double Ivalue = Iptr->getHash();
	if(Ivalue<0 || this->HashValue<0) return 2;
	if(this->value < Ivalue) return -1;
	else if(Ivalue == this->value) return 0;
	return 1;
}

void DoubleDataValue::updateTo(DataValue& I) {
	if(type!=I.getType()) return;
	DoubleDataValue * Iptr = (DoubleDataValue*)I.objectReference();
	this->value = Iptr->getValue();
}

DataValue& DoubleDataValue::createCopy() {
	DataValue * newVal = new DoubleDataValue(value);
	return *newVal;
}

void DoubleDataValue::setHash() {
	/*Define hash function such that more preferred value gets lesser HashValue*/
	HashValue = (double)value;
}

void DoubleDataValue::printDataValue() {
	cout << "(double) " << value << " @ " << this << endl;
}

double StringDataValue::MIN_HASH = 0.0;
const int StringDataValue::type = 3; //Should be unique

int StringDataValue::compareWith(DataValue &I) {
	if(type!=I.getType()) return 2;
	/*Can implement lexiographic comparisions*/
	return 2;
}

void StringDataValue::updateTo(DataValue &I) {
	if(type!=I.getType()) return;
	StringDataValue * Iptr = (StringDataValue*)I.objectReference();
	this->value = Iptr->getValue();
}

DataValue& StringDataValue::createCopy() {
	DataValue * newVal = new StringDataValue(value);
	return *newVal;
}

void StringDataValue::setHash() {
	/*Define hash function such that more preferred value gets lesser HashValue*/
	HashValue = -1.0;
}

void StringDataValue::printDataValue() {
	cout << "(string) " << value << " @ " << this << endl;
}
