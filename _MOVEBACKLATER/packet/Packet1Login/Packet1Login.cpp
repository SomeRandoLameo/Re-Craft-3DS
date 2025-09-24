#include <iostream>

#include "Packet1Login.hpp"

void Packet1Login::readPacketData(DataInputStream input){
    //Packet ID should already be read as 0
    this->protocolVersion = input.readInt();
	this->username =        input.readString(16);
	this->mapSeed =         input.readLong();
	std::string var2 =      input.readString(16); // mistery enumWorldType val
    /*
	this->field_46032_d = EnumWorldType.func_46135_a(var2);
	if(this->field_46032_d == null) {
		this->field_46032_d = EnumWorldType.DEFAULT;
	}
*/
	this->serverMode =        input.readInt();
	this->worldType =         input.readByte();
	this->difficultySetting = input.readByte();
	this->worldHeight =       input.readByte();
	this->maxPlayers =        input.readByte();
}

void Packet1Login::writePacketData(DataOutputStream& output){
    output.writeByte(  this->packetID);        // Packet ID should ideally be set before, for now this is enough
    
    output.writeInt(   this->protocolVersion); // protocol version
    output.writeString(this->username);        // username
    output.writeLong(  this->mapSeed);         // mapSeed 
    
    output.writeString("");                    // EnumWorldType string 
    /* TODO: If WorldType is implemented and the packets are somewhere else, implement this

		if(this.field_46032_d == null) {
			writeString("", var1);
		} else {
			writeString(this.field_46032_d.name(), var1);
		}
    */

    output.writeInt( this->serverMode);        // serverMode
    output.writeByte(this->worldType);         // worldType
    output.writeByte(this->difficultySetting); // difficulty
    output.writeByte(this->worldHeight);       // worldHeight
    output.writeByte(this->maxPlayers);        // maxPlayers
}

int Packet1Login::getPacketSize() {
    int var1 = 0;

    // If EnumWorldType was implemented:
    // if(this->field_46032_d != nullptr) {
    //     var1 = this->field_46032_d->name().length();
    // }

    return 4                                  // protocolVersion (int)
        + static_cast<int>(this->username.length()) // username length
        + 4                                   // mapSeed (long counted as 4 here in MC beta code)
        + 7                                   // constant overhead
        + 4                                   // serverMode (int)
        + var1;                               // EnumWorldType name length
}

