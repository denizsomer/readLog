#include "FunctionsProgram.h"

       static byte _encodeMask []= { 1, 3, 7, 15, 31, 63, 127 };
       static byte _decodeMask []=  { 128, 192, 224, 240, 248, 252, 254 };

/*
Set the Serial Baud rate.
Reconfigure the UART for 8 data bits, no parity, and 1 stop bit.
and flush the serial port.
*/
        /// <summary>
        /// Packs an unpacked 7 bit array to an 8 bit packed array according to the GSM
        /// protocol.
        /// </summary>
        /// <param name="unpackedBytes">The byte array that should be packed.</param>
        /// <param name="replaceInvalidChars">Indicates if invalid characters should be replaced by the default character.</param>
        /// <param name="defaultChar">The character that replaces invalid characters.</param>
        /// <returns>The packed bytes array.</returns>
  byte* PackBytes(byte* unpackedBytes, int unpackedBytesLength, int &shiftedBytesLength)
        {
			bool replaceInvalidChars=false;
char defaultChar=' ';

			int i; 
            byte b, defaultByte = (byte)defaultChar;
			shiftedBytesLength=unpackedBytesLength - unpackedBytesLength / 8;
				byte* shiftedBytes = new byte[shiftedBytesLength];

            int shiftOffset = 0;
            int shiftIndex = 0;

            // Shift the unpacked bytes to the right according to the offset (position of the byte)
           for(i=0; i<unpackedBytesLength; i++)
            {
				b=unpackedBytes[i];
                byte tmpByte = b;

                // Handle invalid characters (bytes out of range)
                if (tmpByte > 127)
                {
                    if (!replaceInvalidChars)
                    {
                        // throw exception and exit the method
						return NULL;
                       // throw new Exception("Invalid character detected: " + tmpByte.ToString("X2"));
                    }
                    else
                    {
                        tmpByte = defaultByte;
                    }
                }

                // Perform the byte shifting
                if (shiftOffset == 7)
                {
                    shiftOffset = 0;
                }
                else
                {
                    shiftedBytes[shiftIndex] = (byte)(tmpByte >> shiftOffset);
                    shiftOffset++;
                    shiftIndex++;
                }
            }

            int moveOffset = 1;
            int moveIndex = 1;
            int packIndex = 0;
            byte* packedBytes = new byte[shiftedBytesLength];

            // Move the bits to the appropriate byte (pack the bits)
           for(i=0; i<unpackedBytesLength; i++)
            {
				b=unpackedBytes[i];

                if (moveOffset == 8)
                {
                    moveOffset = 1;
                }
                else
                {
                    if (moveIndex != unpackedBytesLength)
                    {
                        // Extract the bits to be moved
                        int extractedBitsByte = (unpackedBytes[moveIndex] & _encodeMask[moveOffset - 1]);
                        // Shift the extracted bits to the proper offset
                        extractedBitsByte = (extractedBitsByte << (8 - moveOffset));
                        // Move the bits to the appropriate byte (pack the bits)
                        int movedBitsByte = (extractedBitsByte | shiftedBytes[packIndex]);

                        packedBytes[packIndex] = (byte)movedBitsByte;

                        moveOffset++;
                        packIndex++;
                    }
                    else
                    {
                        packedBytes[packIndex] = shiftedBytes[packIndex];
                    }
                }

                moveIndex++;
            }

		   delete [] shiftedBytes;  shiftedBytes=NULL;
            return packedBytes;
        }


        /// <summary>
        ///  Unpacks a packed 8 bit array to a 7 bit unpacked array according to the GSM
        ///  Protocol.
        /// </summary>
        /// <param name="packedBytes">The byte array that should be unpacked.</param>
        /// <returns>The unpacked bytes array.</returns>
byte* UnpackBytes(byte* packedBytes, int packedBytesLength, int &shiftedBytesLength)
        {
			int i;
			byte b;

			shiftedBytesLength=(packedBytesLength * 8) / 7;
            byte* shiftedBytes = new byte[shiftedBytesLength];

            int shiftOffset = 0;
            int shiftIndex = 0;

            // Shift the packed bytes to the left according to the offset (position of the byte)
           for(i=0; i<packedBytesLength; i++)
            {
				b=packedBytes[i];
                if (shiftOffset == 7)
                {
                    shiftedBytes[shiftIndex] = 0;
                    shiftOffset = 0;
                    shiftIndex++;
                }

                shiftedBytes[shiftIndex] = (byte)((b << shiftOffset) & 127);

                shiftOffset++;
                shiftIndex++;
            }

            int moveOffset = 0;
            int moveIndex = 0;
            int unpackIndex = 1;
            byte* unpackedBytes = new byte[shiftedBytesLength];

            // 
            if (shiftedBytesLength > 0)
            {
                unpackedBytes[unpackIndex - 1] = shiftedBytes[unpackIndex - 1];
            }

            // Move the bits to the appropriate byte (unpack the bits)
           for(i=0; i<packedBytesLength; i++)
            {
				b=packedBytes[i];
                if (unpackIndex != shiftedBytesLength)
                {
                    if (moveOffset == 7)
                    {
                        moveOffset = 0;
                        unpackIndex++;
                        unpackedBytes[unpackIndex - 1] = shiftedBytes[unpackIndex - 1];
                    }

                    if (unpackIndex != shiftedBytesLength)
                    {
                        // Extract the bits to be moved
                        int extractedBitsByte = (packedBytes[moveIndex] & _decodeMask[moveOffset]);
                        // Shift the extracted bits to the proper offset
                        extractedBitsByte = (extractedBitsByte >> (7 - moveOffset));
                        // Move the bits to the appropriate byte (unpack the bits)
                        int movedBitsByte = (extractedBitsByte | shiftedBytes[unpackIndex]);

                        unpackedBytes[unpackIndex] = (byte)movedBitsByte;

                        moveOffset++;
                        unpackIndex++;
                        moveIndex++;
                    }
                }
            }

		    delete [] shiftedBytes;  shiftedBytes=NULL;

            // Remove the padding if exists
            if (unpackedBytes[shiftedBytesLength - 1] == 0)
            {
                byte* finalResultBytes = new byte[shiftedBytesLength - 1];

			 for(i=0; i<shiftedBytesLength - 1; i++)
				finalResultBytes[i]=unpackedBytes[i];

				 delete [] unpackedBytes;  unpackedBytes=NULL;

                return finalResultBytes;
            }

            return unpackedBytes;
        }

void testPDU(){
byte *unpackedBytes,  *packedBytes;

int i,unpackedBytesLength=6,  packedBytesLength;
char * unpackedBytesc ="hello";

packedBytes=PackBytes((byte*) unpackedBytesc, unpackedBytesLength,  packedBytesLength);

if(packedBytesLength-1<16) cout<<'0';
cout<<std::hex <<packedBytesLength-1;
for(i=0; i<packedBytesLength-1; i++){
	if((int)packedBytes[i]<16) cout<<'0';
	cout<<std::hex <<(int)packedBytes[i];}
cout << "\n";

unpackedBytes=UnpackBytes( packedBytes, packedBytesLength,  unpackedBytesLength);

for(i=0; i<unpackedBytesLength-1; i++) cout<< (char)unpackedBytes[i];
cout << "\n";

delete [] packedBytes; packedBytes=NULL; 
delete [] unpackedBytes; unpackedBytes=NULL;

}