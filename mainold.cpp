
#include "FunctionsProgram.h"
#include "CSerial.h"
CSerial serial;


int main(int argc, char **argv)
{ 
	const char serialBuffer[6] = "hello";
		int  serialOpen, nBytesRead;


	serialOpen=serial.Open(6,9600);
	cout<<serialOpen;


	if(serialOpen){

		nBytesRead=serial.SendData(serialBuffer,strlen(serialBuffer));
		//ASSERT(nBytesRead==strlen(serialBuffer));
		//delete [] serialBuffer;
	
	}

	serial.Close();

  cout << "\n";
  cout << "=========================================================\n";
  cout << "getTristarLog aborted. \n\n";

  return 0;
  
}


