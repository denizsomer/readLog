
#include "FunctionsProgram.h"
#include "Data.h"
#include "CSerial.h"
CSerial serial;

using namespace std;
bool debug=false;
bool once=true;
ofstream fout;

int processTristarLog(char * filename){
MyString name1, name2, note, dir;
char fct[] = "readData";
ifstream Ifile;
MyString line, *word;
int i, nw, ientry=0, iline=0;

cout<<"reading "<<filename<<endl;

Ifile.open(filename);
if(!Ifile.is_open()) {cout << "File not found \n"; return 0;}

	int len, *msg;

  if (!Ifile) return 0;

  while (iline<50)
  {

         line.getNextLine(Ifile);
	     
	     nw = line.split(&word);

		 len=strlen(word->asCharArray())/2;

		 	//allocate memory for the incoming query message
	msg = new int [len];

for (i=0; i<len; i++) 
	msg[i]=charToInt(word[0][2*i])*16+charToInt(word[0][2*i+1]);

//if(processMessagefromTristar(len, msg)!=0) break;



		 if (nw < 1) {cout<<"finished reading "<<filename<<endl; cout<<endl;break;}   

		 		// cout<<word[0]<<word[1]<<endl;

		 		 iline++;

         
		  for (i=0; i<nw; i++) word[i].free(); delete [] word;

		  delete [] msg;

  }
  
  Ifile.close();
    if(fout.is_open())	fout.close();


  return iline;
  }



int charToInt(char c){
	 if( c =='1') return 1;
else if( c =='2') return 2;
else if( c =='3') return 3;
else if( c =='4') return 4;
else if( c =='5') return 5;
else if( c =='6') return 6;
else if( c =='7') return 7;
else if( c =='8') return 8;
else if( c =='9') return 9;
else if( c =='a' || c =='A') return 10;
else if( c =='b' || c =='B') return 11;
else if( c =='c' || c =='C') return 12;
else if( c =='d' || c =='D') return 13;
else if( c =='e' || c =='E') return 14;
else if( c =='f' || c =='F') return 15;
else		return 0;
}

int processMessagefromTristar(void)
{
	
	byte funcType;
	long num;
	byte mask=1;
	char tmp[50];
	
	int i,j,j0, ndata, nobj;
		
		//copy the query byte for byte to the new buffer
		//copy the function type from the incoming query
	funcType = _msg[1];


		if(funcType == IDENTIFY)
	{
		nobj = _msg[7];
		j0=7;

		for(j = 0 ; j < nobj ; j++){
			j0++;
			j0++;

		ndata = _msg[j0];

		for(i = 0 ; i < ndata ; i++) deviceID.append((char)_msg[++j0]);
		deviceID.append(" ");

		}
		deviceOK=true;
	}
	else if(funcType == READ_AI || funcType == READ_AO)
	{
		ndata = _msg[2];
		


			if(serialOK){

		j0=3;

		if(ndata>=32)
		{

		if(once){

			cout<<"   hour logger      alarms bitfield         Vmax    Vmin       Ah   faults bitfield   Tmax Tmin habsorb heq hfloat\n";
			fout<<"   hour logger      alarms                  Vmax    Vmin       Ah   faults            Tmax Tmin habsorb heq hfloat\n";		

		once=false;
		}
		
			while(j0<ndata)
			{

			//hourmeter
			num=((long) _msg[j0++]) <<16;
			num|=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%6d",num/(256));
			{cout<<tmp; fout<<tmp;} 

			//logger flags
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%6d",num);
			{cout<<tmp; fout<<tmp;}

			//alarms
			num=((long) _msg[j0++]) <<16;
			num|=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];

			cout<<" '"; fout<<" '";
			mask=000000000000000000000001;

			for (i=0; i<24; i++)
			{
				if(num & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' "; fout<<"' ";}

			//Vmax
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%8.2f",(double)num*dfact[0]);
			{cout<<tmp; fout<<tmp;}

			//Vmin
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%8.2f",(double)num*dfact[0]);
			{cout<<tmp; fout<<tmp;}

			//Ah
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%9.1f",(double) num*0.1);
			{cout<<tmp; fout<<tmp;}

			 //faults
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			cout<<" '"; fout<<" '";

			mask=0000000000000001;
			for (i=0; i<16; i++)
			{
				if(num & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' "; fout<<"' ";}

			//Tmax
			sprintf(tmp,"%5d",_msg[j0++]);
			{cout<<tmp; fout<<tmp;}

			//Tmin
			sprintf(tmp,"%5d",_msg[j0++]);
			{cout<<tmp; fout<<tmp;}

			//time_absorb
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%6.1f",(double)num/256.);
			{cout<<tmp; fout<<tmp;}

			//time_eq
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%6.1f",(double)num/256.);
			{cout<<tmp; fout<<tmp;}

			//time_float
			num=((long) _msg[j0++]) <<8;
			num|=_msg[j0++];
			sprintf(tmp,"%6.1f",(double)num/256.);
			{cout<<tmp; fout<<tmp;}

			j0=j0+8;

			{cout<<endl; fout<<"\n";}

			} //while(j0<ndata)
	} //if(ndata>=32)
			else{
				switch(icmd){
			case 6: //8 9
cout<<"Vbat: ";  cout<<dfact[0]*((_msg[3] << 8) | _msg[4]); cout<<"V\n"; 
cout<<"Vsens: "; cout<<dfact[1]*((_msg[5] << 8) | _msg[6]); cout<<"V\n"; 
cout<<"Vsol: ";  cout<<dfact[2]*((_msg[7] << 8) | _msg[8]); cout<<"V\n"; 
cout<<"Achg: ";  cout<<dfact[3]*((_msg[9] << 8) | _msg[10]); cout<<"A\n";  
cout<<"Vbat_s: ";cout<<dfact[5]*((_msg[13] << 8) | _msg[14]); cout<<"V\n"; 
cout<<"T_hs: ";  cout<<dfact[6]*((_msg[15] << 8) | _msg[16]); cout<<"C\n"; 
cout<<"T_batt: ";cout<<dfact[7]*((_msg[17] << 8) | _msg[18]); cout<<"C\n"; 
cout<<"Vref: ";  cout<<dfact[8]*((_msg[19] << 8) | _msg[20]); cout<<"V\n"; 


fout<<"Vbat: ";  fout<<dfact[0]*((_msg[3] << 8) | _msg[4]); fout<<"V\n";
fout<<"Vsens: "; fout<<dfact[1]*((_msg[5] << 8) | _msg[6]); fout<<"V\n";
fout<<"Vsol: ";  fout<<dfact[2]*((_msg[7] << 8) | _msg[8]); fout<<"V\n";
fout<<"Achg: ";  fout<<dfact[3]*((_msg[9] << 8) | _msg[10]); fout<<"A\n";
fout<<"Vbat_s: ";fout<<dfact[5]*((_msg[13] << 8) | _msg[14]); fout<<"V\n";
fout<<"T_hs: ";  fout<<dfact[6]*((_msg[15] << 8) | _msg[16]); fout<<"C\n";
fout<<"T_batt: ";fout<<dfact[7]*((_msg[17] << 8) | _msg[18]); fout<<"C\n";
fout<<"Vref: ";  fout<<dfact[8]*((_msg[19] << 8) | _msg[20]); fout<<"V\n";
					 break;

					case 7: //17 2
					field1=(_msg[3] << 8) | _msg[4]; 
					num=((long)field1) <<16;
					num|=(_msg[5] << 8) | _msg[6];
					cout<<"Ah_r: ";  cout<<num*0.1; cout<<"Ah\n";
					fout<<"Ah_r: ";  fout<<num*0.1; fout<<"Ah\n"; 
    				 break;

					case 8: //19 2
					field1=(_msg[3] << 8) | _msg[4]; 
					num=((long)field1) <<16;
					num|=(_msg[5] << 8) | _msg[6];
					cout<<"Ah_t: ";  cout<<num*0.1; cout<<"Ah\n";
					fout<<"Ah_t: ";  fout<<num*0.1; fout<<"Ah\n"; 
    				 break;

					case 9: //21 2
					field1=(_msg[3] << 8) | _msg[4]; 
					num=((long)field1) <<16;
					num|=(_msg[5] << 8) | _msg[6];
					cout<<"hourmeter: ";  cout<<num; cout<<"hrs\n";
					fout<<"hourmeter: ";  fout<<num; fout<<"hrs\n"; 
    				 break;

			 case 10: //23 1
			//alarms
			field1=(_msg[3] << 8) | _msg[4]; 
			cout<<"alarms '"; fout<<"alarms '";

			mask=0000000000000001;
			for (i=0; i<16; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			 case 11: //24 1
			//faults
			field1=(_msg[3] << 8) | _msg[4]; 
			cout<<"faults '"; fout<<"faults '";

			mask=0000000000000001;
			for (i=0; i<16; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			 case 12:  //25 1
			//dip switch
			field1=_msg[4];  
			cout<<"dip switch '"; fout<<"dip switch '";

			mask=00000001;
			for (i=0; i<8; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			case 13: //27 1
			num=(_msg[3] << 8) | _msg[4];
			cout<<"State: ";  cout<<State[num]; cout<<"\n";
			fout<<"State: ";  fout<<State[num]; fout<<"\n"; 
			break;

			case 14: //28 1 
			num=(_msg[3] << 8) | _msg[4];
			if(num>=230){
			cout<<"PWM duty cycle: 100%\n";
			fout<<"PWM duty cycle: 100%\n"; 
			}
			else{
			cout<<"PWM duty cycle: ";  cout<<num/2.3; cout<<"%\n";
			fout<<"PWM duty cycle: ";  fout<<num/2.3; fout<<"%\n"; 
			}
			break;


			 case 15: //0x754C //29 1
			//alarms
			field1=_msg[3]; 
		//	field1=(_msg[3] << 8) | _msg[4]; analogctr++;
			cout<<"alarms '"; fout<<"alarms '";

			mask=00000001;
			for (i=0; i<8; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			 default:
				 break;

			}
		}
	} //if(serialOK)
	else if(deviceOK)
			{

				for(i = 0 ; i < ndata/2 ; i++) 
				{
				 serialNO.append((char) _msg[4+2*i]);
				 serialNO.append((char) _msg[3+2*i]);
				}

				if(serialctr++>2)
				{
				fout.open(serialNO.append(".log").asCharArray());
				cout<<deviceID.asCharArray(); fout<<deviceID.asCharArray();
				cout<<endl; fout<<"\n";
				serialOK=true;
				}

			}
  }
return 0;
}




void connectDevice(void){
if(icmd!=-1) return;
if(icd==1) {icmd=-1; return;}
icmd=icd++;
if(dbg) cout<<"connecting Device"<<"\n";

getStatus(0x03);
}

void readDeviceIdentification(void){
	if(icmd!=-1) return;
if(iid==1) {icmd=-1; return;}

icmd=1+iid++;
if(dbg&&iid==1) cout<<"reading Device Identification"<<"\n";

_len =  3;
//allow room for the Device ID byte, Function type byte, and crc word
_len += 4;

//allocate memory for the query response
_msg = (byte *) malloc(_len);

//write the device ID
_msg[0]=1;//_device->getId();
//write the function type
_msg[1]=0x2B;
_msg[2]=0x0E;
_msg[3]=0x01;
_msg[4]=0x00;

//generate the crc for the query reply and append it
calcCrc();
_msg[_len - 2] = _crc >> 8;
_msg[_len - 1] = _crc & 0xFF;
}

void readDeviceSerial(void){
if(icmd!=-1) return;
if(isr==4)  {icmd=-1; return;}
icmd=2+isr++; 
if(dbg&&isr==1) cout<<"reading Device Serial"<<"\n";
getStatus(0x03);}

void calcCrc(void)
{
	byte	CRCHi = 0xFF,
			CRCLo = 0x0FF,
			Index,
			msgLen,
			*msgPtr;

	msgLen = _len-2;
	msgPtr = _msg;

	while(msgLen--)
	{
		Index = CRCHi ^ *msgPtr++;
		CRCHi = CRCLo ^ _auchCRCHi[Index];
		CRCLo = _auchCRCLo[Index];
	}
	_crc = (CRCHi << 8) | CRCLo;
}

void readLog(void){
	if(icmd!=-1) return;
if(ilg==12)  {icmd=-1; return;}
icmd=16+ilg++;
if(dbg&&ilg==1) cout<<"reading Logs"<<"\n";
getStatus(0x03); }

void getAnalogStatus(void){
		if(icmd!=-1) return;
if(ian==10)  {icmd=-99; return;}

icmd=6+ian++;
if(dbg&&ian==1) cout<<"reading Analog Status"<<"\n";
getStatus(0x03);}

void getStatus(byte funcType)
{

	// message length
	//for each register queried add 2 bytes
	_len =  4;
	//allow room for the Device ID byte, Function type byte, and crc word
	_len += 4;

	//allocate memory for the query response
	_msg = (byte *) malloc(_len);

	//write the device ID
	_msg[0] = 1;//_device->getId();
	//write the function type
	_msg[1] = funcType;

		//write the high byte of the register value
		_msg[2]  =_COMMAND[4*icmd];
		//write the low byte of the register value
		_msg[3] = _COMMAND[4*icmd+1];

		//write the high byte of the register value
		_msg[4]  =_COMMAND[4*icmd+2];
		//write the low byte of the register value
		_msg[5] = _COMMAND[4*icmd+3];


	//generate the crc for the query reply and append it
	calcCrc();
	_msg[_len - 2] = _crc >> 8;
	_msg[_len - 1] = _crc & 0xFF;
}

void getTristarLog(bool useRealTerm)
{
	int i;
	icmd=-1;
	MyString cmd;
	char tmp[30];
	ofstream fout;


	if(useRealTerm){
fout.open("FROMtristar.log");
 fout.close();
	}

	while(icmd!=-99)
	{
		connectDevice();
		readDeviceIdentification();
		readDeviceSerial();
		readLog();
		getAnalogStatus();

		if(icmd==-99) break;

		if(_len) {
			if(useRealTerm) getLogRealTerm("FROMtristar.log");
			else {if (getLogCSerial()<0) {
				cout << "Failed to connect Tristar\n";
				break;}}
		}

	icmd=-1;


	}
	
return;
}


/*void Receive(void)
{
	byte deviceId;
	byte funcType;
	word field1;
	word field2;
	long num;
	byte mask=1;
	
	int i,j,j0, ndata, nobj;

		
	//check for data in the recieve buffer
	this->checkSerial();

	//if there is nothing in the recieve buffer, bail.
	if(_len == 0) return;

	//retrieve the query message from the serial uart
	this->serialRx();


	//if the message id is not 255, and
	// and device id does not match bail
	if( (_msg[0] != 0xFF) && 
		(_msg[0] != _device->getId()) )
	{
		return;
	}

	//calculate the checksum of the query message minus the checksum it came with.
	this->calcCrc();
	
	//if the checksum does not match, ignore the message
	if ( _crc != ((_msg[_len - 2] << 8) + _msg[_len - 1]))
		return;

	Serial.println("here3");


		//copy the query byte for byte to the new buffer
		//copy the function type from the incoming query
	funcType = _musg[1];


	if(funcType == READ_AI || funcType == READ_AO)
	{
		ndata = _msg[2];

		word startreg	= (_COMMAND[4*icmd] << 8) | _COMMAND[4*icmd+1];

		if(ndata==32){
		//print and capture

			//hourmeter
			num=((long) _msg[3]) <<16;
			num|=((long) _msg[4]) <<8;
			num|=_msg[5];
			cout<<num;	 cout<<" ";

			//logger flags
			num=((long) _msg[6]) <<8;
			num|=_msg[7];
			cout<<num;	 cout<<" ";

			//alarm
			num=((long) _msg[8]) <<16;
			num|=((long) _msg[9]) <<8;
			num|=_msg[10];

bool alarmsexist=false;
mask=000000000000000000000001;
 for (i=0; i<24; i++){
	 if(num & mask) {Serial.print(i);
	cout<<",";
	 alarmsexist=true;
	 }
	 mask<<=1;
 }
 if(!alarmsexist)  cout<<"-1";
 cout<<" ";


			//cout<<num;	 cout<<" ";

			//Vmax
			num=((long) _msg[11]) <<8;
			num|=_msg[12];
			Serial.print(num*100/32768);	 cout<<" ";

			//Vmin
			num=((long) _msg[13]) <<8;
			num|=_msg[14];
			Serial.print(num*100/32768);	 cout<<" ";

			//Ah
			num=((long) _msg[15]) <<8;
			num|=_msg[16];
			Serial.print(num*0.1);	 cout<<" ";

			//faults
			num=((long) _msg[17]) <<8;
			num|=_msg[18];
			//cout<<num;	 cout<<" ";

bool faultsexist=false;
mask=0000000000000001;
 for (i=0; i<16; i++){
	 if(num & mask) {Serial.print(i);
	cout<<",";
	faultsexist=true;}
	 mask<<=1;
 }
 if(!faultsexist)  cout<<"-1";
 cout<<" ";



			//Tmin
			Serial.print(_msg[19]);	 cout<<" ";

			//Tmax
			Serial.print(_msg[20]);	 cout<<" ";

			//time_absorb
			num=((long) _msg[21]) <<8;
			num|=_msg[22];
			cout<<num;	 cout<<" ";

			//time_eq
			num=((long) _msg[23]) <<8;
			num|=_msg[24];
			cout<<num;	 cout<<" ";

			//time_float
			num=((long) _msg[25]) <<8;
			num|=_msg[26];
			cout<<num;	 cout<<" ";


			cout<<endl;

		}
		else{
			for(i = 0 ; i < ndata/2 ; i++) 
			{
			//copy field 1 from the incoming query
			field1  = (_msg[3+2*i] << 8) | _msg[4+2*i];

			this->setStatus(startreg+i, field1);

			}
		}
	}
	else if(funcType == IDENTIFY)
	{
		deviceOK=true;

		nobj = _msg[7];
		j0=7;

		for(j = 0 ; j < nobj ; j++){
			j0++;
			j0++;

		ndata = _msg[j0];

		for(i = 0 ; i < ndata ; i++) Serial.write(_msg[++j0]);
		
		cout<<endl;
		}
	}

	//free the allocated memory for the query message

	free(_msg);
	//reset the message length;
	_len = 0;
	icmd=-1;
	


}



*/

void delay(unsigned int period)
{
	unsigned int t0;

t0= clock();

while(clock()-t0<period)
{
//cout<<"wait";
}


}


  int getLogCSerial(void){
	char serialBuffer[256];
		int  serialOpen, nBytesRead, i, messageProcessed;

	serialOpen=serial.Open(8,9600);

	if(serialOpen){

		for(i = 0 ; i < _len ; i++) 
		{
		serial.SendNumber((int*) &_msg[i],1);
		}
	
	}

	free(_msg);
	_len=0;
	delay(750);

		//allocate memory for the query response
	_msg = (byte *) malloc(256);


	_len=serial.ReadData(_msg, 256);

     messageProcessed=-1;

		if(_len>0) messageProcessed=processMessagefromTristar();

			
			free(_msg);
			_len=0;

			serial.Close();

			return messageProcessed;
  }

  void getLogRealTerm(char * filename)
{
	int i;
	icmd=-1;
	MyString cmd;
	char tmp[30];

		{
 if(!fout.is_open()) fout.open(filename,ios_base::app);
 else{fout.close(); fout.open(filename,ios_base::app);}
 fout<<"\n";
 fout.close();


cmd.append("start realterm.exe baud=9600 port=16 CAPTURE=2  CAPHEX CAPfile=");
cmd.append("\"");
cmd.append(filename);
cmd.append("\"");
system(cmd.asCharArray());
delay(1000);

cmd.free();
			cmd.append("start realterm.exe FIRST CR LF SENDNUM=");
			cmd.append("\"");
			for(i = 0 ; i < _len ; i++)
			{
				if(i < _len-1) sprintf(tmp,"%d ",(int)_msg[i]);
				else sprintf(tmp,"%d",(int)_msg[i]);
				cmd.append(tmp); 
			}
			cmd.append("\" \\n");

			system(cmd.asCharArray());

			//system("start realterm.exe QUIT");

			free(_msg); cmd.free();
			//reset the message length
			_len = 0;

				delay(1000);
	system("start realterm.exe FIRST QUIT");
	delay(1000);
		}

return;
}



  /*  void getTristarLog(char * filename)
{
	int i;
	icmd=-1;
	MyString cmd;
	char tmp[30];
	ofstream fout;

fout.open(filename);
 fout.close();



	while(icmd!=-99)
	{


		connectDevice();
		readDeviceIdentification();
		readDeviceSerial();
		readLog();
		getAnalogStatus();

		if(icmd==-99) break;


		if(_len)
		{
 if(!fout.is_open()) fout.open(filename,ios_base::app);
 else{fout.close(); fout.open(filename,ios_base::app);}
 fout<<"\n";
 fout.close();


cmd.append("start realterm.exe baud=9600 port=16 CAPTURE=2  CAPHEX CAPfile=");
cmd.append("\"");
cmd.append(filename);
cmd.append("\"");
system(cmd.asCharArray());
delay(1000);

cmd.free();
			cmd.append("start realterm.exe FIRST CR LF SENDNUM=");
			cmd.append("\"");
			for(i = 0 ; i < _len ; i++)
			{
				if(i < _len-1) sprintf(tmp,"%d ",(int)_msg[i]);
				else sprintf(tmp,"%d",(int)_msg[i]);
				cmd.append(tmp); 
			}
			cmd.append("\" \\n");

			system(cmd.asCharArray());

			//system("start realterm.exe QUIT");

			free(_msg); cmd.free();
			//reset the message length
			_len = 0;
		}


	icmd=-1;

	delay(1000);
	system("start realterm.exe FIRST QUIT");
	delay(1000);
	}
	
return;
}

*/
/*int processMessagefromTristar(int _len, int* imsg)
{
	
	byte funcType;
	long num;
	byte mask=1;
	char tmp[50];
	
	int i,j,j0, ndata, nobj;
		
		//copy the query byte for byte to the new buffer
		//copy the function type from the incoming query
	funcType = imsg[1];


		if(funcType == IDENTIFY)
	{
		nobj = imsg[7];
		j0=7;

		for(j = 0 ; j < nobj ; j++){
			j0++;
			j0++;

		ndata = imsg[j0];

		for(i = 0 ; i < ndata ; i++) deviceID.append((char)imsg[++j0]);
		deviceID.append(" ");

		}
		deviceOK=true;
	}
	else if(funcType == READ_AI || funcType == READ_AO)
	{
		ndata = imsg[2];
		


			if(serialOK){

		j0=3;

		if(ndata>=32)
		{

		if(once){

			cout<<"   hour logger      alarms bitfield         Vmax    Vmin       Ah   faults bitfield   Tmax Tmin habsorb heq hfloat\n";
			fout<<"   hour logger      alarms                  Vmax    Vmin       Ah   faults            Tmax Tmin habsorb heq hfloat\n";		

		once=false;
		}
		
			while(j0<ndata)
			{

			//hourmeter
			num=((long) imsg[j0++]) <<16;
			num|=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%6d",num/(256));
			{cout<<tmp; fout<<tmp;} 

			//logger flags
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%6d",num);
			{cout<<tmp; fout<<tmp;}

			//alarms
			num=((long) imsg[j0++]) <<16;
			num|=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];

			cout<<" '"; fout<<" '";
			mask=000000000000000000000001;

			for (i=0; i<24; i++)
			{
				if(num & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' "; fout<<"' ";}

			//Vmax
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%8.2f",(double)num*dfact[0]);
			{cout<<tmp; fout<<tmp;}

			//Vmin
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%8.2f",(double)num*dfact[0]);
			{cout<<tmp; fout<<tmp;}

			//Ah
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%9.1f",(double) num*0.1);
			{cout<<tmp; fout<<tmp;}

			 //faults
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			cout<<" '"; fout<<" '";

			mask=0000000000000001;
			for (i=0; i<16; i++)
			{
				if(num & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' "; fout<<"' ";}

			//Tmax
			sprintf(tmp,"%5d",imsg[j0++]);
			{cout<<tmp; fout<<tmp;}

			//Tmin
			sprintf(tmp,"%5d",imsg[j0++]);
			{cout<<tmp; fout<<tmp;}

			//time_absorb
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%6.1f",(double)num/256.);
			{cout<<tmp; fout<<tmp;}

			//time_eq
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%6.1f",(double)num/256.);
			{cout<<tmp; fout<<tmp;}

			//time_float
			num=((long) imsg[j0++]) <<8;
			num|=imsg[j0++];
			sprintf(tmp,"%6.1f",(double)num/256.);
			{cout<<tmp; fout<<tmp;}

			j0=j0+8;

			{cout<<endl; fout<<"\n";}

			} //while(j0<ndata)
	} //if(ndata>=32)
			else{
				switch(analogctr){
			case 8:
cout<<"Vbat: ";  cout<<dfact[0]*((imsg[3] << 8) | imsg[4]); cout<<"V\n"; analogctr++;
cout<<"Vsens: "; cout<<dfact[1]*((imsg[5] << 8) | imsg[6]); cout<<"V\n"; analogctr++;
cout<<"Vsol: ";  cout<<dfact[2]*((imsg[7] << 8) | imsg[8]); cout<<"V\n"; analogctr++;
cout<<"Achg: ";  cout<<dfact[3]*((imsg[9] << 8) | imsg[10]); cout<<"A\n"; analogctr++;
cout<<"Ald: ";   cout<<dfact[4]*((imsg[11] << 8) | imsg[12]); cout<<"A\n"; analogctr++;
cout<<"Vbat_s: ";cout<<dfact[5]*((imsg[13] << 8) | imsg[14]); cout<<"V\n"; analogctr++;
cout<<"T_hs: ";  cout<<dfact[6]*((imsg[15] << 8) | imsg[16]); cout<<"C\n"; analogctr++;
cout<<"T_batt: ";cout<<dfact[7]*((imsg[17] << 8) | imsg[18]); cout<<"C\n"; analogctr++;
cout<<"Vref: ";  cout<<dfact[8]*((imsg[19] << 8) | imsg[20]); cout<<"V\n"; analogctr++;


fout<<"Vbat: ";  fout<<dfact[0]*((imsg[3] << 8) | imsg[4]); fout<<"V\n";
fout<<"Vsens: "; fout<<dfact[1]*((imsg[5] << 8) | imsg[6]); fout<<"V\n";
fout<<"Vsol: ";  fout<<dfact[2]*((imsg[7] << 8) | imsg[8]); fout<<"V\n";
fout<<"Achg: ";  fout<<dfact[3]*((imsg[9] << 8) | imsg[10]); fout<<"A\n";
fout<<"Ald: ";   fout<<dfact[4]*((imsg[11] << 8) | imsg[12]); fout<<"A\n";
fout<<"Vbat_s: ";fout<<dfact[5]*((imsg[13] << 8) | imsg[14]); fout<<"V\n";
fout<<"T_hs: ";  fout<<dfact[6]*((imsg[15] << 8) | imsg[16]); fout<<"C\n";
fout<<"T_batt: ";fout<<dfact[7]*((imsg[17] << 8) | imsg[18]); fout<<"C\n";
fout<<"Vref: ";  fout<<dfact[8]*((imsg[19] << 8) | imsg[20]); fout<<"V\n";
					 break;

					case 17:
					field1=(imsg[3] << 8) | imsg[4]; analogctr++;
					num=((long)field1) <<16;
					num|=(imsg[5] << 8) | imsg[6];
					cout<<"Ah_r: ";  cout<<num*0.1; cout<<"Ah\n";
					fout<<"Ah_r: ";  fout<<num*0.1; fout<<"Ah\n"; analogctr++;
    				 break;

					case 19:
					field1=(imsg[3] << 8) | imsg[4]; analogctr++;
					num=((long)field1) <<16;
					num|=(imsg[5] << 8) | imsg[6];
					cout<<"Ah_t: ";  cout<<num*0.1; cout<<"Ah\n";
					fout<<"Ah_t: ";  fout<<num*0.1; fout<<"Ah\n"; analogctr++;
    				 break;

					case 21:
					field1=(imsg[3] << 8) | imsg[4]; analogctr++;
					num=((long)field1) <<16;
					num|=(imsg[5] << 8) | imsg[6];
					cout<<"hourmeter: ";  cout<<num; cout<<"hrs\n";
					fout<<"hourmeter: ";  fout<<num; fout<<"hrs\n"; analogctr++;
    				 break;

			 case 23:
			//alarms
			field1=(imsg[3] << 8) | imsg[4]; analogctr++;
			cout<<"alarms '"; fout<<"alarms '";

			mask=0000000000000001;
			for (i=0; i<16; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			 case 24:
			//alarms
			field1=(imsg[3] << 8) | imsg[4]; analogctr++;
			cout<<"faults '"; fout<<"faults '";

			mask=0000000000000001;
			for (i=0; i<16; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			 case 25: //0x754C)
			//alarms
			field1=imsg[4]; analogctr++;
			cout<<"dip switch '"; fout<<"dip switch '";

			mask=00000001;
			for (i=0; i<8; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			case 26:
			num=(imsg[3] << 8) | imsg[4];
			cout<<"State: ";  cout<<num; cout<<"\n";
			fout<<"State: ";  fout<<num; fout<<"\n"; analogctr++;
			break;

			case 27:
			num=(imsg[3] << 8) | imsg[4];
			if(num>=230){
			cout<<"PWM duty cycle: 100%\n";
			fout<<"PWM duty cycle: 100%\n"; analogctr++;
			}
			else{
			cout<<"PWM duty cycle: ";  cout<<num/2.3; cout<<"%\n";
			fout<<"PWM duty cycle: ";  fout<<num/2.3; fout<<"%\n"; analogctr++;
			}
			break;


			 case 28: //0x754C
			//alarms
			field1=imsg[3]; analogctr++;
		//	field1=(imsg[3] << 8) | imsg[4]; analogctr++;
			cout<<"alarms '"; fout<<"alarms '";

			mask=00000001;
			for (i=0; i<8; i++)
			{
				if(field1 & mask) {cout<<"1"; fout<<"1";}
				else {cout<<"0"; fout<<"0";}
				mask<<=1;
			}
			{cout<<"' \n"; fout<<"' \n";}

			break;

			 default:
				 break;

			}
		}
	} //if(serialOK)
	else if(deviceOK)
			{

				for(i = 0 ; i < ndata/2 ; i++) 
				{
				 serialNO.append((char) imsg[4+2*i]);
				 serialNO.append((char) imsg[3+2*i]);
				}

				if(serialctr++>2)
				{
				fout.open(serialNO.append(".log").asCharArray());
				cout<<deviceID.asCharArray(); fout<<deviceID.asCharArray();
				cout<<endl; fout<<"\n";
				serialOK=true;
				}

			}
  }
return 0;
}



*/

  int testCSerial(void){

	int serialOpen=serial.Open(17,9600);

	if(serialOpen){

		while(1) 
		{
		for (int i = 0 ; i < 1000 ; i++) serial.SendNumber(&i,1);
		delay(3000);
		}
	
	}

	

			serial.Close();

			return 0;
  }
