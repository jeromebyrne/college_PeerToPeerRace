class PacketStream
{
private:
	
ostringstream outputStream;// (use in write methods)
istringstream inputStream; //(use in read methods)


public:

// Constructor	
PacketStream(){}

void fromCharArray(char* arrayIn); // – populates the packetStream with the data stored in arrayIn

void toCharArray(char* arrayIn); // – populates the arrayIn with the data stored in the packetStream

void writeInt(int value); // – writes an integer to the stream  

void readInt(int& value); // – reads an integer from the stream

void writeFloat(float value); // – writes a float to the stream

void readFloat(float& value); // – reads a float from the stream

};