#include "precompiled.h"
#include "PacketStream.h"

void PacketStream::fromCharArray(char* arrayIn)
{
	inputStream.str(""); // clear the old stream
	inputStream.str(arrayIn);//populate inputStream 
}

void PacketStream::toCharArray(char* arrayOut)
{
	string s = outputStream.str();  // convert outputStream to a string  
	memcpy(arrayOut, s.c_str(),s.length()); // copies string s to arrayOut
	outputStream.str(""); // clears the outputStream
}

void PacketStream::writeInt(int value)
{
	outputStream << value << " ";
}

void PacketStream::readInt(int& value)
{
	inputStream >> value;
}

void PacketStream::writeFloat(float value)
{
	outputStream << value << " ";
}

void PacketStream::readFloat(float& value)
{
	inputStream >> value;
}